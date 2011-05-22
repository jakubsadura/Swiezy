/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blCgnsFileWriter_h
#define blCgnsFileWriter_h

// basic includes
#include <string>
#include <vector>
using std::string;

// CGNSLIB
#include "cgnslib.h"

#include "BaseLibVTKWin32Header.h"
// VTK
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetAttributes.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

/** 
Writer for CGNS data entities. Writes a single time step. 

IMPORTANT:
This class is used to export the data in gimias into a CGNS file.
Since the data in gimias is only a subset of the original cgns file data,
this class should be considered a partial exporter rather than a "writer".

\author Albert Sanchez
\date 19 May 2010
*/


class BASELIBVTK_EXPORT blCgnsFileWriter : public blLightObject
{
	public:
		//! Typedefs
		typedef blCgnsFileWriter Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );

		//! Sets the filenames for this time step
		void SetFilename(const std::string &fileName);

		//! 
		void SetVTKData(vtkSmartPointer<vtkUnstructuredGrid> gridWithFlowSolution);

		//! Writes the data to the specified file
		void Update();

		//! Set the scalar point data you want to write
		void SetScalarsListPoint(std::vector<std::string> &scalarsNamesToAdd);
		//! Get the scalar point data names
		std::vector<std::string> GetScalarsListPoint();

		//! Set the vector point data you want to write
		void SetVectorsListPoint(std::vector<std::string> &vectorsNamesToAdd);
		//! Get the vector point data names
		std::vector<std::string> GetVectorsListPoint();

		//! Set the scalar point data you want to write
		void SetScalarsListCell(std::vector<std::string> &scalarsNamesToAdd);
		//! Get the scalar point data names
		std::vector<std::string> GetScalarsListCell();

		//! Set the vector point data you want to write
		void SetVectorsListCell(std::vector<std::string> &vectorsNamesToAdd);
		//! Get the vector point data names
		std::vector<std::string> GetVectorsListCell();

	protected:
		//!
		blCgnsFileWriter();
		//!
		virtual ~blCgnsFileWriter();

	private:
		//! Purposely not implemented
		blCgnsFileWriter( const blCgnsFileWriter& );	

		//! Purposely not implemented
		void operator = ( const blCgnsFileWriter& );

		std::string translateFieldName(std::string input, int n, int i);

	private:
		std::vector<std::string> m_scalarsNamesToAddPoint;
		std::vector<std::string> m_vectorsNamesToAddPoint;
		std::vector<std::string> m_scalarsNamesToAddCell;
		std::vector<std::string> m_vectorsNamesToAddCell;

		vtkSmartPointer<vtkUnstructuredGrid> m_gridWithFlowSolution;
		std::string m_fileName;

};

#endif // blCgnsFileWriter_h
