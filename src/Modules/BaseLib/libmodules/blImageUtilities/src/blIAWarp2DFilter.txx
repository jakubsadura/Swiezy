/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

//implementation of the image warping class

#ifndef __blIAWarp2DFilter_txx
#define __blIAWarp2DFilter_txx

//#include "blIAWarp2DFilter.h"


template <class TImage>
blIAWarp2DFilter<TImage>::blIAWarp2DFilter() : 
	_precalculated(false), 
	_A(NULL),
	_B(NULL),
	_C(NULL),
	_determinant(NULL)
{
};

template <class TImage>
blIAWarp2DFilter<TImage>::~blIAWarp2DFilter() 
{
	if (_A!=NULL) delete[] _A;
	if (_B!=NULL) delete[] _B;
	if (_C!=NULL) delete[] _C;
	if (_determinant!=NULL) delete[] _determinant;
};


template <class TImage>
void blIAWarp2DFilter<TImage>::Update()
{
	//simple validity check 
	if (_shapeIn->GetNumberOfLandmarks() != _shapeOut->GetNumberOfLandmarks()) 
	{
		std::cout << "blIAWarp2DFilter::Update() - shapes are different. Aborting." << std::endl;
		return;
	}
	//:~ validity check


	//If precalculation was never called - this is the right time to do it
	if ( !_precalculated ) Precalculate();

	//-------------------------------------------
	//
	//    variables
	//
	//-----------------------------------------
	int vertexCount; //for loops
	int numberOfVertices = _NumberOfDimensions+1; 
	//int i;
	int indexInArray;

	double AX, AY;
	double BX, BY;
	double CX, CY;

	const blPDShape2D::PointIdsContainer* verticesRaw; //use: vertices = triangles[i] - vertices of i-th triangle
	int triangleID; 

	ContinuousPoint2D vertexIn[3]; //triangle vertices of input shape
	//ContinuousPoint2D vertexOut[3]; //triangle vertices of output shape

	//continuous point coordinates for interpolator
	typename LinearInterpolatorType::ContinuousIndexType pointInOriginalImage; 

	typename MaskType::IndexType pointCoordinates; //point coordinates in the mask

	//-----------------------------------------
	//
	//-----------------------------------------


	//get landmarks of varying input shape
	vnlVectorDouble landmarksInRaw( _shapeIn->GetNumberOfLandmarks() * _NumberOfDimensions );
	_shapeIn->GetPoints(landmarksInRaw); //get all the landmarks (not IDs, but real points)
	VectorOf2DContinuousPoints landmarksIn=(VectorOf2DContinuousPoints)&landmarksInRaw[0]; //landmarks as 2D points
	//:~ get landmarks


	const typename ImageType::RegionType imageRegion = _originalImage->GetBand(0)->GetLargestPossibleRegion();

	//region is precalculated and includes the whole shape,
	//all the triangles
	const typename MaskType::RegionType* maskRegion = _mask -> GetMaskRegion();

	// -------------------------------------------
	// 
	//    iterate through pixels
	//
	// ------------------------------------------


	for( int i=0; i<_originalImage->GetNumberOfBands(); i++ )
	{
		if( _originalImage->CanSample(i) )
		{
			MaskIteratorTypeWithIndex maskIt( _mask, *maskRegion ); //just in case. There was a reusability issue
			IteratorType fragmentIt	( _warpedImage->GetBand(i), *maskRegion );

			//create interpolator for real coordinates
			_interpolator->SetInputImage( _originalImage->GetBand(i) );

			for( maskIt.GoToBegin(), fragmentIt.GoToBegin(); 
					!maskIt.IsAtEnd(); ++maskIt, ++fragmentIt )
			{
				triangleID = maskIt.Get() - 1; 

				if (triangleID>=0)  //if pixel is not masked
				{

					//get all the vertices
					verticesRaw = &(*_triangles)[triangleID]; //correspondence: verticesRaw[i] <-> landmarks[verticesRaw[i]]

					//copy vertex coordinates
					for( vertexCount=0; vertexCount < numberOfVertices; vertexCount++)
					{
						for( i=0; i<_NumberOfDimensions; i++)
						{
							indexInArray = (*verticesRaw)[vertexCount];

							vertexIn[vertexCount][i] = landmarksIn[indexInArray][i];
						} 
					}
					

					// --------------------------------
					//
					//   precalculate constants for coordinate convertion (constant for each triangle)
					//
					// ----------------------------------
				
					AX = ( _A[triangleID][0] * vertexIn[0].X() - 
						_A[triangleID][1] * vertexIn[1].X() + 
						_A[triangleID][2] * vertexIn[2].X() )/_determinant[triangleID];
					AY = ( _A[triangleID][0] * vertexIn[0].Y() - 
						_A[triangleID][1] * vertexIn[1].Y() + 
						_A[triangleID][2] * vertexIn[2].Y() )/_determinant[triangleID];

					BX = 0;
					BY = 0;
					CX = 0;
					CY = 0;

					for (i=0; i<3; i++)
					{
						BX += _B[triangleID][i] * vertexIn[i].X();
						BY += _B[triangleID][i] * vertexIn[i].Y();
						CX += _C[triangleID][i] * vertexIn[i].X();
						CY += _C[triangleID][i] * vertexIn[i].Y();
					}
					
					BX /= _determinant[triangleID];
					BY /= _determinant[triangleID];
					CX /= _determinant[triangleID];
					CY /= _determinant[triangleID];


					// --------------------------------------
					// :~ precalculate constants
					// ---------------------------------------




					//--------------------------------------------------------------------------
					//
					// for each nonmasked pixel in fragment calculate pixel in _originalImage and copy it
					//
					//---------------------------------------------------------------------------
					//get coordinates of the current pixel of the fragment
					pointCoordinates = maskIt.GetIndex(); 

					//system solved by Cramer's rule, and output poit is described in terms of input point
					pointInOriginalImage[0] = AX + pointCoordinates[0] * BX + pointCoordinates[1] * CX;
					pointInOriginalImage[1] = AY + pointCoordinates[0] * BY + pointCoordinates[1] * CY;

					//insert the requested pixel
					//interpolator returns bloat
					if( imageRegion.IsInside(pointInOriginalImage) )
						fragmentIt.Set( (typename ImageType::PixelType) _interpolator->EvaluateAtContinuousIndex(pointInOriginalImage) );
					else
					{
						//std::cout<<"blIAWarp2DFilter: input image index out of range. Setting to 0"<<std::endl;
						fragmentIt.Set( (typename ImageType::PixelType) 0 );
					}


				} //if notmasked
			} //:~ for ( triangleID...
		}
	} //:~ for i<NUmberOfBands


} //:~ void blIAWarp2DFilter::Update()



/** \brief Precalculate some thngs that depend only on output shape shapeOut
*  because in iterative pprocessing all the images are warped onto shapeOut
*	but shapeOut never changes.
*  
*  If you don't call it explictly, it will be called from Update()
*/
template <class TImage>
void blIAWarp2DFilter<TImage>::Precalculate()
{
	_NumberOfDimensions = _shapeOut->GetDimension();

	//init mask
	_mask = MaskType::New();
	_mask -> SetRegions( _warpedImage->GetBand(0)->GetLargestPossibleRegion() );
	_mask -> Allocate();
	_mask -> ZeroMask();

	_landmarksOutRaw.set_size( _shapeOut->GetNumberOfLandmarks() * _NumberOfDimensions );
	_shapeOut->GetPoints(_landmarksOutRaw); //get all the landmarks (not IDs, but real points)
	VectorOf2DContinuousPoints landmarksOut=(VectorOf2DContinuousPoints)&_landmarksOutRaw[0]; //landmarks as 2D points
	
	//get mesh
	_shapeOut->Tesselate();
	_triangles = &_shapeOut->GetTesselationCells();
	_NumberOfTriangles = _triangles->size();

	//create sampler
	_sampler = TriangularSamplerType::New(); 
	_sampler -> SetInputImage( _warpedImage->GetBand(0) );
	_sampler -> SetImageMask( _mask );

	//create interpolator
	_interpolator = LinearInterpolatorType::New();		

	//-----------------------------------------------
	//
	//          precalculate constants
	//
	//----------------------------------------------

	//	cear arrays
	if (_A!=NULL) delete[] _A;
	if (_B!=NULL) delete[] _B;
	if (_C!=NULL) delete[] _C;
	if (_determinant!=NULL) delete[] _determinant;


	_A =			new TRIPLE_OF_DOUBLES [_NumberOfTriangles];
	_B =			new TRIPLE_OF_DOUBLES [_NumberOfTriangles];
	_C =			new TRIPLE_OF_DOUBLES [_NumberOfTriangles];
	_determinant =  new double [_NumberOfTriangles];

	//some local variables for loop
	const blPDShape2D::PointIdsContainer* verticesRaw; //use: vertices = triangles[i] - vertices of i-th triangle
	int triangleID; 

	ContinuousPoint2D vertexOut[3]; //triangle vertices of output shape
	int numberOfVertices = _NumberOfDimensions+1; 
	int indexInArray;
	int vertexCount; //for loops
	int i;


	//local bounding box of each triangle
//	const MaskType::RegionType* triangleRegion;


	for (triangleID=0; triangleID < _NumberOfTriangles; triangleID++)
	{
		//get all the vertices
		verticesRaw = &(*_triangles)[triangleID]; //correspondence: verticesRaw[i] <-> landmarks[verticesRaw[i]]

		//copy vertex coordinates
		for( vertexCount=0; vertexCount < numberOfVertices; vertexCount++)
		{
			for( i=0; i<_NumberOfDimensions; i++)
			{
				indexInArray = (*verticesRaw)[vertexCount];

				vertexOut[vertexCount][i] = landmarksOut[indexInArray][i];
			} 
		} //for( vertexCount
		
		//determinant of the system
		_A[triangleID][0] = ( vertexOut[1].X()*vertexOut[2].Y() - vertexOut[1].Y()*vertexOut[2].X() );
		_A[triangleID][1] = ( vertexOut[0].X()*vertexOut[2].Y() - vertexOut[0].Y()*vertexOut[2].X() );
		_A[triangleID][2] = ( vertexOut[0].X()*vertexOut[1].Y() - vertexOut[0].Y()*vertexOut[1].X() );

		_determinant[triangleID] = _A[triangleID][0] - _A[triangleID][1] + _A[triangleID][2];

		_B[triangleID][0] = (vertexOut[1].Y() - vertexOut[2].Y());
		_B[triangleID][1] = (vertexOut[2].Y() - vertexOut[0].Y());
		_B[triangleID][2] = (vertexOut[0].Y() - vertexOut[1].Y());


		_C[triangleID][0] = (vertexOut[2].X() - vertexOut[1].X());
		_C[triangleID][1] = (vertexOut[0].X() - vertexOut[2].X());
		_C[triangleID][2] = (vertexOut[1].X() - vertexOut[0].X());


		//sample triangle
		_sampler->SetTriangleVertices(vertexOut);
		_sampler->SetMaskValue( triangleID + 1 );
		_sampler->Update();

	} //:~ for ( triangleID...

	
	//-----------------------------------------------
	//
	//          end of precalculate constants
	//
	//----------------------------------------------

	
	//set global bounding box of all triangles
	typename MaskType::RegionType globalRegion; 

	const blPDShape2D::MeshType::BoundingBoxType* BoundingBox = _shapeOut->GetMeshBoundingBox();
	itk::FixedArray<double,4> Bounds = BoundingBox->GetBounds();

	typename MaskType::IndexType index;
	typename MaskType::SizeType size;

	index[0] = (long int)floor(Bounds[0]);
	index[1] = (long int)floor(Bounds[2]);
	size[0] = (long int)ceil(Bounds[1] - Bounds[0]);
	size[1] = (long int)ceil(Bounds[3] - Bounds[2]);

	globalRegion.SetIndex(index);
	globalRegion.SetSize(size);

	_mask -> SetMaskRegion (globalRegion);

	_precalculated = true; //set blag
} //:~ void Precalculate()





#endif
