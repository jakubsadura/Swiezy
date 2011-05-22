/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "pacsAPIStoreSCU.h"

// Init static variables
OFBool PACS::StoreSCU::opt_verbose = OFFalse;

OFBool PACS::StoreSCU::opt_haltOnUnsuccessfulStore = OFTrue;
OFBool PACS::StoreSCU::unsuccessfulStoreEncountered = OFFalse;
int PACS::StoreSCU::lastStatusCode = STATUS_Success;

OFCmdUnsignedInt PACS::StoreSCU::opt_repeatCount = 1;
OFCmdUnsignedInt PACS::StoreSCU::opt_inventPatientCount = 25;
OFCmdUnsignedInt PACS::StoreSCU::opt_inventStudyCount = 50;
OFCmdUnsignedInt PACS::StoreSCU::opt_inventSeriesCount = 100;
OFBool PACS::StoreSCU::opt_inventSOPInstanceInformation = OFFalse;
OFBool PACS::StoreSCU::opt_correctUIDPadding = OFFalse;
OFString PACS::StoreSCU::patientNamePrefix("OFFIS^TEST_PN_");   // PatientName is PN (maximum 16 chars)
OFString PACS::StoreSCU::patientIDPrefix("PID_"); // PatientID is LO (maximum 64 chars)
OFString PACS::StoreSCU::studyIDPrefix("SID_");   // StudyID is SH (maximum 16 chars)
OFString PACS::StoreSCU::accessionNumberPrefix;  // AccessionNumber is SH (maximum 16 chars)



int PACS::StoreSCU::secondsSince1970()
{
	time_t t = time(NULL);
	return (int)t;
}

OFString PACS::StoreSCU::intToString(int i)
{
	char numbuf[32];
	sprintf(numbuf, "%d", i);
	return numbuf;
}

OFString PACS::StoreSCU::makeUID(OFString basePrefix, int counter)
{
	OFString prefix = basePrefix + "." + intToString(counter);
	char uidbuf[65];
	OFString uid = dcmGenerateUniqueIdentifier(uidbuf, prefix.c_str());
	return uid;
}

OFBool PACS::StoreSCU::updateStringAttributeValue(DcmItem* dataset, const DcmTagKey& key, OFString& value)
{
	DcmStack stack;
	DcmTag tag(key);

	OFCondition cond = EC_Normal;
	cond = dataset->search(key, stack, ESM_fromHere, OFFalse);
	if (cond != EC_Normal) {
		CERR << "error: updateStringAttributeValue: cannot find: " << tag.getTagName()
			<< " " << key << ": "
			<< cond.text() << endl;
		return OFFalse;
	}

	DcmElement* elem = (DcmElement*) stack.top();

	DcmVR vr(elem->ident());
	if (elem->getLength() > vr.getMaxValueLength()) {
		CERR << "error: updateStringAttributeValue: INTERNAL ERROR: " << tag.getTagName()
			<< " " << key << ": value too large (max "
			<< vr.getMaxValueLength() << ") for " << vr.getVRName() << " value: " << value << endl;
		return OFFalse;
	}

	cond = elem->putOFStringArray(value);
	if (cond != EC_Normal) {
		CERR << "error: updateStringAttributeValue: cannot put string in attribute: " << tag.getTagName()
			<< " " << key << ": "
			<< cond.text() << endl;
		return OFFalse;
	}

	return OFTrue;
}

void PACS::StoreSCU::replaceSOPInstanceInformation(DcmDataset* dataset)
{
	static OFCmdUnsignedInt patientCounter = 0;
	static OFCmdUnsignedInt studyCounter = 0;
	static OFCmdUnsignedInt seriesCounter = 0;
	static OFCmdUnsignedInt imageCounter = 0;
	static OFString seriesInstanceUID;
	static OFString seriesNumber;
	static OFString studyInstanceUID;
	static OFString studyID;
	static OFString accessionNumber;
	static OFString patientID;
	static OFString patientName;

	if (seriesInstanceUID.length() == 0) seriesInstanceUID=makeUID(SITE_SERIES_UID_ROOT, (int)seriesCounter);
	if (seriesNumber.length() == 0) seriesNumber = intToString((int)seriesCounter);
	if (studyInstanceUID.length() == 0) studyInstanceUID = makeUID(SITE_STUDY_UID_ROOT, (int)studyCounter);
	if (studyID.length() == 0) studyID = studyIDPrefix + intToString((int)secondsSince1970()) + intToString((int)studyCounter);
	if (accessionNumber.length() == 0) accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
	if (patientID.length() == 0) patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
	if (patientName.length() == 0) patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);

	if (imageCounter >= opt_inventSeriesCount) {
		imageCounter = 0;
		seriesCounter++;
		seriesInstanceUID = makeUID(SITE_SERIES_UID_ROOT, (int)seriesCounter);
		seriesNumber = intToString((int)seriesCounter);
	}
	if (seriesCounter >= opt_inventStudyCount) {
		seriesCounter = 0;
		studyCounter++;
		studyInstanceUID = makeUID(SITE_STUDY_UID_ROOT, (int)studyCounter);
		studyID = studyIDPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
		accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
	}
	if (studyCounter >= opt_inventPatientCount) {
		// we create as many patients as necessary */
		studyCounter = 0;
		patientCounter++;
		patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
		patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
	}

	OFString sopInstanceUID = makeUID(SITE_INSTANCE_UID_ROOT, (int)imageCounter);
	OFString imageNumber = intToString((int)imageCounter);

	if (opt_verbose) {
		COUT << "Inventing Identifying Information (" <<
			"pa" << patientCounter << ", st" << studyCounter <<
			", se" << seriesCounter << ", im" << imageCounter << "): " << endl;
		COUT << "  PatientName=" << patientName << endl;
		COUT << "  PatientID=" << patientID << endl;
		COUT << "  StudyInstanceUID=" << studyInstanceUID << endl;
		COUT << "  StudyID=" << studyID << endl;
		COUT << "  SeriesInstanceUID=" << seriesInstanceUID << endl;
		COUT << "  SeriesNumber=" << seriesNumber << endl;
		COUT << "  SOPInstanceUID=" << sopInstanceUID << endl;
		COUT << "  ImageNumber=" << imageNumber << endl;
	}

	updateStringAttributeValue(dataset, DCM_PatientsName, patientName);
	updateStringAttributeValue(dataset, DCM_PatientID, patientID);
	updateStringAttributeValue(dataset, DCM_StudyInstanceUID, studyInstanceUID);
	updateStringAttributeValue(dataset, DCM_StudyID, studyID);
	updateStringAttributeValue(dataset, DCM_SeriesInstanceUID, seriesInstanceUID);
	updateStringAttributeValue(dataset, DCM_SeriesNumber, seriesNumber);
	updateStringAttributeValue(dataset, DCM_SOPInstanceUID, sopInstanceUID);
	updateStringAttributeValue(dataset, DCM_InstanceNumber, imageNumber);

	imageCounter++;
}

void PACS::StoreSCU::progressCallback(void * /*callbackData*/,
				 T_DIMSE_StoreProgress *progress,
				 T_DIMSE_C_StoreRQ * /*req*/)
{
	if (opt_verbose) {
		switch (progress->state) {
		case DIMSE_StoreBegin:
			printf("XMIT:"); break;
		case DIMSE_StoreEnd:
			printf("\n"); break;
		default:
			putchar('.'); break;
		}
		fflush(stdout);
	}
}


OFBool PACS::StoreSCU::store(
			 PACS::Association* Assoc, 
			const char *fname)

/*
* This function will read all the information from the given file,
* figure out a corresponding presentation context which will be used
* to transmit the information over the network to the SCP, and it
* will finally initiate the transmission of all data to the SCP.
*
* Parameters:
*   assoc - [in] The association (network connection to another DICOM application).
*   fname - [in] Name of the file which shall be processed.
*/		
{
	
	T_ASC_Association *ofAssoc = Assoc->getOFAssociation();
	DIC_US msgId = ofAssoc->nextMsgID++;
	T_DIMSE_C_StoreRQ req;
	T_DIMSE_C_StoreRSP rsp;
	DIC_UI sopClass;
	DIC_UI sopInstance;
	DcmDataset *statusDetail = NULL;
	T_ASC_PresentationContextID presId;

	if (opt_verbose) {
		printf("--------------------------\n");
		printf("Sending file: %s\n", fname);
	}

	/* read information from file. After the call to DcmFileFormat::loadFile(...) the information */
	/* which is encapsulated in the file will be available through the DcmFileFormat object. */
	/* In detail, it will be available through calls to DcmFileFormat::getMetaInfo() (for */
	/* meta header information) and DcmFileFormat::getDataset() (for data set information). */
	DcmFileFormat dcmff;
	OFCondition cond = dcmff.loadFile(fname);

	/* figure out if an error occurred while the file was read*/
	if (cond.bad()) {
		printf("Bad DICOM file: %s: %s", fname, cond.text());
		return OFFalse;
	}

	/* if required, invent new SOP instance information for the current data set (user option) */
	if (opt_inventSOPInstanceInformation) {
		replaceSOPInstanceInformation(dcmff.getDataset());
	}

	/* figure out which SOP class and SOP instance is encapsulated in the file */
	if (!DU_findSOPClassAndInstanceInDataSet(dcmff.getDataset(),
		sopClass, sopInstance, opt_correctUIDPadding)) {
			printf("No SOP Class & Instance UIDs in file: %s", fname);
			return OFFalse;
	}



	/* prepare the transmission of data */
	bzero((char*)&req, sizeof(req));
	req.MessageID = msgId;
	strcpy(req.AffectedSOPClassUID, sopClass);
	strcpy(req.AffectedSOPInstanceUID, sopInstance);
	req.DataSetType = DIMSE_DATASET_PRESENT;
	req.Priority = DIMSE_PRIORITY_LOW;

	/* if required, dump some more general information */
	if (opt_verbose) {
		printf("Store SCU RQ: MsgID %d, (%s)\n", msgId, dcmSOPClassUIDToModality(sopClass));
	}


	presId = ASC_findAcceptedPresentationContextID(ofAssoc,sopClass);

	if (presId == 0) {
		printf("No presentation context");
		//return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
		return OFFalse;
	}

	/* finally conduct transmission of data */
	cond = DIMSE_storeUser(ofAssoc, presId , &req,
		NULL, dcmff.getDataset(), progressCallback, NULL,
		DIMSE_BLOCKING, 0,
		&rsp, &statusDetail, NULL, DU_fileSize(fname));

	/*
	* If store command completed normally, with a status
	* of success or some warning then the image was accepted.
	*/
	if (cond == EC_Normal && (rsp.DimseStatus == STATUS_Success || DICOM_WARNING_STATUS(rsp.DimseStatus))) {
		unsuccessfulStoreEncountered = OFFalse;
	}

	/* remember the response's status for later transmissions of data */
	lastStatusCode = rsp.DimseStatus;

	/* dump some more general information */
	if (cond == EC_Normal)
	{
		if (opt_verbose) {
			DIMSE_printCStoreRSP(stdout, &rsp);
		}
	}
	else
	{
		printf("Store Failed, file: %s:", fname);
		DimseCondition::dump(cond);
	}

	/* dump status detail information if there is some */
	if (statusDetail != NULL) {
		printf("  Status Detail:\n");
		statusDetail->print(COUT);
		delete statusDetail;
	}
	/* return */
	return (cond == EC_Normal);

}

