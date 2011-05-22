/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "coreBoostWorkflowReader.h"
#include "coreWorkflowSerialize.h"

#include <fstream>
#include <boost/archive/xml_iarchive.hpp>


Core::BoostWorkflowReader::BoostWorkflowReader() 
{
}

Core::BoostWorkflowReader::~BoostWorkflowReader()
{
}

void Core::BoostWorkflowReader::InternalUpdate()
{
	std::ifstream file( m_Filename.c_str() );
	if ( !file.is_open() )
	{
		return;
	}

	try
	{
		m_Data = Core::Workflow::New( "" );
		boost::archive::xml_iarchive xmlArchive(file);
		using boost::serialization::make_nvp;
		xmlArchive >> BOOST_SERIALIZATION_NVP( m_Data );
		file.close();
	}
	catch( ... )
	{
		m_Data = NULL;
	}
}
