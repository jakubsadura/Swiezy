// Copyright 2009 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "coreSelectionToolWidget.h"


Core::Widgets::SelectionToolWidget::SelectionToolWidget( )
{
}

void Core::Widgets::SelectionToolWidget::Init( )
{
	OnInit();
}

Core::Widgets::SelectionToolWidget::~SelectionToolWidget()
{

}

void Core::Widgets::SelectionToolWidget::OnInit()
{

}

boost::any Core::Widgets::SelectionToolWidget::GetData()
{
	return NULL;
}

Core::DataEntity::Pointer Core::Widgets::SelectionToolWidget::GetDataEntity()
{
	return NULL;
}
