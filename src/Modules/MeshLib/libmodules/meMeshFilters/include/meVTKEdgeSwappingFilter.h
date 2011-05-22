/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKEdgeSwappingFilter_h
#define _meVTKEdgeSwappingFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"
#include "meMeshTypes.h"
/**
\brief EdgeSwapping for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKEdgeSwappingFilter: public vtkPolyDataAlgorithm
{
	public:

		static meVTKEdgeSwappingFilter *New();

		//!
		void SetParams( meMeshEdgeSwappingParamsPtr params);

		//!
		meMeshEdgeSwappingParamsPtr GetParams( );

	protected:
		
		//! brief Default Constructor.
		meVTKEdgeSwappingFilter();

		//! Destructor.
		virtual ~meVTKEdgeSwappingFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKEdgeSwappingFilter( const meVTKEdgeSwappingFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKEdgeSwappingFilter& );	

	private:
		//!
		meMeshEdgeSwappingParamsPtr m_params;
};


#endif // meVTKEdgeSwappingFilter_h
