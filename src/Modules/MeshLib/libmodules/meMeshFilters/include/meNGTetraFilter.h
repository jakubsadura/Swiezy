/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNGTetraFilter_h
#define _meNGTetraFilter_h


// BaseLib
#include "BaseLibWin32Header.h"

#include "meMeshTypes.h"
#include "meNGBaseMeshFilter.h"
#include "vtkSmartPointer.h"
#include "vtkUnstructuredGrid.h"


/**
\brief Tetra filter of Netgen

\author Chiara Riccobene 
\date 2 Feb 2010
*/

class meNGTetraFilter: public meNGBaseMeshFilter
{
	public:
		//! Typedefs
		typedef meNGTetraFilter Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );

		void Update();
		
		//!
		void SetOptParams( meMeshParamsOptimizePtr params );

		//!
		meMeshParamsOptimizePtr GetOptParams( );

		vtkSmartPointer<vtkUnstructuredGrid> GetVTK();

		//!
		void SetParams( meMeshTetraGenerationParamsPtr params);

		meMeshTetraGenerationParamsPtr GetParams( );

	protected:
		
		//! brief Default Constructor.
		meNGTetraFilter();

		//! Destructor.
		virtual ~meNGTetraFilter();

	private:

		//! Purposely not implemented
		meNGTetraFilter( const meNGTetraFilter& );	

		//! Purposely not implemented
		void operator = ( const meNGTetraFilter& );	

	private:
	
		meMeshParamsOptimizePtr m_param;

		meMeshTetraGenerationParamsPtr m_params;

		vtkSmartPointer<vtkUnstructuredGrid> m_volume;
		
};


#endif // _meNGTetraFilter_h
