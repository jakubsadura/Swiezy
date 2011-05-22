/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CORECLIPPINGPROCESSOR_H
#define CORECLIPPINGPROCESSOR_H

#include "CILabNamespaceMacros.h"
#include "coreDataEntityHolder.h"
#include "coreSmartPointerMacros.h"
#include "coreObject.h"

//VTK
#include "vtkPlane.h"
#include "vtkSmartPointer.h"

namespace Core{

/**
Stores information about the plane used for clipping.
All the information about the clipping plane are stored in plane 
(vtkImplicitFunction)

\ingroup MeshEditorPlugin
\author 
\date dec 2008
*/

class ClippingProcessorData : public Core::SmartPointerObject
{
private:
	ClippingProcessorData();
	~ClippingProcessorData(){};

public:
	coreDeclareSmartPointerClassMacro(
		ClippingProcessorData,
		Core::SmartPointerObject);

	vtkSmartPointer<vtkPlane> m_planeImplFct;
	double m_box[6];
	double m_center[3];
	double m_normal[3];
};


/**
Clip using a the clipping plane.

\author Chiara Riccobene
\date dec 2008
*/

class PLUGIN_EXPORT ClippingProcessor 
	: public Core::SmartPointerObject
{
public:
	coreDeclareSmartPointerClassMacro(
		ClippingProcessor,
		Core::SmartPointerObject);

	typedef Core::DataHolder< Core::ClippingProcessorData::Pointer > 
		ClippingProcessorDataHolder;
	/**
	\param data - Input data for the processor. 
	If the default is used, then the processor will create and use a new
	ClippingProcessorData.
	*/
	ClippingProcessor(
		ClippingProcessorData::Pointer data = ClippingProcessorData::Pointer());
	//!
	ClippingProcessorDataHolder::Pointer GetProcessorDataHolder() const;
	//!
	void SetProcessorDataHolder(ClippingProcessorDataHolder::Pointer val);
	//!
	void Run();
	//!
	void ResetPlane();
	//!
	Core::DataEntityHolder::Pointer GetInputDataEntityHolder() const;
	//!
	void SetInputDataEntityHolder(Core::DataEntityHolder::Pointer val);
	//!
	Core::DataEntityHolder::Pointer GetOutputDataEntityHolder() const;
	//!
	void SetOutputDataEntityHolder(Core::DataEntityHolder::Pointer val);

private:
	//! The cropping parameters
	ClippingProcessorDataHolder::Pointer m_ProcessorDataHolder;
	//! The input to the clipping
	Core::DataEntityHolder::Pointer m_Input;
	//! The output of the clipping
	Core::DataEntityHolder::Pointer m_Output;
};

} // Core

#endif //CORECLIPPINGPROCESSOR_H
