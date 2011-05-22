/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meVTKBinImageToPolyDataFilter_h
#define _meVTKBinImageToPolyDataFilter_h

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataAlgorithm.h>

// BaseLib
#include "BaseLibWin32Header.h"

/**
\brief Creates a surface VtkPolyData from a binary vtkImageData

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class  meVTKBinImageToPolyDataFilter: public vtkPolyDataAlgorithm
{
	public:
		static meVTKBinImageToPolyDataFilter *New();
		
		void SetIsoValue (double val);
		double GetIsoValue ();

	protected:
		
		//! brief Default Constructor.
		meVTKBinImageToPolyDataFilter();

		//! Destructor.
		virtual ~meVTKBinImageToPolyDataFilter();

		virtual int RequestData(vtkInformation *, 
			vtkInformationVector **, vtkInformationVector *);

		virtual int FillInputPortInformation(int port, vtkInformation *info);

	private:

		//! Purposely not implemented
		meVTKBinImageToPolyDataFilter( const meVTKBinImageToPolyDataFilter& );	

		//! Purposely not implemented
		void operator = ( const meVTKBinImageToPolyDataFilter& );	
		
		//! value for the marching cubes
		double IsoValue;

};


#endif // _meVTKBinImageToPolyDataFilter_h
