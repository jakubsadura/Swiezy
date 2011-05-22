/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//Implementation file for the triangular sampler.
#ifndef __blIATriangularSampler_txx
#define __blIATriangularSampler_txx

//#include "blIATriangularSampler.h"

template<class TPixel>
blIATriangularSampler<TPixel>::blIATriangularSampler()
{
};

template<class TPixel>
blIATriangularSampler<TPixel>::~blIATriangularSampler() 
{
};



//----------------------------------------------------
/** \brief Sample requested area
 *
 */
template<class TPixel>
void blIATriangularSampler<TPixel>::Update(bool createNewImage)
{

	//-----------------------------------------
	//
	// stage one: exract region of interest
	//
	//-----------------------------------------
	//VectorOf2DPoints vertices = (VectorOf2DPoints)&_triangleVertices[0];
	
	//calculate bounding box of the triangle
	//Y coordinate
	BoundingBox2D _boundingBox;

	if ( this->_vertex[0].Y() >= this->_vertex[1].Y() )
	{
		_boundingBox.m_fYMin = this->_vertex[1].Y();
		_boundingBox.m_fYMax = this->_vertex[0].Y();
	}
	else
	{
		_boundingBox.m_fYMin = this->_vertex[0].Y();
		_boundingBox.m_fYMax = this->_vertex[1].Y();
	}
	if ( this->_vertex[2].Y() > _boundingBox.m_fYMax ) 
		_boundingBox.m_fYMax = this->_vertex[2].Y();
	else if ( this->_vertex[2].Y() < _boundingBox.m_fYMin ) 
		_boundingBox.m_fYMin = this->_vertex[2].Y();

	//x coordinate
	if ( this->_vertex[0].X() >= this->_vertex[1].X() )
	{
		_boundingBox.m_fXMin = this->_vertex[1].X();
		_boundingBox.m_fXMax = this->_vertex[0].X() ;
	}
	else
	{
		_boundingBox.m_fXMin = this->_vertex[0].X();
		_boundingBox.m_fXMax = this->_vertex[1].X() ;
	}


	if ( this->_vertex[2].X() > _boundingBox.m_fXMax ) 
		_boundingBox.m_fXMax = this->_vertex[2].X();
	else if ( this->_vertex[2].X() < _boundingBox.m_fXMin ) 
		_boundingBox.m_fXMin = this->_vertex[2].X();

	// must have integer data
	_boundingBox.m_fYMin = floor( _boundingBox.m_fYMin );
	_boundingBox.m_fYMax = ceil( _boundingBox.m_fYMax );
	_boundingBox.m_fXMin = floor( _boundingBox.m_fXMin );
	_boundingBox.m_fXMax = ceil( _boundingBox.m_fXMax );

	//:~ bounding box calculated


	//start point of ROI
	typename MaskType::IndexType start;
	start[0] = (long) _boundingBox.m_fXMin;
	start[1] = (long) _boundingBox.m_fYMin;

	//size of ROI
	typename MaskType::SizeType size;
	size[0] = (long) ( _boundingBox.m_fXMax - _boundingBox.m_fXMin ); 
	size[1] = (long) ( _boundingBox.m_fYMax - _boundingBox.m_fYMin ); 

	//create region
	//ImageType::RegionType desiredRegion;
	this->_boundingRegion.SetSize( size );
	this->_boundingRegion.SetIndex( start );

	//extract region
	//extract fragment from the image, corresponding to the bounding box
        typename ROIFilter::Pointer filter;
	if (createNewImage)
	{
		filter = ROIFilter::New();
		filter->SetRegionOfInterest( this->_boundingRegion );
		filter->SetInput( this->_originalImage ); 
		filter->Update();
		this->_sampledImage = filter->GetOutput();
	}
	//if we are not creating new image, then sampledImage points to originalImage
	else this->_sampledImage = this->_originalImage;

	//-----------------------------------------
	//
	// stage two: create triangular mask in extracted image.
	//
	//-----------------------------------------

	//create empty mask 
	if ( this->_imageMask.IsNull() )  //if mask exists, do not recreate
	{
		this->_imageMask = MaskType::New();
		this->_imageMask->SetRegions( this->_sampledImage->GetLargestPossibleRegion() );
		this->_imageMask->Allocate();
		this->_imageMask->ZeroMask();
	}
	//there has been previous mask - zero only nonzero area
	//else _imageMask->ZeroMask(false); 
	//don't touch the mask, let the user handle zeroing

	//set region for iterator
	const typename MaskType::RegionType* region;

	if (createNewImage) //iterate through whole mask
	{
		region = &this->_imageMask->GetLargestPossibleRegion();
	}
	else //iterate only through the region of interest, everything else is already 0
	{
		region = &this->_boundingRegion;
	}

	//set the region of unmasked pixels
	//must be after zeroing mask, because if
	//there has been previous mask then it will be 
	//zeroed in the previous region to save computations
	this->_imageMask->SetMaskRegion(*region);

	//iterate through each pixel and check whether it is inside triangle


	MaskIteratorTypeWithIndex outputIt( this->_imageMask, *region );
	typename MaskType::IndexType idx;

	//both loops are mostly the same, but I had to make two, because
	//checking variable value inside the loop is a big slowdown
	if (!createNewImage)  //must be relative to the image, not fragment
	{
		for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt )
		{
			if( IsPointInsideTriangle( outputIt.GetIndex() ) ) 
				outputIt.Set( this->_nonMaskedPixelValue );
		}
	}
	else 
	{
		for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt )
		{
			idx = outputIt.GetIndex();

			idx[0] += (long) _boundingBox.m_fXMin;
			idx[1] += (long) _boundingBox.m_fYMin;

			if( IsPointInsideTriangle(idx) ) outputIt.Set( this->_nonMaskedPixelValue );
		}
	}

}



template<class TPixel>
void blIATriangularSampler<TPixel>::SetTriangleVertices(const ContinuousPoint2D* triangleVertices)
{
	//copy vertices
	memcpy(_vertex, triangleVertices, 3*sizeof(ContinuousPoint2D));

	//precalculate some parameters, constant for the triangle
	int i, i2, i3; //indexes for vertices
	double K1, K2, A, B, C, D, E;

	//calculate same things, but for the different triples of vertices
	//  v[0], v[1], v[2]
	//  v[1], v[2], v[0]
	//  v[2], v[0], v[1]
	// and store in corresponding rows of the _param matrix

	for (i=0; i<3; i++)
	{
		i2 = (i+1) % 3;
		i3 = (i+2) % 3; 

		K1 = ( _vertex[i2].Y() - _vertex[i].Y() );
		K2 = ( _vertex[i2].X() - _vertex[i].X() );

		A = _vertex[i3].X() - _vertex[i].X();
		B = _vertex[i3].Y() - _vertex[i].Y();
		C = K1 * A;
		D = K2 * B;
		E = C - D;

		this->_param[i][0] = K1 * E;
		this->_param[i][1] = - K2 * E;
		this->_param[i][2] = K2 * _vertex[i].Y() * ( K1 * A - D ) +
					   K1 * _vertex[i].X() * ( K2 * B - C );
	}

	//to check if the point (X,Y) is inside triangle
	//_param[i][0] * X + _param[i][1] * Y + _param[i][2] >= 0
	//for all i
	//
	//this criterion follows from the following considerations
	//suppose vertices are v1,v2,v3 and point p
	//take v1 and v2, create line equation, put p into equation and calculate sign
	//put v3 into equation and take sign. Compare signs - if coincide then 
	//point is to the correct side of the line.
	// Do same with each line (v2-v3 versus v1 and p, v3-v1 versus v2 and p)
	// if in all cases signs are equal, then point is inside triangle, if in at least 
	// in one case signs differ, then point is outside.
	// if equation after calculating at point p becomes 0 then point is on the line and
	// signs are considered equal.
	//
	//to check whether signs are he same it is possible to multiply values 
	//if two values are of the same sign, then product will be always nonnegative
	//
	//current formula is obtained after multiplication and reducing of the result to the form 
	//Ax+By+C >=0, so A,B,C can be precalculated
}



//for description of the algortihm see SetTriangleVertices(...);
template<class TPixel>
inline bool blIATriangularSampler<TPixel>::IsPointInsideTriangle(const typename MaskType::IndexType& index)
{
	int i;

	bool result=true;

	for (i=0; i<3; i++)
	{
		
		if ( this->_param[i][0] * index[0] + this->_param[i][1] * index[1] + this->_param[i][2] < 0)
		{
			result = false;
			break; //exit loop, no reason to continue
		}
	}

	return result;
}


#endif

