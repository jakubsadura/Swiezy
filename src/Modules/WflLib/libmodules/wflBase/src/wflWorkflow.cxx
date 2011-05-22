/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "wflWorkflow.h"


wflWorkflow::wflWorkflow()
{

}

wflWorkflow::~wflWorkflow()
{

}

std::vector<wflInputPort::Pointer> &wflWorkflow::GetInputPorts()
{
	return m_InputPorts;
}

std::vector<wflOutputPort::Pointer> &wflWorkflow::GetOutputPorts()
{
	return m_OutputPorts;
}
