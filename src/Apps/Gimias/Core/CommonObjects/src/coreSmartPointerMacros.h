/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreSmartPointer_H
#define coreSmartPointer_H

#include "coreCommonMacros.h"
#include <itkSmartPointer.h>

//! Definitions for allowing SWIG wrap the smart pointer members
#ifdef coreDeclareSwigWrappableSmartPointerTypeMacro
#undef coreDeclareSwigWrappableSmartPointerTypeMacro
#endif
#ifdef SWIG_WRAPPING
	#define coreDeclareSwigWrappableSmartPointerTypeMacro(classname) classname*
#else
	#define coreDeclareSwigWrappableSmartPointerTypeMacro(classname) itk::SmartPointer<classname>
#endif


#ifdef coreSmartPtrType
#undef coreSmartPtrType
#endif
/**
\brief Short name for coreDeclareSwigWrappableSmartPointerTypeMacro, 
that is used not only for making swig able to wrap itk smartpointers, 
but also (and as its main use) to allow forward declarations by avoiding 
the use of ::Pointer member 
\ingroup gmCommonObjects
*/
#define coreSmartPtrType(classname) coreDeclareSwigWrappableSmartPointerTypeMacro(classname)

//! refers to the itkFactorylessNewMacro
#define coreFactorylessNewMacro(x) itkFactorylessNewMacro(x)

//! It creates a New() method with two parameters of type given as input to this macro
#define coreFactorylessNewMacro1Param(x,param1) \
	static Pointer New(param1 p1) \
{ \
	Pointer smartPtr; \
	x *rawPtr = new x(p1); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

//! It creates a New() method with two parameters of type given as input to this macro
#define coreFactorylessNewMacro2Param(x,param1,param2) \
	static Pointer New(param1 p1, param2 p2) \
{ \
	Pointer smartPtr; \
	x *rawPtr = new x(p1, p2); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

//! It creates a New() method with two parameters of type given as input to this macro
#define coreFactorylessNewMacro3Param(x,param1,param2,param3) \
	static Pointer New(param1 p1, param2 p2, param3 p3) \
{ \
	Pointer smartPtr; \
	x *rawPtr = new x(p1, p2, p3); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

//! It creates a New() method with two parameters of type given as input to this macro
#define coreFactorylessNewMacro4Param(x,param1,param2,param3,param4) \
	static Pointer New(param1 p1, param2 p2, param3 p3, param4 p4) \
{ \
	Pointer smartPtr; \
	x *rawPtr = new x(p1, p2, p3, p4); \
	smartPtr = rawPtr; \
	rawPtr->UnRegister(); \
	return smartPtr; \
}

//! Overrides the itkNewMacro
#define coreWithFactoryNewMacro(className, factoryName) \
static Pointer New(void) \
{ \
	Pointer smartPtr = factoryName::CreateInstance(); \
	if(smartPtr.GetPointer() == NULL) \
		{ \
		smartPtr = new x; \
		} \
	smartPtr->UnRegister(); \
	return smartPtr; \
} \
virtual className::Pointer CreateAnother(void) const \
{ \
	className::Pointer smartPtr; \
	smartPtr = x::New().GetPointer(); \
	return smartPtr; \
}

/** 
\def coreDeclareSmartPointerTypesMacro 
\ingroup gmCommonObjects
This macro allows the definition of the types used by smart pointer classes. 
Remember to place this macro at the public section of your class. 
*/
#define coreDeclareSmartPointerTypesMacro(className,SuperClassName) \
typedef className               Self; \
typedef SuperClassName          Superclass; \
typedef itk::SmartPointer<Self> Pointer; \
typedef itk::SmartPointer<const Self>  ConstPointer;
	

/** 
\def coreDeclareSmartPointerClassMacro 
\ingroup gmCommonObjects
This macro allows the definition of the current class as handled by 
SmartPointers, and remember to place this macro at the public section of 
your class
*/
#define coreDeclareSmartPointerClassMacro(className,SuperClassName) \
coreDeclareSmartPointerTypesMacro(className,SuperClassName) \
coreFactorylessNewMacro(className) \
coreClassNameMacro(className)


/** 
\def coreDeclareSmartPointerClassMacro1Param 
\ingroup gmCommonObjects
This macro allows the definition of the current class as handled by 
SmartPointers, and remember to place this macro at the public section of your class.
It creates a New() method with one parameter of type given as input to this macro
*/
#define coreDeclareSmartPointerClassMacro1Param(className,SuperClassName,param1) \
coreDeclareSmartPointerTypesMacro(className,SuperClassName) \
coreFactorylessNewMacro1Param(className,param1) \
coreClassNameMacro(className)

/** 
\def coreDeclareSmartPointerClassMacro2Param
\ingroup gmCommonObjects
This macro allows the definition of the current class as handled by 
SmartPointers, and remember to place this macro at the public section of your class.
It creates a New() method with two parameters of type given as input to this macro
*/
#define coreDeclareSmartPointerClassMacro2Param(className,SuperClassName,param1,param2) \
coreDeclareSmartPointerTypesMacro(className,SuperClassName) \
coreFactorylessNewMacro2Param(className,param1,param2) \
coreClassNameMacro(className)

#define coreDeclareSmartPointerClassMacro3Param(className,SuperClassName,param1,param2,param3) \
	coreDeclareSmartPointerTypesMacro(className,SuperClassName) \
	coreFactorylessNewMacro3Param(className,param1,param2,param3) \
	coreClassNameMacro(className)

#define coreDeclareSmartPointerClassMacro4Param(className,SuperClassName,param1,param2,param3,param4) \
	coreDeclareSmartPointerTypesMacro(className,SuperClassName) \
	coreFactorylessNewMacro4Param(className,param1,param2,param3,param4) \
	coreClassNameMacro(className)

/** 
\def coreDeclareNoCopyConstructors
\ingroup gmCommonObjects
As long as we're using itk::SmartPointers, it is not possible to 
define copy-constructors. They're listed here in order to remind it. 
It is recommended to do the same on child classes, and remember to
place this macro at the private section of your class
*/
#define coreDeclareNoCopyConstructors(className) \
  className(const Self&); \
  void operator=(const Self&);

#endif
