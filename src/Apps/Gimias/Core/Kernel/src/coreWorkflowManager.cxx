/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreWorkflowManager.h"
#include "coreSettings.h"
#include "coreKernel.h"
#include "coreWorkflowSerialize.h"
#include "coreFrontEndPluginManager.h"
#include "coreWxMitkGraphicalInterface.h"
#include "coreWorkflowReader.h"
#include "coreWorkflowWriter.h"

const std::string STR_WORKFLOW_FOLDER = "Workflows";
#include "itksys/SystemTools.hxx"

Core::WorkflowManager::WorkflowManager( )
{
	m_ActiveWorkflowHolder = WorkflowHolderType::New();

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	m_WorkflowsFolder = settings->GetProjectHomePath() + Core::IO::SlashChar + STR_WORKFLOW_FOLDER;
}

void Core::WorkflowManager::AddWorkflow( Workflow::Pointer workflow )
{
	if ( GetWorkflow( workflow->GetName() ) )
	{
		std::ostringstream strError;
		strError << "A workflow with the name " << workflow->GetName() << " already exists!";
		throw Exceptions::Exception( "WorkflowManager::AddWorkflow", strError.str().c_str() );
	}
	
	m_WorkflowList.push_back( workflow );
	SaveWorkflow( workflow );
}

Core::WorkflowManager::WorkflowHolderType::Pointer 
Core::WorkflowManager::GetActiveWorkflowHolder() const
{
	return m_ActiveWorkflowHolder;
}

Core::Workflow::Pointer Core::WorkflowManager::GetActiveWorkflow() const
{
	return m_ActiveWorkflowHolder->GetSubject( );
}

void Core::WorkflowManager::SetActiveWorkflow( const std::string &name )
{
	Core::Workflow::Pointer workflow = GetWorkflow( name );

	// Load required plugins
	if ( workflow.IsNotNull() )
	{
		Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
		settings->SetActiveWorkflow( workflow->GetName() );
		settings->SetPerspective( Core::Runtime::PERSPECTIVE_WORKFLOW );
	}

	// Notify observers
	m_ActiveWorkflowHolder->SetSubject( workflow );
}

void Core::WorkflowManager::Initialize()
{
	CreateWorkflowsFolder();

	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	ScanDirectory( settings->GetProjectHomePath() );

	if ( settings->GetPerspective() == Core::Runtime::PERSPECTIVE_WORKFLOW )
	{
		SetActiveWorkflow( settings->GetActiveWorkflow() );
	}
}

void Core::WorkflowManager::ScanDirectory( const std::string &projectPath )
{
	std::string path = projectPath + Core::IO::SlashChar + STR_WORKFLOW_FOLDER;

	Core::IO::Directory::Pointer workflowsDir = Core::IO::Directory::New();
	workflowsDir->SetDirNameFullPath( path );

	Core::IO::FileNameList list;
	list = workflowsDir->GetContents( );

	Core::IO::FileNameList::iterator it;
	for ( it = list.begin() ; it != list.end() ; it++ )
	{
		Core::Workflow::Pointer workflow = OpenWorkflow( (*it) );
		if ( workflow.IsNotNull() && GetWorkflow( workflow->GetName() ).IsNull() )
		{
			m_WorkflowList.push_back( workflow );
		}
	}
	
}

void Core::WorkflowManager::SaveAll()
{
	WorkflowListType::iterator it;
	for ( it = m_WorkflowList.begin() ; it != m_WorkflowList.end() ; it++ )
	{
		SaveWorkflow( *it );
	}

}

Core::WorkflowManager::~WorkflowManager()
{
	SaveAll();
}

Core::WorkflowManager::WorkflowListType Core::WorkflowManager::GetWorkflowList() const
{
	return m_WorkflowList;
}

Core::Workflow::Pointer Core::WorkflowManager::GetWorkflow( const std::string &name )
{
	WorkflowListType::iterator it;
	for ( it = m_WorkflowList.begin() ; it != m_WorkflowList.end() ; it++ )
	{
		Core::Workflow::Pointer workflow = *it;
		if ( workflow->GetName() == name )
		{
			return workflow;
		}
	}

	return NULL;
}

void Core::WorkflowManager::RemoveWorkflow( const std::string &name )
{
	Core::Workflow::Pointer workflow;
	workflow = GetWorkflow( name );
	if ( workflow.IsNull() )
	{
		return;
	}

	WorkflowListType::iterator it;
	it = std::find( m_WorkflowList.begin(), m_WorkflowList.end(), workflow );
	if ( it == m_WorkflowList.end() )
	{
		return;
	}

	m_WorkflowList.erase( it );

	// Delete from disk
	Core::IO::File::Pointer file = Core::IO::File::New( );
	file->SetFileNameFullPath( GetWorkflowFileName( workflow->GetName() ) );
	file->Delete( );
}

void Core::WorkflowManager::SaveWorkflow( Core::Workflow::Pointer workflow )
{
	try
	{
		Core::WorkflowWriter::Pointer writer = Core::WorkflowWriter::New( );
		writer->SetFilename( GetWorkflowFileName( workflow->GetName() ).c_str() );
		writer->SetInput( workflow );
		writer->Update();
	}
	coreCatchExceptionsLogAndNoThrowMacro(WorkflowManager::SaveWorkflow);
}

Core::Workflow::Pointer Core::WorkflowManager::OpenWorkflow( const std::string &path )
{
	Core::Workflow::Pointer workflow;

	try
	{
		Core::WorkflowReader::Pointer reader = Core::WorkflowReader::New( );
		reader->SetFilename( path.c_str() );
		reader->Update();
		workflow = reader->GetOutput();

		UpdateOldWorkflow( workflow );
	}
	catch(...)
	{
		return NULL;
	}

	return workflow;
}

void Core::WorkflowManager::CreateWorkflowsFolder()
{
	Core::IO::Directory::Pointer workflowsDir = Core::IO::Directory::New();
	workflowsDir->SetDirNameFullPath( m_WorkflowsFolder );

	// Create dest dir
	workflowsDir->Create();

	// Scan source dir
	Core::Runtime::Settings::Pointer settings = Core::Runtime::Kernel::GetApplicationSettings();
	Core::IO::Directory::Pointer sourceWorkflowsDir = Core::IO::Directory::New();
	sourceWorkflowsDir->SetDirNameFullPath( 
		settings->GetResourcePath() + Core::IO::SlashChar + "Workflows" );

	Core::IO::FileNameList list;
	list = sourceWorkflowsDir->GetContents( );

	Core::IO::FileNameList::iterator it;
	for ( it = list.begin() ; it != list.end() ; it++ )
	{
		std::string dest = GetWorkflowsFolder( ) + 
			Core::IO::SlashChar + itksys::SystemTools::GetFilenameName( *it );
		if ( !itksys::SystemTools::FileExists( dest.c_str() ) )
		{
			itksys::SystemTools::CopyAFile( it->c_str(), dest.c_str() );
		}
	}
}

std::string Core::WorkflowManager::GetWorkflowsFolder() const
{
	return m_WorkflowsFolder;
}

std::string Core::WorkflowManager::GetWorkflowFileName( const std::string &name )
{
	return GetWorkflowsFolder( ) + Core::IO::SlashChar + name + ".xml";
}

void Core::WorkflowManager::RenameWorkflow( const std::string &name, const std::string &newName )
{
	if ( GetWorkflow( newName ) )
	{
		std::ostringstream strError;
		strError << "A workflow with the name " << newName << " already exists!";
		throw Exceptions::Exception( "WorkflowManager::AddWorkflow", strError.str().c_str() );
	}

	Core::Workflow::Pointer workflow;
	workflow = GetWorkflow( name );
	if ( workflow.IsNull() )
	{
		return;
	}

	Core::IO::File::Pointer file = Core::IO::File::New( );
	file->SetFileNameFullPath( GetWorkflowFileName( workflow->GetName() ) );
	file->Delete( );

	workflow->SetName( newName );

	SaveWorkflow( workflow );
}

void Core::WorkflowManager::UpdateOldWorkflow( Core::Workflow::Pointer workflow )
{
	if ( workflow.IsNull() )
	{
		return;
	}

	Core::Workflow::StepListType steps = workflow->GetStepVector();
	Core::Workflow::StepListType::iterator it;
	for ( it = steps.begin() ; it != steps.end( ) ; it++ )
	{
		WorkflowStep::WindowListType::iterator itWin;
		for ( itWin = (*it)->GetWindowList().begin();
			itWin != (*it)->GetWindowList().end();
			itWin++ )
		{
			if ( *itWin == "class Core::Widgets::UserHelper")
			{
				break;
			}
		}

		if ( itWin != (*it)->GetWindowList().end() )
		{
			(*it)->GetWindowList().erase( itWin );
		}
	}
}
