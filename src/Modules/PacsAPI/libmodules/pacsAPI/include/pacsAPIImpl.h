/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPIImpl_H
#define _pacsAPIImpl_H


//---------------------------------------------------
// HEADERS
//---------------------------------------------------

#include "pacsAPI.h"
#include "PacsAPIWin32Header.h"
#include "pacsAPIDimse.h"
#include "pacsAPIFindSCU.h"
#include "pacsAPIMoveSCU.h"
#include "pacsAPIStoreSCU.h"

#include <map>

//---------------------------------------------------
// CLASS DEFINITION
//---------------------------------------------------

namespace PACS
{

/**
* \brief Implementation of the PACS API
* \ingroup PacsAPI
*
*/
class PacsAPIImpl : public PACS::PacsAPI
{

// ----------------------------------
// OPERATIONS
// ----------------------------------
public:

	//!
	PacsAPIImpl();

	//!
	virtual ~PacsAPIImpl();

	//@{ 
	/// \name Redefined

	void SetNetworkParams( NetworkParams params );
	void SetQueryParams( QueryParams params );
	void SetStoreParams( StoreParams  params);
	void SetAssociationParams(AssociationParams params);
	QueryParams GetQueryParams( );
	void SetMoveParams( MoveParams params );
	void Query( );
	void Move( );
	void Store( );
	std::string GetQueryResultsFileName( );
	std::string GetMoveResultsFolder( );
	//@}


private:

	//! \param [in] send if its true, use clientPortSend else clientPortRetrieve
	void Connect( bool send );

	//!
	void DoQuery( );

	//!
	void DoMove( );

	//!
	void DoStore( );

	//!
	void BuildQuery( std::vector<const char*> &query );

private:

	//!
	NetworkParams m_NetworkParams;

	//!
	QueryParams m_QueryParams;

	//!
	MoveParams m_MoveParams;

	//!
	StoreParams m_StoreParams;

	//!
	AssociationParams m_AssociationParams;

	//!
	PACS::Association m_Association;

	//!
	static std::map<QueryRetriveLevel, std::string> m_QueryRetriveLevelTable;

	//!
	std::string m_QueryResultsFilename;

	//!
	std::string m_MoveResultsFolder;

	//!
	PACS::FindSCU m_Find;

	//!
	PACS::MoveSCU m_Move;

	//!
	PACS::StoreSCU m_Store;

};


} // namespace PACS

#endif // _pacsAPIImpl_H
