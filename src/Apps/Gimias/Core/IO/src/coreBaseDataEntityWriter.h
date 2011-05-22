/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreBaseDataEntityWriter_H
#define coreBaseDataEntityWriter_H

#include "gmIOWin32Header.h"
#include "coreObject.h"
#include "coreDataEntity.h"
#include "coreBaseIO.h"

namespace Core
{
namespace IO
{

/** 
\brief This is the base class for specialized file writers and it is 
responsible for exporting the processing data of a DataEntity to a file, 
being of a fixed format and data entity type.

This class is the base class for DataEntityWriters. If you want to 
create a writer of your own, first specify the data object type for you 
are going to create the writer (f.i. <vtkPolydata>), and then subclass it so as to
provide a body for the required functions (the ones pure virtual).

Provide a body for the function Update( ) that outputs the processing data 
of a DataEntity to the file.
Usually to achieve this behaviour is no more difficult than just to use a 
specific data writer on the file given as parameter. Retrieve the processing 
data of a DataEntity by calling its member GetProcessingRepresentation()

\ingroup gmIO
\author Juan Antonio Moya
\date 20 Mar 2008
*/
class GMIO_EXPORT BaseDataEntityWriter : public Core::IO::BaseIO
{
public:
	typedef std::list<std::string> ValidExtensionsListType;
	typedef std::list<DataEntityType> ValidTypesListType;

public:
	coreDeclareSmartPointerTypesMacro(BaseDataEntityWriter, Core::IO::BaseIO);

	coreClassNameMacro(BaseDataEntityWriter);

	virtual void Update();

	//!
	bool CanWrite( Core::DataEntityType type );

	//!
	bool CanWrite( std::string ext );

protected:
	BaseDataEntityWriter(void);;
	virtual ~BaseDataEntityWriter(void);;

	coreDeclareNoCopyConstructors(BaseDataEntityWriter);

	/**
	\brief Write all TimeSteps using the function WriteSingleTimeStep( )

	The names of the files will be constructed using fileName. For example:
	pa73_phs_r_.vtk -> pa73_phs_r_01.vtk, pa73_phs_r_02.vtk, ...

	If there's only one TimeStep, the name will be fileName
	*/
	void WriteAllTimeSteps( );

	//! Write Single TimeStep
	virtual void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity,
		int iTimeStep );

	//!
	virtual bool WriteMetaData();

	//!
	virtual void WriteData( );

	//!
	std::string GetFilename( int timeStep );
protected:
};

}
}

#endif
