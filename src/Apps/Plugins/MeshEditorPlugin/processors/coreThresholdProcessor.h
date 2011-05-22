/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreThresholdProcessor_H
#define _coreThresholdProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "CILabExceptionMacros.h"

namespace Core{

/**
Threshold of the surface.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 4 nov 2009
*/

class PLUGIN_EXPORT ThresholdProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(ThresholdProcessor, Core::BaseProcessor);

	//!
	cilabDeclareExceptionMacro( Exception, std::exception );

	//!
	void Update();

	const char* GetScalarsArrayName() const;
	void SetScalarsArrayName( const char* name );

	unsigned int GetLowerThreshold() const;
	void SetLowerThreshold(unsigned int val);

	unsigned int GetUpperThreshold() const;
	void SetUpperThreshold(unsigned int val);

private:
	/**
	*/
	ThresholdProcessor( );

	//!
	std::string	 m_ScalarsArrayName;

	//!
	unsigned int m_LowerThreshold;

	//!
	unsigned int m_UpperThreshold;
};

} // Core

#endif //_coreThresholdProcessor_H
