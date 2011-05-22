/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKSkeletonizationFilter_h
#define _meVTKSkeletonizationFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"

#include "meMeshTypes.h"

/**
\brief Skeletonization for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKSkeletonizationFilter: public vtkPolyDataAlgorithm
{
	public:
		
		static meVTKSkeletonizationFilter *New();

		//!
		void SetParams( meMeshSkeletonizationParamsPtr params);

		//!
		meMeshSkeletonizationParamsPtr GetParams( );

	protected:
		
		//! brief Default Constructor.
		meVTKSkeletonizationFilter();

		//! Destructor.
		virtual ~meVTKSkeletonizationFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKSkeletonizationFilter( const meVTKSkeletonizationFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKSkeletonizationFilter& );	

	private:
		//!
		meMeshSkeletonizationParamsPtr m_params;
};


#endif // meVTKSkeletonizationFilter_h
