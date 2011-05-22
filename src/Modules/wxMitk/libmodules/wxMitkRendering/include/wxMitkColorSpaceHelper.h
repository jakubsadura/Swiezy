/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkColorSpaceHelper_H
#define wxMitkColorSpaceHelper_H

#include "wxMitkRenderingWin32Header.h"

class wxColour;

namespace mitk
{

/**
The wxMitkColorSpaceHelper is a helper class for handling colorspaces and 
converting from RGB to HSV or HSL

\ingroup wxMitkCore
\author Juan Antonio Moya
\date 12 Dec 2007
*/
class WXMITKRENDERING_EXPORT wxMitkColorSpaceHelper
{
public:
	static void RGBtoHSV(const unsigned int rgb[3], double hsv[3]);
	static void RGBtoHSV(const double rgb[3], double hsv[3]);
	static void RGBtoHSL(const unsigned int rgb[3], unsigned int hsl[3]);
	static void RGBtoHSL(const double rgb[3], double hsl[3]);

	static void HSVtoRGB(const double hsv[3], unsigned int rgb[3]);
	static void HSVtoRGB(const double hsv[3], double rgb[3]);

	static void HSLtoRGB(const double hsl[3], double rgb[3]);
	static void HSLtoRGB(const unsigned int hsl[3], unsigned int rgb[3]);
};
 
} // namespace mitk


#endif // wxMitkColorGradientCanvas_H

