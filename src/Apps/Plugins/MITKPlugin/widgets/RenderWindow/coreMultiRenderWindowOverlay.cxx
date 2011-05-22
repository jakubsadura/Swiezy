/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "coreMultiRenderWindowOverlay.h"
#include "coreMultiRenderWindowMITK.h"
#include "coreDataEntity.h"
#include "coreDataEntityInfoHelper.h"
#include "wxMitkRenderWindow.h"
#include "wxMitkSelectableGLWidget.h"
#include "vtkLinearTransform.h"
#include "coreDataEntityHelper.h"

#include "mitkLine.h"
#include "mitkImageTimeSelector.h"
#include "mitkImageAccessByItk.h"

Core::Widgets::MultiRenderWindowOverlay::MultiRenderWindowOverlay()
{
	m_MultiRenderWindow = NULL;
	m_PosStepperChangedCommand = itk::SimpleMemberCommand<MultiRenderWindowOverlay>::New();
	m_PosStepperChangedCommand->SetCallbackFunction(
		this, 
		&MultiRenderWindowOverlay::OnPosStepperChanged);

	m_TimeStepperChangedCommand = itk::SimpleMemberCommand<MultiRenderWindowOverlay>::New();
	m_TimeStepperChangedCommand->SetCallbackFunction(
		this, 
		&MultiRenderWindowOverlay::OnTimestepChanged);
}

Core::Widgets::MultiRenderWindowOverlay::~MultiRenderWindowOverlay()
{
	if ( m_SelectedDataEntity.IsNotNull() )
	{
		m_SelectedDataEntity->RemoveObserver( 
			this, 
			&MultiRenderWindowOverlay::OnSelectedModified );
	}

	RemoveOberversToSteppers();
}

void Core::Widgets::MultiRenderWindowOverlay::Init( 
	Core::Widgets::MultiRenderWindowMITK* multiRenderWindow, 
	Core::DataEntityHolder::Pointer selectedDataEntity )
{
	m_MultiRenderWindow = multiRenderWindow;
	m_SelectedDataEntity = selectedDataEntity;

	m_SelectedDataEntity->AddObserver( 
		this, 
		&MultiRenderWindowOverlay::OnSelectedModified );

	m_MultiRenderWindow->GetMetadataHolder()->AddObserver( 
		this,
		&MultiRenderWindowOverlay::RefreshOverlayText );

	InitOberversToSteppers( );
}

void Core::Widgets::MultiRenderWindowOverlay::OnSelectedModified()
{
	RefreshOverlayText( );
	
	if(this->m_SelectedDataEntity->GetSubject().IsNotNull())
	{
		this->m_SelectedDataEntity->GetSubject()->GetMetadata()->AddObserverOnChangedTag<MultiRenderWindowOverlay>(
			this, 
			&MultiRenderWindowOverlay::OnChangedMetadata);
	}
			
}


void Core::Widgets::MultiRenderWindowOverlay::OnChangedMetadata (
	blTagMap* tagMap,
	unsigned long tagId)
{
	this->RefreshOverlayText( );
}


void Core::Widgets::MultiRenderWindowOverlay::OnPosStepperChanged()
{
	RefreshOverlayText( );
}

void Core::Widgets::MultiRenderWindowOverlay::OnTimestepChanged()
{
	RefreshOverlayText( );
}

void Core::Widgets::MultiRenderWindowOverlay::RefreshOverlayText()
{
	if ( m_MultiRenderWindow == NULL || 
		m_SelectedDataEntity.IsNull() ||
		m_SelectedDataEntity->GetSubject().IsNull( ) ||
		!m_MultiRenderWindow->IsEnabled() ||
		( GetStepperX() == NULL || GetStepperY() == NULL || GetStepperZ() == NULL ) ||
		!m_MultiRenderWindow->GetMetadata( )->GetTagValue<bool>( "CornerAnnotationsShown" ) )
	{
		return;
	}

	Core::DataEntity::Pointer dataEntity;
	dataEntity = m_SelectedDataEntity->GetSubject();

	// Recollect information
	Core::DataEntityMetadata::Pointer meta = dataEntity->GetMetadata();
	Core::ModalityType modalityType;
	std::string patientName, patientSex, patientBirthDate, modality;

	blTagMap::Pointer patient = meta->GetTagValue<blTagMap::Pointer>( "Patient" );

	if ( patient.IsNotNull( ) )
	{
		std::string auxPatientName;
		if ( patient->FindTagByName( "Patient name" ).IsNotNull() )
		{
			patient->FindTagByName( "Patient name" )->GetValue<std::string>(auxPatientName);
			patientName = "Patient name: " + auxPatientName;
		}

		std::string auxPatientSex;
		if ( patient->FindTagByName( "Patient sex" ).IsNotNull() )
		{
			patient->FindTagByName( "Patient sex" )->GetValue<std::string>(auxPatientSex);
			patientSex = "Patient sex: " +  auxPatientSex;
		}

		std::string auxPatientBirthDate;
		if ( patient->FindTagByName( "Patient birth date" ).IsNotNull() )
		{
			patient->FindTagByName( "Patient birth date" )->GetValue<std::string>(auxPatientBirthDate);
			patientBirthDate = "Patient birth date: " + auxPatientBirthDate;
		}
	}

	if ( meta->FindTagByName( "Modality" ).IsNotNull() )
	{
		meta->FindTagByName( "Modality" )->GetValue<Core::ModalityType>(modalityType);
		modality =  "Modality: " + Core::DataEntityInfoHelper::GetModalityTypeAsString( modalityType );
	}

	int posX = GetStepperX()->GetPos();
	int posY = GetStepperY()->GetPos();
	int posZ = GetStepperZ()->GetPos();

	// Print it to a stream
	std::stringstream sstreamUpLeft;
	sstreamUpLeft 
		<< patientName << std::endl
		<< patientBirthDate << std::endl
		<< patientSex << std::endl
		<< modality << std::endl;

	std::stringstream sstreamUpRightX;
	std::stringstream sstreamUpRightY;
	std::stringstream sstreamUpRightZ;
	sstreamUpRightX << "Slice: " << posX << std::endl;
	sstreamUpRightY << "Slice: " << posY << std::endl;
	sstreamUpRightZ << "Slice: " << posZ << std::endl;

	//find the center in physicalcoordinates
	mitk::Point3D axisCenter;
	bool bAxisCenterComputed = m_MultiRenderWindow->GetIntersectionPoint(axisCenter);

	//now take the image as itk (or vtk) and get the pixel value
	if ( bAxisCenterComputed && !m_SelectedDataEntity->GetSubject().IsNull()
		&& m_SelectedDataEntity->GetSubject()->IsImage() )
	{
		mitk::BaseData::Pointer mitkData;
		boost::any anyData = m_SelectedDataEntity->GetSubject()->GetRenderingData( typeid( mitkData ) );
		Core::CastAnyProcessingData( anyData, mitkData );
		mitk::Image* image;
		image = dynamic_cast<mitk::Image*>(mitkData.GetPointer()); 

		std::string pixelValue;
		GetPixelValue( 
			image, 
			axisCenter,
			m_MultiRenderWindow->Get3D()->GetMitkRenderWindow( )->GetRenderer()->GetTimeStep(),
			pixelValue );
		sstreamUpRightX << "Intensity: " << pixelValue << std::endl;
		sstreamUpRightY << "Intensity: " << pixelValue << std::endl;
		sstreamUpRightZ << "Intensity: " << pixelValue << std::endl;

	}


	// Print it to a stream and window
	mitk::wxMitkSelectableGLWidget* xWidget = m_MultiRenderWindow->GetX();
	mitk::wxMitkSelectableGLWidget* yWidget = m_MultiRenderWindow->GetY();
	mitk::wxMitkSelectableGLWidget* zWidget = m_MultiRenderWindow->GetZ();

	xWidget->GetCornerAnnotation( )->SetUpLeftAnnotation( sstreamUpLeft.str().c_str() );
	yWidget->GetCornerAnnotation( )->SetUpLeftAnnotation( sstreamUpLeft.str().c_str() );
	zWidget->GetCornerAnnotation( )->SetUpLeftAnnotation( sstreamUpLeft.str().c_str() );

	xWidget->GetCornerAnnotation( )->SetUpRightAnnotation( sstreamUpRightX.str().c_str() );
	yWidget->GetCornerAnnotation( )->SetUpRightAnnotation( 
		sstreamUpRightY.str().c_str() );
	zWidget->GetCornerAnnotation( )->SetUpRightAnnotation( sstreamUpRightZ.str().c_str() );

	mitk::RenderingManager::GetInstance( )->RequestUpdateAll( 
		mitk::RenderingManager::REQUEST_UPDATE_2DWINDOWS );

}

void Core::Widgets::MultiRenderWindowOverlay::InitOberversToSteppers()
{
	if ( GetStepperX() )
	{
		m_PosStepperChangedCommandTagX = GetStepperX()->AddObserver(
			itk::ModifiedEvent(), 
			m_PosStepperChangedCommand);
	}
	if ( GetStepperY() )
	{
		m_PosStepperChangedCommandTagY = GetStepperY()->AddObserver(
			itk::ModifiedEvent(), 
			m_PosStepperChangedCommand);
	}
	if ( GetStepperZ() )
	{
		m_PosStepperChangedCommandTagZ = GetStepperZ()->AddObserver(
			itk::ModifiedEvent(), 
			m_PosStepperChangedCommand);
	}

	if ( GetTime() )
	{
		m_TimeStepperChangedCommandTagT = GetTime()->AddObserver(
			itk::ModifiedEvent(), 
			m_TimeStepperChangedCommand);
	}
}

mitk::Stepper* Core::Widgets::MultiRenderWindowOverlay::GetStepperX()
{
	if ( m_MultiRenderWindow->GetX( ) == NULL )
	{
		return NULL;
	}
	return m_MultiRenderWindow->GetX( )->GetSliceNavigationController()->GetSlice();
}

mitk::Stepper* Core::Widgets::MultiRenderWindowOverlay::GetStepperY()
{
	if ( m_MultiRenderWindow->GetY( ) == NULL )
	{
		return NULL;
	}
	return m_MultiRenderWindow->GetY( )->GetSliceNavigationController( )->GetSlice();
}

mitk::Stepper* Core::Widgets::MultiRenderWindowOverlay::GetStepperZ()
{
	if ( m_MultiRenderWindow->GetZ( ) == NULL )
	{
		return NULL;
	}
	return m_MultiRenderWindow->GetZ( )->GetSliceNavigationController( )->GetSlice();
}

mitk::Stepper* Core::Widgets::MultiRenderWindowOverlay::GetTime()
{
	if ( m_MultiRenderWindow->GetTimeNavigationController() == NULL )
	{
		return NULL;
	}
	return m_MultiRenderWindow->GetTimeNavigationController()->GetTime();
}

template <class T>
std::string to_string( const T& input )
{
  std::stringstream sstream;
  sstream << input;
  return sstream.str();
}

std::string to_string( const char& input )
{
  std::stringstream sstream;
  sstream << int(input);
  return sstream.str();
}

std::string to_string( const unsigned char& input )
{
  std::stringstream sstream;
  sstream << int(input);
  return sstream.str();
}

template < typename TPixel, unsigned int VImageDimension > 
void InternalCompute(
	itk::Image< TPixel, VImageDimension >* itkImage, 
	mitk::Point3D axisCenter,
	std::string &pixelValue )
{
	typename itk::Image< TPixel, VImageDimension >::PointType physicalPoint;

	physicalPoint[ 0 ] = axisCenter[ 0 ];
	physicalPoint[ 1 ] = axisCenter[ 1 ];

	// just for 3D images
	if(VImageDimension == 3)
		physicalPoint[ 2 ] = axisCenter[ 2 ];

	typename itk::Image< TPixel, VImageDimension >::IndexType pixelIndex;

	bool withinImage;
	withinImage = itkImage->TransformPhysicalPointToIndex( physicalPoint, pixelIndex );
	if ( withinImage )
	{
		pixelValue = to_string( itkImage->GetPixel( pixelIndex ) );
	}
	else
	{
		pixelValue = "None";
	}
}

void Core::Widgets::MultiRenderWindowOverlay::GetPixelValue( 
	mitk::Image* image,
	mitk::Point3D axisCenter,
	unsigned int timeStep,
	std::string &pixelValue )
{
	if ( image == NULL )
	{
		return;
	}

	image->SetRequestedRegionToLargestPossibleRegion(); //@todo without this, Image::GetScalarMin does not work for dim==3 (including sliceselector!)
	image->Update();
	mitk::ImageTimeSelector::Pointer timeSelector=mitk::ImageTimeSelector::New();
	timeSelector->SetInput( image );
	timeSelector->SetTimeNr( timeStep );
	timeSelector->UpdateLargestPossibleRegion();
	AccessByItk_2( timeSelector->GetOutput() , InternalCompute, axisCenter, pixelValue );
}

void Core::Widgets::MultiRenderWindowOverlay::RemoveOberversToSteppers()
{

	if ( m_MultiRenderWindow != NULL )
	{
		if ( GetStepperX() )
		{
			GetStepperX()->RemoveObserver( m_PosStepperChangedCommandTagX );
		}
		if ( GetStepperY() )
		{
			GetStepperY()->RemoveObserver( m_PosStepperChangedCommandTagY );
		}
		if ( GetStepperZ() )
		{
			GetStepperZ()->RemoveObserver( m_PosStepperChangedCommandTagZ );
		}
		if ( GetTime() )
		{
			GetTime()->RemoveObserver(m_TimeStepperChangedCommandTagT);
		}
	}

}

