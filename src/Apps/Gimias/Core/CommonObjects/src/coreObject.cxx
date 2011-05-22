/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreObject.h"

using namespace Core;

Core::Object::~Object()
{

}

Core::SmartPointerObject::SmartPointerObject( void ) : itk::LightObject()
{

}

Core::SmartPointerObject::~SmartPointerObject( void )
{

}

/**
* Return the modification for this object.
*/
unsigned long Core::SmartPointerObject::GetMTime() const
{
	return m_MTime.GetMTime();
}


/**
* Make sure this object's modified time is greater than all others.
*/
void
Core::SmartPointerObject::Modified() const
{
	m_MTime.Modified();
	m_OnModified( );
	m_DataModifiedSignal1( (SmartPointerObject*) this );
}
