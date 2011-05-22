/**
* Copyright (c) 2010,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef ITKCUBICROOTIMAGEFILTER_H
#define ITKCUBICROOTIMAGEFILTER_H

#include <itkUnaryFunctorImageFilter.h>
#include <vnl/vnl_math.h>

namespace itk
{
  
/** \class CubicRootImageFilter
 * \brief Computes the vcl_log(x) pixel-wise
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace function
{  
  
template< class TInput, class TOutput>
class CubicRoot
{
public:
    CubicRoot() {}
    ~CubicRoot() {}
    bool operator!=( const CubicRoot & ) const { return false; }
    bool operator==( const CubicRoot & other ) const { return !(*this != other); }
    inline TOutput operator()( const TInput & rhs ) const
    {
        return static_cast<TOutput>( vcl_pow( static_cast<double>( rhs ), static_cast<double>( .333333333333 ) ) );
    }
}; // class CubicRoot

} // namespace function

template <class TInputImage, class TOutputImage>
class ITK_EXPORT CubicRootImageFilter : public UnaryFunctorImageFilter<
    TInputImage,
    TOutputImage, 
    function::CubicRoot< 
        typename TInputImage::PixelType, 
        typename TOutputImage::PixelType> >
{
public:
    /** Standard class typedefs. */
    typedef CubicRootImageFilter Self;
    typedef UnaryFunctorImageFilter<
        TInputImage,
        TOutputImage, 
        function::CubicRoot< 
            typename TInputImage::PixelType, 
            typename TOutputImage::PixelType> > Superclass;
  
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(CubicRootImageFilter, UnaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
    /** Begin concept checking */
    itkConceptMacro(InputConvertibleToDoubleCheck,
        (Concept::Convertible<typename TInputImage::PixelType, double>) );
    itkConceptMacro(DoubleConvertibleToOutputCheck,
        (Concept::Convertible<double, typename TOutputImage::PixelType>) );
    /** End concept checking */
#endif

protected:
    CubicRootImageFilter() {}
    virtual ~CubicRootImageFilter() {}

private:
    CubicRootImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

}; // class CubicRootImageFilter

} // end namespace itk

#endif // ITKCUBICROOTIMAGEFILTER_H

