/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreBaseDataEntityReader_H
#define coreBaseDataEntityReader_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreBaseIO.h"

namespace Core
{
namespace IO
{


/** 
\brief The BaseDataEntityReader class is the base class for specialized file 
readers and it is responsible for importing data from a file, being of 
a fixed format and data entity type.

This class is the base class for DataEntityReaders. If you want to 
create a reader of your own, first specify the data object type for you 
are going to create the reader (f.i. <vtkPolydata>), and then subclass it 
so as to provide a body for the required functions (the ones pure virtual).

Format providers may inherit this class and use their own file reader 
internally, so as to retrieve a data object from a file. 

Provide a body for the function Update( ) that reads the file using a 
specific data reader.

You are required to fill in as many parameters as you can, of the 
header of the data entity being returned (such as modality or type). 

\ingroup gmIO
\author Juan Antonio Moya
\date 20 Mar 2008
*/
class GMIO_EXPORT BaseDataEntityReader : public Core::IO::BaseIO
{
public:

public:
	coreDeclareSmartPointerTypesMacro(BaseDataEntityReader, Core::IO::BaseIO);
	coreClassNameMacro(BaseDataEntityReader);

	virtual void Update( );

	//!
	bool CanRead( Core::DataEntityType type );

	//!
	bool CanRead( std::string ext );

	//! Read data file header to retrieve general information about the data
	virtual bool ReadHeader( );

protected:
	BaseDataEntityReader(void);;
	virtual ~BaseDataEntityReader(void);

	coreDeclareNoCopyConstructors(BaseDataEntityReader);

	/**
	\brief Read all TimeSteps calling the function ReadSingleTimeStep( )
	and creates a Core::DataEntity
	*/
	void ReadAllTimeSteps( 
		Core::DataEntityType type);

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename ) = 0;

	//! Read metadata file
	virtual bool ReadMetaData( );

	//!
	virtual void ReadData( );

protected:

};


}
}

#endif
