/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "pacsAPIMoveSCU.h"



// Init static variables
OFBool PACS::MoveSCU::opt_verbose = OFTrue;
OFBool PACS::MoveSCU::opt_debug = OFFalse;
OFBool PACS::MoveSCU::opt_abortAssociation = OFFalse;
OFCmdUnsignedInt PACS::MoveSCU::opt_maxReceivePDULength = ASC_DEFAULTMAXPDU;
OFCmdUnsignedInt PACS::MoveSCU::opt_repeatCount = 1;
OFBool PACS::MoveSCU::opt_extractResponsesToFile = OFTrue;
const char * PACS::MoveSCU::opt_abstractSyntax = UID_MOVEPatientRootQueryRetrieveInformationModel/*UID_FINDModalityWorklistInformationModel*/;
OFCmdSignedInt PACS::MoveSCU::opt_cancelAfterNResponses = -1;
DcmDataset* PACS::MoveSCU::overrideKeys = NULL;
E_TransferSyntax PACS::MoveSCU::opt_networkTransferSyntax = EXS_Unknown;
T_DIMSE_BlockingMode PACS::MoveSCU::opt_blockMode = DIMSE_BLOCKING;
int PACS::MoveSCU::opt_dimse_timeout = 0;
OFBool PACS::MoveSCU::opt_ignorePendingDatasets=OFTrue;
E_TransferSyntax  PACS::MoveSCU::opt_in_networkTransferSyntax = EXS_Unknown ;
E_TransferSyntax  PACS::MoveSCU::opt_out_networkTransferSyntax = EXS_Unknown;
OFString PACS::MoveSCU::m_queryRetriveLevel = "PATIENT";



OFCmdUnsignedInt  PACS::MoveSCU::opt_sleepAfter = 0;
OFCmdUnsignedInt  PACS::MoveSCU::opt_sleepDuring = 0;
OFCmdUnsignedInt  PACS::MoveSCU::opt_maxPDU = ASC_DEFAULTMAXPDU;
OFBool            PACS::MoveSCU::opt_useMetaheader = OFTrue;
E_TransferSyntax  PACS::MoveSCU::opt_writeTransferSyntax = EXS_Unknown;
E_GrpLenEncoding  PACS::MoveSCU::opt_groupLength = EGL_recalcGL;
E_EncodingType    PACS::MoveSCU::opt_sequenceType = EET_ExplicitLength;
E_PaddingEncoding PACS::MoveSCU::opt_paddingType = EPD_withoutPadding;
OFCmdUnsignedInt  PACS::MoveSCU::opt_filepad = 0;
OFCmdUnsignedInt  PACS::MoveSCU::opt_itempad = 0;
OFBool            PACS::MoveSCU::opt_bitPreserving = OFFalse;
OFBool            PACS::MoveSCU::opt_ignore = OFFalse;
OFBool            PACS::MoveSCU::opt_abortDuringStore = OFFalse;
OFBool            PACS::MoveSCU::opt_abortAfterStore = OFFalse;
OFBool            PACS::MoveSCU::opt_correctUIDPadding = OFFalse;
OFCmdUnsignedInt  PACS::MoveSCU::opt_retrievePort = 0;
const char *      PACS::MoveSCU::opt_moveDestination = NULL;
int               PACS::MoveSCU::opt_acse_timeout = 30;
char	 		  PACS::MoveSCU::outputStorageFolder[300] = "./"; //current folder of the application



PACS::MoveSCU::MoveSCU()
{
}


OFBool PACS::MoveSCU::addOverrideKey(const char* s)
{
	if(!PACS::addKeyToDataset(s,&overrideKeys))
		return OFFalse;

	return OFTrue;
}

OFBool PACS::MoveSCU::substituteOverrideKeys(DcmDataset *dset)
{
	if(!PACS::substituteKeysInDataset(dset,overrideKeys))
		return OFFalse;

	return OFTrue;
}



void PACS::MoveSCU::moveCallback(void *callbackData, T_DIMSE_C_MoveRQ *request,
			 int responseCount, T_DIMSE_C_MoveRSP *response)
{
	OFCondition cond = EC_Normal;
	MyCallbackInfo *myCallbackData;

	myCallbackData = (MyCallbackInfo*)callbackData;


	/* should we send a cancel back ?? */
	if (opt_cancelAfterNResponses == responseCount) {
		if (opt_verbose) {
			printf("Sending Cancel RQ, MsgId: %d, PresId: %d\n",
				request->MessageID, myCallbackData->presId);
		}
		cond = DIMSE_sendCancelRequest(myCallbackData->assoc,
			myCallbackData->presId, request->MessageID);
		if (cond != EC_Normal) {
			printf("Cancel RQ Failed:");
			DimseCondition::dump(cond);
		}
	}
}


void PACS::MoveSCU::setQueryRetriveLevel(const char *levelString)
{
	if (PACS::checkCorrectQueryRetriveLevel(levelString))
		m_queryRetriveLevel="PATIENT";
	else
		m_queryRetriveLevel=levelString;

}

void PACS::MoveSCU::setOutputStorageFolder(const char *outputFolder)
{
	strcpy(MoveSCU::outputStorageFolder,outputFolder);
}

OFBool PACS::MoveSCU::move(PACS::Association* assoc, const char *queryString[],
								unsigned int nQueryStrings, unsigned long retriveport)
{
	
	T_ASC_PresentationContextID presId;
	T_DIMSE_C_MoveRQ    req;
	T_DIMSE_C_MoveRSP   rsp;
	T_ASC_Association *ofAssoc=assoc->getOFAssociation();
	T_ASC_Network *ofNet=assoc->getOFNetwork();
	DIC_US              msgId = ofAssoc->nextMsgID++;
	DcmDataset          *rspIds = NULL;
	DcmDataset          *statusDetail = NULL;
	MyCallbackInfo      callbackData;

	DcmFileFormat dcmff;

	for(unsigned int ii=0;ii<nQueryStrings;ii++)
		addOverrideKey(queryString[ii]); 

	char qRLevelComplete[256];
	sprintf(qRLevelComplete,"0008,0052=%s",m_queryRetriveLevel.c_str());
	addOverrideKey(qRLevelComplete);

	/* replace specific keys by those in overrideKeys */
	substituteOverrideKeys(dcmff.getDataset());

	/* figure out which of the accepted presentation contexts should be used */
	for(unsigned int ii=0;ii<nQueryInfoModels;ii++)
	{    
		presId = ASC_findAcceptedPresentationContextID(ofAssoc, queryInfoModels[ii]);
		if(presId != 0) {
			opt_abstractSyntax=queryInfoModels[ii];
			break;
		}
	}

	if (presId == 0) {
		printf("No presentation context");
		//return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
		return OFFalse;
	}

	if (opt_verbose) {
        printf("Move SCU RQ: MsgID %d\n", msgId);
        printf("Request:\n");
        dcmff.getDataset()->print(COUT);
    }

	/* prepare the transmission of data */
	bzero((char*)&req, sizeof(req));
	req.MessageID = msgId;
	strcpy(req.AffectedSOPClassUID, opt_abstractSyntax);
	req.DataSetType = DIMSE_DATASET_PRESENT;
	req.Priority = DIMSE_PRIORITY_LOW;

	/* prepare the callback data */
	callbackData.assoc = ofAssoc;
	callbackData.presId = presId;

    if (opt_moveDestination == NULL) {
        /* set the destination to be me */
        ASC_getAPTitles(ofAssoc->params, req.MoveDestination,
            NULL, NULL);
    } else {
        strcpy(req.MoveDestination, opt_moveDestination);
    }

	opt_retrievePort=retriveport;

	/* if required, dump some more general information */
	if (opt_verbose) {
		printf("Move SCU RQ: MsgID %d\n", msgId);
		printf("REQUEST:\n");
		dcmff.getDataset()->print(COUT);
		printf("--------\n");
	}

	/* finally conduct transmission of data */
	OFCondition cond = DIMSE_moveUser(ofAssoc, presId, &req, dcmff.getDataset(),
		moveCallback, &callbackData, opt_blockMode, opt_dimse_timeout,ofNet,
		subOpCallback, NULL,
		&rsp, &statusDetail, &rspIds, opt_ignorePendingDatasets);


    if (cond == EC_Normal) {
        if (opt_verbose) {
            DIMSE_printCMoveRSP(stdout, &rsp);
            if (rspIds != NULL) {
                printf("Response Identifiers:\n");
                rspIds->print(COUT);
            }
        }
    } else {
        printf("Move Failed:");
        DimseCondition::dump(cond);
    }

	if (statusDetail != NULL) {
		printf("  Status Detail:\n");
		statusDetail->print(COUT);
		delete statusDetail;
	}

	if (rspIds != NULL) delete rspIds;

	delete overrideKeys;
	overrideKeys=NULL;  //now we are ready for a new query!
	//return cond;
	return OFTrue;
}


OFCondition PACS::MoveSCU::acceptSubAssoc(T_ASC_Network * aNet, T_ASC_Association ** assoc)
{
    const char* knownAbstractSyntaxes[] = {
        UID_VerificationSOPClass
    };
    const char* transferSyntaxes[] = { NULL, NULL, NULL, NULL };
    int numTransferSyntaxes;

    OFCondition cond = ASC_receiveAssociation(aNet, assoc, opt_maxPDU);
	if (cond.good())
    {
      switch (opt_in_networkTransferSyntax)
      {
        case EXS_LittleEndianImplicit:
          /* we only support Little Endian Implicit */
          transferSyntaxes[0]  = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 1;
          break;
        case EXS_LittleEndianExplicit:
          /* we prefer Little Endian Explicit */
          transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[2]  = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 3;
          break;
        case EXS_BigEndianExplicit:
          /* we prefer Big Endian Explicit */
          transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2]  = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 3;
          break;
        case EXS_JPEGProcess14SV1TransferSyntax:
          /* we prefer JPEGLossless:Hierarchical-1stOrderPrediction (default lossless) */
          transferSyntaxes[0] = UID_JPEGProcess14SV1TransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 4;
          break;
        case EXS_JPEGProcess1TransferSyntax:
          /* we prefer JPEGBaseline (default lossy for 8 bit images) */
          transferSyntaxes[0] = UID_JPEGProcess1TransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 4;
          break;
        case EXS_JPEGProcess2_4TransferSyntax:
          /* we prefer JPEGExtended (default lossy for 12 bit images) */
          transferSyntaxes[0] = UID_JPEGProcess2_4TransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 4;
          break;
        case EXS_JPEG2000:
          /* we prefer JPEG2000 Lossy */
          transferSyntaxes[0] = UID_JPEG2000TransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 4;
          break;
        case EXS_JPEG2000LosslessOnly:
          /* we prefer JPEG2000 Lossless */
          transferSyntaxes[0] = UID_JPEG2000LosslessOnlyTransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 4;
          break;
        case EXS_RLELossless:
          /* we prefer RLE Lossless */
          transferSyntaxes[0] = UID_RLELosslessTransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 4;
          break;
        default:
          /* We prefer explicit transfer syntaxes.
           * If we are running on a Little Endian machine we prefer
           * LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
           */
          if (gLocalByteOrder == EBO_LittleEndian)  /* defined in dcxfer.h */
          {
            transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
            transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
          } else {
            transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
            transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
          }
          transferSyntaxes[2] = UID_LittleEndianImplicitTransferSyntax;
          numTransferSyntaxes = 3;
          break;

        }

        /* accept the Verification SOP Class if presented */
        cond = ASC_acceptContextsWithPreferredTransferSyntaxes(
            (*assoc)->params,
            knownAbstractSyntaxes, DIM_OF(knownAbstractSyntaxes),
            transferSyntaxes, numTransferSyntaxes);

        if (cond.good())
        {
            /* the array of Storage SOP Class UIDs comes from dcuid.h */
            cond = ASC_acceptContextsWithPreferredTransferSyntaxes(
                (*assoc)->params,
                dcmAllStorageSOPClassUIDs, numberOfAllDcmStorageSOPClassUIDs,
                transferSyntaxes, numTransferSyntaxes);
        }
    }
    if (cond.good()) cond = ASC_acknowledgeAssociation(*assoc);
    if (cond.bad()) {
        ASC_dropAssociation(*assoc);
        ASC_destroyAssociation(assoc);
    }
    return cond;
}

OFCondition PACS::MoveSCU::echoSCP(
  T_ASC_Association * assoc,
  T_DIMSE_Message * msg,
  T_ASC_PresentationContextID presID)
{
  if (opt_verbose)
  {
    printf("Received ");
    DIMSE_printCEchoRQ(stdout, &msg->msg.CEchoRQ);
  }

  /* the echo succeeded !! */
  OFCondition cond = DIMSE_sendEchoResponse(assoc, presID, &msg->msg.CEchoRQ, STATUS_Success, NULL);
  if (cond.bad())
  {
    fprintf(stderr, "storescp: Echo SCP Failed:\n");
    DimseCondition::dump(cond);
  }
  return cond;
}

struct StoreCallbackData
{
  char* imageFileName;
  DcmFileFormat* dcmff;
  T_ASC_Association* assoc;
};


void PACS::MoveSCU::storeSCPCallback(
    /* in */
    void *callbackData,
    T_DIMSE_StoreProgress *progress,    /* progress state */
    T_DIMSE_C_StoreRQ *req,             /* original store request */
    char *imageFileName, DcmDataset **imageDataSet, /* being received into */
    /* out */
    T_DIMSE_C_StoreRSP *rsp,            /* final store response */
    DcmDataset **statusDetail)
{
    DIC_UI sopClass;
    DIC_UI sopInstance;

    if ((opt_abortDuringStore && progress->state != DIMSE_StoreBegin) ||
        (opt_abortAfterStore && progress->state == DIMSE_StoreEnd)) {
        if (opt_verbose) {
            printf("ABORT initiated (due to command line options)\n");
        }
        ASC_abortAssociation(((StoreCallbackData*) callbackData)->assoc);
        rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
        return;
    }

    if (opt_sleepDuring > 0)
    {
      OFStandard::sleep((unsigned int)opt_sleepDuring);
    }

    if (opt_verbose)
    {
      switch (progress->state)
      {
        case DIMSE_StoreBegin:
          printf("RECV:");
          break;
        case DIMSE_StoreEnd:
          printf("\n");
          break;
        default:
          putchar('.');
          break;
      }
      fflush(stdout);
    }

    if (progress->state == DIMSE_StoreEnd)
    {
       *statusDetail = NULL;    /* no status detail */

       /* could save the image somewhere else, put it in database, etc */
       /*
        * An appropriate status code is already set in the resp structure, it need not be success.
        * For example, if the caller has already detected an out of resources problem then the
        * status will reflect this.  The callback function is still called to allow cleanup.
        */
       // rsp->DimseStatus = STATUS_Success;

       if ((imageDataSet)&&(*imageDataSet)&&(!opt_bitPreserving)&&(!opt_ignore))
       {
         StoreCallbackData *cbdata = (StoreCallbackData*) callbackData;
         const char* fileName = cbdata->imageFileName;

         E_TransferSyntax xfer = opt_writeTransferSyntax;
         if (xfer == EXS_Unknown) xfer = (*imageDataSet)->getOriginalXfer();

         OFCondition cond = cbdata->dcmff->saveFile(fileName, xfer, opt_sequenceType, opt_groupLength,
           opt_paddingType, (Uint32)opt_filepad, (Uint32)opt_itempad, !opt_useMetaheader);
         if (cond.bad())
         {
           fprintf(stderr, "storescp: Cannot write image file: %s\n", fileName);
           rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
         }

        /* should really check the image to make sure it is consistent,
         * that its sopClass and sopInstance correspond with those in
         * the request.
         */
        if ((rsp->DimseStatus == STATUS_Success)&&(!opt_ignore))
        {
          /* which SOP class and SOP instance ? */
          if (! DU_findSOPClassAndInstanceInDataSet(*imageDataSet, sopClass, sopInstance, opt_correctUIDPadding))
          {
             fprintf(stderr, "storescp: Bad image file: %s\n", imageFileName);
             rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
          }
          else if (strcmp(sopClass, req->AffectedSOPClassUID) != 0)
          {
            rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
          }
          else if (strcmp(sopInstance, req->AffectedSOPInstanceUID) != 0)
          {
            rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
          }
        }
      }
    }
    return;
}

OFCondition PACS::MoveSCU::storeSCP(
  T_ASC_Association *assoc,
  T_DIMSE_Message *msg,
  T_ASC_PresentationContextID presID)
{
    OFCondition cond = EC_Normal;
    T_DIMSE_C_StoreRQ *req;
    char imageFileName[2048];

    req = &msg->msg.CStoreRQ;

    if (opt_ignore)
    {
#ifdef _WIN32
        tmpnam(imageFileName);
#else
        strcpy(imageFileName, NULL_DEVICE_NAME);
#endif
    } else {
        sprintf(imageFileName, "%s.%s",
            dcmSOPClassUIDToModality(req->AffectedSOPClassUID),
            req->AffectedSOPInstanceUID);
    }

	//add the path to the image
	char fullImageFileName[2048];
	strcpy(fullImageFileName,outputStorageFolder);
	strcat(fullImageFileName,imageFileName);

	if (opt_verbose)
    {
      printf("Received ");
      DIMSE_printCStoreRQ(stdout, req);
    }

    StoreCallbackData callbackData;
    callbackData.assoc = assoc;
    //callbackData.imageFileName = imageFileName;
	callbackData.imageFileName = fullImageFileName;
    DcmFileFormat dcmff;
    callbackData.dcmff = &dcmff;

    DcmDataset *dset = dcmff.getDataset();

    if (opt_bitPreserving)
    {
      cond = DIMSE_storeProvider(assoc, presID, req, imageFileName, opt_useMetaheader,
        NULL, storeSCPCallback, (void*)&callbackData, opt_blockMode, opt_dimse_timeout);
    } else {
      cond = DIMSE_storeProvider(assoc, presID, req, (char *)NULL, opt_useMetaheader,
        &dset, storeSCPCallback, (void*)&callbackData, opt_blockMode, opt_dimse_timeout);
    }

    if (cond.bad())
    {
      fprintf(stderr, "storescp: Store SCP Failed:\n");
      DimseCondition::dump(cond);
      /* remove file */
      if (!opt_ignore)
      {
        if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0) unlink(imageFileName);
      }
#ifdef _WIN32
    } else if (opt_ignore) {
        if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0) unlink(imageFileName); // delete the temporary file
#endif
    }

    if (opt_sleepAfter > 0)
    {
      OFStandard::sleep((unsigned int)opt_sleepAfter);
    }
    return cond;
}

OFCondition
PACS::MoveSCU::subOpSCP(T_ASC_Association **subAssoc)
{
    T_DIMSE_Message     msg;
    T_ASC_PresentationContextID presID;

    if (!ASC_dataWaiting(*subAssoc, 0)) /* just in case */
        return DIMSE_NODATAAVAILABLE;

    OFCondition cond = DIMSE_receiveCommand(*subAssoc, opt_blockMode, opt_dimse_timeout, &presID,
            &msg, NULL);

    if (cond == EC_Normal) {
        switch (msg.CommandField) {
        case DIMSE_C_STORE_RQ:
            cond = storeSCP(*subAssoc, &msg, presID);
            break;
        case DIMSE_C_ECHO_RQ:
            cond = echoSCP(*subAssoc, &msg, presID);
            break;
        default:
            cond = DIMSE_BADCOMMANDTYPE;
            break;
        }
    }
    /* clean up on association termination */
    if (cond == DUL_PEERREQUESTEDRELEASE)
    {
        cond = ASC_acknowledgeRelease(*subAssoc);
        ASC_dropSCPAssociation(*subAssoc);
        ASC_destroyAssociation(subAssoc);
        return cond;
    }
    else if (cond == DUL_PEERABORTEDASSOCIATION)
    {
    }
    else if (cond != EC_Normal)
    {
        printf("DIMSE Failure (aborting sub-association):\n");
        DimseCondition::dump(cond);
        /* some kind of error so abort the association */
        cond = ASC_abortAssociation(*subAssoc);
    }

    if (cond != EC_Normal)
    {
        ASC_dropAssociation(*subAssoc);
        ASC_destroyAssociation(subAssoc);
    }
    return cond;
}

void PACS::MoveSCU::subOpCallback(void * /*subOpCallbackData*/ ,
        T_ASC_Network *aNet, T_ASC_Association **subAssoc)
{

    if (aNet == NULL) return;   /* help no net ! */

    if (*subAssoc == NULL) {
        /* negotiate association */
        acceptSubAssoc(aNet, subAssoc);
    } else {
        /* be a service class provider */
        subOpSCP(subAssoc);
    }
}
