/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNGSmoothFilter_h
#define _meNGSmoothFilter_h

// BaseLib
#include "BaseLibWin32Header.h"
#include "meNGBaseMeshFilter.h"
#include "meMeshTypes.h"
using namespace nglib;
/**
\brief Netgen filter for smoothing surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meNGSmoothFilter: public meNGBaseMeshFilter
{
	public:
		//! Typedefs
		typedef meNGSmoothFilter Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );

		//!
		void Update();
		//!
		void SetParams( meMeshSmoothingParamsPtr params );

	protected:
		
		//! brief Default Constructor.
		meNGSmoothFilter();

		//! Destructor.
		virtual ~meNGSmoothFilter();

	private:

		//! Purposely not implemented
		meNGSmoothFilter( const meNGSmoothFilter& );	

		//! Purposely not implemented
		void operator = ( const meNGSmoothFilter& );	

	private:

		Ng_Meshing_Parameters* m_param;
};


#endif // _meNGSmoothFilter_h
