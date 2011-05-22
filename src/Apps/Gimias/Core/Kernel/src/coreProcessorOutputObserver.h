/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreProcessorOutputObserver_H
#define _coreProcessorOutputObserver_H

#include "gmKernelWin32Header.h"
#include "coreObject.h"
#include "coreRenderingTree.h"

namespace Core
{

/** 
\brief Observer of one output DataEntity of a processor. Adds it to the 
data list and to the rendering tree

\ingroup gmKernel
\author Xavi Planes
\date 06 Nov 2009
*/
class GMKERNEL_EXPORT ProcessorOutputObserver : public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro( ProcessorOutputObserver, Core::SmartPointerObject );

	//!
	void SetOutputNumber( int outputNumber );

	//!
	void SetProcessor(Core::BaseFilter::Pointer val);

	//!
	void SetRenderingTree(Core::RenderingTree::Pointer val);

	//!
	void SetHideInput(bool val, int inputNumber = 0 );

	//!
	void SetInitializeViews(bool val);

	//!
	void SelectDataEntity(bool val);

	//!
	void SetAddToDataList(bool val);

	//!
	void SetAddToRederingTree(bool val);

	//! Modify property of current output
	void AddProperty( blTag::Pointer property );

	//!
	void SetOpacityInput( bool val, int inputNumber, double opacity );

	//! Push property to a list, and apply it when the output is modified
	void PushProperty( blTag::Pointer property );

	//! Enable this observer
	bool GetEnable() const;
	void SetEnable(bool val);

private:

	//!
	ProcessorOutputObserver( );

	//!
	~ProcessorOutputObserver( );

	//!
	void AddObservers( );

	//!
	void RemoveObservers( );

	//!
	void OnModifiedOutputDataEntity();

	//!
	void HideInput( );

	//!
	void AddToDataEntityList( );

	//!
	Core::DataEntity::Pointer GetOutputDataEntity( );

	//!
	void SelectDataEntity( );

	//!
	void RenderDataEntity( );

	//!
	void OpacityInput( );

	//!
	void ApplyPropertyList( );

protected:

	//!
	Core::BaseFilter::Pointer m_Processor;

	//!
	Core::RenderingTree::Pointer m_RenderingTree;

	//!
	int m_OutputNumber;

	//! Hide input if is different from output and output is not empty
	bool m_hideInput;

	//!
	int m_HideInputNumber;

	//! Initialize views
	bool m_InitializeViews;

	//! Set selected the added output
	bool m_SetSelected;

	//! Add output to data list
	bool m_AddToDataList;

	//! Add output to rendering tree
	bool m_AddToRederingTree;

	//! Set the input to m_opacity value
	bool m_opacityInput;

	//! opacity value
	double m_opacityValue;

	//!
	blTagMap::Pointer m_PropertyList;

	//!
	bool m_Enable;
};

}

#endif // _coreProcessorOutputObserver_H
