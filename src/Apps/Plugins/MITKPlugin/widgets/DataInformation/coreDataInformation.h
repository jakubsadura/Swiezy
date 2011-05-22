/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _coreDataInformation_H
#define _coreDataInformation_H

#include "wxMitkVolumeImageInformationWidget.h"
#include "wxMitkMeasurementInformationWidget.h"
#include "wxMitkPolyDataInformationWidget.h"
#include "wxMitkUnstructuredGridInformationWidget.h"
#include "wxMitkSignalInformationWidget.h"
#include "coreDataEntityInformation.h"
#include "coreVTKUnstructuredGridHolder.h"

namespace Core
{
namespace Widgets
{
	class VolumeImageInformationWidget : 
		public mitk::wxMitkVolumeImageInformationWidget,
		public DataInformationWidgetBase
	{
	public:
		//!
		coreDefineBaseWindowFactory( VolumeImageInformationWidget );

		//!
		VolumeImageInformationWidget(wxWindow* parent, 
			int id, 
			const wxPoint& pos=wxDefaultPosition, 
			const wxSize& size=wxDefaultSize, 
			long style=0) : mitk::wxMitkVolumeImageInformationWidget( parent, id, pos, size, style ) {}

		//!
		bool IsValidData( Core::DataEntity::Pointer dataEntity )
		{
			return dataEntity->IsImage();
		}
		//!
		void SetInputDataEntity( Core::DataEntity::Pointer dataEntity )
		{
			Core::DataEntity::Pointer tempDataEntity;
			tempDataEntity = Core::DataEntity::New( Core::ImageTypeId );
			tempDataEntity->SwitchImplementation( typeid( Core::vtkImageDataPtr ) );
			tempDataEntity->Copy( dataEntity, Core::gmReferenceMemory );

			Core::vtkImageDataPtr inputImage;
			tempDataEntity->GetProcessingData( inputImage );
			UpdateData(inputImage);
		}

		//!
		virtual void Clear( )
		{
			UpdateData( NULL );
		}

	};

	class MeasurementInformationWidget : 
		public mitk::wxMitkMeasurementInformationWidget,
		public DataInformationWidgetBase
	{
	public:
		//!
		coreDefineBaseWindowFactory( MeasurementInformationWidget );

		//!
		MeasurementInformationWidget(wxWindow* parent, 
			int id, 
			const wxPoint& pos=wxDefaultPosition, 
			const wxSize& size=wxDefaultSize, 
			long style=0) : mitk::wxMitkMeasurementInformationWidget( parent, id, pos, size, style ) {}
		//!
		bool IsValidData( Core::DataEntity::Pointer dataEntity )
		{
			return dataEntity->IsMeasurement( );
		}
		//!
		void SetInputDataEntity( Core::DataEntity::Pointer dataEntity )
		{
			Core::vtkPolyDataPtr inputMeasure;
			if ( dataEntity->GetProcessingData( inputMeasure ) )
			{
				UpdateData(inputMeasure);
			}
		}
		//!
		virtual void Clear( )
		{
			UpdateData( NULL );
		}

	};

	class PolyDataInformationWidget : 
		public mitk::wxMitkPolyDataInformationWidget,
		public DataInformationWidgetBase
	{
	public:
		//!
		coreDefineBaseWindowFactory( PolyDataInformationWidget );

		//!
		PolyDataInformationWidget(wxWindow* parent, 
			int id, 
			const wxPoint& pos=wxDefaultPosition, 
			const wxSize& size=wxDefaultSize, 
			long style=0) : mitk::wxMitkPolyDataInformationWidget( parent, id, pos, size, style ) {}
		//!
		bool IsValidData( Core::DataEntity::Pointer dataEntity )
		{
			return dataEntity->IsSurfaceMesh();
		}
		//!
		void SetInputDataEntity( Core::DataEntity::Pointer dataEntity )
		{
			try
			{
				//! \todo When implmenetation is not vtkPolyData -> Throw exception
				Core::vtkPolyDataPtr inputPolyData;
				if ( dataEntity->GetProcessingData( inputPolyData) )
				{
					UpdateData(inputPolyData);
				}
			}
			catch (...)
			{
			}

		}
		//!
		virtual void Clear( )
		{
			UpdateData( NULL );
		}

	};

	class UnstructuredGridInformationWidget : 
		public mitk::wxMitkUnstructuredGridInformationWidget,
		public DataInformationWidgetBase
	{
	public:
		//!
		coreDefineBaseWindowFactory( UnstructuredGridInformationWidget );

		//!
		UnstructuredGridInformationWidget(wxWindow* parent, 
			int id, 
			const wxPoint& pos=wxDefaultPosition, 
			const wxSize& size=wxDefaultSize, 
			long style=0) : mitk::wxMitkUnstructuredGridInformationWidget( parent, id, pos, size, style ) {}
		//!
		bool IsValidData( Core::DataEntity::Pointer dataEntity )
		{
			return dataEntity->IsVolumeMesh( );
		}
		//!
		void SetInputDataEntity( Core::DataEntity::Pointer dataEntity )
		{
			Core::vtkUnstructuredGridPtr inputMesh;
			if ( dataEntity->GetProcessingData( inputMesh ) )
			{
				UpdateData(inputMesh);
			}
		}
		//!
		virtual void Clear( )
		{
			UpdateData( NULL );
		}

	};

	class SignalInformationWidget : 
		public mitk::wxMitkSignalInformationWidget,
		public DataInformationWidgetBase
	{
	public:
		//!
		coreDefineBaseWindowFactory( SignalInformationWidget );

		//!
		SignalInformationWidget(wxWindow* parent, 
			int id, 
			const wxPoint& pos=wxDefaultPosition, 
			const wxSize& size=wxDefaultSize, 
			long style=0) : mitk::wxMitkSignalInformationWidget( parent, id, pos, size, style ) {}
		//!
		bool IsValidData( Core::DataEntity::Pointer dataEntity )
		{
			return dataEntity->IsSignal( );
		}
		//!
		void SetInputDataEntity( Core::DataEntity::Pointer dataEntity )
		{
			blSignalCollective::Pointer signalCollective = blSignalCollective::New();
			if ( dataEntity->GetProcessingData( signalCollective) )
			{
				blSignal::Pointer inputSignal = signalCollective->GetSignal(0);
				UpdateData(inputSignal);
			}
		}
		//!
		virtual void Clear( )
		{
			UpdateData( NULL );
		}

	};
} // Widgets
} // Core

#endif _coreDataInformation_H
