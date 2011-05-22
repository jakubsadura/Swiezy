/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blObject.h"

unsigned long
blObject::GetMTime() const
{
	return m_MTime.GetMTime();
}


/**
* Make sure this object's modified time is greater than all others.
*/
void
blObject::Modified() const
{
	m_MTime.Modified();
}

blObject::blObject()
{
	Modified( );
}

blObject::~blObject()
{

}
