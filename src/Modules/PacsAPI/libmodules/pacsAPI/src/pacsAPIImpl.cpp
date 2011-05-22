/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	// SMoAPI
	#include "pacsAPIImpl.h"

	#include <fstream>
	#include <stdexcept>

	#include "boost/filesystem.hpp"


	//---------------------------------------------------
	// DLL ENTRY POINT
	// GLOBAL FUNCTION FOR RETURNING THE POINTER OF PACS::PacsAPI
	//---------------------------------------------------
	PACS::PacsAPI* PACS::CreateObjectOfPacsAPI()
	{
		return new PACS::PacsAPIImpl();
	}

	void PACS::DestroyObjectOfPacsAPI( PACS::PacsAPI* pPointer )
	{
		if ( pPointer != 0 )
		{
			delete pPointer;
		}
	}

	//---------------------------------------------------
	//  END
	//---------------------------------------------------


	std::map<PACS::QueryRetriveLevel, std::string> PACS::PacsAPIImpl::m_QueryRetriveLevelTable;

	//---------------------------------------------------
	// OPERATIONS
	//---------------------------------------------------


PACS::PacsAPIImpl::PacsAPIImpl()
{
	m_QueryRetriveLevelTable[ PATIENT].assign("PATIENT");
	m_QueryRetriveLevelTable[ STUDY ].assign("STUDY");
	m_QueryRetriveLevelTable[ SERIES ].assign("SERIES");
	m_QueryRetriveLevelTable[ IMAGE ].assign("IMAGE");

	m_QueryResultsFilename = "PACSQuery.txt";
}

PACS::PacsAPIImpl::~PacsAPIImpl()
{

}

void PACS::PacsAPIImpl::Connect( bool send )
{
	OFBool result;

	m_Association.initAssociation(); //16KB max PDU length
	m_Association.setApplicationCtx(UID_StandardApplicationContext);

	const char *transferSyntaxes[1]={UID_LittleEndianImplicitTransferSyntax};  //just the default transfer syntax

	//This association proposes a C-Find service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(
		UID_FINDPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,
		1);

	//This association proposes a C-Move service for Patient Root Query/Retrive Information Model
	m_Association.addPresentationCtx(
		UID_MOVEPatientRootQueryRetrieveInformationModel,
		transferSyntaxes,
		1);

	if(m_AssociationParams.extraPresentationCtx.length()>0)
	{
		//This association proposes the service in the added sopclass
		m_Association.addPresentationCtx(
			(char *)m_AssociationParams.extraPresentationCtx.c_str(),
			transferSyntaxes,
			1);
	}

	//Add presentation context for all standard store SOP uids supported:
	for(int i=0;i<SUPPORTEDSTOREUID;i++)
		m_Association.addPresentationCtx((char *)StoreClassUID[i],transferSyntaxes,1);
	
	m_Association.setAETitles(m_NetworkParams.CallingAETitle.c_str(),
		m_NetworkParams.CalledAETitle.c_str());  //AE titles!
	result = m_Association.networkConnect(
		m_NetworkParams.serverIP.c_str(), 
		m_NetworkParams.serverPort, 
		m_NetworkParams.timeout,
		NET_ACCEPTORREQUESTOR, 
		send? m_NetworkParams.clientPortSend : m_NetworkParams.clientPortRetrieve, 
		OFFalse);

	if(result != OFTrue)
	{
		throw std::runtime_error( "Network connection to PACS server failed" );
	}
}

void PACS::PacsAPIImpl::SetNetworkParams( NetworkParams params )
{
	m_NetworkParams = params;
}

void PACS::PacsAPIImpl::Query()
{
	Connect( false );

	DoQuery();
}

void PACS::PacsAPIImpl::Move()
{
	Connect( false );

	DoMove();
}

void PACS::PacsAPIImpl::Store()
{

	Connect( true );

	DoStore();
}

void PACS::PacsAPIImpl::SetQueryParams( QueryParams params )
{
	m_QueryParams = params;
}

std::string PACS::PacsAPIImpl::GetQueryResultsFileName()
{
	return m_QueryResultsFilename;
}

void PACS::PacsAPIImpl::SetMoveParams( MoveParams params )
{
	m_MoveParams = params;
}

void PACS::PacsAPIImpl::DoQuery()
{
	OFBool result;
	result = m_Association.associate();
	if( result == OFFalse)
	{
		throw std::runtime_error( "Network connection to PACS server failed" );
	}

	m_QueryResultsFilename = m_QueryResultsFilename;
	// Clean file
	std::ofstream queryFile;
	queryFile.open ( m_QueryResultsFilename.c_str() );
	queryFile.close();

	std::string queryLevel = m_QueryRetriveLevelTable[ m_QueryParams.retrieveLevel ];
	m_Find.setQueryRetriveLevel( queryLevel.c_str() );
	m_Find.setOutTxtFile( m_QueryResultsFilename.c_str() );

	// Convert query to const char*
	std::vector<const char*> query;
	BuildQuery( query );

	// Do query
	result = m_Find.query( &m_Association, &query[ 0 ], query.size() );
	if( result == OFFalse)
	{
		throw std::runtime_error( "Query failed" );
	}

	m_Association.close(OFFalse);
}

void PACS::PacsAPIImpl::DoMove()
{
	OFBool result;
	result = m_Association.associate();
	if( result == OFFalse)
	{
		throw std::runtime_error( "Network connection to PACS server failed" );
	}

	// Create results folder
	std::stringstream filenameStream;
	clock_t time = clock();
	filenameStream 
		<< m_MoveParams.outputStorageFolder 
		<< "/" << "PACS" << time << "/";
	m_MoveResultsFolder = filenameStream.str();

	try {
		boost::filesystem::create_directory ( m_MoveResultsFolder );
	}
	catch (... ) {
		std::ostringstream strError;
		strError 
			<< "Directory creation failed: "
			<< m_MoveResultsFolder
			<< std::endl;
		throw std::runtime_error( strError.str( ) );
	} 

	std::string queryLevel = m_QueryRetriveLevelTable[ m_QueryParams.retrieveLevel ];
	m_Move.setQueryRetriveLevel( queryLevel.c_str() );
	m_Move.setOutputStorageFolder( m_MoveResultsFolder.c_str() );

	// Convert query to const char*
	std::vector<const char*> query;
	BuildQuery( query );

	// Do query
	result = m_Move.move( &m_Association, &query[ 0 ], query.size(), m_MoveParams.retriveport );
	if( result == OFFalse)
	{
		throw std::runtime_error( "Move failed" );
	}

	m_Association.close(OFFalse);
}

void PACS::PacsAPIImpl::SetStoreParams( StoreParams  params)
{
	m_StoreParams = params;
}

void PACS::PacsAPIImpl::SetAssociationParams( AssociationParams  params)
{
	m_AssociationParams = params;
}

void PACS::PacsAPIImpl::DoStore()
{
	OFBool result;
	result = m_Association.associate();
	if( result == OFFalse)
	{
		throw std::runtime_error( "Network connection to PACS server failed" );
	}

	// Do store
	for(std::vector<string>::iterator it=m_StoreParams.fullfilenames.begin(); 
		it != m_StoreParams.fullfilenames.end(); ++it)
	{
		std::string fullfilename=(*it);

		result = m_Store.store(&m_Association, fullfilename.c_str() );
		if( result == OFFalse)
		{
			throw std::runtime_error( "Store failed" );
		}
	}

	m_Association.close(OFFalse);

}

PACS::QueryParams PACS::PacsAPIImpl::GetQueryParams()
{
	return m_QueryParams;
}

std::string PACS::PacsAPIImpl::GetMoveResultsFolder()
{
	return m_MoveResultsFolder;
}

void PACS::PacsAPIImpl::BuildQuery( std::vector<const char*> &query )
{
	for ( unsigned i = 0 ; i < m_QueryParams.query.size() ; i++ )
	{

		//m_QueryParams.query[ i ].GetString( '=' );  //fill the m_TagInTextFormat
		query.push_back(m_QueryParams.query[ i ].GetString('=')->c_str( ) );
	}

	if ( !m_QueryParams.birthDateFrom.m_value.empty() )
	{
		query.push_back( m_QueryParams.birthDateFrom.GetString( '>' )->c_str( ) );
	}

	if ( !m_QueryParams.birthDateTo.m_value.empty() )
	{
		query.push_back( m_QueryParams.birthDateTo.GetString( '<' )->c_str( ) );
	}
}


//int main()
//{
//	PACS::PacsAPIImpl impl;
//	PACS::StoreParams sParams;
//	PACS::NetworkParams nParams;
//	PACS::AssociationParams aParams;
//
//	nParams.CalledAETitle.assign("ANYSCP");
//	nParams.clientPort=1235;
//	//nParams.CallingAETitle.assign("pacsAPILocal");
//	nParams.CallingAETitle.assign("g127.0.0.1");
//	nParams.serverIP="127.0.0.1";
//	nParams.serverPort=1234;
//	
//	impl.SetNetworkParams(nParams);
//	sParams.fullfilenames.push_back("c:/Code/fundus/fundus images/image1.dcm");
//	sParams.fullfilenames.push_back("c:/Code/fundus/fundus images/image2.dcm");
//	impl.SetStoreParams(sParams);
//
//	//aParams.extraPresentationCtx=UID_MRImageStorage;
//	//impl.SetAssociationParams(aParams);
//
//	impl.Store();
//}