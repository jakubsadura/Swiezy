/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wflWorkflowFactory.h"
#include "wflT2FlowReader.h"
#include "itksys/Directory.hxx"
#include "blTextUtils.h"

wflWorkflowFactory::wflWorkflowFactory()
{

}

wflWorkflowFactory::~wflWorkflowFactory()
{

}

void wflWorkflowFactory::SetScanDirectory( const std::string &path )
{
	m_SearchPaths = path;
}

void wflWorkflowFactory::Update()
{
	if ( m_SearchPaths.empty() )
	{
		return;
	}

	std::list<std::string> modulePaths;
#ifdef _WIN32
	char delim = ';';
#else
	char delim = ':';
#endif
	blTextUtils::ParseLine( m_SearchPaths, delim, modulePaths);

	std::list<std::string>::const_iterator pit;
	for (pit = modulePaths.begin(); pit != modulePaths.end(); ++pit)
	{
		itksys::Directory directory;
		directory.Load( (*pit).c_str() );
		for ( unsigned int ii=0; ii < directory.GetNumberOfFiles(); ++ii)
		{
			const char *filename = directory.GetFile(ii);

			try
			{
				// library name 
				std::string fullLibraryPath = std::string(directory.GetPath())
					+ "/" + filename;

				wflT2FlowReader::Pointer reader = wflT2FlowReader::New( );
				reader->SetFilename( fullLibraryPath.c_str() );
				reader->Update();

				wflWorkflow::Pointer workflow = reader->GetOutput();
				if ( workflow.IsNull() )
				{
					continue;
				}

				if ( m_Workflows.find( workflow->GetProperty( "name" ) ) != m_Workflows.end() )
				{
					continue;
				}

				m_Workflows[ workflow->GetProperty( "name" ) ] = workflow;
			}
			catch (...)
			{

			}
		}
	}
}

wflWorkflowFactory::WorkflowMapType & wflWorkflowFactory::GetWorkflows()
{
	return m_Workflows;
}
