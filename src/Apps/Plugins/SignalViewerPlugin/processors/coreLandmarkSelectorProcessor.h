/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef coreLandmarkSelectorProcessor_H
#define coreLandmarkSelectorProcessor_H

#include "corePointInteractorPointSelect.h"
#include "corePluginMacros.h"
#include "blSignal.h"
#include "coreBaseFilter.h"

namespace Core
{
/**
\brief A processor for plotting a signal containing the scalars for a given CELLID

\ingroup SignalViewerPlugin
\author Martin Bianculli
\date  September 2009

*/
class PLUGIN_EXPORT LandmarkSelectorProcessor : public Core::BaseFilter
{
public:
	coreDeclareSmartPointerClassMacro(LandmarkSelectorProcessor, Core::BaseFilter);

	//!
	void Update( );

	//! Set the interactor that holds the cell id
	void SetInteractor(Core::PointInteractorPointSelect::Pointer interactor);


protected:
	LandmarkSelectorProcessor(void);
	virtual ~LandmarkSelectorProcessor(void);

	blSignal::Pointer CreateScalarSignal();

private:
	Core::PointInteractorPointSelect::Pointer m_PointInteractor;


private:
	coreDeclareNoCopyConstructors(LandmarkSelectorProcessor);

};

} // namespace Core

#endif

