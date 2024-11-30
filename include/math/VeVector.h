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
 * \file VeVector.h
 * \brief The vector of Vedo Render
 */

#pragma once

#include <include/skia/VeSkia.h>

#include <cmath>

namespace Vedo {
/**
 * The 3D vector in vedo render.
 */
using Vec3 = SkV3;
/**
 * The 2D vector in vedo render.
 */
using Vec2 = SkV2;
/**
 * The 3D vector with a parameter in vedo render.
 */
using Vec4 = SkV4;

/**
 * The alias of the vector
 */
using Point = Vec3;

/**
 * The static class provide API for uniform variable converting
 */
class MathUniform {
public:
	/**
	 * Return the uniform string of a Vec4 structure
	 * @param Vector The vector to be converted
	 * @return The uniform string
	 */
	static std::string UniformVec4(const Vec4 &Vector) {
		return std::format("vec4({}, {}, {}, {})", Vector.x, Vector.y, Vector.z, Vector.w);
	}
	/**
	 * Return the uniform string of a Vec3 structure
	 * @param Vector The vector to be converted
	 * @return The uniform string
	 */
	static std::string UniformVec3(const Vec3 &Vector) {
		return std::format("vec3({}, {}, {})", Vector.x, Vector.y, Vector.z);
	}
	/**
	 * Return the uniform string of a Vec2 structure
	 * @param Vector The vector to be converted
	 * @return The uniform string
	 */
	static std::string UniformVec2(const Vec2 &Vector) {
		return std::format("vec2({}, {})", Vector.x, Vector.y);
	}
};
}