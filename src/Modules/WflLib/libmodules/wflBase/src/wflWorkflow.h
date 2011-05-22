/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflWorkflow_H
#define _wflWorkflow_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include <map>
#include <vector>

#include "wflProperties.h"
#include "wflInputPort.h"
#include "wflOutputPort.h"

/**
A Workflow with input and output ports

Properties:
- "name"
- "version"
- "producedBy"

\author Xavi Planes
\date 22 Dec 2010
\ingroup WflLib
*/
class WFLLIB_EXPORT wflWorkflow : 
	public blLightObject, 
	public wflProperties
{
public:
	typedef wflWorkflow Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	//!
	std::vector<wflInputPort::Pointer> &GetInputPorts();

	//!
	std::vector<wflOutputPort::Pointer> &GetOutputPorts();

protected:
	//!
	wflWorkflow( );

	//!
	~wflWorkflow( );

protected:
	//!
	std::vector<wflInputPort::Pointer> m_InputPorts;

	//!
	std::vector<wflOutputPort::Pointer> m_OutputPorts;
};

#endif //_wflWorkflow_H
