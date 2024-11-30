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
 * \file VeCamera.h
 * \brief The camera class in Vedo Render
 */

#pragma once

#include <include/math/VeVector.h>
#include <include/shader/VeShader.h>

namespace Vedo {
/**
 * The camera of the Vedo Render
 */
class Camera : public Vedo::IShaderStructureUniform {
public:
	Camera();

public:
	/**
	 * Init the camera
	 */
	void Init();

public:
	std::vector<std::string> PropertyList() override {
		return {"Ratio", "Width", "SPP", "Depth", "LookFrom", "LookAt", "VUP", "FOV", "FocusDistance", "DeFocusAngle", "Height", "Center", "PixelDeltaU", "PixelDeltaV", "Pixel100Loc", "U", "V", "W", "DeFocusDiskU", "DeFocusDiskV"};
	}
	std::map<std::string, std::string> PropertyValue() override {
		return {
			{ "Ratio", std::to_string(Ratio) },
			{ "Width", std::to_string(Width) },
			{ "SPP", std::to_string(SPP) },
			{ "Depth", std::to_string(Depth) },
			{ "LookFrom", Vedo::MathUniform::UniformVec3(LookFrom) },
			{ "LookAt", Vedo::MathUniform::UniformVec3(LookAt) },
			{ "VUP", Vedo::MathUniform::UniformVec3(VUP) },
			{ "FOV", std::to_string(FOV) },
			{ "FocusDistance", std::to_string(FocusDistance) },
			{ "DeFocusAngle", std::to_string(DeFocusAngle) },
			{ "Height", std::to_string(Height) },
			{ "Center", Vedo::MathUniform::UniformVec3(Center) },
			{ "PixelDeltaU", Vedo::MathUniform::UniformVec3(PixelDeltaU) },
			{ "PixelDeltaV", Vedo::MathUniform::UniformVec3(PixelDeltaV) },
			{ "Pixel100Loc", Vedo::MathUniform::UniformVec3(Pixel100Loc) },
			{ "U", Vedo::MathUniform::UniformVec3(U) },
			{ "V", Vedo::MathUniform::UniformVec3(V) },
			{ "W", Vedo::MathUniform::UniformVec3(W) },
			{ "DeFocusDiskU", Vedo::MathUniform::UniformVec3(DeFocusDiskU) },
			{ "DeFocusDiskV", Vedo::MathUniform::UniformVec3(DeFocusDiskV) }
		};
	}
	[[nodiscard]] std::string Type() const override {
		return "Camera";
	}

public:
	float Ratio;
	float Width;
	float SPP;
	float Depth;

	Point LookFrom;
	Point LookAt;
	Point VUP;

	float FOV;

	float FocusDistance;
	float DeFocusAngle;

private:
	float Height;
	Point Center;

	Vec3 PixelDeltaU;
	Vec3 PixelDeltaV;
	Vec3 Pixel100Loc;

	Vec3 U;
	Vec3 V;
	Vec3 W;
	Vec3 DeFocusDiskU;
	Vec3 DeFocusDiskV;

public:
	friend class Render;
};
}