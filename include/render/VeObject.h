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
 * \file VeObject.h
 * \brief The object of the Vedo renderer
 */

#pragma once

#include <include/math/VeVector.h>
#include <include/shader/VeShader.h>

namespace Vedo {
constexpr int LambertMaterial	 = 0;
constexpr int MetalMaterial		 = 1;
constexpr int DielectricMaterial = 2;

constexpr int SphereGeometry = 0;

/**
 * The object of the world object
 */
class Object : public IShaderStructureUniform {
public:
	std::vector<std::string> PropertyList() override {
		return {"Material", "Shape", "Center", "Radius", "Albedo", "Fuzz", "IndexRefraction"};
	}
	std::map<std::string, std::string> PropertyValue() override {
		return {
			{ "Material", std::to_string(Material) },
			{ "Shape", std::to_string(Shape) },
			{ "Center", MathUniform::UniformVec3(Center) },
			{ "Radius", std::to_string(Radius) },
			{ "Albedo", MathUniform::UniformVec3(Albedo) },
			{ "Fuzz", std::to_string(Fuzz) },
			{ "IndexRefraction", std::to_string(IndexRefraction) }
		};
	}
	[[nodiscard]] std::string Type() const override {
		return "Object";
	}

public:
	int Material;
	int Shape;
	Vec3 Center;
	Vec3 Albedo;
	float Radius;
	float Fuzz;
	float IndexRefraction;
};
} // namespace Vedo