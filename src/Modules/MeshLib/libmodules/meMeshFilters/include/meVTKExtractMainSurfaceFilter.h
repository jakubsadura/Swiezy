/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKExtractMainSurfaceFilter_h
#define _meVTKExtractMainSurfaceFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"

/**
\brief Extract Main Surface for vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class  meVTKExtractMainSurfaceFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKExtractMainSurfaceFilter *New();

	protected:
		
		//! brief Default Constructor.
		meVTKExtractMainSurfaceFilter();

		//! Destructor.
		virtual ~meVTKExtractMainSurfaceFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

	private:

		//! Purposely not implemented
		meVTKExtractMainSurfaceFilter( const meVTKExtractMainSurfaceFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKExtractMainSurfaceFilter& );	

};


#endif // _blIDSImageFileReader_h
