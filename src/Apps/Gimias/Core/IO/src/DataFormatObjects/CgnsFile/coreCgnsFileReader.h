/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreCgnsFileReader_H
#define coreCgnsFileReader_H

#include "coreBaseDataEntityReader.h"
#include "coreVTKUnstructuredGridHolder.h"
#include "vtkUnstructuredGrid.h"

#include <vector>
#include <string>


namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityReader class for CGNS File objects.
If wraps the CGNSFileReader class to be used as a DataEntityReader that 
can be registered by the Core.

\ingroup gmIO
\author Chiara Riccobene
\author Albert Sanchez
\date 04 May 2010
*/

class GMIO_EXPORT CGNSFileReader : public BaseDataEntityReader
{
public:
	coreDeclareSmartPointerClassMacro(CGNSFileReader, BaseDataEntityReader);

	virtual bool ReadHeader( );
	virtual void Update ( );

	void SetScalarsList ( std::vector<std::string> &scalarsNamesToAdd );
	std::vector<std::string> GetScalarsList ( );

	void SetVectorsList ( std::vector<std::string> &vectorsNamesToAdd );
	std::vector<std::string> GetVectorsList ( );

	void setCompress ( bool onOff );
	bool GetCompress ( );

protected:
	CGNSFileReader ( );
	virtual ~CGNSFileReader ( );

	//!
	virtual boost::any ReadSingleTimeStep( 
		int iTimeStep, 
		const std::string &filename );

private:
	std::vector<std::string> m_scalarsNamesPoint;
	std::vector<std::string> m_vectorsNamesPoint;	
	std::vector<std::string> m_scalarsNamesCell;
	std::vector<std::string> m_vectorsNamesCell;
	bool m_compress;

	std::map<std::string,int> m_tagsTypeMap;

	coreDeclareNoCopyConstructors(CGNSFileReader);
};

}
}

#endif
