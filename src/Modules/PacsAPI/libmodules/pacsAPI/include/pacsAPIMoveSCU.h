/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPIMoveSCU_h
#define _pacsAPIMoveSCU_h

#include "pacsAPIDimse.h"
#include "PacsAPIWin32Header.h"





namespace PACS
{

	/* DICOM MOVE information models*/
	static const unsigned int nQueryInfoModels = 3;
	static const char* queryInfoModels[] = {
		// Worklist and Query/Retrieve
		UID_MOVEPatientRootQueryRetrieveInformationModel,
		UID_MOVEStudyRootQueryRetrieveInformationModel,  
		UID_MOVEPatientStudyOnlyQueryRetrieveInformationModel
	};


	/**
	* \brief PacsAPIMoveSCU. Query/Retrieve Service Class User (C-MOVE operation)
	* \ingroup PacsAPI 
	* \author Luigi Carotenuto
	*
	* The API provides functions for connecting to a PACS Server, based on
	* the Library dcmtk http://dicom.offis.de/index.php.en
	*
	*
	*/
	class PACSAPI_EXPORT MoveSCU
	{

		// ----------------------------------
		// OPERATIONS
		// ----------------------------------
	public:
		//! Default Constructor
		MoveSCU();

		//! Destructor
		~MoveSCU(){};

		/**
		Move file from PACS Server.
		\param assoc - [in] A valid accepted Association. 
		\param queryString - [in] A set of strings containing the query 
		      tags in the form ffff,gggg[=value]
		      where ffff and gggg is the element tag and value is its value. 
		       In case =value is missing,
			   the tag will be used as a return only tag (will be filled 
				by the server)
			   The acceptable Tags depend on the choosen Information Model 
		       (Patient, Study, Patient Study Root Model)
			   and the query/retrive level (PATIENT, STUDY, SERIES, IMAGE). 
		       More info on these topics could be found
			   in cfr. C.6.1.1 - C.6.3.1 of the chapter 4 of the Dicom3 
		       Standard (aka chapter 3.4) 
				(2008 edition or previous)
		\param nQueryString - [in] The number of strings in the queryString vector.
		\param retrivePort - [in] The TCP port on which a storeSCP service 
		       will be started in order to receive files 
			   from the PACS server.
		\return true if the query operation has success.
		*/
		OFBool move(
			PACS::Association* assoc, 
			const char *queryString[],
			unsigned int nQueryStrings, 
			unsigned long retriveport);
	
		/**
		Set the 0008,0052 query/retrive level to the value specified in the levelString
		\note This function must be called previous to the query function
	
		\param levelString- [in] a (const) string containing one of the 
		    permitted queryRetriveLevels 
			(i.e. "PATIENT","STUDY","SERIES","IMAGE"); if levelString do not 
		    match with any of them
		   "PATIENT" level is set by default
		*/
		static void setQueryRetriveLevel(const char *levelString);
		
		/**
		Set the folder where the files retrived from PACS server will be stored
		\param outputFolder - [in] the path of the folder (the folder MUST 
		   have a slash or back slash at the end
		  (e.g. .\dicomStore\  or ./dicomStore/)
		
		\note the folder must exist!!! and be accessible for writing 
		   for the current user: no error 
		will be returned if the folder doesn't exist or if it is not accessible!
		*/
		static void setOutputStorageFolder(const char *outputFolder);

	private:

		/**
		** We prefer to use Explicitly encoded transfer syntaxes.
		** If we are running on a Little Endian machine we prefer
		** LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
		** Some SCP implementations will just select the first transfer
		** syntax they support (this is not part of the standard) so
		** organise the proposed transfer syntaxes to take advantage
		** of such behaviour.
		**
		** The presentation contexts proposed here are only used for
		** C-FIND and C-MOVE, so there is no need to support compressed
		** transmission.
		*/

		static void moveCallback(void *callbackData, T_DIMSE_C_MoveRQ *request,
			 int responseCount, T_DIMSE_C_MoveRSP *response);


		
		static OFBool addOverrideKey( const char* s);
		static OFBool substituteOverrideKeys(DcmDataset *dset);
		
		/**
		* This function.is used to indicate progress when movescu receives search results over the
		* network. This function will simply cause some information to be dumped to stdout.
		*
		* Parameters:
		*   callbackData        - [in] data for this callback function
		*   request             - [in] The original find request message.
		*   responseCount       - [in] Specifies how many C-FIND-RSP were 
		*                              received including the current one.
		*   rsp                 - [in] the C-FIND-RSP message which was 
		*                              received shortly before the call to
		*                              this function.
		*   responseIdentifiers - [in] Contains the record which was received. 
		*                              This record matches the search
		*                              mask of the C-FIND-RQ which was sent.
		*/
		static OFCondition acceptSubAssoc(T_ASC_Network * aNet, T_ASC_Association ** assoc);

		static OFCondition echoSCP(T_ASC_Association * assoc,T_DIMSE_Message * msg,
			T_ASC_PresentationContextID presID);

		static void storeSCPCallback( void *callbackData,T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req,             /* original store request */
				char *imageFileName, DcmDataset **imageDataSet, 
				T_DIMSE_C_StoreRSP *rsp,  DcmDataset **statusDetail);

		static OFCondition storeSCP(T_ASC_Association *assoc,T_DIMSE_Message *msg,
			T_ASC_PresentationContextID presID);
		
		static OFCondition subOpSCP(T_ASC_Association **subAssoc);

		static void subOpCallback(void * /*subOpCallbackData*/ , T_ASC_Network *aNet,
			T_ASC_Association **subAssoc);
		
		static void
			progressCallback(
			void *callbackData,
			T_DIMSE_C_MoveRQ *request,
			int responseCount,
			T_DIMSE_C_MoveRSP *rsp,
			DcmDataset *responseIdentifiers
			);

		// ----------------------------------
		// MEMBERS
		// ----------------------------------
	private:
		static OFString m_queryRetriveLevel;		
		//!
		static OFBool opt_verbose;
		//!
		static OFBool opt_debug;
		//!
		static OFBool opt_abortAssociation;
		//!
		static OFCmdUnsignedInt opt_maxReceivePDULength;
		//!
		static OFCmdUnsignedInt opt_repeatCount;
		//!
		static OFBool opt_extractResponsesToFile;
		//! Information model level to perform the query
		static const char *opt_abstractSyntax;
		//!
		static OFCmdSignedInt opt_cancelAfterNResponses;
		//!
		static DcmDataset *overrideKeys;
		//!
		static E_TransferSyntax opt_networkTransferSyntax;
		//!
		static T_DIMSE_BlockingMode opt_blockMode;
		//!
		static int opt_dimse_timeout;

		static OFBool opt_ignorePendingDatasets;
		//!
		static int opt_acse_timeout;
		//!
		T_ASC_Network *net;
		//!
		T_ASC_Parameters *params;
		//!
		const char *opt_peer;
		//!
		OFCmdUnsignedInt opt_port;
		//!
		DIC_NODENAME localHost;
		//!
		DIC_NODENAME peerHost;
		//!
		T_ASC_Association *assoc;
		//!
		const char *opt_peerTitle;
		//!
		const char *opt_ourTitle;
		//!
		OFList<OFString> m_FileNameList;
		//!
		OFBool opt_secureConnection; /* default: no secure connection */

		static E_TransferSyntax  opt_in_networkTransferSyntax;
		
		static E_TransferSyntax  opt_out_networkTransferSyntax;

		static OFCmdUnsignedInt  opt_sleepAfter;
		static OFCmdUnsignedInt  opt_sleepDuring;
		static OFCmdUnsignedInt  opt_maxPDU;
		static OFBool opt_useMetaheader;
		static E_TransferSyntax  opt_writeTransferSyntax;
		static E_GrpLenEncoding  opt_groupLength;
		static E_EncodingType    opt_sequenceType;
		static E_PaddingEncoding opt_paddingType;
		static OFCmdUnsignedInt  opt_filepad ;
		static OFCmdUnsignedInt  opt_itempad;
		static OFBool            opt_bitPreserving;
		static OFBool            opt_ignore;
		static OFBool            opt_abortDuringStore;
		static OFBool            opt_abortAfterStore;
		static OFBool            opt_correctUIDPadding;
		static OFCmdUnsignedInt  opt_retrievePort;
		static const char *      opt_moveDestination;
		static char				 outputStorageFolder[300];


	}; // MoveSCU

}//namespace PACS



#endif //_pacsAPIMoveSCU