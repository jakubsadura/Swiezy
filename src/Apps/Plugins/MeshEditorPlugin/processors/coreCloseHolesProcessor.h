/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreCloseHolesProcessor_H
#define _coreCloseHolesProcessor_H

#include "corePluginMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreBaseFilter.h"
#include "coreBaseProcessor.h"
#include "meCloseHoles.h"

namespace Core{

/**
Close the surface using using meCloseHoles and meHoleFiller.
The user can choose different options.

\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 24 sept 2010
*/

class PLUGIN_EXPORT CloseHolesProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(CloseHolesProcessor, Core::BaseProcessor);
	//!
	void Update();

	//!
	void ComputeHoles();
	//!
	bool CheckInputs();

	//!
	void SetParams (meMeshVolumeClosingParamsPtr param);
	meMeshVolumeClosingParamsPtr GetParams();

	//! see m_closeAll
	void SetCloseAll(bool val);
	
	//! see m_CloseHoleMethod
	void SetMethod( int method);

	//! selects the hole you want to fill in ( with m_closeAll false)
	void SelectHole();

	//! see m_PatchOutput
	void SetPatchOnly( bool val);
	
private:
	/**
	*/
	CloseHolesProcessor( );
	
	//! José-Maria Algorithm to close holes ( methods 0-4)
	meCloseHoles::Pointer	m_meCloseHoles;

	//! parameters for CloseRefineFair algo
	meMeshVolumeClosingParamsPtr m_param;

	//! Choose to fill every hole or to not
	bool m_closeAll;

	//! Method to fill the hole
	int m_CloseHoleMethod;

	//! true returns patch, false returns closed mesh
	bool m_PatchOutput;
	
};

} // Core

#endif //_coreCloseHolesProcessor_H
