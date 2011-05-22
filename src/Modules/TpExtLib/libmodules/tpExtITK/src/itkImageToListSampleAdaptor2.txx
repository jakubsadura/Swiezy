/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToListSampleAdaptor2.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-08 16:55:05 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToListSampleAdaptor2_txx
#define __itkImageToListSampleAdaptor2_txx

#include "itkImageToListSampleAdaptor2.h"

namespace itk { 
namespace Statistics {

template < class TImage>
ImageToListSampleAdaptor2< TImage>
::ImageToListSampleAdaptor2()
{
  m_Image = 0;
  m_UsePixelContainer = true;
}

template < class TImage>
const typename ImageToListSampleAdaptor2< TImage >::MeasurementVectorType&
ImageToListSampleAdaptor2< TImage>
::GetMeasurementVector(InstanceIdentifier id) const 
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  if ( m_UsePixelContainer )
    {
    MeasurementVectorTraits::Assign( m_MeasurementVectorInternal,
                    (*m_PixelContainer)[id]); 
    }
  else
    {
    MeasurementVectorTraits::Assign( m_MeasurementVectorInternal,
                    m_Image->GetPixel( m_Image->ComputeIndex( id ) ) ); 
    }

  return m_MeasurementVectorInternal;
}

/** returns the number of measurement vectors in this container*/
template < class TImage>
typename ImageToListSampleAdaptor2< TImage>::InstanceIdentifier
ImageToListSampleAdaptor2< TImage>
::Size() const
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return m_Image->GetPixelContainer()->Size();
}

template < class TImage>
inline typename ImageToListSampleAdaptor2< TImage>::AbsoluteFrequencyType
ImageToListSampleAdaptor2< TImage>
::GetFrequency( InstanceIdentifier ) const 
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return NumericTraits< AbsoluteFrequencyType >::One;
}


template < class TImage>
void
ImageToListSampleAdaptor2< TImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Image: ";
  if ( m_Image.IsNotNull() )
    {
    os << m_Image << std::endl;
    }
  else
    {
    os << "not set." << std::endl;
    }
  os << indent << "UsePixelContainer: "
     << this->GetUsePixelContainer() << std::endl;
}

template < class TImage>
void
ImageToListSampleAdaptor2< TImage>
::SetImage(const TImage* image) 
{ 
  m_Image = image; 
  m_PixelContainer = image->GetPixelContainer();
  this->Modified();
}

template < class TImage>
const TImage*
ImageToListSampleAdaptor2< TImage>
::GetImage() const
{
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return m_Image.GetPointer(); 
}  

template < class TImage>
typename ImageToListSampleAdaptor2< TImage>::TotalAbsoluteFrequencyType
ImageToListSampleAdaptor2< TImage>
::GetTotalFrequency() const
{ 
  if( m_Image.IsNull() )
    {
    itkExceptionMacro("Image has not been set yet");
    }

  return this->Size(); 
}

} // end of namespace Statistics 
} // end of namespace itk

#endif
