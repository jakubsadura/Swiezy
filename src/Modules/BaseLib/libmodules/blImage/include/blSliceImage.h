/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef blSliceImage_H
#define blSliceImage_H

//#include "BaseLibWxWidgetsWin32Header.h"
#include "blLightObject.h"
#include "blMacro.h"
#include "blITKImagesDefines.h"

#include <vector>
#include "CILabExceptionMacros.h"
#include "itkImage.h"

class SliceBase
{
public:
	base::VectorFloatType orientation; //better itk::Direction or something like that?
	base::VectorFloatType position;
	std::string pixelTypeAsString;
};

template< class TPixel>
class Slice : public SliceBase
{
public:
	typedef itk::Image< TPixel,2> internalImageType;
	typename internalImageType::Pointer image;
	Slice( ) : SliceBase( ) {}
};

/**
SliceImage like non parallel sliced images

\author Chiara Riccobene
\date 12 Aug 2009
*/
class blSliceImage : public blLightObject 
{
public:
	typedef blSliceImage Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);
	cilabDeclareExceptionMacro(Exception, std::exception)
	typedef Slice<double> InternalSliceType;
	
public:

	//!
	void SetNumberOfSlices(int number);
	int GetNumberOfSlices();

	//!
	template <class TPixel>
	std::vector< Slice< TPixel > > GetData() const;

	/**
	\note The returned variable is local. When the variable is destroyed, all
	internal memory will be destroyed.
	Slice has an itk::Image. If TPixel is the same as the internal type, 
	the itk::Image will be the internal one. If TPixel is different from the
	internal type, the image will be casted and the caller should take care
	of the reference counting.
	*/
	template <class TPixel>
	Slice< TPixel > GetSliceImage( int i ) const;

	//!
	SliceBase GetBaseSlice( int i ) const;

	//!
	void SetSlicePosition( base::VectorFloatType slicePosition, int id );

	//!
	void SetSlicePosition( double slicePosition[3], int id );

	//!
	base::VectorFloatType GetSlicePosition( int id  );

	//!
	void SetSliceOrientation( base::VectorFloatType sliceOrientation, int id );

	//!
	void SetSliceOrientation( double sliceOrientation[6], int id );
	
	//!
	base::VectorFloatType GetSliceOrientation( int id );

	//!
	template< class TPixel>
	void SetInternalImage(typename itk::Image<TPixel,2>::Pointer slice, int i);

	//!
	std::string GetScalarTypeAsString( );

protected:
	//! Default Constructor. 
	blSliceImage( );

	~blSliceImage( );

private:
	
	//!
	std::vector<InternalSliceType> m_slices;

};

#include "blSliceImage.txx"

#endif //blSliceImage_H
