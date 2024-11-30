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
 * \file VeCamera.cpp
 * \brief The camera class in Vedo Render
 */

#include <include/render/VeCamera.h>

namespace Vedo {
Camera::Camera() {

}
void Camera::Init() {
	Height = Width / Ratio;
	Height = (Height < 1) ? 1 : Height;

	Center = LookFrom;

	const auto theta		  = FOV * 3.1415926f / 180.f;
	const auto h			  = tan(theta / 2);
	const auto viewportHeight = 2 * h * FocusDistance;
	const auto viewportWidth  = viewportHeight * (static_cast<float>(Width) / Height);

	W = (LookFrom - LookAt).normalize();
	U = VUP.cross(W).normalize();
	V = W.cross(U);

	auto viewportU = viewportWidth * U;
	auto viewportV = viewportHeight * -V;

	PixelDeltaU = viewportU * (1.f / Width);
	PixelDeltaV = viewportV * (1.f / Height);
	
	const auto viewportUpperLeft =
		Center - (FocusDistance * W) - viewportU * 0.5f - viewportV * 0.5f;
	Pixel100Loc = viewportUpperLeft + 0.5f * (PixelDeltaU + PixelDeltaV);

	auto defocusRadius = FocusDistance * tan((DeFocusAngle / 2) * 3.1415926f / 180.f);
	DeFocusDiskU = defocusRadius * U;
	DeFocusDiskV = defocusRadius * V;
}
}