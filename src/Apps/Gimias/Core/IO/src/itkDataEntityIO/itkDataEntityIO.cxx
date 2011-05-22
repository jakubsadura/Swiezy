/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/


#include "itkDataEntityIO.h"
#include "itkMetaDataObject.h"

#include "coreDataEntity.h"
#include "coreVTKImageDataHolder.h"

#include "vtkPointData.h"
#include "vtkDataArray.h"

#include <string>

namespace itk {

DataEntityIO
::DataEntityIO()
{
}

DataEntityIO
::~DataEntityIO()
{
}

bool
DataEntityIO
::IsADataEntity(const char* filename)
{
  Core::DataEntity::Pointer dataEntity;
  dataEntity = this->FileNameToDataEntity(filename);

  if (dataEntity.IsNotNull())
    {
    return true;
    }

  return false;
}

Core::DataEntity*
DataEntityIO
::FileNameToDataEntity(const char* filename)
{
  std::string fname = filename;
  std::string::size_type loc;

  // check that the filename starts with "DataEntity:"
  loc = fname.find("DataEntity:");
  if (loc != std::string::npos && (loc == 0))
  {
    // now pull off the scene
	Core::DataEntity::Pointer dataEntity;
	Core::DataEntity* ptr;
	sscanf(filename, "DataEntity:%p", &ptr);
	dataEntity = ptr;

      if (dataEntity.IsNull())
        {
        // not a valid scene pointer
        return 0;
        }
    return dataEntity;
  }

  return 0;
}


bool
DataEntityIO
::CanReadFile(const char* filename)
{
  return this->IsADataEntity(filename);
}

void
DataEntityIO
::ReadImageInformation()
{
  Core::DataEntity::Pointer dataEntity;

  dataEntity = this->FileNameToDataEntity( m_FileName.c_str() );
  if (dataEntity.IsNotNull())
  {

	Core::vtkImageDataPtr imageData;
	dataEntity->GetProcessingData( imageData );
	if ( imageData == NULL )
	{
		return;
	}

    // VTK is only 3D
    this->SetNumberOfDimensions( 3 );

    m_Direction.resize(3);
    m_Origin.resize(3);
    m_Spacing.resize(3);

	m_Origin[ 0 ] = imageData->GetOrigin()[ 0 ];
	m_Origin[ 1 ] = imageData->GetOrigin()[ 1 ];
	m_Origin[ 2 ] = imageData->GetOrigin()[ 2 ];
	m_Spacing[ 0 ] = imageData->GetSpacing()[ 0 ];
	m_Spacing[ 1 ] = imageData->GetSpacing()[ 1 ];
	m_Spacing[ 2 ] = imageData->GetSpacing()[ 2 ];

    this->SetDimensions(0, imageData->GetDimensions()[0]);
    this->SetDimensions(1, imageData->GetDimensions()[1]);
    this->SetDimensions(2, imageData->GetDimensions()[2]);

    // PixelType
    if (this->GetNumberOfComponents() == 1)
      {
      this->SetPixelType(SCALAR);
      }

      
    // ComponentType
    switch (imageData->GetScalarType())
      {
      case VTK_FLOAT: this->SetComponentType(FLOAT); break;
      case VTK_DOUBLE: this->SetComponentType(DOUBLE); break;
      case VTK_INT: this->SetComponentType(INT); break;
      case VTK_UNSIGNED_INT: this->SetComponentType(UINT); break;
      case VTK_SHORT: this->SetComponentType(SHORT); break;
      case VTK_UNSIGNED_SHORT: this->SetComponentType(USHORT); break;
      case VTK_LONG: this->SetComponentType(LONG); break;
      case VTK_UNSIGNED_LONG: this->SetComponentType(ULONG); break;
      case VTK_CHAR: this->SetComponentType(CHAR); break;
      case VTK_UNSIGNED_CHAR: this->SetComponentType(UCHAR); break;
      default: itkWarningMacro("Unknown scalar type.");
        this->SetComponentType(UNKNOWNCOMPONENTTYPE);
        break;
      }

    }    
}

// Read from the MRML scene
void
DataEntityIO
::Read(void *buffer)
{
	Core::DataEntity::Pointer dataEntity;

  dataEntity = this->FileNameToDataEntity( m_FileName.c_str() );
  if (dataEntity.IsNotNull())
    {
		Core::vtkImageDataPtr imageData;
		dataEntity->GetProcessingData( imageData );

    // buffer is preallocated, memcpy the data
      memcpy(buffer, imageData->GetScalarPointer(), 
             this->GetImageSizeInBytes());
    }
}

bool
DataEntityIO
::CanWriteFile(const char* filename)
{
  return this->IsADataEntity(filename);
}

// Write to the MRML scene

void
DataEntityIO
::WriteImageInformation()
{
}


void
DataEntityIO
::WriteImageInformation(Core::DataEntity::Pointer dataEntity, vtkImageData *img)
{

  // Fill in dimensions
  // VTK is only 3D, only copy the first 3 dimensions, fill in with
  // reasonable defaults for the rest
  if (this->GetNumberOfDimensions() > 3)
    {
    itkWarningMacro("Dimension of image is too high for VTK (Dimension = "
                    << this->GetNumberOfDimensions() << ")" );
    }
  
  int dim[3];
  double origin[3];
  double spacing[3];
  
  for (int i=0; (i < this->GetNumberOfDimensions()) && (i < 3); ++i)
    {
    dim[i] = this->GetDimensions(i);
    origin[i] = this->GetOrigin(i);
    spacing[i] = this->GetSpacing(i);
    }
  if (this->GetNumberOfDimensions() < 3)
    {
    // VTK is only 3D, fill in remaining dimensions
    for (int i = 0 ; i < 3; ++i)
      {
      dim[i] = 1;
      origin[i] = 0.0;
      spacing[i] = 1.0;
      }
    }
  img->SetDimensions(dim);
  img->SetOrigin(origin);
  img->SetSpacing(spacing);

  // ComponentType
  switch (this->GetComponentType())
    {
    case FLOAT: img->SetScalarTypeToFloat(); break;
    case DOUBLE: img->SetScalarTypeToDouble(); break;
    case INT: img->SetScalarTypeToInt(); break;
    case UINT: img->SetScalarTypeToUnsignedInt(); break;
    case SHORT: img->SetScalarTypeToShort(); break;
    case USHORT: img->SetScalarTypeToUnsignedShort(); break;
    case LONG: img->SetScalarTypeToLong(); break;
    case ULONG: img->SetScalarTypeToUnsignedLong(); break;
    case CHAR: img->SetScalarTypeToChar(); break;
    case UCHAR: img->SetScalarTypeToUnsignedChar(); break;
    default:
      // What should we do?
      itkWarningMacro("Unknown scalar type.");
      img->SetScalarTypeToShort();
      break;
    }

	// Scalar, Diffusion Weighted, or Vector image
	img->SetNumberOfScalarComponents(this->GetNumberOfComponents());

}

// Write to the MRML scene

void
DataEntityIO
::Write(const void *buffer)
{
  Core::DataEntity::Pointer dataEntity;
  dataEntity = this->FileNameToDataEntity( m_FileName.c_str() );
  if (dataEntity.IsNotNull())
    {
    
    // Need to create a VTK ImageData to hang off the node if there is
    // not one already there
    //
	Core::vtkImageDataPtr img;
	dataEntity->GetProcessingData( img );
    if (!img)
      {
		  img = Core::vtkImageDataPtr::New();
		  dataEntity->SetTimeStep( img );
      }

    // Configure the information on the node/image data
    //
    //
    this->WriteImageInformation(dataEntity, img);

    // Allocate the data, copy the data 
    //
    //
    // Everything but tensor images are passed in the scalars
    img->AllocateScalars();

    memcpy(img->GetScalarPointer(), buffer,
            img->GetPointData()->GetScalars()->GetNumberOfComponents() *
            img->GetPointData()->GetScalars()->GetNumberOfTuples() *
            img->GetPointData()->GetScalars()->GetDataTypeSize()
		);
	}
}


void
DataEntityIO
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
 
}

} // end namespace itk
