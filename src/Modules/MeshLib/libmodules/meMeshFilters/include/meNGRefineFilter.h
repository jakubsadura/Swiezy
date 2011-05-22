/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNGRefineFilter_h
#define _meNGRefineFilter_h


// BaseLib
#include "BaseLibWin32Header.h"

#include "meMeshTypes.h"
#include "meNGBaseMeshFilter.h"

/**
\brief Base class Meshing for editing vtkPolyData surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meNGRefineFilter: public meNGBaseMeshFilter
{
	public:
		//! Typedefs
		typedef meNGRefineFilter Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );	

		//!
		void Update();

	protected:
		
		//! brief Default Constructor.
		meNGRefineFilter();

		//! Destructor.
		virtual ~meNGRefineFilter();

	private:

		//! Purposely not implemented
		meNGRefineFilter( const meNGRefineFilter& );	

		//! Purposely not implemented
		void operator = ( const meNGRefineFilter& );	
};


#endif // _meNGRefineFilter_h
