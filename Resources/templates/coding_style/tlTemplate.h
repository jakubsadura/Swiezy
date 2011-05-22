// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef tlTemplate_h
#define tlTemplate_h

// Add the include file of your library, to use the TESTLIB_EXPORT macro
#include "TestLibWin32Header.h"

// baselib
#include "blLightObject.h"

namespace tl{

/** 
\brief Brief description of this class

\ingroup TestLib
\author Your name
\date 11 April 2008
*/
class TESTLIB_EXPORT Template : public blLightObject{

public:
	typedef Template Self;
	typedef blSmartPointer<Self> Pointer;

public:
	//! Constructor
	blNewMacro( Self );

protected:
	//!
	Template( );

	//!
	~Template( );

private:
	//! Purposely not implemented
	Template( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );

private:
	int m_lWidth;

};

}

#endif //tlTemplate_h
