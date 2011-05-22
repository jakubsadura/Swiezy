/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "pacsAPIDimse.h"

OFBool PACS::Association::initAssociation(int maxReceivePDULength)
{
	/* initialize association parameters, i.e. create an instance of T_ASC_Parameters*. */
	//Add the only info required for the User Information Model
	OFCondition cond = ASC_createAssociationParameters(&m_Params, maxReceivePDULength);  
	if (cond.bad()) {
		DimseCondition::dump(cond);
		return OFFalse;
	}
	associationStatus=ASS_NOTCONNECTED;
	return OFTrue;
}

OFBool PACS::Association::setApplicationCtx(char *applicationContextName)
{

	if (strlen(applicationContextName)>DUL_LEN_NAME)
		return OFFalse;

	strcpy(m_Params->DULparams.applicationContextName,applicationContextName);
	return OFTrue;
}

OFBool PACS::Association::addPresentationCtx(char *abstractSyntax, const char *transferSyntaxes[], int nTransferSyntaxes)
{
	
	int presentationContextID = 2*(m_nPresenations+1)-1; /* it is the next proposed presentationCtx ID and must be an  
												   odd progressive byte value 1, 3, 5, .. 255 */
			
	const char *transfSyntaxDefault[] = {UID_LittleEndianImplicitTransferSyntax};
	
	OFCondition cond; //the result condition
	if((transferSyntaxes==TRANSFSYNTAXDEFAULT) || (nTransferSyntaxes==0))
		cond = ASC_addPresentationContext(m_Params, presentationContextID, abstractSyntax,
			transfSyntaxDefault, 1);
	else
		cond = ASC_addPresentationContext(m_Params, presentationContextID, abstractSyntax,
			transferSyntaxes, nTransferSyntaxes);
	if (cond.bad())
	{
		DimseCondition::dump(cond);
		return OFFalse;
	}
	m_nPresenations++;
	return OFTrue;
}


OFBool PACS::Association::setAETitles(const char *callerAETitle, const char *receiverAETitle)
{
	if ((strlen(callerAETitle)>DUL_LEN_TITLE) || (strlen(receiverAETitle)>DUL_LEN_TITLE))
		return OFFalse;

	/* sets this application's title and the called application's title in the m_Params */
	/* structure. The default values to be set here are "ANYESCU" and "ANY-SCP". */
	ASC_setAPTitles(m_Params, callerAETitle, receiverAETitle, NULL);
	return OFTrue;
}


//Init and start a network connection with the peer entity (on specified TCP port)
//If secureTransLayer and OpenSSL is available, a secure connection is started
OFBool PACS::Association::networkConnect(const char *peer,unsigned long peerPort,int timeout, 
										T_ASC_NetworkRole netRole,unsigned long acceptorPort,
										OFBool secureTransLayer)
{
	this->m_Opt_port=peerPort;
	strcpy(this->m_Opt_peer,peer);
	
	#ifdef HAVE_GUSI_H
		/* needed for Macintosh */
		GUSISetup(GUSIwithSIOUXSockets);
		GUSISetup(GUSIwithInternetSockets);
	#endif

	#ifdef HAVE_WINSOCK_H
		WSAData winSockData;
		/* we need at least version 1.1 */
		WORD winSockVersionNeeded = MAKEWORD( 1, 1 );
		WSAStartup(winSockVersionNeeded, &winSockData);
	#endif

	/* make sure data dictionary is loaded */
	if (!dcmDataDict.isDictionaryLoaded()) {
		fprintf(stderr, "Warning: no data dictionary loaded, check environment variable: %s\n",
			DCM_DICT_ENVIRONMENT_VARIABLE);
	}	
	
	// initialize network, i.e. create an instance of T_ASC_Network*. 
	OFCondition cond = ASC_initializeNetwork(netRole, acceptorPort, timeout, &m_Net);
	if (cond.bad()) {
		DimseCondition::dump(cond);
		return OFFalse;
	}

	// Set the transport layer type (type of network connection) in the m_Params 
	// structure. The default is an insecure connection; where OpenSSL is  
	// available the user is able to request an encrypted,secure connection.
	cond = ASC_setTransportLayerType(m_Params, secureTransLayer);
	if (cond.bad()) {
		DimseCondition::dump(cond);
		return OFFalse;
	}

	// Figure out the presentation addresses and copy the 
	// corresponding values into the association parameters.
	gethostname(m_LocalHost, sizeof(m_LocalHost) - 1);
	sprintf(m_PeerHost, "%s:%d", peer, (int) peerPort);
	ASC_setPresentationAddresses(m_Params, m_LocalHost, m_PeerHost);
	
	associationStatus=ASS_NOTASSOCIATED;
	return OFTrue;

}


OFBool PACS::Association::associate()
{
	// Print Parameters
	printf("\nRequest Parameters:\n");
	ASC_dumpParameters(m_Params, COUT);

	// create association, i.e. try to establish a network connection to another 
	// DICOM application. This call creates an instance of T_ASC_Association*. 
	printf("\nRequesting Association...\n");
	OFCondition cond = ASC_requestAssociation(m_Net, m_Params, &m_Assoc);
	
	if (cond.bad()) {
		if (cond == DUL_ASSOCIATIONREJECTED)
		{
			T_ASC_RejectParameters rej;

			ASC_getRejectParameters(m_Params, &rej);
			printf("Association Rejected: ");
			ASC_printRejectParameters(stderr, &rej);
			return OFFalse;
		} else {
			printf("Association Request Failed: ");
			DimseCondition::dump(cond);
			return OFFalse;
		}
	}

	// dump the presentation contexts which have been accepted/refused
	printf("Association Parameters Negotiated:\n");
	ASC_dumpParameters(m_Params, COUT);

	// count the presentation contexts which have been accepted by the SCP
	// If there are none, finish the execution
	if (ASC_countAcceptedPresentationContexts(m_Params) == 0) {
		printf("No Acceptable Presentation Contexts");
		return OFFalse;
	}

	// dump general information concerning the establishment of the network connection if required 
	printf("Association Accepted (Max Send PDV: %lu)\n",
		m_Assoc->sendPDVLength);
	
	associationStatus=ASS_ASSOCIATED;
	return OFTrue;
}

//Release the association (or abort it) should be called with release if the association has already been
//closed by the peer or for any other error occurred
OFBool PACS::Association::close(OFBool abortAssociation)
{
	// tear down association, i.e. terminate network connection to SCP
	OFCondition cond;	
	if (abortAssociation) {
		printf("Aborting Association\n");
		cond = ASC_abortAssociation(m_Assoc);
		if (cond.bad())
		{
			printf("Association Abort Failed:");
			DimseCondition::dump(cond);
			return OFFalse;
		}
	} else {
		// release association 
		printf("Releasing Association\n");
		cond = ASC_releaseAssociation(m_Assoc);
		if (cond.bad())
		{
			printf("Association Release Failed:");
			DimseCondition::dump(cond);
			return OFFalse;
		}
	}

	// destroy the association, i.e. free memory of T_ASC_Association* structure. This 
	// call is the counterpart of ASC_requestAssociation(...) which was called above.
	cond = ASC_destroyAssociation(&m_Assoc);
	if (cond.bad()) {
		DimseCondition::dump(cond);
		return OFFalse;
	}

	associationStatus=ASS_NOTINITIALIZED;

	// drop the network, i.e. free memory of T_ASC_Network* structure. This call
	// is the counterpart of ASC_initializeNetwork(...) which was called above. 
	cond = ASC_dropNetwork(&m_Net);
	if (cond.bad()) {
		DimseCondition::dump(cond);
		return OFFalse;
	}
	return OFTrue;
}

PACS::Association::Association()
{
	associationStatus=ASS_NOTINITIALIZED;
}

PACS::Association::~Association()
{

}

//Other Dimse generic functions (cross services functions) defined in the PACS namespace

//Write a specified Dataset in a dicom file (name of the file speficied in ofname)
OFBool PACS::writeToFile( const char* ofname, DcmDataset *dataset )
{
	/* write out as a file format */

	DcmFileFormat fileformat(dataset); // copies dataset
	OFCondition ec = fileformat.error();
	if (ec.bad()) {
		printf("error writing file: %s: %s", ofname, ec.text());
		return OFFalse;
	}

	ec = fileformat.saveFile(ofname, dataset->getOriginalXfer());
	if (ec.bad()) {
		printf("error writing file: %s: %s", ofname, ec.text());
		return OFFalse;
	}

	return OFTrue;
}


//Add a key to the specified dataset; doesn't check if the key is already present!
//Input:
//	s (the key: tags and value); i.e. a sting in the format ffff,gggg[=value] 
//	(the =value part is optional; if none is specified, the key is a "return" key only)
//	datasetPtr the address of a dcmDataset pointer where the key will be added
//
//Output:
//	an OFBool value representing the success of the operation (OFTrue=Success)
//If the tag (ffff,gggg) is not badly formatted or it is not found in the current dictionary, 
//the function fails (return OFFalse)
OFBool PACS::addKeyToDataset(const char *s, DcmDataset **datasetPtr)
{
	unsigned int g = 0xffff;
	unsigned int e = 0xffff;
	int n = 0;
	OFString valStr;
	char val[1024];
	OFString msg;
	val[0] = '\0';

	//// first of all try to parse group, element number and value from the s string
	n = sscanf(s, "%x,%x=%s", &g, &e, val);
	OFString toParse = s;
	size_t eqPos = toParse.find('=');
	
	if (n < 2)  // tag could not be parsed
	{
		printf("bad tag format");
		return OFFalse;	
	}
	
	if (eqPos != OFString_npos)
		valStr = toParse.substr(eqPos+1,toParse.length());

	DcmTag tag(g,e);
	if (tag.error() != EC_Normal) {
		printf("unknown tag: (%04x,%04x)", g, e);
		return OFFalse;
	}
	
	DcmElement *elem = newDicomElement(tag);
	if (elem == NULL) {
		printf("cannot create element for tag: (%04x,%04x)", g, e);
		return OFFalse;
	}

	if (valStr.length() > 0) {
		if (elem->putString(valStr.c_str()).bad())
		{
			printf("cannot put tag value: (%04x,%04x)=\"", g, e);
			return OFFalse;
		}
	}

	if ((*datasetPtr) == NULL) (*datasetPtr) = new DcmDataset;
	if ((*datasetPtr)->insert(elem, OFTrue).bad())
	{
		printf("cannot insert tag: (%04x,%04x)", g, e);
		return OFFalse;
	}
	return OFTrue;
}

//substitute the keys present in dset with the one in the overrideKeys dataset
OFBool PACS::substituteKeysInDataset(DcmDataset *dset, DcmDataset *overrideKeys)
{
	if (overrideKeys == NULL) {
		return OFTrue; /* nothing to do */
	}

	/* copy the override keys */
	DcmDataset keys(*overrideKeys);

	/* put the override keys into dset replacing existing tags */
	unsigned long elemCount = keys.card();
	for (unsigned long i=0; i<elemCount; i++) {
		DcmElement *elem = keys.remove((unsigned long)0);

		dset->insert(elem, OFTrue);
	}
	return OFTrue;
}

//Given a keyTag (in the ffff,gggg format) and a dataset pointer, returns a string
//containing the value of the element (should be used only for string element tags)
//Input:
//	keyTag: a string contanining the tag of the key in the format gggg,ffff
//	datasetPtr: a pointer to the dataset that should contain the specified key
//
//Output:
//	stringOut: a string containing the value of the key. stringOut must be allocated elsewhere
//
//returns OFTrue if the search was successfull, OFalse otherwise
OFBool PACS::getElementValueAsString(const char *keyTag, DcmDataset *datasetPtr, OFString *stringOut)
{
	unsigned int g = 0xffff;
	unsigned int e = 0xffff;
	if (datasetPtr == NULL) {
		return OFFalse; /* nothing to do */
	}
	

	int n = sscanf(keyTag, "%x,%x", &g, &e);
	if (n<2)
		return OFFalse;

	/* find the key in the dataset and return its value*/
	unsigned long elemCount = datasetPtr->card();
	for (unsigned long i=0; i<elemCount; i++) {
		DcmElement *elem = datasetPtr->getElement(i);
		if((elem->getGTag()==g)&&(elem->getETag()==e))
		{
			char *stringAddr;
			elem->getString(stringAddr);
			//strcpy(stringOut,stringAddr);
			*stringOut=stringAddr;
			return OFTrue;
		}
	}
	return OFFalse;
}


OFBool PACS::addToTextFile(const char* ofname, DcmDataset *dataset, OFString *queryString, unsigned int nQueryStrings)
{
	/* write out as a file text file format */
	FILE *fid;
	fid=fopen(ofname,"a+t");
	OFString stringValue;

	if(!fid)
		return OFFalse;

	for(unsigned ii=0;ii<(unsigned int) nQueryStrings;ii++)
	{
		if (!PACS::getElementValueAsString(queryString[ii].c_str(),dataset,&stringValue))
			stringValue="";	
		
		fprintf(fid,"%s=%s\n",queryString[ii].c_str(),stringValue.c_str());
	}
	fclose(fid);

	return OFTrue;
}


OFBool PACS::checkCorrectQueryRetriveLevel(const char *levelString){
	if((strcmp(levelString,"PATIENT")+strcmp(levelString,"STUDY")+
		strcmp(levelString,"SERIES")+strcmp(levelString,"IMAGE"))!=0)
		return OFFalse;
	else
		return OFTrue;
}





