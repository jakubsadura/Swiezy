/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// For compilers that don't support precompilation, include "wx/wx.h"

#include "wxMitkLevelWindowHelper.h"

using namespace mitk;


/**
Computes a valid range for a Level Window, taken care first of max and min values, second fitting the level and window
without trespassing the boundaries. All values are returned by reference.
Returns false if the range boundaries or the window were not valid, and does not modify the variables passed by parameter.
*/
bool wxMitkLevelWindowHelper::ComputeValidRange(int& levelValue, unsigned int& windowValue, int& rangeMin, int& rangeMax)
{
	// Ensure valid values
	if(rangeMax <= rangeMin)
		return false;
	if(windowValue < 1 || (int)(windowValue) > (rangeMax - rangeMin)) 
		return false;

	// Center the level in the middle of the window
	if(levelValue + (int)(windowValue / 2) > rangeMax)
	{
		levelValue = rangeMax - (int)(windowValue / 2);
		windowValue = (rangeMax - levelValue) * 2;
	}

	if (levelValue - (int)(windowValue / 2) < rangeMin)
	{
		levelValue = rangeMin + (int)(windowValue / 2);
		windowValue = (levelValue - rangeMin) * 2;
	}		

	return true;
}

/**
Same than function above, but accepting parameters of type <i>long</i>
*/
bool wxMitkLevelWindowHelper::ComputeValidRange(long& levelValue, long& windowValue, long& rangeMin, long& rangeMax)
{
	// Ensure valid values
	if(rangeMax <= rangeMin)
		return false;
	if(windowValue < 1 || windowValue > (rangeMax - rangeMin)) 
		return false;

	// Center the level in the middle of the window
	if(levelValue + (windowValue / 2) > rangeMax)
	{
		levelValue = rangeMax - (windowValue / 2);
		windowValue = (rangeMax - levelValue) * 2;
	}

	if (levelValue - (windowValue / 2) < rangeMin)
	{
		levelValue = rangeMin + (windowValue / 2);
		windowValue = (rangeMin + levelValue) * 2;
	}		

	return true;
}

bool wxMitkLevelWindowHelper::ComputeValidRange(
	mitk::ScalarType& levelValue, 
	mitk::ScalarType& windowValue, 
	mitk::ScalarType& rangeMin, 
	mitk::ScalarType& rangeMax)
{
	// Ensure valid values
	if(rangeMax <= rangeMin)
		return false;
	if(windowValue < 1 || windowValue > (rangeMax - rangeMin)) 
		return false;

	// Center the level in the middle of the window
	if(levelValue > rangeMax - (windowValue / 2)  )
	{
		levelValue = rangeMax - (windowValue / 2);
	}

	if (levelValue < rangeMin + (windowValue / 2) )
	{
		levelValue = rangeMin + (windowValue / 2);
	}		

	return true;
}
