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
 * \file VeShaderWrapper.h
 * \brief The shader wrapper of Skia SKSL, enabling it with size-changeable array input
 */

#pragma once

#include <include/VeBase.h>
#include <include/skia/VeSkia.h>

#include <thirdparty/stb_c_lexer/stb_c_lexer.h>

#include <fstream>
#include <map>

namespace Vedo {

VeRegisterException(ShaderInvalidFile, R"(Could not open the shader file "{}")");
VeRegisterException(ShaderCreateFailure, "Could not create the shader with error:\n{}");
VeRegisterException(ShaderInvalidUniform, R"(Vedo Shader : Unknown uniform structure "{}")");
VeRegisterException(ShaderInvalidVariable, R"(Vedo Shader : Unknown variable "{}")");

/**
 * The interface for uniform passable structure, when a structure needs to be passed by Vedo
 * Shader, it must inherit this interface to meet the requirement of the shader maker
 */
class IShaderStructureUniform {
public:
	/**
	 * Get the property list of the structure
	 * @return The property list in std::vector<std::string> type
	 */
	virtual std::vector<std::string> PropertyList() = 0;
	/**
	 * Get a mapping which provides a map of (Property Name)->(Value in String Format) form
	 * @return The specified map
	 */
	virtual std::map<std::string, std::string> PropertyValue() = 0;
	/**
	 * Get the type name of the uniform structure
	 * @return The type name of the uniform structure
	 */
	[[nodiscard]] virtual std::string Type() const = 0;
};

/**
 * The shader wrapper for SKSL, it will process the array length
 * predefine in the shader for Vedo renderer
 */
class Shader {
public:
	/**
	 * Make a Vedo shader from the code
	 * @param Code The code string
	 * @return The shader instance in Vedo renderer
	 */
	static std::unique_ptr<Shader> MakeFromString(const std::string &Code) {
		return std::make_unique<Shader>(Shader(Code.c_str()));
	}
	/**
	 * Make a Vedo shader from the file
	 * @param Path The path to file
	 * @return The shader instance in Vedo renderer
	 */
	static std::unique_ptr<Shader> MakeFromFile(const std::string &Path) {
		std::string	  file;
		std::string	  temp;
		std::ifstream stream(Path);

		if (stream.is_open()) {
			while (!stream.eof()) {
				std::getline(stream, temp);
				file.append(temp);
				file.append("\n");
			}

			return std::make_unique<Shader>(Shader(file.c_str()));
		} else {
			throw ShaderInvalidFile(Path.c_str());
		}
	}

public:
	/**
	 * Clone a new shader from this object
	 * @return The new shader object reference
	 */
	std::unique_ptr<Shader> Clone() {
		return std::make_unique<Shader>(Shader(_code.c_str()));
	}

public:
	/**
	 * Link the static predefine variable in the shader code
	 * @tparam DataType The data type of the specified data must support to be converted to
	 * string by std::to_string
	 * @param Tag The tag of the data
	 * @param Data The data instance
	 */
	template <class DataType> void Link(const char *Tag, const DataType &Data) {
		_linkReplacement[std::format("${}$", Tag)] = std::to_string(Data);
	}

	/**
	 * Bind an array uniform to the shader
	 * @tparam UniformType The uniform structure type
	 * @param Name The name of the structure
	 * @param arrayList The array list instance
	 */
	template<class UniformType>
		requires std::is_base_of_v<IShaderStructureUniform, UniformType>
	void BindUniform(const std::string &Name, std::vector<UniformType*> arrayList) {
		_uniformReplacement[Name] = arrayList;
	}

public:
	/**
	 * Make SkRuntimeEffect by Vedo shader object, when the compiler reports an error about shader
	 * it will throw a ShaderCreateFailure exception
	 * @return The SkRuntimeEffect Skia safe pointer reference
	 */
	sk_sp<SkRuntimeEffect> &MakeEffect();

private:
	/**
	 * Preprocess the shader code and store in the linked code
	 * @return The returned preprocessed string
	 */
	std::string Preprocess();

private:
	explicit Shader(const char *ShaderCode);

private:
	std::string _code;

private:
	std::map<std::string, std::string> _linkReplacement;
	std::map<std::string, std::vector<IShaderStructureUniform*> > _uniformReplacement;
};
} // namespace Vedo