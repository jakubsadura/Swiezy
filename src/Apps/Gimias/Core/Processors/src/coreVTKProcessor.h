/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreVTKProcessor_H
#define _coreVTKProcessor_H

#include "gmProcessorsWin32Header.h"
#include "coreBaseProcessor.h"
#include "vtkSmartPointer.h"

namespace Core{

/**
Templated Processor for VTK Filters

\ingroup gmProcessors
\author Xavi Planes
\date 19 April 2010
*/
template < class T >
class VTKProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(VTKProcessor, Core::BaseProcessor);
	//!
	void Update();

	/// Return internal filter
	T* GetFilter( );

	//!
	int ComputeNumberOfTimeSteps( );

private:
	/**
	*/
	VTKProcessor( );

	//! Pass input data from input port to vtk filter
	template<class DataType>
	void PassInputData( int portNum, int timeStep );

protected:
	//!
	vtkSmartPointer<T> m_Filter;
};

} // Core

#include "coreVTKProcessor.txx"

#endif //_coreVTKProcessor_H
