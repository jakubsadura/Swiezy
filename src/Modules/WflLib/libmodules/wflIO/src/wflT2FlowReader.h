/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _wflT2FlowReader_H
#define _wflT2FlowReader_H

#include "WflLibWin32Header.h"
#include "blLightObject.h"
#include "wflIOBase.h"

class TiXmlElement;

/**
Reader for Taverna 2 workflow

\author Xavi Planes
\date 22 Dec 2010
\ingroup WflLib
*/
class WFLLIB_EXPORT wflT2FlowReader : public wflIOBase
{
public:
	typedef wflT2FlowReader Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	wflT2FlowReader( );

	~wflT2FlowReader( );

private:
	//!
	void InternalUpdate( );

	//!
	void ReadPort( TiXmlElement* portElem, std::map<std::string,std::string> &properties );

};


#endif // _wflT2FlowReader_H

