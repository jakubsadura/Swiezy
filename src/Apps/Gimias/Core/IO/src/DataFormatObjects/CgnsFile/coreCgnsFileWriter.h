/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreCgnsFileWriter_H
#define coreCgnsFileWriter_H

#include "coreBaseDataEntityWriter.h"
#include "vtkUnstructuredGrid.h"

namespace Core
{
namespace IO
{
/** 
A specialization of the DataEntityWriter class for CGNS File objects.
If wraps the CGNSFileWriter class to be used as a DataEntityWriter that 
can be registered by the Core.

\ingroup gmIO
\author Chiara Riccobene
\date 18 Nov 2008
*/
class GMIO_EXPORT CGNSFileWriter : public BaseDataEntityWriter
{
public:
	coreDeclareSmartPointerClassMacro(CGNSFileWriter, BaseDataEntityWriter);

	void WriteData( );

	//! Generate Metadata from dataEntity
	static void GenerateMetadata( Core::DataEntity::Pointer dataEntity );

protected:
	CGNSFileWriter ( );
	virtual ~CGNSFileWriter ();

	void WriteSingleTimeStep( 
		const std::string& fileName, 
		Core::DataEntity::Pointer dataEntity, 
		int iTimeStep );

private:
	void FillScalarsAndVectors(Core::DataEntity::Pointer dataEntity, int iTimePoint, int iTimeStep);
	bool SuffixIsNum(std::string str);
	bool CheckString(std::string str, int n);
	coreDeclareNoCopyConstructors(CGNSFileWriter);

private:
	std::vector<std::string> m_scalarsNamesPoint;
	std::vector<std::string> m_vectorsNamesPoint;
	std::vector<std::string> m_scalarsNamesCell;
	std::vector<std::string> m_vectorsNamesCell;	
};

}
}

#endif
