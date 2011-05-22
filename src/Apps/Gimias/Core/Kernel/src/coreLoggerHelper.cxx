/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreLoggerHelper.h"
#include "coreKernel.h"
#include "coreReportExceptionMacros.h"
#include "coreWxMitkGraphicalInterface.h"

Core::LoggerHelper::~LoggerHelper()
{

}

void Core::LoggerHelper::ShowMessage( 
						std::string strMessage,
						MessageType type,
						bool showAlsoDialog )
{
	try
	{
		Core::Runtime::wxMitkGraphicalInterface::Pointer graphicalInterface;
		graphicalInterface = Core::Runtime::Kernel::GetGraphicalInterface();

		if ( graphicalInterface.IsNull() )
		{
			return;
		}

		switch ( type )
		{
		case MESSAGE_TYPE_INFO:
			graphicalInterface->ReportMessage( strMessage, showAlsoDialog );
			break;
		case MESSAGE_TYPE_WARNING:
			graphicalInterface->ReportWarning( strMessage, showAlsoDialog );
			break;
		case MESSAGE_TYPE_ERROR:
			graphicalInterface->ReportError( strMessage, showAlsoDialog );
			break;
		}

	}coreCatchExceptionsReportAndNoThrowMacro( 
		"Core::LoggerHelper::ShowMessage" );
}


