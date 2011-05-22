// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.


#include "cxxtest/TestSuite.h"
#include "SkeletonAPI.h"
#include "nglib.h"

// This test applies only for windows, since the test reproduces a problem with MFC.
#ifdef WIN32

// This 3 lines produces the memory leaks
#undef _WINDOWS_
#define _AFXDLL
#include <afxmt.h>

#endif

/** 
\brief Test memory leaks in MeshLib
\ingroup MeshLibTest
\author Xavi Planes
\date 30-05-08

This test produces memory leaks in VS2003 in debug mode because this
Application is using MFC in a shared DLL (_AFXDLL) and the evoAPI
DLL is using gdcm library that has some static global variables like
datadir. 

To solve this problem I've added the constant NO_PARALLEL_THREADS to
the Netgen project and then the MFC is not linked to the library.

If you remove these lines from this file, you will not have memory leaks:
\code
#undef _WINDOWS_
#define _AFXDLL
#include <afxmt.h>
\endcode

The memory leaks cannot be detected by a memory leak detector tool.

I've sent an email to Microsoft forum:
http://forums.msdn.microsoft.com/en-US/vclanguage/thread/f01f0c72-c3e7-4f47-8df4-1ea3a4e4ad3c

*/
class meTestMemoryLeaks : public CxxTest::TestSuite 
{
public:

	void Test0( )
	{
		nglib::Ng_Init( );
		nglib::Ng_Exit( );
		SkeletonAPI* api = CreateObjectOfSkeletonAPI();
		DestroyObjectOfSkeletonAPI( api );
	}


	void setUp() 
	{ 
		std::cout << std::endl;
	}

	void tearDown() 
	{ 
		// The memory leaks cannot be detected by a memory leak detector tool.
		//#if defined(_DEBUG) && defined(WIN32)
		//	
		//	// We cannot test memory leaks because there are some static instances
		//	// that will be created when the Instance( ) function is called and
		//	// destroyed when application finishes

		//	//// We will have CMemLeakDetect memory leak
		//	//m_memLeakDetect->dumpMemoryTrace( );
		//	//TS_ASSERT( m_memLeakDetect->GetNumMemoryLeaks( ) == 1 );
		//	//std::cout << "Num memory leaks: " <<  m_memLeakDetect->GetNumMemoryLeaks( ) << std::endl;
		//	//delete m_memLeakDetect;
		//	//m_memLeakDetect = NULL;
		//#else
		//	//// This test can only run in DEBUG and WINDOWS
		//	//std::cout 
		//	//	<< "meTestMemoryLeaks Waring: This test can only run in DEBUG and WINDOWS" 
		//	//	<< std::endl;
		//	//TS_ASSERT( true );
		//#endif
	}



private:

};
