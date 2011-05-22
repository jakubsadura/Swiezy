/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkComputeFAFilter.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:51 $
  Version:   $Revision: 1.16 $

=========================================================================*/
#ifndef __itkComputeFAFilter_h
#define __itkComputeFAFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
	namespace Function {
		template< class TInput, class TOutput>
		class FA
		{
		public:
			FA() {}
			~FA() {}
			inline TOutput operator()( const TInput & A )
			{
				return A.GetFractionalAnisotropy();
			}
		};
	}


template <class TInputImage, class TOutputImage>
class ITK_EXPORT ComputeFAFilter :
	public UnaryFunctorImageFilter<  TInputImage,TOutputImage, Function::FA< typename TInputImage::PixelType, typename TOutputImage::PixelType >   >
{
public:
  /** Standard class typedefs. */
  typedef ComputeFAFilter           Self;
  typedef UnaryFunctorImageFilter<  TInputImage,  TOutputImage,  Function::FA< typename TInputImage::PixelType, typename TOutputImage::PixelType>   > 
	                                Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  ComputeFAFilter() {}
  virtual ~ComputeFAFilter() {}

private:
  ComputeFAFilter(const Self&);      //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
