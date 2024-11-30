/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file VeShaderWrapper.cpp
 * \brief The shader wrapper of Skia SKSL, enabling it with size-changeable array input
 */

#include <include/shader/VeShader.h>

namespace Vedo {
Shader::Shader(const char *ShaderCode) : _code(ShaderCode) {
}
std::string Shader::MakeCode() {
	auto linkedCode = Preprocess();

	auto [instance, error] = std::move(SkRuntimeEffect::MakeForShader(SkString(linkedCode.c_str())));
	if (!error.isEmpty()) {
		throw ShaderCreateFailure(error.c_str());
	}

	return linkedCode;
}
std::string Shader::Preprocess() {
	stb_lexer	lexer;
	const char *code = _code.c_str();
	stb_c_lexer_init(&lexer, code, code + strlen(code), (char *)malloc(0x10000), 0x10000);

	std::string result = "void init_vedo();";

	while (stb_c_lexer_get_token(&lexer)) {
		if (lexer.token == '@' && stb_c_lexer_get_token(&lexer) && strcmp(lexer.string, "uniform") == 0) {
			// Skip white space
			stb_c_lexer_get_token(&lexer);
			stb_c_lexer_get_token(&lexer);
			stb_c_lexer_get_token(&lexer);
			stb_c_lexer_get_token(&lexer);
			std::string type = lexer.string;

			stb_c_lexer_get_token(&lexer);
			if (_uniformReplacement.contains(lexer.string)) {
				auto id		   = lexer.string;
				auto structure = _uniformReplacement[lexer.string];

				// Add size measure
				result.append(std::format("\nconst int l_{} = {};\n", id, structure.size()));
				result.append(std::format("\nconst float lf_{} = {};\n", id, structure.size()));

				// Define variable
				result.append(std::format("\n{} {}[{}];\n", type, id, structure.size()));

				std::string initBodyCode;
				for (int count = 0; count < structure.size(); ++count) {
					auto property = structure[count]->PropertyValue();
					for (auto &instance : property) {
						initBodyCode.append(std::format("{}[{}].{} = {};\n", id, count, instance.first, instance.second));
					}
				}

				result.append(std::format("void func_init_{}() {{\n {} }}\n", id, initBodyCode));
			} else {
				throw ShaderInvalidUniform(lexer.string);
			}

			stb_c_lexer_get_token(&lexer);
		} else if (lexer.string[0] == '$' && lexer.token == CLEX_id) {
			if (_linkReplacement.contains(lexer.string)) {
				result.append(_linkReplacement[lexer.string]);
			} else {
				throw ShaderInvalidVariable(lexer.string);
			}
		} else {
			switch (lexer.token) {
			case CLEX_id:
				result.append(lexer.string);
				break;
			case CLEX_eq:
				result.append("==");
				break;
			case CLEX_noteq:
				result.append("!=");
				break;
			case CLEX_lesseq:
				result.append("<=");
				break;
			case CLEX_greatereq:
				result.append(">=");
				break;
			case CLEX_andand:
				result.append("&&");
				break;
			case CLEX_oror:
				result.append("||");
				break;
			case CLEX_shl:
				result.append("<<");
				break;
			case CLEX_shr:
				result.append(">>");
				break;
			case CLEX_plusplus:
				result.append("++");
				break;
			case CLEX_minusminus:
				result.append("--");
				break;
			case CLEX_arrow:
				result.append("->");
				break;
			case CLEX_andeq:
				result.append("&=");
				break;
			case CLEX_oreq:
				result.append("|=");
				break;
			case CLEX_xoreq:
				result.append("^=");
				break;
			case CLEX_pluseq:
				result.append("+=");
				break;
			case CLEX_minuseq:
				result.append("-=");
				break;
			case CLEX_muleq:
				result.append("*=");
				break;
			case CLEX_diveq:
				result.append("/=");
				break;
			case CLEX_modeq:
				result.append("%%=");
				break;
			case CLEX_shleq:
				result.append("<<=");
				break;
			case CLEX_shreq:
				result.append(">>=");
				break;
			case CLEX_eqarrow:
				result.append("=>");
				break;
			case CLEX_dqstring:
				result.append(std::format("\"{}\"", lexer.string));
				break;
			case CLEX_sqstring:
				result.append(std::format("'\"{}\"'", lexer.string));
				break;
			case CLEX_charlit:
				result.append(std::format("'{}'", lexer.string));
				break;
			case CLEX_intlit:
				result.append(std::format("{}", lexer.int_number));
				break;
			case CLEX_floatlit:
				result.append(std::format("{}", lexer.real_number));
				break;
			default:
				result.append(std::format("{}", char(lexer.token)));
				break;
			}

			result.append(" ");
		}
	}

	result.append("\nvoid init_vedo() {\n");
	for (auto& uniform : _uniformReplacement) {
		result.append(std::format("func_init_{}();\n", uniform.first));
	}
	result.append("\n}");

	printf("%s\n", result.c_str());

	_flushall();

	return result;
}
} // namespace Vedo