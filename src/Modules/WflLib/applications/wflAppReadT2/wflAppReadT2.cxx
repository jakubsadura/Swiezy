/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "wflWorkflowFactory.h"

typedef std::map<std::string,std::string> map_ss;

std::ostream &operator<<(std::ostream& os,const map_ss& paramMap) 
{
	for ( map_ss::const_iterator it = paramMap.begin() ; it != paramMap.end() ; it++ )
	{
		os << it->first.c_str( ) << " :" << it->second.c_str( ) << std::endl;
	}
	return os;
}

//std::ostream &operator<<(std::ostream& os, const wflWorkflow &w) {os << static_cast<map_ss>(w);return os;}
//std::ostream &operator<<(std::ostream& os, const wflInputPort &i) {os << i;return os;}


int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: <workflow path>" << std::endl;
		return EXIT_FAILURE;
	}


	try
	{
		wflWorkflowFactory::Pointer factory = wflWorkflowFactory::New( );
		factory->SetScanDirectory( argv[1] );
		factory->Update();
		wflWorkflowFactory::WorkflowMapType workflows = factory->GetWorkflows();

		wflWorkflowFactory::WorkflowMapType::iterator it;
		for ( it = workflows.begin() ; it != workflows.end() ; it++ )
		{
			wflWorkflow::Pointer workflow = it->second;

			std::cout << workflow->GetProperties( ) << std::endl;
			for ( size_t i = 0 ; i < workflow->GetInputPorts().size() ; i++ )
			{
				std::cout << "Input port:" << std::endl;
				std::cout << "  " << workflow->GetInputPorts()[ i ]->GetProperties( ) << std::endl;
			}

			for ( size_t i = 0 ; i < workflow->GetOutputPorts().size() ; i++ )
			{
				std::cout << "Output port:" << std::endl;
				std::cout << "  " << workflow->GetOutputPorts()[ i ]->GetProperties( ) << std::endl;
			}

		}

	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}


