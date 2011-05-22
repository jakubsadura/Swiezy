/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNGToVtkPolydataFilter_h
#define _meNGToVtkPolydataFilter_h


// BaseLib
#include "BaseLibWin32Header.h"

#include "meMeshTypes.h"
#include "meNGBaseMeshFilter.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
using namespace nglib;
/**
\brief convert Netgen Mesh to vtkPolyData surfaces

\author Chiara Riccobene 
\date 29 March 2010
*/

class meNGToVtkPolydataFilter: public meNGBaseMeshFilter
{
	public:
		//! Typedefs
		typedef meNGToVtkPolydataFilter Self;
		typedef blSmartPointer< Self > Pointer;
	
		//! Static constructor.
		blNewMacro( Self );	

		//! converter
		void Update();

		//! returns output 
		vtkSmartPointer<vtkPolyData> GetPolyData();

	protected:
		
		//! brief Default Constructor.
		meNGToVtkPolydataFilter();

		//! Destructor.
		virtual ~meNGToVtkPolydataFilter();

	private:

		//! Purposely not implemented
		meNGToVtkPolydataFilter( const meNGToVtkPolydataFilter& );	

		//! Purposely not implemented
		void operator = ( const meNGToVtkPolydataFilter& );	

		//!
		vtkSmartPointer<vtkPolyData> m_poly;
};


#endif // _meNGToVtkPolydataFilter_h
