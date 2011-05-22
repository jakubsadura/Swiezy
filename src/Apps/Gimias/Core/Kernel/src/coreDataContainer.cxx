/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataContainer.h"

using namespace Core;

//!
DataContainer::DataContainer(void)
{
	// Initialize the holder for the data entity list
	this->m_DataEntityList = DataEntityList::New();
}

//!
DataContainer::~DataContainer(void)
{
}

//!
DataEntityList::Pointer DataContainer::GetDataEntityList(void) const
{ 
	return static_cast<DataEntityList*>(this->m_DataEntityList.GetPointer());
}

