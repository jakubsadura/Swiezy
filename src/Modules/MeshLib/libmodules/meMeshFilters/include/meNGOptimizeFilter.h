/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNGOptimizeFilter_h
#define _meNGOptimizeFilter_h


// BaseLib
#include "BaseLibWin32Header.h"

#include "meMeshTypes.h"
#include "meNGBaseMeshFilter.h"

/**
\brief Optimize filter of Netgen

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class meNGOptimizeFilter: public meNGBaseMeshFilter
{
	public:
		//! Typedefs
		typedef meNGOptimizeFilter Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );

		void Update();
		
		//!
		void SetParams( meMeshParamsOptimizePtr params );

		//!
		meMeshParamsOptimizePtr GetParams( );

	protected:
		
		//! brief Default Constructor.
		meNGOptimizeFilter();

		//! Destructor.
		virtual ~meNGOptimizeFilter();

	private:

		//! Purposely not implemented
		meNGOptimizeFilter( const meNGOptimizeFilter& );	

		//! Purposely not implemented
		void operator = ( const meNGOptimizeFilter& );	

	private:
	
		meMeshParamsOptimizePtr m_param;
		
};


#endif // _meNGOptimizeFilter_h
