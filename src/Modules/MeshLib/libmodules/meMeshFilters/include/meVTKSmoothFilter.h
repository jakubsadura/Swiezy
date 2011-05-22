/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKSmoothFilter_h
#define _meVTKSmoothFilter_h


#include "vtkPolyDataAlgorithm.h"
#include "meMeshTypes.h"

/**
\brief Smoothing for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class  meVTKSmoothFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKSmoothFilter *New();

		//!
		void SetParams( meMeshSmoothingParamsPtr params);

	protected:
		
		//! brief Default Constructor.
		meVTKSmoothFilter();

		//! Destructor.
		virtual ~meVTKSmoothFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKSmoothFilter( const meVTKSmoothFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKSmoothFilter& );	

	private:

		meMeshSmoothingParamsPtr m_params;
};


#endif // _meVTKSmoothFilter_h
