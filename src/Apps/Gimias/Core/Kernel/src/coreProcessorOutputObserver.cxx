/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreProcessorOutputObserver.h"

#include "coreReportExceptionMacros.h"
#include "coreDataContainer.h"
#include "coreKernel.h"

Core::ProcessorOutputObserver::ProcessorOutputObserver()
{
	m_OutputNumber = 0;
	m_hideInput = true;
	m_HideInputNumber = 0;
	m_InitializeViews = true;
	m_SetSelected = true;
	m_AddToDataList = true;
	m_AddToRederingTree = true;
	m_opacityInput = false;
	m_opacityValue = 1;
	m_PropertyList = blTagMap::New();
	m_Enable = true;
}

Core::ProcessorOutputObserver::~ProcessorOutputObserver()
{
	RemoveObservers( );
}

void Core::ProcessorOutputObserver::SetProcessor( Core::BaseFilter::Pointer val )
{
	RemoveObservers( );
	
	m_Processor = val;

	AddObservers();
}

void Core::ProcessorOutputObserver::SetRenderingTree( Core::RenderingTree::Pointer val )
{
	m_RenderingTree = val;
}

void Core::ProcessorOutputObserver::AddObservers()
{
	m_Processor->GetOutputDataEntityHolder( m_OutputNumber )->AddObserver( 
		this, 
		&ProcessorOutputObserver::OnModifiedOutputDataEntity );
}

void Core::ProcessorOutputObserver::RemoveObservers()
{
	if ( m_Processor.IsNull() )
	{
		return;
	}

	m_Processor->GetOutputDataEntityHolder( m_OutputNumber )->RemoveObserver( 
		this, 
		&ProcessorOutputObserver::OnModifiedOutputDataEntity );
}

void Core::ProcessorOutputObserver::OnModifiedOutputDataEntity()
{
	try{

		if ( GetOutputDataEntity( ).IsNull() || !m_Enable )
		{
			return;
		}

		// We need to hide input after we add the child data entity
		// because by default it renders the father automatically
		HideInput( );

		AddToDataEntityList( );

		RenderDataEntity();

		SelectDataEntity();

		ApplyPropertyList( );
	}
	coreCatchExceptionsLogAndNoThrowMacro( 
		"CardiacInitializationPanelWidget::OnModifiedOutputDataEntity")

}

void Core::ProcessorOutputObserver::HideInput()
{
	if ( !m_hideInput || m_Processor->GetNumberOfInputs() == 0 )
	{
		if (m_opacityInput)
			OpacityInput();
		return;
	}

	Core::DataEntity::Pointer inputDataEntity;
	inputDataEntity = m_Processor->GetInputDataEntity( m_HideInputNumber );
	// Hide input if is different from output and output is not empty
	if ( m_Processor->GetOutputDataEntity( m_OutputNumber ) != inputDataEntity
		 && m_RenderingTree.IsNotNull() )
	{
		m_RenderingTree->Show( inputDataEntity, false );
	}

}

void Core::ProcessorOutputObserver::OpacityInput()
{
	if ( m_Processor->GetNumberOfInputs() == 0 )
	{
		return;
	}

	Core::DataEntity::Pointer inputDataEntity;
	inputDataEntity = m_Processor->GetInputDataEntity( m_HideInputNumber );
	
	if ( m_RenderingTree.IsNull() )
		return;

	m_RenderingTree->SetProperty( inputDataEntity, blTag::New( "opacity", m_opacityValue ) );
}

void Core::ProcessorOutputObserver::SetOutputNumber( int outputNumber )
{
	m_OutputNumber = outputNumber;
}

void Core::ProcessorOutputObserver::SetHideInput( bool val, int inputNumber )
{
	m_hideInput = val;
	m_HideInputNumber = inputNumber;
}

void Core::ProcessorOutputObserver::SetOpacityInput( bool val, int inputNumber, double opacity )
{
	m_opacityInput = val;
	m_hideInput = !val;
	m_HideInputNumber = inputNumber;
	m_opacityValue = opacity;
}

void Core::ProcessorOutputObserver::SetInitializeViews( bool val )
{
	m_InitializeViews = val;
}

void Core::ProcessorOutputObserver::SelectDataEntity( bool val )
{
	m_SetSelected = val;
}

void Core::ProcessorOutputObserver::SelectDataEntity()
{
	if ( !m_SetSelected )
	{
		return;
	}

	Core::DataEntityHolder::Pointer holder;
	Core::DataContainer::Pointer dataContainer;
	dataContainer = Core::Runtime::Kernel::GetDataContainer();
	holder = dataContainer->GetDataEntityList()->GetSelectedDataEntityHolder();

	// In CardiacSegmentationPlugin we need to notify the 
	// QuantificationWidget in order to compute the volume
	// each time the output mesh is modified
	holder->SetSubject( GetOutputDataEntity(), true );
}

void Core::ProcessorOutputObserver::SetAddToDataList( bool val )
{
	m_AddToDataList = val;
}

void Core::ProcessorOutputObserver::SetAddToRederingTree( bool val )
{
	m_AddToRederingTree = val;
}

void Core::ProcessorOutputObserver::AddToDataEntityList()
{
	if ( !m_AddToDataList )
	{
		return;
	}

	// Add to list and connect holders
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	Core::DataEntityList::Pointer list = dataContainer->GetDataEntityList();
	list->Add( GetOutputDataEntity( ) );

	list->ConnectOutputHolder( m_Processor->GetOutputDataEntityHolder( m_OutputNumber ) );
}

Core::DataEntity::Pointer Core::ProcessorOutputObserver::GetOutputDataEntity()
{
	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_Processor->GetOutputDataEntity( m_OutputNumber );
	return dataEntity;
}

void Core::ProcessorOutputObserver::RenderDataEntity()
{
	if ( !m_AddToRederingTree || m_RenderingTree.IsNull() )
	{
		return;
	}

	// Render the generated mesh
	m_RenderingTree->Add( GetOutputDataEntity(), true, m_InitializeViews );
}


void Core::ProcessorOutputObserver::AddProperty( blTag::Pointer property )
{
	if ( m_RenderingTree.IsNull() )
	{
		return;
	}

	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_Processor->GetOutputDataEntity( m_OutputNumber );
	m_RenderingTree->SetProperty( dataEntity, property );
}

void Core::ProcessorOutputObserver::PushProperty( blTag::Pointer property )
{
	m_PropertyList->AddTag( property );
}

void Core::ProcessorOutputObserver::ApplyPropertyList()
{
	blTagMap::Iterator it;
	for ( it = m_PropertyList->GetIteratorBegin() ; it != m_PropertyList->GetIteratorEnd() ; it++ )
	{
		AddProperty( it->second );
	}
}

bool Core::ProcessorOutputObserver::GetEnable() const
{
	return m_Enable;
}

void Core::ProcessorOutputObserver::SetEnable( bool val )
{
	m_Enable = val;
}
