/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// For compilers that don't support precompilation, include "wx/wx.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "coreImageInfoWidget.h"
#include "CILabBoostMacros.h"
#include "boost/format.hpp"
#include <wx/textctrl.h>
#include "coreReportExceptionMacros.h"
#include "coreDisplayPixelValuePolicy.h"
#include "coreImageDataEntityMacros.h"
#include "coreDataEntityUtilities.h"
#include "coreDataEntityUtilities.txx"
#include "mitkGlobalInteraction.h"

using namespace Core::Widgets;

ImageInfoWidget::ImageInfoWidget(
	wxWindow* parent, int id, const wxPoint& pos /*= wxDefaultPosition*/, 
	const wxSize& size /*= wxDefaultSize*/, long style /*= 0*/)
	: ImageWidgetUI(parent, id, pos, size, style)
{
}

ImageInfoWidget::~ImageInfoWidget(void)
{
	if ( m_ImageInformationHolder.IsNotNull() )
	{
		m_ImageInformationHolder->RemoveObserver<ImageInfoWidget>(this, &ImageInfoWidget::UpdateWidget);
	}
}

void ImageInfoWidget::Init
(
	Core::ImageInformationHolder::Pointer imageInformationHolder
)
{
	this->m_ImageInformationHolder = imageInformationHolder;
	m_ImageInformationHolder->AddObserver<ImageInfoWidget>(this, &ImageInfoWidget::UpdateWidget);
	this->UpdateWidget();
}

void ImageInfoWidget::OnInit()
{
    m_ImageMediator = Core::ImageInfoMediator::New();
	Core::DataContainer::Pointer dataContainer = Core::Runtime::Kernel::GetDataContainer();
	m_ImageMediator->Init(this, dataContainer->GetDataEntityList()->GetSelectedDataEntityHolder());
}

void ImageInfoWidget::UpdateWidget(void)
{
	Core::ImageInformation* imageInfo = this->m_ImageInformationHolder->GetSubject();
	if( imageInfo && imageInfo->coordinateIsInsideImage )
	{
		//std::string x = gbl::ValueConverter::NumberToText(imageInfo->coordinate[0], 2);
		//std::string y = gbl::ValueConverter::NumberToText(imageInfo->coordinate[1], 2);
		//std::string z = gbl::ValueConverter::NumberToText(imageInfo->coordinate[2], 2);
		//cilabBoostFormatMacro( "Coord = %1%/%2%/%3%", x % y % z, coordText );

		wxString coordText;
		coordText = wxString::Format( wxT("Coord = %.2f/%.2f/%.2f"), 
			imageInfo->coordinate[0], 
			imageInfo->coordinate[1], 
			imageInfo->coordinate[2] );
		this->leCoordinates->SetValue( coordText );
		
		wxString coordValue;
		coordValue = wxString::Format( wxT("Value = %.2f"), imageInfo->pixelValue );
		//cilabBoostFormatMacro( "Value = %1%", imageInfo->pixelValue, coordValue );
		this->leIntensity->SetValue( coordValue );
	}
	else
	{
		this->leCoordinates->SetValue( wxT("Coord = None") );
		this->leIntensity->SetValue( wxT("Value = None") );
	}
}

Core::BaseProcessor::Pointer Core::Widgets::ImageInfoWidget::GetProcessor()
{
	return NULL;
}
