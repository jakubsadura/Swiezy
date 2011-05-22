// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _coreProcessorWidgetsBuilder_H
#define _coreProcessorWidgetsBuilder_H

#include "gmWidgetsWin32Header.h"
#include "coreBaseProcessor.h"
#include "coreObject.h"

class wxSizer;

namespace Core{

namespace Widgets{ 
	class DataEntityListBrowser;
	class ProcessorInputWidget; }

/**
\brief Add AcquiredInputControl widgets to the sizer depending on the
Processor input port properties

\ingroup gmWidgets
\author Xavi Planes
\date 06 11 2009
*/
class GMWIDGETS_EXPORT ProcessorWidgetsBuilder
{
public:

	/**
	\param [in] val Input processor
	\param [in] parent Parent window of the AcquiredInputControl
	\param [in] list Data entity list browser
	*/
	static void Init( 
		Core::BaseFilter::Pointer val,
		Core::BaseWindow* baseWindow,
		Core::Widgets::DataEntityListBrowser* list,
		bool automaticSelection );

	//!
	static Core::Widgets::ProcessorInputWidget* Init( 
		int iPos,
		Core::BaseFilterInputPort::Pointer port,
		wxWindow* parent,
		Core::Widgets::DataEntityListBrowser* list,
		bool automaticSelection );

private:
	//!
	ProcessorWidgetsBuilder( );

	//!
	~ProcessorWidgetsBuilder();

private:
};

} // Core

#endif //_coreProcessorWidgetsBuilder_H
