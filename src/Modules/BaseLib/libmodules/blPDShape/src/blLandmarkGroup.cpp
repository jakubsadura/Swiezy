/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

/*
This class defines a group of landmarks.
*/

#include "blLandmarkGroup.h"


/** 
 * This constructor creates a new landmark group
 *
 * \param groupID identifier for the landmark group
 * \param numberOfLandmarks number of landmarks in this group
 * \param landmarkIds array with the landmark identifiers in this group
 */
//--------------------------------------------------
blLandmarkGroup::blLandmarkGroup(unsigned int groupID, 
								 unsigned int numberOfLandmarks,
								 unsigned int * landmarkIds)
//--------------------------------------------------
{

	this->groupID = groupID;
	this->numberLandmarks = numberOfLandmarks;
	this->landmarks = landmarkIds;
}




/** 
 *	This method returns true if the landmark whose landmark id is specified
 *	belongs to this group.
 *
 *	\param landmarkId identifier of the specified landmark
 */
//--------------------------------------------------
bool blLandmarkGroup::LandmarkInGroup(unsigned int landmarkID)
//--------------------------------------------------
{
	// linear search

	unsigned int i;
	for (i = 0; i < this->numberLandmarks; i++) 
	{
		if (landmarks[i] == landmarkID)
		{
			return true;
		}		
	}
	return false;
}

