/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynFilter_H
#define _dynFilter_H

#include "DynLibWin32Header.h"

#include <string>
#include <list>

#include "dynLib.h"
#include "dynFilterFunction.h"

#include "blLightObject.h"
#include "blSmartPointer.h"

class dynFilterIFace;

/**
Generic dynamic Filter execution

Executes a processing filter located in a dynamic library
providing the function declarations.

\sec Example

For example, to execute vtkImageThreshold, you should use this function names:
- vtkImageThreshold::New()
- vtkImageThreshold::ThresholdByUpper(double)
- vtkImageThreshold::ThresholdByLower(double)
- vtkImageThreshold::SetOutValue(double)
- vtkImageAlgorithm::SetInput(int...
- vtkAlgorithm::Update()
- vtkImageAlgorithm::GetOutput(int)
- vtkObjectBase::Delete()

\author Xavi Planes
\date 09 July 2010
\ingroup DynLib
*/
class DYNLIB_EXPORT dynFilter : public blLightObject
{
public:
	typedef dynFilter Self;
	typedef blSmartPointer<Self> Pointer;

	//!
	void Init( 
		const std::string &filterName, 
		dynFilterFunction funcNameConstructor, 
		dynFilterFunction funcNameDestructor, 
		const char* filterDynLib );

	//!
	virtual void SetBool(dynFilterFunction func, bool val);
	virtual void SetInt(dynFilterFunction func, int val);
	virtual void SetUInt(dynFilterFunction func, int val);
	virtual void SetFloat(dynFilterFunction func, float val);
	virtual void SetDouble(dynFilterFunction func, double val);
	virtual void SetString(dynFilterFunction func, const char* val);
	virtual void SetPoint(dynFilterFunction func, double,double,double);

	virtual void SetInput(dynFilterFunction func, int num, void* input );
	virtual void Update(dynFilterFunction func );
	virtual void* GetOutput(dynFilterFunction func, int num );

	//!
	virtual dynFilterFunction GetFunctionSetInput( ) = 0;
	virtual dynFilterFunction GetFunctionGetOutput( ) = 0;


protected:
	//!
	dynFilter( );

	//!
	virtual ~dynFilter( );

	//!
	virtual void NewInstance( ) = 0;

	//!
	virtual void Delete( ) = 0;

	//!
	virtual dynFilterIFace* GetInstance( ) = 0;

	//!
	dynLib::SymbolPointer FindSymbol( dynFilterFunction func );

protected:
	//!
	std::string m_Name;
	//!
	dynFilterFunction m_FuncNameConstructor;
	//!
	dynFilterFunction m_FuncNameDestructor;
	//!
	dynLib* m_DynamicLib;
};



#endif // _dynFilter_H

