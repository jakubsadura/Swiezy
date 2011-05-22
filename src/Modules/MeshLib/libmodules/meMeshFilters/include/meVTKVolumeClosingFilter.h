/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKVolumeClosingFilter_h
#define _meVTKVolumeClosingFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"
#include "meMeshTypes.h"

/**
\brief VolumeClosing for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meVTKVolumeClosingFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKVolumeClosingFilter *New();

		//!
		void SetParams( meMeshVolumeClosingParamsPtr params);

		//!
		meMeshVolumeClosingParamsPtr GetParams( );

	protected:
		
		//! brief Default Constructor.
		meVTKVolumeClosingFilter();

		//! Destructor.
		virtual ~meVTKVolumeClosingFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);


	private:

		//! Purposely not implemented
		meVTKVolumeClosingFilter( const meVTKVolumeClosingFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKVolumeClosingFilter& );	

	private:
		//!
		meMeshVolumeClosingParamsPtr m_params;
};


#endif // _blIDSImageFileReader_h
