/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreDataTreeHelper.h"


/**
*/
void Core::DataTreeHelper::PublishOutput( 
	Core::DataEntityHolder::Pointer dataEntityHolder,
	Core::RenderingTree::Pointer tree,
	bool bInitializeViews /*= true*/,
	bool bSetSelected /*=true*/,
	bool bShow /* = true*/)
{

	Core::DataEntity::Pointer dataEntity = dataEntityHolder->GetSubject( );

	if ( dataEntity.IsNotNull() )
	{
		// Add to list and connect holders
		DataEntityHelper::AddDataEntityToList( dataEntityHolder, false );

		if ( tree.IsNotNull()  )
		{
			// Render the generated mesh
			tree->Add( dataEntity, bShow, bInitializeViews );
		}

		// If properly loaded, auto-select it (after rendering it)
		if ( bSetSelected )
		{
			Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
			Core::DataEntityHolder::Pointer holder;
			holder = dataContainer->GetDataEntityList()->GetSelectedDataEntityHolder();

			// In CardiacSegmentationPlugin we need to notify the 
			// QuantificationWidget in order to compute the volume
			// each time the output mesh is modified
			holder->SetSubject( dataEntity, true );
		}
	}

}
