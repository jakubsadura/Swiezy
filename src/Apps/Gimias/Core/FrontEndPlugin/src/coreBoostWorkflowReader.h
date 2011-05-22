/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BoostWorkflowReader_H
#define _BoostWorkflowReader_H

#include "coreBaseWorkflowIO.h"

namespace Core{

/**
XML Boost reader using serialize. This reader is deprecated because
the format of the XML file is difficult to read for humans.
Please use XMLWorkflowReader.

\code
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<!DOCTYPE boost_serialization>
<boost_serialization signature="serialization::archive" version="5">
<workflow class_id="0" tracking_level="0" version="2">
	<Name>AngioImages</Name>
	<Version>0.0.0</Version>
	<StepVectorSize>3</StepVectorSize>
	<WorkflowStep class_id="1" tracking_level="0" version="1">
		<Name>Visualize &amp; Crop</Name>
		<WindowList class_id="2" tracking_level="0" version="0">
...
\endcode

\author Xavi Planes
\date Nov 2010
\ingroup gmFrontEndPlugin
*/
class BoostWorkflowReader : public BaseWorkflowIO 
{
public:
	coreDeclareSmartPointerClassMacro( BoostWorkflowReader, BaseWorkflowIO );

protected:
	BoostWorkflowReader( );

	~BoostWorkflowReader( );

	//!
	void InternalUpdate();

private:

};

} // Core

#endif //_BoostWorkflowReader_H
