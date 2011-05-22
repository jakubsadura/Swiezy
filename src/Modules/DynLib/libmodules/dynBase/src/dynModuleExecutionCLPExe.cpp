/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "dynModuleExecution.h"
#include "dynModuleExecutionCLPExe.h"
#include <sstream>
#include <stdio.h>
#include <time.h>

#include <itksys/RegularExpression.hxx>
#include "itksys/DynamicLoader.hxx"
#include "itkExceptionObject.h"


dynModuleExecutionCLPExe::dynModuleExecutionCLPExe( )
{
}

dynModuleExecutionCLPExe::~dynModuleExecutionCLPExe()
{
}

void dynModuleExecutionCLPExe::BuildCommandLineLocation()
{
	m_CommandLineAsString.clear();

	std::string location;
	std::string target;
	if ( GetModule()->GetType() == "CommandLineModule" )
	{
		location = GetModule( )->GetLocation();
		target = GetModule( )->GetTarget();
	}
	else if ( GetModule()->GetAlternativeType() == "CommandLineModule" )
	{
		location = GetModule( )->GetAlternativeLocation();
		target = GetModule( )->GetAlternativeTarget();
	}

	if ( !location.empty() && location != target )
	{
		m_CommandLineAsString.push_back( location );
	}

	if ( target != "Unknown" )
	{
		m_CommandLineAsString.push_back( target );
	}
}

void dynModuleExecutionCLPExe::RunFilter()
{
	m_Process = itksysProcess_New();
	itksysProcess_SetCommand(m_Process, m_Command);
	if ( !GetWorkingDirectory( ).empty() )
	{
		itksysProcess_SetWorkingDirectory( m_Process, GetWorkingDirectory( ).c_str() );
	}
	itksysProcess_SetOption(m_Process,
		itksysProcess_Option_Detach, 0);
	itksysProcess_SetOption(m_Process,
		itksysProcess_Option_HideWindow, 1);
	itksysProcess_Execute(m_Process);

}

void dynModuleExecutionCLPExe::WaitForData()
{
	int pipe;
	char *tbuffer;
	int length;
	const double timeoutlimit = 0.1;    // tenth of a second
	double timeout = timeoutlimit;
	std::string::size_type tagend;
	std::string::size_type tagstart;
	while ((pipe = itksysProcess_WaitForData(m_Process ,&tbuffer,
		&length, &timeout)) != 0)
	{
		// reset the timeout value 
		timeout = timeoutlimit;

		if (GetModule()->GetProcessInformation()->Abort)
		{
			itksysProcess_Kill(m_Process);
		}

		// Capture the output from the filter
		if (length != 0 && tbuffer != 0)
		{
			if (pipe == itksysProcess_Pipe_STDOUT)
			{
				//std::cout << "STDOUT: " << std::string(tbuffer, length) << std::endl;
				m_stdoutbuffer = m_stdoutbuffer.append(tbuffer, length);

				bool foundTag = false;
				// search for the last occurence of </filter-progress>
				tagend = m_stdoutbuffer.rfind("</filter-progress>");
				if (tagend != std::string::npos)
				{
					tagstart = m_stdoutbuffer.rfind("<filter-progress>");
					if (tagstart != std::string::npos)
					{
						std::string progressString(m_stdoutbuffer, tagstart+17,
							tagend-tagstart-17);
						GetModule( )->GetProcessInformation()->Progress = atof(progressString.c_str());
						foundTag = true;
					}
				}
				// search for the last occurence of </filter-stage-progress>
				tagend = m_stdoutbuffer.rfind("</filter-stage-progress>");
				if (tagend != std::string::npos)
				{
					tagstart = m_stdoutbuffer.rfind("<filter-stage-progress>");
					if (tagstart != std::string::npos)
					{
						std::string progressString(m_stdoutbuffer, tagstart+23,
							tagend-tagstart-23);
						GetModule( )->GetProcessInformation()->StageProgress = atof(progressString.c_str());
						foundTag = true;
					}
				}

				// search for the last occurence of </filter-name>
				tagend = m_stdoutbuffer.rfind("</filter-name>");
				if (tagend != std::string::npos)
				{
					tagstart = m_stdoutbuffer.rfind("<filter-name>");
					if (tagstart != std::string::npos)
					{
						std::string filterString(m_stdoutbuffer, tagstart+13,
							tagend-tagstart-13);
						strncpy(GetModule( )->GetProcessInformation()->ProgressMessage, filterString.c_str(), 1023);
						foundTag = true;
					}
				}

				// search for the last occurence of </filter-comment>
				tagend = m_stdoutbuffer.rfind("</filter-comment>");
				if (tagend != std::string::npos)
				{
					tagstart = m_stdoutbuffer.rfind("<filter-comment>");
					if (tagstart != std::string::npos)
					{
						std::string progressMessage(m_stdoutbuffer, tagstart+16,
							tagend-tagstart-16);
						strncpy (GetModule( )->GetProcessInformation()->ProgressMessage, progressMessage.c_str(), 1023);
						foundTag = true;
					}
				}
				if (foundTag)
				{
					//this->GetApplicationLogic()->RequestModified( node0 );
				}
			}
			else if (pipe == itksysProcess_Pipe_STDERR)
			{
				m_stderrbuffer = m_stderrbuffer.append(tbuffer, length);
			}
		}
	}

	itksysProcess_WaitForExit(m_Process, 0);
}

void dynModuleExecutionCLPExe::ProcessOutputInformation()
{
	// Note that itksys::RegularExpression gives begin()/end() as
	// size_types not iterators. So we need to use the version of
	// erase that takes a position and length to erase.
	//
	itksys::RegularExpression filterProgressRegExp("<filter-progress>[^<]*</filter-progress>[ \t\n\r]*");
	while (filterProgressRegExp.find(m_stdoutbuffer))
	{
		m_stdoutbuffer.erase(filterProgressRegExp.start(),
			filterProgressRegExp.end()
			- filterProgressRegExp.start());
	}
	itksys::RegularExpression filterStageProgressRegExp("<filter-stage-progress>[^<]*</filter-stage-progress>[ \t\n\r]*");
	while (filterStageProgressRegExp.find(m_stdoutbuffer))
	{
		m_stdoutbuffer.erase(filterStageProgressRegExp.start(),
			filterStageProgressRegExp.end()
			- filterStageProgressRegExp.start());
	}
	itksys::RegularExpression filterNameRegExp("<filter-name>[^<]*</filter-name>[ \t\n\r]*");
	while (filterNameRegExp.find(m_stdoutbuffer))
	{
		m_stdoutbuffer.erase(filterNameRegExp.start(),
			filterNameRegExp.end()
			- filterNameRegExp.start());
	}
	itksys::RegularExpression filterCommentRegExp("<filter-comment>[^<]*</filter-comment>[ \t\n\r]*");
	while (filterCommentRegExp.find(m_stdoutbuffer))
	{
		m_stdoutbuffer.erase(filterCommentRegExp.start(),
			filterCommentRegExp.end()
			- filterCommentRegExp.start());
	}
	itksys::RegularExpression filterStartRegExp("<filter-start>[^<]*</filter-start>[ \t\n\r]*");
	while (filterStartRegExp.find(m_stdoutbuffer))
	{
		m_stdoutbuffer.erase(filterStartRegExp.start(),
			filterStartRegExp.end()
			- filterStartRegExp.start());
	}
	itksys::RegularExpression filterEndRegExp("<filter-end>[^<]*</filter-end>[ \t\n\r]*");
	while (filterEndRegExp.find(m_stdoutbuffer))
	{
		m_stdoutbuffer.erase(filterEndRegExp.start(),
			filterEndRegExp.end()
			- filterEndRegExp.start());
	}


	if (m_stdoutbuffer.size() > 0)
	{
		std::string tmp(" standard output:\n\n");
		m_stdoutbuffer.insert(0, GetModule( )->GetTitle()+tmp);
		// vtkSlicerApplication::GetInstance()->InformationMessage
		std::cout << m_stdoutbuffer.c_str();
	}
	if (m_stderrbuffer.size() > 0)
	{
		std::string tmp(" standard error:\n\n");
		m_stderrbuffer.insert(0, GetModule( )->GetTitle()+tmp);
		std::cerr << m_stderrbuffer.c_str();
	}

	// check the exit state / error state of the process
	int result = itksysProcess_GetState(m_Process);
	if (result == itksysProcess_State_Exited)
	{
		// executable exited cleanly and must of done
		// "something" 
		if (itksysProcess_GetExitValue(m_Process) == 0)
		{
			// executable exited without errors,
			std::stringstream information;
			information << GetModule( )->GetTitle()
				<< " completed without errors" << std::endl;
			// vtkSlicerApplication::GetInstance()->InformationMessage
			std::cout << information.str().c_str();

		}
		else
		{
			std::stringstream information;
			information << GetModule( )->GetTitle()
				<< " completed with errors" << std::endl;
			std::cerr << information.str().c_str();
			//node0->SetStatus(vtkMRMLCommandLineModuleNode::CompletedWithErrors, false);
			//this->GetApplicationLogic()->RequestModified( node0 );
		}
	}
	else if (result == itksysProcess_State_Expired)
	{
		std::stringstream information;
		information << GetModule( )->GetTitle()
			<< " timed out" << std::endl;
		std::cerr << information.str().c_str();
		//node0->SetStatus(vtkMRMLCommandLineModuleNode::CompletedWithErrors, false);
		//this->GetApplicationLogic()->RequestModified( node0 );
	}
	else
	{
		std::stringstream information;
		if (result == itksysProcess_State_Exception)
		{
			information << GetModule( )->GetTitle();
			int excResult = itksysProcess_GetExitException(m_Process);
			switch (excResult)
			{
			case itksysProcess_Exception_None:
				information << " terminated with no exceptions." << std::endl;
				break;
			case itksysProcess_Exception_Fault:
				information << " terminated with a fault." << std::endl;
				break;
			case itksysProcess_Exception_Illegal:
				information << " terminated with an illegal instruction." << std::endl;
				break;
			case itksysProcess_Exception_Interrupt:
				information << " terminated with an interrupt." << std::endl;
				break;
			case itksysProcess_Exception_Numerical:
				information << " terminated with a numerical fault." << std::endl;
				break;
			case itksysProcess_Exception_Other:
				information << " terminated with an unknown exception." << std::endl;
				break;
			}
		}
		else if (result == itksysProcess_State_Error)
		{
			information << GetModule( )->GetTitle()
				<< " process was null when state was queried, result (" << result << ") = Error." << std::endl;
		} 
		else
		{
			information << GetModule( )->GetTitle()
				<< " unknown termination. Result = " << result << std::endl;
		}
		std::cerr << information.str().c_str();
		//node0->SetStatus(vtkMRMLCommandLineModuleNode::CompletedWithErrors, false);
		//this->GetApplicationLogic()->RequestModified( node0 );
	}

	// clean up
	itksysProcess_Delete(m_Process);
}

