/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef coreMitkProperties_H
#define coreMitkProperties_H

#include <mitkBaseProperty.h>
#include <coreObject.h>
#include "gmWidgetsWin32Header.h"

namespace Core{

/**
\brief Template class for generating properties for classes with standard 
pointers, and any other type not covered by the properties provided by mitk.

This class template can be instantiated for all classes/internal types.

Note: you must use the macro mitkSpecializeAnyTypeProperty to provide 
specializations for concrete types (e.g. BoolProperty). You should use 
the coreSpecializeAnyTypePropertyMacro() 
for creating your custom specializations.
\ingroup gmKernel
*/
template <class T>
class PLUGIN_EXPORT AnyTypeProperty : public mitk::BaseProperty
{
public:

	mitkClassMacro(AnyTypeProperty, mitk::BaseProperty);

	itkNewMacro(AnyTypeProperty);

	T GetValue( ) const { return this->m_Value; };

	virtual bool operator==(const mitk::BaseProperty& other) const 
	{
		try
		{
			const Self *otherProp = dynamic_cast<const Self*>(&other);
			if(otherProp == NULL) 
				return false;
			if (this->m_Value == otherProp->m_Value) 
				return true;
		}
		catch (std::bad_cast)
		{
			// nothing to do now - just return false
		}

		return false;
	}

	virtual std::string GetValueAsString(void) const 
	{
		std::stringstream myStr;
		myStr << this->GetValue() ;
		return myStr.str(); 
	}

	virtual bool Assignable(const mitk::BaseProperty& other) const
	{
		try
		{
			// dear compiler, please don't optimize this away!
			dynamic_cast<const Self&>(other); 
			return true;
		}
		catch (std::bad_cast)
		{
			// nothing to do now - just return false
		}
		return false;
	}

	virtual mitk::BaseProperty& operator=(const mitk::BaseProperty& other)
	{
		try
		{
			const Self& otherProp( dynamic_cast<const Self&>(other) );

			if (this->m_Value != otherProp.m_Value)
			{
				this->m_Value = otherProp.m_Value;
				this->Modified();
			}
		}
		catch (std::bad_cast)
		{
			// nothing to do then
		}
		return *this;
	}

protected:
	AnyTypeProperty(T x) : m_Value(x) {}

	virtual ~AnyTypeProperty() {}

	AnyTypeProperty(void) {} 

protected:
	T m_Value;
};



/**
\brief Generates a specialized subclass of AnyTypeProperty. 
This way, GetNameOfClass() returns the value provided by PropertyName.

\ingroup gmKernel
\param PropertyName the name of the instantiation of AnyTypeProperty
\param Type the value type of the AnyTypeProperty
*/
#define coreSpecializeAnyTypePropertyMacro(PropertyName,Type,DefaultValue)  \
class PLUGIN_EXPORT PropertyName: public AnyTypeProperty<Type>			\
{																\
public:															\
  mitkClassMacro(PropertyName, AnyTypeProperty<Type>);			\
  itkNewMacro(PropertyName);									\
  mitkNewMacro1Param(PropertyName, Type);						\
protected:\
  PropertyName(void) : AnyTypeProperty<Type>() { this->m_Value = DefaultValue; };	\
  PropertyName(Type x) : AnyTypeProperty<Type>(x) {};			\
  virtual ~PropertyName(void) {};								\
};


class DataEntity;

/** 
\biref A Property that holds a weak pointer to a DataEntity. It is used to 
link the Rendering Nodes of the RenderingTree
internals, to the mapped DataEntity that they represent. It will allow to 
have a 1 to N mapping between DataEntity
and Rendering Nodes. We do not use a SmartPointerProperty for that because 
we don't want the reference counting
and destroy mechanisms being handled by the Rendering Nodes.

\ingroup gmKernel
\author Juan Antonio Moya
\date 04 Feb 2007
*/
coreSpecializeAnyTypePropertyMacro(DataEntityProperty, DataEntity*, NULL);


} // Core

#endif
