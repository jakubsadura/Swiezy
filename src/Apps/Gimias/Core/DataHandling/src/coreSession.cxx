/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreSession.h"

using namespace Core;

Session::Session()
{
}

Session::~Session()
{
	for(int i=0; i<m_dataList.size(); i++)
	{
		if(m_dataList.at(i) != NULL)
			delete m_dataList.at(i);
	}
}

void Session::AddData(LightData* data)
{
	m_dataList.push_back(data);
}
