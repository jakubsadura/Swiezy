/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKLocalRefinerFilter_h
#define _meVTKLocalRefinerFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"
#include "meMeshTypes.h"


/**
\brief Local Refiner for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKLocalRefinerFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKLocalRefinerFilter *New();

		//!
		void SetParams( meMeshLocalRefinerParamsPtr params);

		//!
		meMeshLocalRefinerParamsPtr GetParams( );

	protected:
		
		//! brief Default Constructor.
		meVTKLocalRefinerFilter();

		//! Destructor.
		virtual ~meVTKLocalRefinerFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKLocalRefinerFilter( const meVTKLocalRefinerFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKLocalRefinerFilter& );	

	private:
		//!
		meMeshLocalRefinerParamsPtr m_params;
};


#endif // _blIDSImageFileReader_h
