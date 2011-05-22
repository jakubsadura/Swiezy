/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKVolumeClippingFilter_h
#define _meVTKVolumeClippingFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include "vtkImplicitFunction.h"
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"

/**
\brief VolumeClipping for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKVolumeClippingFilter: public vtkPolyDataAlgorithm
{
	public:
		//! 
		static meVTKVolumeClippingFilter *New();
		//!
		void SetImplicitFunction( vtkSmartPointer<vtkImplicitFunction> implicitFunction,
									bool insideOut);

	protected:
		
		//! brief Default Constructor.
		meVTKVolumeClippingFilter();

		//! Destructor.
		virtual ~meVTKVolumeClippingFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKVolumeClippingFilter( const meVTKVolumeClippingFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKVolumeClippingFilter& );	

	private:
		//!
		vtkSmartPointer<vtkImplicitFunction> paramImplicitFunction;
		//!
		bool paramInsideOut;
};


#endif // meVTKVolumeClippingFilter
