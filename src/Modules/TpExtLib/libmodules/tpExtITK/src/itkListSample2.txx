/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkListSample2.txx,v $
  Language:  C++
  Date:      $Date: 2009-05-04 15:47:46 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkListSample2_txx
#define __itkListSample2_txx

#include "itkListSample2.h"

namespace itk { 
namespace Statistics {

template< class TMeasurementVector >
ListSample2< TMeasurementVector >
::ListSample2()
{
}

template< class TMeasurementVector >
void 
ListSample2< TMeasurementVector >
::Resize( InstanceIdentifier newsize )
{
  this->m_InternalContainer.resize( newsize );
}


template< class TMeasurementVector >
void 
ListSample2< TMeasurementVector >
::Clear()
{
  this->m_InternalContainer.clear();
}

template< class TMeasurementVector >
void 
ListSample2< TMeasurementVector >
::PushBack( const MeasurementVectorType & mv )
{
  if( this->GetMeasurementVectorSize() != MeasurementVectorTraits::GetLength( mv ) )
    {
    itkExceptionMacro("MeasurementVector instance doesn't match MeasurementVectorSize");
    }
  this->m_InternalContainer.push_back( mv );
}

template< class TMeasurementVector >
typename ListSample2< TMeasurementVector >::InstanceIdentifier
ListSample2< TMeasurementVector >
::Size() const
{
  return static_cast<InstanceIdentifier>( 
    this->m_InternalContainer.size() );
}

template< class TMeasurementVector >
typename ListSample2< TMeasurementVector >::TotalAbsoluteFrequencyType
ListSample2< TMeasurementVector >
::GetTotalFrequency() const
{
  // Since the entries are unique, the total
  // frequency is equal to the numbe of entries.
  return this->Size();
}


template< class TMeasurementVector >
const typename ListSample2< TMeasurementVector >::MeasurementVectorType &
ListSample2< TMeasurementVector >
::GetMeasurementVector(InstanceIdentifier instanceId) const 
{
  if ( instanceId < m_InternalContainer.size() )
    {
    return m_InternalContainer[instanceId];
    }
  itkExceptionMacro("MeasurementVector " << instanceId << " does not exist");
}

template< class TMeasurementVector >
void 
ListSample2< TMeasurementVector >
::SetMeasurement( InstanceIdentifier instanceId, 
                  unsigned int dim,
                  const MeasurementType &value)
{
  if ( instanceId < m_InternalContainer.size() )
    {
    m_InternalContainer[instanceId][dim] = value;
    }
}

template< class TMeasurementVector >
void
ListSample2< TMeasurementVector >
::SetMeasurementVector( InstanceIdentifier instanceId, 
                        const MeasurementVectorType &mv)
{
  if ( instanceId < m_InternalContainer.size() )
    {
    m_InternalContainer[instanceId] = mv;
    }
}

template< class TMeasurementVector >
typename ListSample2< TMeasurementVector >::AbsoluteFrequencyType 
ListSample2< TMeasurementVector >
::GetFrequency( InstanceIdentifier instanceId ) const
{
  if ( instanceId < m_InternalContainer.size() )
    {
    return 1;
    }
  else
    {
    return 0;
    }
}

template< class TMeasurementVector >
void 
ListSample2< TMeasurementVector >
::Graft( const DataObject *thatObject )
{
  this->Superclass::Graft(thatObject);

  const Self *thatConst = dynamic_cast< const Self * >(thatObject);
  if (thatConst)
    {
    Self *that = const_cast< Self * >(thatConst); 
    this->m_InternalContainer = that->m_InternalContainer;
    }
}

template< class TMeasurementVector >
void 
ListSample2< TMeasurementVector >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Internal Data Container: "
     << &m_InternalContainer << std::endl;
  os << indent << "Number of samples: " 
     << this->m_InternalContainer.size() << std::endl;
}
} // end of namespace Statistics
} // end of namespace itk 

#endif
