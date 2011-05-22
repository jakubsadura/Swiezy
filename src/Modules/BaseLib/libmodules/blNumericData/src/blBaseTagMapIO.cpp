/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "blBaseTagMapIO.h"


blBaseTagMapIO::blBaseTagMapIO() 
{
	m_Version = 0;
}

blBaseTagMapIO::~blBaseTagMapIO()
{
}

void blBaseTagMapIO::SetFilename( const char* filename )
{
	m_Filename = filename;
}

void blBaseTagMapIO::SetInput(blTagMap::Pointer tagMap )
{
	m_Data = tagMap;
}

void blBaseTagMapIO::Update()
{
	InternalUpdate();
}

blTagMap::Pointer blBaseTagMapIO::GetOutput()
{
	return m_Data;
}

