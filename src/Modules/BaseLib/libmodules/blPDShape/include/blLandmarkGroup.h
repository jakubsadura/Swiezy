/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/



#ifndef __blLandmarkGroup_h
#define __blLandmarkGroup_h

/**
 * \brief This class defines a group of landmarks
 * \ingroup blPDShape
 *
 *	A Landmark Group is a container class that gives information about the
 *	landmarks in a landmark group.
 */
class blLandmarkGroup 
{

    public:
		/** \brief Constructor */
		blLandmarkGroup (unsigned int groupID, unsigned int numberOfLandmarks, 
							unsigned int * landmarkIds);

		/** \brief get the landmark group identifier */
		unsigned int GetGroupID() const
			{ return this->groupID; };

		/** \brief return a pointer to the landmark ids */
		const unsigned int * GetLandmarks() const
			{ return this->landmarks; };

		/** \brief return the number of landmarks */
		unsigned int GetNumberOfLandmarks() const
			{ return this->numberLandmarks; };		

		/** \brief is the landmark id in this group 
		 *	\return an array of landmarks ids
		 */
		bool LandmarkInGroup(unsigned int landmarkID);

    private:
        unsigned int groupID;			//!< group identifier 
        unsigned int numberLandmarks;   //!< number of landmarks in this group
		unsigned int * landmarks;		//!< landmarks (id) in this group

};

#endif
