/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPI_H
#define _pacsAPI_H

//---------------------------------------------------
// HEADERS
//---------------------------------------------------

#include "PacsAPIWin32Header.h"
#include "pacsAPITypes.h"
#include "CILabExceptionMacros.h"

namespace PACS
{

	class PacsAPI;


//---------------------------------------------------
// 
/**
* \brief Create an object for this API
* \ingroup PacsAPI
*/
extern "C" PACSAPI_EXPORT PACS::PacsAPI* CreateObjectOfPacsAPI();

/**
* \brief Destroy an object for this API
* \ingroup PacsAPI
*/
extern "C" PACSAPI_EXPORT void DestroyObjectOfPacsAPI( PACS::PacsAPI* );



//---------------------------------------------------
// CLASS DEFINITION
//---------------------------------------------------

/**
* \brief PACS API
*
* Facade class for all functionalities
*
* Query:
* - SetNetworkParams( )
* - SetQueryParams( )
* - Query( )
* - GetQueryResultsFileName( )
*
* Retrieve:
* - SetNetworkParams( )
* - SetQueryParams( )
* - SetMoveParams( )
* - Move( )
* - GetMoveResultsFolder( )
*
* Store (as SCU):
* - SetStoreParams( )
* - Store( )
*
* \ingroup PacsAPI
* \author Xavi Planes/Luigi Carotenuto
* \date 06 October 2009
*
*/
class PacsAPI
{

// ----------------------------------
// OPERATIONS
// ----------------------------------
public:

	//!
	PacsAPI();

	//!
	virtual ~PacsAPI();

	//!
	virtual void SetNetworkParams( NetworkParams params ) = 0;

	//!
	virtual void SetQueryParams( QueryParams params ) = 0;

	//!
	virtual void SetAssociationParams( AssociationParams params ) = 0;

	//!
	virtual void SetStoreParams( StoreParams params ) = 0;

	//!
	virtual QueryParams GetQueryParams( ) = 0;

	//!
	virtual void SetMoveParams( MoveParams params ) = 0;

	//!
	virtual void Query( ) = 0;

	//!
	virtual void Move( ) = 0;

	//!
	virtual void Store( ) = 0;

	//!
	virtual std::string GetQueryResultsFileName( ) = 0;

	//!
	virtual std::string GetMoveResultsFolder( ) = 0;

};


/**
* \brief Exception
* \ingroup PacsAPI
*/
cilabDeclareDllExportExceptionMacro(Exception, std::exception,PACSAPI_EXPORT)

} // namespace PACS


#endif // _pacsAPI_H
