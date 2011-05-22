/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPIFindSCU_h
#define _pacsAPIFindSCU_h


#include "pacsAPIDimse.h"
#include "PacsAPIWin32Header.h"



namespace PACS
{

/* DICOM FIND information models*/
static const int nFindInfoModels = 5;
static const char* findInfoModels[] = {
// Worklist and Query/Retrieve
	UID_FINDPatientRootQueryRetrieveInformationModel,
	UID_FINDStudyRootQueryRetrieveInformationModel,
	UID_FINDPatientStudyOnlyQueryRetrieveInformationModel,
	UID_FINDModalityWorklistInformationModel,
	UID_FINDGeneralPurposeWorklistInformationModel
};




/**
* \brief PacsAPIFindSCU. Query/Retrieve Service Class User (C-FIND operation)
* \ingroup PacsAPI
* \authors Martin Bianculli/ Luigi Carotenuto
*
* The API provides functions for connecting to a PACS Server, based on
* the Library dcmtk http://dicom.offis.de/index.php.en
*
* Functions:
* 
*/
class PACSAPI_EXPORT FindSCU
{

// ----------------------------------
// OPERATIONS
// ----------------------------------
public:
	//! Default Constructor
	FindSCU();

	//! Destructor
	~FindSCU();

	/**
	Query to a PACS Server.

	\param assoc - [in] A valid Dicom Association with a PACS peer entity.
	\param queryString - [in] A vector of strings containing the DIcom tags 
	    to be queried, in the form gggg,ffff[=value],
		where gggg,ffff is the element tag and value its value. If the 
		"=value" is not specified, the value will
		be treated as a return key (i.e. a key for return only).
	\param nQueryStrings - [in] The number of strings in the queryString vector.
	\return true if the query was correctly performed.

	query results are appended in a file that could be specified via 
	setTxtFileName function (by default res.txt)
	in the current directory. If the file doesn't exist, a new one will 
	be created. 
	
	example: to query for all patients id, the queryString should contain 
	0010,0020 (with no values spefied)
	If you want to search for all the studies of a certain patient, one should 
	0. Associate with a Patient Info Model (let's say 
	UID_FINDPatientRootQueryRetrieveInformationMode)
	1. Set the queryRetriveLevel to "STUDY" (with FindSCU::setQueryRetriveLevel("STUDY") 
	2. call this function with a specific Patient ID and a blank study ID
	0010,0020=PatientName
	0020,000E

	\note if you associate directly in a Study Info Model at point 0, 
	you could provide directly the blank 0020,000E
	field (without the 0010,0020) and get all studies from all patients!! 
	/point 1 is still necessary if you want a
	study level!! (PATIENT level should be unavailable)
	For more queryStings and different Info Models and levels cfr. 
	C.6.1.1 - C.6.3.1 of the chapter 4 of the Dicom3 
	Standard (aka chapter 3.4) (2008 edition or previous)
	*/
	bool query(PACS::Association* assoc,const char *queryString[],
		unsigned int nQueryStrings);

	/** Set the Query Retrive Level (Tag 0008,0052)
	
	Set the 0008,0052 query/retrive level to the value specified in the 
	levelString

	\note This function must be called previous to the query function
	
	\param levelString- [in] a (const) string containing one of the 
	    permitted queryRetriveLevels 
		(i.e. "PATIENT","STUDY","SERIES","IMAGE"); if levelString do not 
		match with any of them
	    "PATIENT" level is set by default
	*/
	static void setQueryRetriveLevel(const char *levelString);

	/**
	Set the name of the output file name (a txt file). Default 
	filename=res.txt in the current dir.
	\parameter outTxtFname - [in] The name of the output text file.
	*/
	static void setOutTxtFile(const char *outTxtFName);

	//! Reset the overrideKeys to nothing
	static void cleanAllKeys();

private:
	/*
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

	

	/**
	* This function.is used to indicate progress when findscu receives 
	  search results over the
	  network. This function will simply cause some information to be dumped 
	  to stdout.
	*
	* \param callbackData        - [in] data for this callback function
	* \param  request             - [in] The original find request message.
	* \param responseCount       - [in] Specifies how many C-FIND-RSP were 
	*                              received including the current one.
	* \param  rsp                 - [in] the C-FIND-RSP message which was 
	*                              received shortly before the call to
	*                              this function.
	* \param  responseIdentifiers - [in] Contains the record which was received. 
	*                              This record matches the search
	*                              mask of the C-FIND-RQ which was sent.
	*/
	static void
		progressCallback(
		void *callbackData,
		T_DIMSE_C_FindRQ *request,
		int responseCount,
		T_DIMSE_C_FindRSP *rsp,
		DcmDataset *responseIdentifiers
		);

	/**
	* This function will read all the information from the given file
	* (this information specifies a search mask), figure out a corresponding
	* presentation context which will be used to transmit a C-FIND-RQ message
	* over the network to the SCP, and it will finally initiate the transmission
	* of data to the SCP.
	*
	* Parameters:
	* \param  assoc - [in] The association (network connection to another DICOM application).
	* \param  fname - [in] Name of the file which shall be processed.
	*/
	static OFCondition findSCU(T_ASC_Association * assoc, const char *fname);

	//!
	static OFBool addOverrideKey( const char* s);

	//!
	static OFBool substituteOverrideKeys(DcmDataset *dset);

	/**
	* This function save the queryString vector specified in the query 
	* method in the local variable m_queryString
	* and removes the "=value" (if present) from each string
	*/
	static void saveCleanQueryStrings(const char **queryString, unsigned int nQueryStrings);

// ----------------------------------
// MEMBERS
// ----------------------------------
private:
	//the query strings
	//!
	static OFString *m_queryString;
	//!
	static unsigned int m_nQueryStrings;
	//!
	static OFString m_queryRetriveLevel;
	//!
	static OFString m_outTextFName;
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
	//!
	int opt_acse_timeout;
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


}; // FindSCU

}//namespace PACS





#endif //_pacsAPIFindSCU