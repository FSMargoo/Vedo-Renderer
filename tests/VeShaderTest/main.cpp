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
 * \file main.cpp
 * \brief The tester for Vedo shader render
 */

#include <include/shader/VeShader.h>

int main() {
	try {
		auto shader = Vedo::Shader::MakeFromString(R"(////////////////////////////////////////////////////////////////
//  vedo_test_shader.sksl
//
//      Descrpition : This shader is a test shader for Vedo
//                    shader wrapper
//      Author      : Margoo (margoo@margoo.icu)
//      Date        : 2024/11/2
//

const int length = $Length$;

uniform float arrayInput[length];

half4 main(vec2 coord) {
	float array[length];
	for (int i = 0; i < length; ++i) {
        array[i] = 0;
    }
	for (int i = 0; i < length; ++i) {
        array[i] += arrayInput[i];
    }
    for (int i = 0; i < length; ++i) {
        array[i] += 1;
    }

    return half4(array[0], array[1], array[2], array[3]);
})");
		shader->Link("Length", 4);
		auto fileShader = Vedo::Shader::MakeFromFile("./shaders/vedo_test_shader.sksl");
		fileShader->Link("Length", 4);

		auto effect		= shader->MakeEffect();
		auto fileEffect = fileShader->MakeEffect();
	} catch (std::exception &e) {
		printf("Error occurred: %s.", e.what());

		exit(-1);
	}

	return 0;
}