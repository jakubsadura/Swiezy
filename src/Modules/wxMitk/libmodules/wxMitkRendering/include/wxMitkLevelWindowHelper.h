/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef wxMitkLevelWindowHelper_H
#define wxMitkLevelWindowHelper_H

#include "wxMitkRenderingWin32Header.h"

namespace mitk
{

/**
This class provides helper functions for dealing with LevelWindows and 
computing their valid values. Works essentially as a Validator.

Centralizes the validation of range values (levelwindow) for many widgets.

\ingroup wxMitkCore
\author Juan Antonio Moya
\date 23 Oct 2007
*/
class WXMITKRENDERING_EXPORT wxMitkLevelWindowHelper
{
public:
	static bool ComputeValidRange(
		int& levelValue, 
		unsigned int& windowValue, 
		int& rangeMin, 
		int& rangeMax);
	static bool ComputeValidRange(
		long& levelValue, 
		long& windowValue, 
		long& rangeMin, 
		long& rangeMax);
	static bool ComputeValidRange(
		float& levelValue, 
		float& windowValue, 
		float& rangeMin, 
		float& rangeMax);
};
 
}

#endif // wxMitkLevelWindowHelper_H
