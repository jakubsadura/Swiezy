/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreLightData.h"

using namespace Core;

LightData::LightData()
{
}

LightData::~LightData()
{
	for(int i=0; i<m_childrenList.size(); i++)
	{
		if(m_childrenList.at(i) != NULL)
			delete m_childrenList.at(i);
	}
}

void LightData::AddChild(LightData* child)
{
	m_childrenList.push_back(child);	
}
