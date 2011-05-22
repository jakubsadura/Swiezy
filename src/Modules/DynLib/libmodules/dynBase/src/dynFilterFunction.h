/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _dynFilterFunction_H
#define _dynFilterFunction_H

#include "DynLibWin32Header.h"
#include <string>
#include <vector>

/**
Generic dynamic Filter function description

It allows to set the name of the function, the parameters and the class 
hierarchy.

For example when calling Delete() function of vtkImageThreshold, the function
is defined in the parent class vtkObjectBase, so we need to iterate on the 
hierarchy to find the correct function.
- vtkImageThreshold::Delete()
- vtkThreadedImageAlgorithm::Delete()
- vtkImageAlgorithm::Delete()
- vtkAlgorithm::Delete()
- vtkObject::Delete()
- vtkObjectBase::Delete()

\author Xavi Planes
\date 14 July 2010
\ingroup DynLib
*/
class dynFilterFunction 
{
public:
	//!
	dynFilterFunction( const std::string name, const std::string hierarchy = "" );

	//!
	dynFilterFunction( const char *name = "" );

	//!
	~dynFilterFunction( );

	//!
	std::string GetName() const;
	void SetName(std::string val);

	//!
	std::string GetHierarcy();
	void AddClass(std::string classname);
	std::string GetClass( int i );

	//!
	std::string& GetParameters();
	void AddParameter(std::string val);

	//!
	std::string GetReturnType();
	void SetReturnType(std::string val);

	/** Remove the last Parenthesis "(int)" -> "(int"
	This function should be called for parameters that we don't know the
	exact type like a pointer to a derived class
	*/
	void OpenParenthesis( );

protected:

private:
	//! Function name
	std::string m_Name;
	//! Function class hierarchy
	std::string m_Hierarcy;
	//! Parameters
	std::string m_Parameters;
	//! Return type
	std::string m_ReturnType;
};



#endif // _dynFilterFunction_H
