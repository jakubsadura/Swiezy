// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "pacsTest.h"
#include "dcmTypes.h"

unsigned long pacsTest::m_serverPort=1234;
const char *pacsTest::m_serverAddress="10.80.4.143";  //"193.145.50.44" is the CDTEAM PACS 
unsigned long pacsTest::m_reciveOnrPort=1235;		//1010 for CDTEAM PACS



void pacsTest::TestAssociation()
{
//uncomment and check the PACS server config to execute the test

	/*
	//Test different kind of associations
	m_Association.initAssociation(); //16KB max PDU length
	m_Association.setApplicationCtx(UID_StandardApplicationContext);
	
	//Set some used transferSyntaxes;
	const char *transferSyntaxes[9]={UID_LittleEndianImplicitTransferSyntax, //the DICOM suggested default
									 UID_LittleEndianExplicitTransferSyntax,
									 UID_BigEndianExplicitTransferSyntax,
									 UID_JPEGProcess1TransferSyntax,
                                     UID_JPEGProcess2_4TransferSyntax,
									 UID_JPEGProcess3_5TransferSyntax,
                                     UID_JPEGProcess6_8TransferSyntax,
									 UID_JPEGLSLosslessTransferSyntax,
									 UID_JPEGLSLossyTransferSyntax};  

	//This association proposte a C-Find service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(UID_FINDPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,9);

	//This association proposte a C-Move service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(UID_MOVEPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,9);

	m_Association.setAETitles("myFINDMOVESCU","ANYSCP");  //AE titles!
	
	//Try to connect to the network
	TS_ASSERT(m_Association.networkConnect(pacsTest::m_serverAddress,pacsTest::m_serverPort,30,
		NET_ACCEPTORREQUESTOR,pacsTest::m_reciveOnrPort,OFFalse),OFTrue);

	//Negotiate the association with the PACS
	TS_ASSERT(m_Association.associate(),OFTrue);
	
	//Close the association with the PACS
	TS_ASSERT(m_Association.close(OFFalse),OFTrue);
	*/
}


void pacsTest::TestQuery()
{
//uncomment and check the PACS server config to execute the test

	/*
   //Do a simple association
	m_Association.initAssociation(); 
	m_Association.setApplicationCtx(UID_StandardApplicationContext);
	
	//Set default transferSyntaxe only;
	const char *transferSyntaxes[1]={UID_LittleEndianImplicitTransferSyntax}; //the DICOM suggested default
	
	//Set different type of CFind operations (on different Query/Retrive Info Models)

	//This association propose a C-Find service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(UID_FINDPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,1);

	//This association propose a C-Find service for Study Root Query/Retrive Information Model
//	m_Association.addPresentationCtx(UID_FINDStudyRootQueryRetrieveInformationModel,
//	transferSyntaxes,1);

	//This association propose a C-Find service for Patient/Study Only Root Query/Retrive Information Model
//	m_Association.addPresentationCtx(UID_FINDPatientStudyOnlyQueryRetrieveInformationModel,
//	transferSyntaxes,1);

	//Do the network connection and associate
	m_Association.setAETitles("myFINDMOVESCU","ANYSCP");  //AE titles!
	m_Association.networkConnect(pacsTest::m_serverAddress,pacsTest::m_serverPort,30,
		NET_ACCEPTORREQUESTOR,pacsTest::m_reciveOnrPort,OFFalse);

	m_Association.associate();
*/
/*
	//********* A SIMPLE QUERY AT THE PATIENT LEVEL (ask for patient name and ids) 
	//Set the query in patient Level
	m_Find.setQueryRetriveLevel("PATIENT");
	

	//Ask for patient ID and Patient birth date
    const char *findString[]={"0010,0020","0010,0030"}; //Patient Id and BirthDate
	m_Find.setOutTxtFile("resPatientsFromPACS.txt");
	TS_ASSERT(m_Find.query(&m_Association,findString,2));
	m_Association.close(OFFalse);
*/
/*
	//********* A COMPLETE QUERY AT THE SERIES LEVEL (ask for lots of things) 
	//Test a complete query until series:
	//a '=' must be at the beginning of each not empty string

	//Set the query in patient Level
	m_Find.setQueryRetriveLevel("SERIES");

	dcmAPI::Tag filterPatId( dcmAPI::tags::PatientId, "" );
	dcmAPI::Tag filterPatName( dcmAPI::tags::PatientName, "" );
	dcmAPI::Tag filterPatDate( dcmAPI::tags::PatientDate, "01/01/1800-01/01/2020" );
	dcmAPI::Tag filterPatSex( dcmAPI::tags::PatientSex, "" );
	dcmAPI::Tag filterStuId( dcmAPI::tags::StudyId, "" );
	dcmAPI::Tag filterStuDate( dcmAPI::tags::StudyDate, "" );
	dcmAPI::Tag filterStuDesc( dcmAPI::tags::StudyDescription, "" );
	dcmAPI::Tag filterRefPhis( dcmAPI::tags::RefPhysician, "" );
	dcmAPI::Tag filterSerNum( dcmAPI::tags::SeriesNumber, "" );
	dcmAPI::Tag filterModality( dcmAPI::tags::Modality, "" );

	const int nStrings=10, nChars=256;
	const char **findString=new const char *[nStrings];
	findString[ 0 ] = filterPatId.GetString()->c_str();
	findString[ 1 ] = filterPatName.GetString()->c_str();
	findString[ 2 ] = filterPatDate.GetString()->c_str();
	findString[ 3 ] = filterPatSex.GetString()->c_str();
	findString[ 4 ] = filterStuId.GetString()->c_str();
	findString[ 5 ] = filterStuDate.GetString()->c_str();
	findString[ 6 ] = filterStuDesc.GetString()->c_str();
	findString[ 7 ] = filterRefPhis.GetString()->c_str();
	findString[ 8 ] = filterSerNum.GetString()->c_str();
	findString[ 9 ] = filterModality.GetString()->c_str();

	m_Find.setOutTxtFile("resCompleteQueryFromPACS.txt");
	m_Find.query(&m_Association, findString, nStrings);
	
	delete [] findString;
	m_Association.close(OFFalse);*/
}

void pacsTest::TestMove()
{
/*	//do the association first
	OFBool result;
	m_Association.initAssociation(); //16KB max PDU length
	m_Association.setApplicationCtx(UID_StandardApplicationContext);
	
	const char *transferSyntaxes[1]={UID_LittleEndianImplicitTransferSyntax};  //just the default transfer syntax

	//This association proposte a C-Find service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(UID_FINDPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,1);

	//This association proposte a C-Move service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(UID_MOVEPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,1);

	m_Association.setAETitles("pacsAPILocal","ANYSCP");  //AE titles!
	result=m_Association.networkConnect(pacsTest::m_serverAddress,pacsTest::m_serverPort,30,
		NET_ACCEPTORREQUESTOR,pacsTest::m_reciveOnrPort,OFFalse);

	m_Association.associate();


	//char *findString[]={"0010,0020=4010506-PACS","0010,0030"}; //Patient Id and BirthDate
	//m_Find.setQueryRetriveLevel("PATIENT");
	//m_Find.setOutTxtFile("resPatientsFromPACS.txt");
	//m_Find.query(&m_Association,findString,2);

	const char *queryString[]={"0010,0020=271360-PACS"};
	m_Move.setQueryRetriveLevel("PATIENT");
	m_Move.setOutputStorageFolder("c:/pippo/");
	m_Move.move(&m_Association,queryString,1,1235);

	m_Association.close(OFFalse);*/
}