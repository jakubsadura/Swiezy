/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPIDimseSCU_h
#define _pacsAPIDimseSCU_h


#include "PacsAPIWin32Header.h"

#include "pacsAPIDcmtkInclude.h"  //include all the needed header files from dcmtk library

namespace PACS
{


#define APPLICATIONTITLE     "ANYSCU"
#define PEERAPPLICATIONTITLE "ANY-SCP"
#define TRANSFSYNTAXDEFAULT  NULL

#define ASS_NOTINITIALIZED 0  //not ready! init first!
#define ASS_NOTCONNECTED 1	  //no network connection
#define ASS_NOTASSOCIATED 2  //connected but not associated
#define ASS_ASSOCIATED	3	//association ok
#define MAXPDULENGTHDEFAULT 16*1024 //default max pdu lenght used when none has been specified


typedef struct {
	T_ASC_Association *assoc;
	T_ASC_PresentationContextID presId;
	OFString **queryString;
	unsigned int nQueryStrings;
} MyCallbackInfo;



/**
* \Define a Class for DIMSE DicomAssociation to Dicom Standard 3.0 
* \other basic functions for dimse services handling are provided 
*
* \ingroup PacsAPI 
* \author Luigi Carotenuto
* 
* The API provides functions for connecting  to a PACS Server
* and query/retrive data from it,
* based on the Library dcmtk http://dicom.offis.de/index.php.en
*
* Other basic function for
* Functions:
* 
*/
class PACSAPI_EXPORT Association
{

	// ----------------------------------
	// OPERATIONS
	// ----------------------------------
	public:
		//!
		Association();

		//!
		~Association();
		
		/**
		Init association: create and fill parameters vector 
		(roughly add User Information Model/Maximum Length Subitem
		*/
		OFBool initAssociation(
			int maxReceivePDULength = MAXPDULENGTHDEFAULT);

		//! Set the [unique] Application Context for the Association
		OFBool setApplicationCtx(
			char *applicationContextName = UID_StandardApplicationContext);
		
		/**
		Add a new Presentation Context with a single abstractSyntax and 
		one or more transferSyntaxes
		 \param [in] abstractSyntax - a string containing a valid SOP class 
		 for a specific Q/R Information Model
		 Typical values for abstractSyntax are defined in the included dcuid.h header file (of dcmtk library) are
		 - UID_FINDPatientRootQueryRetrieveInformationModel           (defines the uid string "1.2.840.10008.5.1.4.1.2.1.1")
		 - UID_MOVEPatientRootQueryRetrieveInformationModel           (defines the uid string "1.2.840.10008.5.1.4.1.2.1.2")
		 - UID_GETPatientRootQueryRetrieveInformationModel            (defines the uid string "1.2.840.10008.5.1.4.1.2.1.3")
		 - UID_FINDStudyRootQueryRetrieveInformationModel             (defines the uid string "1.2.840.10008.5.1.4.1.2.2.1")
		 - UID_MOVEStudyRootQueryRetrieveInformationModel             (defines the uid string "1.2.840.10008.5.1.4.1.2.2.2")
		 - UID_GETStudyRootQueryRetrieveInformationModel              (defines the uid string "1.2.840.10008.5.1.4.1.2.2.3")
		 - UID_FINDPatientStudyOnlyQueryRetrieveInformationModel      (defines the uid string "1.2.840.10008.5.1.4.1.2.3.1")
		 - UID_MOVEPatientStudyOnlyQueryRetrieveInformationModel      (defines the uid string "1.2.840.10008.5.1.4.1.2.3.2")
		 - UID_GETPatientStudyOnlyQueryRetrieveInformationModel       (defines the uid string "1.2.840.10008.5.1.4.1.2.3.3")
		 - UID_FINDModalityWorklistInformationModel                   (defines the uid string "1.2.840.10008.5.1.4.31")
		 - UID_FINDGeneralPurposeWorklistInformationModel             (defines the uid string"1.2.840.10008.5.1.4.32.1")
		
		 \param [in] transferSyntax -  a vector of strings containing 
		 valid transfersyntaxes identifiers 
			The DICOM standard defines:
		  UID_LittleEndianImplicitTransferSyntax   (uid string "1.2.840.10008.1.2") 
		  as the standard suggested transfer syntax
		  i.e. a typical value for transrerSyntax vector is 
		  transferSyntax={UID_LittleEndianImplicitTransferSyntax}
		  the default value for this parameter is TRANSFSYNTAXDEFAULT 
		  (=NULL pointer) that is the same as 
			transferSyntax={UID_LittleEndianImplicitTransferSyntax}   
			in this case the nTransferSyntaxes will be ignored
		
		\param nTransferSyntaxes - [in] the number of transfersintaxes 
		 provided (default = 0;  meaning that transferSyntax will be 
         considered equal toTRANSFSYNTAXDEFAULT, no matter if another 
		 value has been specified)
												
		\return OFTrue if the presentation could be addedd. OFFalse otherwise.
		*/
		OFBool addPresentationCtx(char *abstractSyntax, 
			const char *transferSyntax[]=TRANSFSYNTAXDEFAULT, int nTransferSyntaxes=0);


		/**Add a AETitles for caller and receiver  (in the associateRqPDU).
		\param [in] callerAETitle - the name of the application calling the 
		remote entity (usually a PACS server). default "ANYSCU"
		\param [in] receiverAETitle - the name of the remote application 
		entity (usually a PACS server). default "ANYSCP"
		\return OFTrue
		*/
		OFBool setAETitles(
			const char *callerAETitle = APPLICATIONTITLE, 
			const char *receiverAETitle = PEERAPPLICATIONTITLE);

		/**Start network connection
		 \param [in] peer - the ip address of the peer application entity to be contacted
		 \param [in] port - the tcp port on which the remote AE (peer) is listening
		 \param [in] timeout - timeout for responses
		 \param [in] netRole - our role in the connection 
		      (one of the following NET_ACCEPTOR, NET_REQUESTOR, 
		      NET_ACCEPTORREQUESTOR, according to service that we want to request
		      NET_REQUESTOR for query or ping only, NET_ACCEPTORREQUESTOR for 
		      retrive, NET_ACCEPTOR for storing)
		 \param [in] acceptorPort - the port on which we will listen for 
		      incominc connection (useful only in ACCEPTOR modes)
		 \param [in] secureTransLayer - does the connection require a 
		      secure layer (SSL); openssl library requested...not tested!
		 \return OFTrue if the connection is correctly started; OFFalse otherwise
		*/
		OFBool networkConnect(
			const char *peer, 
			unsigned long port, 
			int timeout, 
			T_ASC_NetworkRole netRole, 
			unsigned long acceptorPort, 
			OFBool secureTransLayer = OFFalse); 

		/** Try to start an association by sending AssociationRequest and 
		processing AssociationResponse (or Abort).

		First of all one should call: initAssociation, setApplicationCtx, 
		setAETitles, addPresentationCtx and networkConnect otherwise the 
		association fails!!
		*/
		OFBool associate();

		/** Close an association; if abortAssociation (OFTrue), the 
		association is brutally closed, without waiting for responses
		*/
		OFBool close(OFBool abortAssociation = OFFalse);

		/** Return a copy of the T_ASC_Association object included in the 
		private members of this class 
		*/
		T_ASC_Association *getOFAssociation(){
			return m_Assoc;};

		/** Return a copy of the T_ASC_Network object included in the 
		private members of this class
		*/
		T_ASC_Network *getOFNetwork(){
			return m_Net;};


	private:
		//int presentationContextID; //2*nPresentations-1
		int m_nPresenations; //number of presentation proposed
		T_ASC_Parameters *m_Params;
		int associationStatus;
		DIC_NODENAME m_LocalHost, m_PeerHost;
		unsigned long m_Opt_port;
		char m_Opt_peer[25];  //ready for 64 bit (IPV6 ip addresses)

		T_ASC_Association *m_Assoc;
		T_ASC_Network *m_Net;
};


OFBool writeToFile(const char* ofname, DcmDataset *dataset);
OFBool addKeyToDataset(const char *s, DcmDataset ** datasetPtr);
OFBool substituteKeysInDataset(DcmDataset *dset, DcmDataset *overrideKeys);
OFBool getElementValueAsString(const char *keyTag, DcmDataset *datasetPtr, OFString *stringOut);
OFBool addToTextFile(const char* ofname, DcmDataset *dataset, OFString *queryString, unsigned int nQueryStrings);
OFBool checkCorrectQueryRetriveLevel(const char *levelString);

} //namespace PACS

#endif //_pacsAPIDimseSCU_h


