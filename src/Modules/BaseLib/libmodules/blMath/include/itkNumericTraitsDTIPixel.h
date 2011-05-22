/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNumericTraitsTensorPixel.h,v $
  Language:  C++
  Date:      $Date: 2006/02/06 22:01:57 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkNumericTraitsDTIPixel_h
#define __itkNumericTraitsDTIPixel_h

#include "itkNumericTraits.h"
#include "itkDTITensor.h"

typedef itk::DTITensor<float> TensorType;
// This file is meant to define numeric traits for tensor pixels types in itk

namespace itk
{

template <> class NumericTraits<DTITensor<bool> > {
public:
  typedef DTITensor<bool> ValueType;
  typedef DTITensor<bool> PrintType;
  typedef DTITensor<bool> AbsType;
  typedef DTITensor<signed char> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef bool ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< bool > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<bool>::Zero; 
    }
    return result; 
  }

  static DTITensor<bool> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=false; 
    }
    return result;
  }
 
  static DTITensor<bool> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=true; 
    }
    return result;
  }

  static DTITensor<bool> NonpositiveMin() {return min(); }

  /*  static bool IsPositive(signed char val) { return val > Zero; }
  static bool IsNonpositive(signed char val) { return val <= Zero; }
  static bool IsNegative(signed char val) { return val < Zero; }
  static bool IsNonnegative(signed char val) {return val >= Zero; }*/
};


template <> class NumericTraits<DTITensor<signed char> > {
public:
  typedef DTITensor<signed char> ValueType;
  typedef DTITensor<signed char> PrintType;
  typedef DTITensor<unsigned char> AbsType;
  typedef DTITensor<short> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef signed char ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< signed char > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<signed char>::Zero; 
    }
    return result; 
  }

  static DTITensor<signed char> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=-128; 
    }
    return result;
  }
 
  static DTITensor<signed char> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=127; 
    }
    return result;
  }

  static DTITensor<signed char> NonpositiveMin() {return min(); }

  /*  static bool IsPositive(signed char val) { return val > Zero; }
  static bool IsNonpositive(signed char val) { return val <= Zero; }
  static bool IsNegative(signed char val) { return val < Zero; }
  static bool IsNonnegative(signed char val) {return val >= Zero; }*/
};

template <> class NumericTraits<DTITensor<char> > {
public:
  typedef DTITensor<char> ValueType;
  typedef DTITensor<char> PrintType;
  typedef DTITensor<unsigned char> AbsType;
  typedef DTITensor<short> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef char ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< char > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<char>::Zero; 
    }
    return result; 
  }

  static DTITensor<char> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<char>::min(); 
    }
    return result;
  }
 
  static DTITensor<char> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<char>::max(); 
    }
    return result;
  }

  static DTITensor<char> NonpositiveMin() {return min(); }

};

template <> class NumericTraits<DTITensor<unsigned char> > {
public:

  typedef DTITensor<unsigned char> ValueType;
  typedef DTITensor<unsigned char> PrintType;
  typedef DTITensor<unsigned char> AbsType;
  typedef DTITensor<unsigned short> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef unsigned char ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< unsigned char > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned char>::Zero; 
    }
    return Zero; 
  }
  
  static DTITensor<unsigned char> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=0; 
    }
    return result;
  }
 
  static DTITensor<unsigned char> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned char>::max(); 
    }
    return result;
  }

  static DTITensor<unsigned char> NonpositiveMin() {return min(); }

};

template <> class NumericTraits<DTITensor<short> > {
 public:
  typedef DTITensor<short> ValueType;
  typedef DTITensor<short> PrintType;
  typedef DTITensor<unsigned short> AbsType;
  typedef DTITensor<int> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef short ScalarValueType;
  typedef double ScalarRealType;
  
  
  static const DTITensor< short > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<short>::Zero; 
    }
    return result; 
  }

  static DTITensor<short> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<short>::min(); 
    }
    return result;
  }
 
  static DTITensor<short> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<short>::max(); 
    }
    return result;
  }
  
  static DTITensor<short> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<short>::NonpositiveMin(); 
    }
    return result;
  }

};
 
template <> class NumericTraits<DTITensor<unsigned short> > {
public:
  typedef DTITensor<unsigned short> ValueType;
  typedef DTITensor<unsigned short> PrintType;
  typedef DTITensor<unsigned short> AbsType;
  typedef DTITensor<unsigned int> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef unsigned short ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< unsigned short > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned short>::Zero; 
    }
    return result; 
  }

  static DTITensor<unsigned short> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned short>::min(); 
    }
    return result;
  }
 
  static DTITensor<unsigned short> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned short>::max(); 
    }
    return result;
  }

  static DTITensor<unsigned short> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned short>::NonpositiveMin(); 
    }
    return result;
  }

};

template <> class NumericTraits<DTITensor<int> > {
public:
  typedef DTITensor<int> ValueType;
  typedef DTITensor<int> PrintType;
  typedef DTITensor<unsigned int> AbsType;
  typedef DTITensor<long> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef int ScalarValueType;
  typedef double ScalarRealType;
  
  static const DTITensor< int > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<int>::Zero; 
    }
    return result; 
  }

  static DTITensor<int> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<int>::min(); 
    }
    return result;
  }
 
  static DTITensor<int> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<int>::max(); 
    }
    return result;
  }

  static DTITensor<int> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<int>::NonpositiveMin(); 
    }
    return result;
  }

};

template <> class NumericTraits<DTITensor<unsigned int> > {
public:
  typedef DTITensor<unsigned int> ValueType;
  typedef DTITensor<unsigned int> PrintType;
  typedef DTITensor<unsigned int> AbsType;
  typedef DTITensor<unsigned long> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef unsigned int ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< unsigned int > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned int>::Zero; 
    }
    return result; 
  }

  static DTITensor<unsigned int> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned int>::min(); 
    }
    return result;
  }
 
  static DTITensor<unsigned int> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned int>::max(); 
    }
    return result;
  }

  static DTITensor<unsigned int> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned int>::NonpositiveMin(); 
    }
    return result;
  }

};

template <> class NumericTraits<DTITensor<long> > {
public:
  typedef DTITensor<long> ValueType;
  typedef DTITensor<long> PrintType;
  typedef DTITensor<unsigned long> AbsType;
  typedef DTITensor<long> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef long ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< long > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long>::Zero; 
    }
    return result; 
  }

  static DTITensor<long> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long>::min(); 
    }
    return result;
  }
 
  static DTITensor<long> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long>::max(); 
    }
    return result;
  }

  static DTITensor<long> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long>::NonpositiveMin(); 
    }
    return result;
  }

};

template <> class NumericTraits<DTITensor<unsigned long> > {
public:
  typedef DTITensor<unsigned long> ValueType;
  typedef DTITensor<unsigned long> PrintType;
  typedef DTITensor<unsigned long> AbsType;
  typedef DTITensor<unsigned long> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef unsigned long ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< unsigned long > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned long>::Zero; 
    }
    return result; 
  }

  static DTITensor<unsigned long> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned long>::min(); 
    }
    return result;
  }
 
  static DTITensor<unsigned long> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned long>::max(); 
    }
    return result;
  }

  static DTITensor<unsigned long> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<unsigned long>::NonpositiveMin(); 
    }
    return result;
  }


};

template <> class NumericTraits<DTITensor<float> > {
public:
  typedef DTITensor<float> ValueType;
  typedef DTITensor<float> PrintType;
  typedef DTITensor<float> AbsType;
  typedef DTITensor<double> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef float ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< float > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<float>::Zero; 
    }
    return result; 
  }

  static DTITensor<float> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<float>::min(); 
    }
    return result;
  }
 
  static DTITensor<float> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<float>::max(); 
    }
    return result;
  }

  static DTITensor<float> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<float>::NonpositiveMin(); 
    }
    return result;
  }

  static DTITensor<bool> IsPositive( DTITensor<float> val) { return val > Zero; }
  static DTITensor< bool> IsNonpositive( DTITensor<float> val) { return val <= Zero; }
  static DTITensor<bool> IsNegative( DTITensor<float> val) { return val < Zero; }
  static DTITensor< bool> IsNonnegative( DTITensor<float> val) {return val >= Zero; }

  static const bool ITKCommon_EXPORT is_signed;


};

template <> class NumericTraits<DTITensor<double> > {
public:
  typedef DTITensor<double> ValueType;
  typedef DTITensor<double> PrintType;
  typedef DTITensor<double> AbsType;
  typedef DTITensor<long double> AccumulateType;
  typedef DTITensor<double> RealType;
  typedef double ScalarValueType;
  typedef double ScalarRealType;
 
 
  static const DTITensor< double > ITKCommon_EXPORT Zero;

  static DTITensor<double> ZeroValue() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<double>::Zero; 
    }
    return result;
  }
  
  static DTITensor<double> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<double>::min(); 
    }
    return result;
  }
 
  static DTITensor<double> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<double>::max(); 
    }
    return result;
  }

  static DTITensor<double> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<double>::NonpositiveMin();
    }
    return result;
  }
  
 
};

template <> class NumericTraits<DTITensor<long double> > {
public:
  typedef DTITensor<long double> ValueType;
  typedef DTITensor<long double> PrintType;
  typedef DTITensor<long double> AbsType;
  typedef DTITensor<long double> AccumulateType;
  typedef DTITensor<long double> RealType;
  typedef long double ScalarValueType;
  typedef double ScalarRealType;

  static const DTITensor< long double > ITKCommon_EXPORT Zero;
  static ValueType ZeroValue() { 
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long double>::Zero; 
    }
    return result; 
  }

  static DTITensor<long double> min() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long double>::min(); 
    }
    return result;
  }
 
  static DTITensor<long double> max() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long double>::max(); 
    }
    return result;
  }

  static DTITensor<long double> NonpositiveMin() {
    ValueType result;
    for(unsigned i=0; i<6; i++){
      result[i]=NumericTraits<long double>::NonpositiveMin(); 
    }
    return result;
  }
  
};


} // end namespace itk

#ifndef __itkNumericTraitsDTIPixel_cxx
#include "itkNumericTraitsDTIPixel.cxx"
#define __itkNumericTraitsDTIPixel_cxx
#endif

#endif // __itkNumericDTITensorPixel_h  
