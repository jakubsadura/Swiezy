/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKTetraGenerationFilter_h
#define _meVTKTetraGenerationFilter_h

#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"

#include "meMeshTypes.h"
#include "blReaders.h"
#include "blMeshTypes.h"
using namespace base::meshio;
/**
\brief TetraGeneration for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKTetraGenerationFilter: public vtkPolyDataAlgorithm
{
	public:
		//! 
		static meVTKTetraGenerationFilter * New();
		//!
		void SetParams( meMeshTetraGenerationParamsPtr params);
		//!
		meMeshTetraGenerationParamsPtr GetParams( );

		vtkUnstructuredGrid* GetVtkOutput();

	protected:
		
		//! brief Default Constructor.
		meVTKTetraGenerationFilter();

		//! Destructor.
		virtual ~meVTKTetraGenerationFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKTetraGenerationFilter( const meVTKTetraGenerationFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKTetraGenerationFilter& );	

		void CreateOutput(iDataReaderStaticI<dVector3D> *points,
			iDataReaderStaticI<tetrahedron> *tetrahedra);

	private:
		//!
		meMeshTetraGenerationParamsPtr m_params;

		vtkSmartPointer<vtkUnstructuredGrid> m_grid;
};


#endif // _meVTKTetraGenerationFilter_h
