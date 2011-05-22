/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPIStoreSCU_h
#define _pacsAPIStoreSCU_h

#include "pacsAPIDimse.h"
#include "PacsAPIWin32Header.h"

namespace PACS
{
	/**
	* \brief PacsAPIStoreSCU. Implements StoreSCU dimse service
	* \ingroup PacsAPI 
	* \author Luigi Carotenuto
	*
	* The API provides functions for connecting to a PACS Server, based on
	* the Library dcmtk http://dicom.offis.de/index.php.en
	*
	*
	*/
	class PACSAPI_EXPORT StoreSCU
	{

		// ----------------------------------
		// OPERATIONS
		// ----------------------------------
	public:
		//! Default Constructor
		StoreSCU(){};

		//! Destructor
		~StoreSCU(){};

		/**
		Send a file to PACS.
		\param assoc - [in] A valid accepted Association. 
		\param filename - [in] the dcm fullfilename (path+filename) to send
		\return true if the query operation has success.
		*/
		OFBool store(PACS::Association* assoc, const char *fullFileName);



	private:

		//OFBool cstore(PACS::Association * Assoc, const OFString& fname);
		static int secondsSince1970();
		static OFString	intToString(int i);
		static OFString makeUID(OFString basePrefix, int counter);
		static OFBool updateStringAttributeValue(DcmItem* dataset, const DcmTagKey& key, OFString& value);
		static void	replaceSOPInstanceInformation(DcmDataset* dataset);
		static void	progressCallback(void * /*callbackData*/,
			T_DIMSE_StoreProgress *progress,
			T_DIMSE_C_StoreRQ * /*req*/);



		// ----------------------------------
		// MEMBERS
		// ----------------------------------
	private:
		static OFBool opt_verbose;

		static OFBool opt_haltOnUnsuccessfulStore;
		static OFBool unsuccessfulStoreEncountered;
		static int lastStatusCode;

		static OFCmdUnsignedInt opt_repeatCount;
		static OFCmdUnsignedInt opt_inventPatientCount;
		static OFCmdUnsignedInt opt_inventStudyCount;
		static OFCmdUnsignedInt opt_inventSeriesCount;
		static OFBool opt_inventSOPInstanceInformation;
		static OFBool opt_correctUIDPadding;
		static OFString patientNamePrefix;   // PatientName is PN (maximum 16 chars)
		static OFString patientIDPrefix; // PatientID is LO (maximum 64 chars)
		static OFString studyIDPrefix;   // StudyID is SH (maximum 16 chars)
		static OFString accessionNumberPrefix;  // AccessionNumber is SH (maximum 16 chars)


	}; // StoreSCU

}//namespace PACS



#endif //_pacsAPIMoveSCU