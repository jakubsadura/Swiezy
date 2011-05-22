// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreMemoryHandlerTest.h"
#include <new>
#include <iostream>

namespace Private{

void NewHandler();
std::new_handler SetUpNewHandler();

} // Private

namespace Private{

class alloc_error : public std::exception {
public:
	alloc_error() : exception() { }
};

void NewHandler( )
{
	std::cout << "\n\n\nThrowing OutOfMemory!\n\n\n";
	throw alloc_error();
	return;
}

std::new_handler SetUpNewHandler()
{
	return std::set_new_handler(Private::NewHandler);
}

} // Private

void myNewHandler()
{
	// This is your own handler.  It can do anything you want.
	throw Private::alloc_error();
}

void MemoryHandlerTest::TestMemoryHandler()
{
	Private::SetUpNewHandler();
	try
	{		
#ifdef WIN64
		int* i = new int[1000000000];
#endif 
#ifdef WIN32
		int* i = new int[100000000];
#endif 
	}
	catch(Private::alloc_error& e)
	{
		std::cout << e.what();
	}
}
