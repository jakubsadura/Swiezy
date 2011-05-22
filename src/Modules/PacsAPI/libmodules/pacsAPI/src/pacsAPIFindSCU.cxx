/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// Verification connect
#include "pacsAPIFindSCU.h"


// Init static variables
OFBool PACS::FindSCU::opt_verbose = OFTrue;
OFBool PACS::FindSCU::opt_debug = OFFalse;
OFBool PACS::FindSCU::opt_abortAssociation = OFFalse;
OFCmdUnsignedInt PACS::FindSCU::opt_maxReceivePDULength = ASC_DEFAULTMAXPDU;
OFCmdUnsignedInt PACS::FindSCU::opt_repeatCount = 1;
OFBool PACS::FindSCU::opt_extractResponsesToFile = OFFalse;
const char * PACS::FindSCU::opt_abstractSyntax = UID_FINDPatientRootQueryRetrieveInformationModel/*UID_FINDModalityWorklistInformationModel*/;
OFCmdSignedInt PACS::FindSCU::opt_cancelAfterNResponses = -1;
DcmDataset* PACS::FindSCU::overrideKeys = NULL;
E_TransferSyntax PACS::FindSCU::opt_networkTransferSyntax = EXS_Unknown;
T_DIMSE_BlockingMode PACS::FindSCU::opt_blockMode = DIMSE_BLOCKING;
int PACS::FindSCU::opt_dimse_timeout = 0;
OFString *PACS::FindSCU::m_queryString=NULL;
unsigned int PACS::FindSCU::m_nQueryStrings=0;
OFString PACS::FindSCU::m_outTextFName="res.txt";
OFString PACS::FindSCU::m_queryRetriveLevel = "PATIENT";


/**
*/
PACS::FindSCU::FindSCU()
{
	opt_acse_timeout = 30;
	//opt_peer = "193.145.50.44"; // just the cdteam pacs for testing with something
	//opt_port = 1010;
	opt_peerTitle = PEERAPPLICATIONTITLE;
	opt_ourTitle = APPLICATIONTITLE;
	opt_secureConnection = OFFalse; /* default: no secure connection */
}

/**
*/
PACS::FindSCU::~FindSCU()
{
	m_nQueryStrings=0;
}



/**
*/
bool PACS::FindSCU::query(PACS::Association* assoc, const char *queryString[],
		unsigned int nQueryStrings)
{
	if (opt_debug)
	{
		DIMSE_debug(OFTrue);
		SetDebugLevel(3);
	}
	
	/*
	** Don't let dcmdata remove tailing blank padding or perform other
	** manipulations.  We want to see the real data.
	*/
	dcmEnableAutomaticInputDataCorrection.set(OFFalse);

	//const char *ovKey = NULL;
	for(unsigned int ii=0;ii<nQueryStrings;ii++)
		addOverrideKey(queryString[ii]);

	char qRLevelComplete[256];
	sprintf(qRLevelComplete,"0008,0052=%s",m_queryRetriveLevel.c_str());
	addOverrideKey(qRLevelComplete);


	saveCleanQueryStrings(queryString,nQueryStrings);

	/* make sure data dictionary is loaded */
	if (!dcmDataDict.isDictionaryLoaded()) {
		fprintf(stderr, "Warning: no data dictionary loaded, check environment variable: %s\n",
			DCM_DICT_ENVIRONMENT_VARIABLE);
	}



	/* do the real work, i.e. send a */
	/* C-FIND-RQ to the other DICOM application and receive corresponding response messages. */
	OFCondition cond = EC_Normal;
	T_ASC_Association *ofAssoc=assoc->getOFAssociation();
	
	cond = findSCU(ofAssoc, NULL);

	/* tear down association, i.e. terminate network connection to SCP */
	if (cond != EC_Normal)
	{
		printf("SCU Failed:");
		DimseCondition::dump(cond);
		return OFFalse;
	}

	delete overrideKeys;
	overrideKeys=NULL;  //now we are ready for a new query!
	return true;
}



/**
*/
OFCondition PACS::FindSCU::findSCU( T_ASC_Association * assoc, const char *fname )
{
	DIC_US msgId = assoc->nextMsgID++;
	T_ASC_PresentationContextID presId;
	T_DIMSE_C_FindRQ req;
	T_DIMSE_C_FindRSP rsp;
	DcmDataset *statusDetail = NULL;
	MyCallbackInfo callbackData;
	DcmFileFormat dcmff;

	/* if there is a valid filename */
	if (fname != NULL) {

		/* read information from file (this information specifies a search mask). After the */
		/* call to DcmFileFormat::read(...) the information which is encapsulated in the file */
		/* will be available through the DcmFileFormat object. In detail, it will be available */
		/* through calls to DcmFileFormat::getMetaInfo() (for meta header information) and */
		/* DcmFileFormat::getDataset() (for data set information). */
		OFCondition cond = dcmff.loadFile(fname);

		/* figure out if an error occurred while the file was read*/
		if (cond.bad()) {
			printf("Bad DICOM file: %s: %s", fname, cond.text());
			return cond;
		}
	}

	/* replace specific keys by those in overrideKeys */
	substituteOverrideKeys(dcmff.getDataset());
	
	/* figure out which of the accepted presentation contexts should be used */
	for(int ii=0;ii<nFindInfoModels;ii++)
	{    
		presId = ASC_findAcceptedPresentationContextID(assoc, findInfoModels[ii]);
		if(presId != 0) {
			opt_abstractSyntax=findInfoModels[ii];
			break;
		}
	}

		
	if (presId == 0) {
		printf("No presentation context");
		return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
	}

	/* prepare the transmission of data */
	bzero((char*)&req, sizeof(req));
	req.MessageID = msgId;
	strcpy(req.AffectedSOPClassUID, opt_abstractSyntax);
	req.DataSetType = DIMSE_DATASET_PRESENT;
	req.Priority = DIMSE_PRIORITY_LOW;

	/* prepare the callback data */
	callbackData.assoc = assoc;
	callbackData.presId = presId;

	/* if required, dump some more general information */
	if (opt_verbose) {
		printf("Find SCU RQ: MsgID %d\n", msgId);
		printf("REQUEST:\n");
		dcmff.getDataset()->print(COUT);
		printf("--------\n");
	}

	/* finally conduct transmission of data */
	OFCondition cond = DIMSE_findUser(assoc, presId, &req, dcmff.getDataset(),
		progressCallback, &callbackData,
		opt_blockMode, opt_dimse_timeout,
		&rsp, &statusDetail);
	

	/* dump some more general information */
	if (cond == EC_Normal) {
		if (opt_verbose) {
			DIMSE_printCFindRSP(stdout, &rsp);
		} else {
			if (rsp.DimseStatus != STATUS_Success) {
				printf("Response: %s\n", DU_cfindStatusString(rsp.DimseStatus));
			}
		}
	} else {
		if (fname) {
			printf("Find Failed, file: %s:", fname);
		} else {
			printf("Find Failed, query keys:");
			dcmff.getDataset()->print(COUT);
		}
		DimseCondition::dump(cond);
	}

	/* dump status detail information if there is some */
	if (statusDetail != NULL) {
		printf("  Status Detail:\n");
		statusDetail->print(COUT);
		delete statusDetail;
	}

	/* return */
	return cond;
}

/**
*/
void PACS::FindSCU::progressCallback( void *callbackData, T_DIMSE_C_FindRQ *request, int responseCount, 
									 T_DIMSE_C_FindRSP *rsp, DcmDataset *responseIdentifiers )
{
	/* dump response number */
	printf("RESPONSE: %d (%s)\n", responseCount,
		DU_cfindStatusString(rsp->DimseStatus));

	/* dump data set which was received */
	responseIdentifiers->print(COUT);


	/* dump delimiter */
	printf("--------\n");



	/* in case opt_extractResponsesToFile is set the responses shall be extracted to a certain file */
	if (opt_extractResponsesToFile) {
		char rspIdsFileName[1024];
		sprintf(rspIdsFileName, "rsp%04d.dcm", responseCount);
		PACS::writeToFile(rspIdsFileName, responseIdentifiers);
	}

	MyCallbackInfo *myCallbackData = OFstatic_cast(MyCallbackInfo *, callbackData);
	PACS::addToTextFile(m_outTextFName.c_str(), responseIdentifiers, m_queryString,m_nQueryStrings);


	/* should we send a cancel back ?? */
	if (opt_cancelAfterNResponses == responseCount)
	{
		if (opt_verbose)
		{
			printf("Sending Cancel RQ, MsgId: %d, PresId: %d\n", request->MessageID, myCallbackData->presId);
		}
		OFCondition cond = DIMSE_sendCancelRequest(myCallbackData->assoc, myCallbackData->presId, request->MessageID);
		if (cond.bad())
		{
			printf("Cancel RQ Failed:");
			DimseCondition::dump(cond);
		}
	}
}



OFBool PACS::FindSCU::addOverrideKey(const char* s )
{
	if(!PACS::addKeyToDataset(s,&overrideKeys))
		return OFFalse;

	return OFTrue;
}

OFBool PACS::FindSCU::substituteOverrideKeys(DcmDataset *dset)
{
	if(!PACS::substituteKeysInDataset(dset,overrideKeys))
		return OFFalse;

	return OFTrue;
}


void PACS::FindSCU::saveCleanQueryStrings(const char **queryString, unsigned int nQueryStrings)
{
	//each queryString[i] must be in the format ffff,gggg=value or ffff,gggg (with no value speficied)
	m_nQueryStrings=nQueryStrings;
	m_queryString = new OFString [nQueryStrings];

	for(unsigned int ii=0;ii<m_nQueryStrings;ii++)
	{
		m_queryString[ii]=queryString[ii];
		
		//now remove the "=value" from the string, if present
		unsigned int found=m_queryString[ii].find_first_of("=");
		if(found!=string::npos)
		  m_queryString[ii].resize(found);

	}
}


void PACS::FindSCU::setQueryRetriveLevel(const char *levelString)
{
	if (PACS::checkCorrectQueryRetriveLevel(levelString))
		m_queryRetriveLevel="PATIENT";
	else
		m_queryRetriveLevel=levelString;
}

void PACS::FindSCU::setOutTxtFile(const char *outTxtFName)
{
	m_outTextFName=outTxtFName;
}


