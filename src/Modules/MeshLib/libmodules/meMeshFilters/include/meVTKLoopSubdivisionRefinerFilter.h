/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKLoopSubdivisionRefinerFilter_h
#define _meVTKLoopSubdivisionRefinerFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"
#include "meMeshTypes.h"

/**
\brief LoopSubdivisionRefiner for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKLoopSubdivisionRefinerFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKLoopSubdivisionRefinerFilter *New();

		//!
		void SetParams( meMeshLoopSubdivisionParamsPtr params);

	protected:
		
		//! brief Default Constructor.
		meVTKLoopSubdivisionRefinerFilter();

		//! Destructor.
		virtual ~meVTKLoopSubdivisionRefinerFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKLoopSubdivisionRefinerFilter( const meVTKLoopSubdivisionRefinerFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKLoopSubdivisionRefinerFilter& );	


	private:
		//!
		meMeshLoopSubdivisionParamsPtr m_params;
};


#endif // meVTKLoopSubdivisionRefinerFilter_h
