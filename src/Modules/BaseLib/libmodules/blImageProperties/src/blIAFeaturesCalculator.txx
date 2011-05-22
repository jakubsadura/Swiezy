/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAFeaturesCalculator_txx
#define __blIAFeaturesCalculator_txx

#include <iostream>

#include "blIAFeaturesCalculator.h"


// ==============================================================
// 			class blGaussianImageContainerNode
// ==============================================================


//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
blGaussianImageContainerNode<TImage, MaxDerivativeOrder>::
blGaussianImageContainerNode()
//--------------------------------------------------
{
	unsigned i;

	for (i = 0; i < MaxDerivativeOrder + 1; i++)
	{
		this->childNodes[i] = 0;	// Null pointers
	}

	this->imagePointer = 0; // NULL
}


//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
blGaussianImageContainerNode<TImage, MaxDerivativeOrder>::
~blGaussianImageContainerNode()
//--------------------------------------------------
{
	// delete child nodes

	unsigned i;
	for (i = 0; i < MaxDerivativeOrder + 1; i++)
	{
		delete this->childNodes[i];
	}
}



// ==============================================================
// 			class blGaussianImageContainer
// ==============================================================


//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
blGaussianImageContainer<TImage, MaxDerivativeOrder>::
blGaussianImageContainer()
//--------------------------------------------------
{
}



//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
blGaussianImageContainer<TImage, MaxDerivativeOrder>::
~blGaussianImageContainer()
//--------------------------------------------------
{

	// clear map and free memory
	this->Clear();

}



/**
 *	This method returns an image from the container.
 *
 *	\param orderVector vector with the order of 
 *	the derivative for each direction
 *	\param numberOfDirections number of directions 
 *	to take into account in the vector
 *
 *	\return a pointer to an image (null if image not found)
 */
//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
typename blGaussianImageContainer<TImage, MaxDerivativeOrder>::ImagePointerType
blGaussianImageContainer<TImage, MaxDerivativeOrder>::
GetImage(double sigmaValue, const std::vector<unsigned int> & orderVector,
							unsigned int numberOfDirections)
//--------------------------------------------------
{
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "--> blGaussianImageContainer::GetImage(): "; 
	std::cout << "sigma: " << sigmaValue; 
	std::cout << ", orderVector: " ; 
	int i;
	for (i = 0; i <numberOfDirections; i++)
	{
		std::cout << orderVector[i] << " ";
	}
	std::cout << std::endl;
#endif


	// v[x, y, z, ..] --> where v[i] specifies order in ith direction

	ImagePointerType imagePointer = 0;	// NULL
	
	typename MapType::iterator mapIter = this->mapContainer.begin();
	mapIter = this->mapContainer.find(sigmaValue);

	// if sigma entry exists in the map
	if (mapIter != this->mapContainer.end())
	{
		// search for corresponding image according to order and direction

		// root node, it does not contain an image
		NodeType * nodePt = mapIter->second;

		//bool found = false;

		unsigned int currentOrder = 0;
		unsigned int currentDirection = 0;

		// in each iteration we go to next level (next direction)
		while (nodePt && currentDirection < numberOfDirections)
		{
			currentOrder = orderVector[currentDirection];
			nodePt = nodePt->childNodes[currentOrder];	

			currentDirection++;
		}

		// if node found
		if (nodePt)
		{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
		std::cout << "    blGaussianImageContainer::GetImage(): " 
					<< "gaussian image found" << std::endl; 
#endif

			imagePointer = nodePt->imagePointer;
		}

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
		else
		{
			std::cout << "    blGaussianImageContainer::GetImage(): " 
					<< "gaussian image NOT found" << std::endl; 
		}
#endif

	}

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	else
	{
		std::cout << "    blGaussianImageContainer::GetImage(): " 
					<< "gaussian image NOT found" << std::endl; 
	}
#endif

	
	return imagePointer;
}



/**
 *	This method inserts a new image in the container.
 *
 *	\param sigmaValue value of sigma
 *	\param orderVector vector with the order of 
 *	the derivative for each direction
 *	\param numberOfDirections number of directions 
 *	to take into account in the vector
 *	\param image pointer to an image
 */
//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
void blGaussianImageContainer<TImage, MaxDerivativeOrder>::
InsertImage(double sigmaValue, const std::vector<unsigned int> & orderVector,
								unsigned int numberOfDirections,
											ImagePointerType image)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "--> blGaussianImageContainer::InsertImage(): "; 
	std::cout << "sigma: " << sigmaValue; 
	std::cout << ", orderVector: " ; 
	int i;
	for (i = 0; i <numberOfDirections; i++)
	{
		std::cout << orderVector[i] << " ";
	}
	std::cout << std::endl;
#endif

	NodePointerType nodePt;

	typename MapType::iterator mapIter = this->mapContainer.begin();
	mapIter = this->mapContainer.find(sigmaValue);

	// if sigma entry exists in the map
	if (mapIter != this->mapContainer.end())
	{ 
		nodePt = mapIter->second; // get pointer to root node
	}
	else
	{
		// create a new pair <sigmaVal, NodePointerType>
		nodePt = new NodeType();
		this->mapContainer[sigmaValue] = nodePt;
	}

	unsigned int currentDirection = 0;
	unsigned int currentOrder = 0;


	// create nodes if they don't exist
	// in each iteration we go to next level (next direction)
	while (currentDirection < numberOfDirections)
	{
		currentOrder = orderVector[currentDirection];

		// node fot current direction already exists
		if (nodePt->childNodes[currentOrder])
		{
			// nothing			
		}
		else
		{
			// create it
			nodePt->childNodes[currentOrder] = new NodeType();

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
			std::cout << "    blGaussianImageContainer::InsertImage(): " 
				<< "new node: direction " << currentDirection <<
				", order " << currentOrder << std::endl; 
#endif
		}

		nodePt = nodePt->childNodes[currentOrder];

		currentDirection++;
	}

	// assign the image pointer
	nodePt->imagePointer = image;
}
		

/**
 * This method removes all elements from this container
 * 
 */
//--------------------------------------------------
template <class TImage, unsigned int MaxDerivativeOrder>
void blGaussianImageContainer<TImage, MaxDerivativeOrder>::Clear()
//--------------------------------------------------
{
	// delete node pointers
  typename MapType::iterator mapIter = this->mapContainer.begin();
	typename MapType::iterator mapIterEnd = this->mapContainer.end();

	while (mapIter != mapIterEnd)
	{
		delete mapIter->second;	
			// delete node, and it will delete its child nodes

		mapIter++;
	}
	
	// clear map
	this->mapContainer.clear();
}




// ==============================================================
// 			class blIAFeaturesCalculator
// ==============================================================




/** 
 *	Constructor
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
blIAFeaturesCalculator<TInputImage, TOutputImage>::
blIAFeaturesCalculator(): 
inputFeaturesInPairs(false), inputFeaturesInVectors(false)			
//--------------------------------------------------
{
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "--> blIAFeaturesCalculator::CONSTRUCTOR" << std::endl; 
#endif

	// create new filter X
	this->filterX = PrimaryFilterType::New();
	filterX->SetDirection(0);

	// filters for the rest of dimensions
	// dimension - 1, since filterX is not in the vector
	this->filterVector.resize(InputImageType::ImageDimension - 1);

	typedef typename SecondaryFilterType::Pointer PointerType;
	typename std::vector<PointerType>::iterator filterIter =
		this->filterVector.begin();

	unsigned int direction = 1;
	while(filterIter != this->filterVector.end())
	{
		*filterIter = SecondaryFilterType::New();
		(*filterIter)->SetDirection(direction);
		filterIter++;
		direction++;
	}

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "blIAFeaturesCalculator(): InputFilter: filterX" << std::endl;
	std::cout << "blIAFeaturesCalculator(): Number of filters in vector " << 
				this->filterVector.size() << std::endl;
#endif

}



/** 
 *	Destructor
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
blIAFeaturesCalculator<TInputImage, TOutputImage>::
~blIAFeaturesCalculator()
//--------------------------------------------------
{
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "--> blIAFeaturesCalculator::DESTRUCTOR" << std::endl; 
#endif

}




/**
 *	This method sets the features to be calculated, 
 *	with some values of sigma.
 *
 *	\param pairs vector of pairs[FeatureID, sigmaValue]
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAFeaturesCalculator<TInputImage, TOutputImage>::
SetFeatureSigmaPairs(const FeatureSigmaPairsVectorType & pairs)
//--------------------------------------------------
{
	this->featureSigmaPairs = pairs;
	this->inputFeaturesInPairs = true;
	this->inputFeaturesInVectors = false;
}



/**
 *	This method sets the features to be calculated.
 *
 *	Sigma values are used in gaussian blurring.
 *
 *	\param features vector of feature IDs
 *	\param sigmaValues vector of sigma values
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAFeaturesCalculator<TInputImage, TOutputImage>::
SetFeatures(const FeatureIDVectorType & features,
					const SigmaVectorType & sigmaValues)
//--------------------------------------------------
{
	this->featuresVector = features;
	this->sigmaVector = sigmaValues;

	this->inputFeaturesInPairs = false;
	this->inputFeaturesInVectors = true;
}



/**
 *	Generates features images.
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAFeaturesCalculator<TInputImage, TOutputImage>::Update()
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	std::cout << "--> blIAFeaturesCalulator::Update()" << std::endl; 
#endif


	// clear outputVector
	this->outputImages.clear();


	typename OutputImageType::Pointer outputImage;

	// ------------------------------------------------------
	// features given as pairs <featureID, sigmaVal>
	// ------------------------------------------------------
	if (this->inputFeaturesInPairs)
	{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
		std::cout << 
			"blIAFeaturesCalulator::Update(): features in pairs" << std::endl; 
#endif

		// init iterator
		FeatureSigmaPairsVectorType::iterator pairIter = 
										this->featureSigmaPairs.begin();

		// ------------------------------------------------
		// iterate over vector of pairs<featureID, sigmaValue>
		// ------------------------------------------------
		while (pairIter != this->featureSigmaPairs.end())
		{			

			FeatureID currentFeatureID = pairIter->first;
			double currentSigma = pairIter->second;

			// calculate current feature image
			outputImage = this->CalculateFeatureImage(currentFeatureID,
														currentSigma);

			// insert new image in output vector
			this->outputImages.push_back(outputImage);

			// increment iterator for next pair<FeatureID, sigmaValue>
			pairIter++;
		}
	}

	// ------------------------------------------------------
	// features given as two vectors 
	// ------------------------------------------------------
	else if (inputFeaturesInVectors)
	{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
		std::cout << 
			"blIAFeaturesCalulator::Update(): features in vectors" << std::endl; 
#endif

		FeatureIDVectorType::iterator featureIter = this->featuresVector.begin();
		SigmaVectorType::iterator sigmaIter;

		// for each feature
		while (featureIter != this->featuresVector.end())
		{
			FeatureID currentFeatureID = *featureIter;			

			sigmaIter = this->sigmaVector.begin();			

			// for each sigma value
			while (sigmaIter != this->sigmaVector.end())
			{
				double currentSigma = *sigmaIter;

				// calculate current feature image
				outputImage = this->CalculateFeatureImage(currentFeatureID,
															currentSigma);

				// insert new image in output vector
				this->outputImages.push_back(outputImage);

				sigmaIter++;
			}

			featureIter++;
		}		
	}

	else
	{
		std::cerr << "\t Features not set " << std::endl;
	}

	// removes all temporary images
	this->gaussianContainer.Clear();
	this->derivativeMap.clear();
}




/**
 *	Returns information about the derivative
 *
 *	\param featureID derivative ID
 *	\param order derivative order
 *	\param partialOrder a vector containing 
 *	the partial order of derivatives
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAFeaturesCalculator<TInputImage, TOutputImage>::
GetParamsForDerivative(FeatureID featureID, 
						unsigned int & order, 
						std::vector<unsigned int> & partialOrder)
//--------------------------------------------------
{
	// resize and fill with 0 order
	partialOrder.resize(InputImageType::ImageDimension);

	std::vector<unsigned int>::iterator iter = partialOrder.begin();
	while (iter != partialOrder.end())
	{
		*iter = 0;
		iter++;
	}


	switch(featureID)
	{

		case blIAFeatures::L:
		{
			order = 0;
		}
		break;

		case blIAFeatures::Lx:
		{
			order = 1;
			partialOrder[0] = 1;
		}
		break;

		case blIAFeatures::Lxx:
		{
			order = 2;
			partialOrder[0] = 2;
		}
		break;


		case blIAFeatures::Lxy:
		case blIAFeatures::Lyx:
		{
			order = 2;
			partialOrder[0] = 1;
			partialOrder[1] = 1;
		}
		break;

		
		case blIAFeatures::Lxz:
		case blIAFeatures::Lzx:
		{
			order = 2;
			partialOrder[0] = 1;
			partialOrder[2] = 1;
		}
		break;


		case blIAFeatures::Ly:
		{
			order = 1;
			partialOrder[1] = 1;
		}
		break;


		case blIAFeatures::Lyy:
		{
			order = 2;
			partialOrder[1] = 2;
		}
		break;

		
		case blIAFeatures::Lyz:
		case blIAFeatures::Lzy:
		{
			order = 2;
			partialOrder[1] = 1;
			partialOrder[2] = 1;
		}
		break;

		case blIAFeatures::Lz:
		{
			order = 1;
			partialOrder[2] = 1;
		}
		break;

		case blIAFeatures::Lzz:
		{
			order = 2;
			partialOrder[2] = 2;
		}
		break;


		default:
		{
			order = -1;
		}
	}
}



/**
 *	Inserts a derivative image in the map[sigmaVal,map[featureID, ImagePt]]
 *
 *	\param featureID derivative ID
 *	\param sigmaVal value of sigma
 *	\param imagePt pointer to image
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage> 
void blIAFeaturesCalculator<TInputImage, TOutputImage>::
InsertDerivativeImage(FeatureID featureID, double sigmaVal, 
								OutputImageType * imagePt)
//--------------------------------------------------
{
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	std::cout << "--> blIAFeaturesCalculator()::InsertDerivativeImage(): ";

	std::cout << "feature = " ;
	this->PrintFeatureID(featureID);
	std::cout << ", and sigma = " << sigmaVal << std::endl;
#endif
	

	typename OutputImageType::Pointer imageSmartPt(imagePt);

	// search for sigma value in sigmaMap
	typename SigmaFeaturesMapType::iterator derivativeMapIter;
	derivativeMapIter = this->derivativeMap.find(sigmaVal);

	if (derivativeMapIter != this->derivativeMap.end())
	{	
		// a feature map for this sigma value already exists.

		// search for feature ID for this sigma value
		FeaturesMapType & featuresMap = derivativeMapIter->second;
		featuresMap[featureID] = imageSmartPt;
	}
	else
	{
		// create a pair <sigma, featureMap>
		FeaturesMapType featuresMap;
		featuresMap[featureID] = imageSmartPt;

		this->derivativeMap[sigmaVal] = featuresMap;
	}

}



/**
 *	Returns a pointer to a derivative image
 *	It searches for a derivative image in the map 
 *	[sigmaVal,map[featureID, ImagePt]]
 *
 *	\param featureID derivative ID
 *	\param sigmaVal value of sigma
 *	\return pointer to a derivative image
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
typename blIAFeaturesCalculator<TInputImage, TOutputImage>::OutputImagePointer 
blIAFeaturesCalculator<TInputImage, TOutputImage>::
GetDerivativeImage(FeatureID featureID, double sigmaVal)
//--------------------------------------------------
{
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	std::cout << "--> blIAFeaturesCalculator::GetDerivativeImage(): ";

	std::cout << "feature = " ;
	this->PrintFeatureID(featureID);
	std::cout << ", and sigma = " << sigmaVal << std::endl;
#endif	


	typename OutputImageType::Pointer outputImagePt = 0;	// NULL --> not calculated

	// search for sigma value in sigmaMap
	typename SigmaFeaturesMapType::iterator derivativeMapIter;
	derivativeMapIter = this->derivativeMap.find(sigmaVal);

	if (derivativeMapIter != this->derivativeMap.end())
	{		
		// search for feature ID for this sigma value
		FeaturesMapType & featuresMap = derivativeMapIter->second;
		typename FeaturesMapType::iterator featuresMapIter = featuresMap.find(featureID);

		if (featuresMapIter != featuresMap.end())
		{
			// feature alredy calculated
			outputImagePt = featuresMapIter->second; // get image pointer
		}

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
		else
		{
			std::cout << " \t  feature key does not exist" << std::endl; 
		}
#endif

	}

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	else
	{
		std::cout << " \t sigma key does not exist " << std::endl; 
	}
#endif

	return 	outputImagePt;
}



/**
 *	Creates a new derivative image with given parameters, 
 *	and inserts it into the map.
 *
 *	\param featureID derivative ID
 *	\param sigmaVal value of sigma
 *	\return a pointer to a derivative image
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
typename blIAFeaturesCalculator<TInputImage, TOutputImage>::OutputImagePointer 
blIAFeaturesCalculator<TInputImage, TOutputImage>::
GenerateDerivativeImage(FeatureID featureID, double sigmaVal)
//--------------------------------------------------
{

	//	Store new feature images and temporary images in the map, 
	//	so as to avoid to recalculate them 

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "--> blIAFeaturesCalculator::GenerateDerivativeImage()" 
				<< std::endl; 
	std::cout << "\t derivative "; 
	this->PrintFeatureID(featureID);
	std::cout << std::endl; 
#endif

	// image pointers
	typename OutputImageType::Pointer previousGaussianImage;
	typename OutputImageType::Pointer currentGaussianImage;
	typename OutputImageType::Pointer currentDerivativeImage;
	

	// identifying params
	unsigned int order = 0;
	std::vector<unsigned int> partialOrderVector(InputImageType::ImageDimension);

	// get params for current feature
	this->GetParamsForDerivative(featureID, order, partialOrderVector);


	// get intermediate gaussian image
	// search for these gaussian with more directions
	// --------------------------------------------------------------
	int numberOfDirections = InputImageType::ImageDimension - 1;

	while (numberOfDirections > 0 && 
			previousGaussianImage.IsNull() )
	{
		// get gaussian if it exists
		previousGaussianImage = 
			this->gaussianContainer.GetImage(sigmaVal, 
												partialOrderVector,
												numberOfDirections);
		numberOfDirections--;
	}

	
	// if no reviousGaussianImage exists, calculate first intermedian image
	if (previousGaussianImage.IsNull())
	{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR	
	std::cout << "    blIAFeaturesCalculator::GenerateDerivativeImage():" 
				<< "previous gaussian is NULL" << std::endl; 
	std::cout << "    blIAFeaturesCalculator::GenerateDerivativeImage():" 
				<< "number of directions " << numberOfDirections << std::endl;
#endif

		// generate all intermediate gaussian images and new derivative image

			// input image
			this->filterX->SetInput(this->inputImage.GetPointer());

			// sigma
			this->filterX->SetSigma(sigmaVal);

			// order
			if (partialOrderVector[0] == 1)
			{					 
				this->filterX->SetOrder(PrimaryFilterType::FirstOrder);
			}
			else if (partialOrderVector[0] == 2)
			{
				this->filterX->SetOrder(PrimaryFilterType::SecondOrder);
			}
			else
			{
				this->filterX->SetOrder(PrimaryFilterType::ZeroOrder);
			}

			this->filterX->Update();
			previousGaussianImage = this->filterX->GetOutput();
			previousGaussianImage->DisconnectPipeline();	
			

			// insert intermediate gaussian images in the gaussian container
			this->gaussianContainer.InsertImage(sigmaVal, 
												partialOrderVector,
												1, previousGaussianImage);
	}



	// numberOfDirections >= 0
	// numberOfDirections tell us that gaussian image is the output
	// from numberOfDirection_th filter

	// build the pipeline 
	//		from (numberOfDirections + 1)th filter up to last filter
	// set sigma values
	// -----------------------------------------------------------------------

	// index of filter in vector of filters
	int firstFilterIndex = numberOfDirections;

	unsigned i;
	for (i = firstFilterIndex; i < this->filterVector.size(); i++)
	{
		// input image
		this->filterVector[i]->SetInput(
									previousGaussianImage.GetPointer());
		// sigma
		this->filterVector[i]->SetSigma(sigmaVal);

		// order
		if (partialOrderVector[i + 1] == 1)
		{					 
			this->filterVector[i]->SetOrder(SecondaryFilterType::FirstOrder);
		}
		else if (partialOrderVector[i + 1] == 2)
		{
			this->filterVector[i]->SetOrder(SecondaryFilterType::SecondOrder);
		}
		else
		{
			this->filterVector[i]->SetOrder(SecondaryFilterType::ZeroOrder);
		}

		this->filterVector[i]->Update();
		currentGaussianImage = this->filterVector[i]->GetOutput();
		currentGaussianImage->DisconnectPipeline();	
		
		// previous = current ... for next iteration
		previousGaussianImage = currentGaussianImage.GetPointer();

		// last filter
		if (i == (this->filterVector.size() - 1))
		{
			// insert derivative image into derivative map
			this->InsertDerivativeImage(featureID, sigmaVal, 
										currentGaussianImage.GetPointer());
		}
		else
		{
			// insert intermediate gaussian images in the gaussian container
			this->gaussianContainer.InsertImage(sigmaVal, 
												partialOrderVector,
												i + 2, currentGaussianImage);
		}
	}

	currentDerivativeImage = currentGaussianImage;
	
	// return the new feature image
	return currentDerivativeImage;
}



/**
 *	Returns a list of derivative IDs of all those derivatives that are needed 
 *	to generate a the given feature image: featureID.
 *
 *	And choose canonical equivalencies. 
 *	That is: Lxy and Lyx are the same, so it will always returns Lxy.
 *
 *	\param featureID feature ID
 *	\param derivatives a vector with derivatives. 
 *	Derivative IDs will be added at the end.	
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAFeaturesCalculator<TInputImage, TOutputImage>::
ExtractDerivativeIDs(FeatureID featureID, 
					 FeatureIDVectorType & derivatives)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	std::cout << "--> blIAFeaturesCalculator::ExtractDerivativeIDs(): ";	
	std::cout << "from feature :";
	this->PrintFeatureID(featureID);
	std::cout << std::endl;
#endif

	switch(featureID)
	{
		// -------------------------------------------
		// these do not have equivalent derivatives
		// -------------------------------------------
		case blIAFeatures::L:

		case blIAFeatures::Lx:
		case blIAFeatures::Lxx:

		case blIAFeatures::Ly:
		case blIAFeatures::Lyy:

		case blIAFeatures::Lz:
		case blIAFeatures::Lzz:
		{
			// they are explicit derivative expressions
			derivatives.push_back(featureID);
		}
		break;

		// -------------------------------------------
		// these have equivalent derivatives
		// -------------------------------------------

		// canonical form: [x][y][z], in this order.
		// 
		// x and y
		case blIAFeatures::Lxy:
		case blIAFeatures::Lyx:
		{
			derivatives.push_back(blIAFeatures::Lxy);	
				// Lxy is the canonical form
		}
		break;
		
		// x and z
		case blIAFeatures::Lxz:
		case blIAFeatures::Lzx:
		{
			derivatives.push_back(blIAFeatures::Lxz);	
				// Lxz is the canonical form
		}

		// y and z
		case blIAFeatures::Lyz:
		case blIAFeatures::Lzy:
		{
			derivatives.push_back(blIAFeatures::Lyz);	
				// Lyz is the canonical form
		}
		break;


		// -------------------------------------------
		// these represent invariants with Einstein convention
		// -------------------------------------------

		// Einstein convention syntax
		case blIAFeatures::Lii:
		{
			derivatives.push_back(blIAFeatures::Lxx);
			derivatives.push_back(blIAFeatures::Lyy);
			if (OutputImageType::ImageDimension == 3)
			{
				derivatives.push_back(blIAFeatures::Lzz);
			}
		}
		break;

		case blIAFeatures::LiLi:
		{
			derivatives.push_back(blIAFeatures::Lx);
			derivatives.push_back(blIAFeatures::Ly);
			if (OutputImageType::ImageDimension == 3)
			{
				derivatives.push_back(blIAFeatures::Lz);
			}
		}
		break;

		case blIAFeatures::LiLijLj:
		{
			derivatives.push_back(blIAFeatures::Lx);
			derivatives.push_back(blIAFeatures::Ly);
			
			derivatives.push_back(blIAFeatures::Lxx);
			derivatives.push_back(blIAFeatures::Lxy);

			//derivatives.push_back(Lyx);
			derivatives.push_back(blIAFeatures::Lyy);
			
			if (OutputImageType::ImageDimension == 3)
			{
				derivatives.push_back(blIAFeatures::Lz);
				derivatives.push_back(blIAFeatures::Lxz);
				derivatives.push_back(blIAFeatures::Lyz);

				//derivatives.push_back(Lzx);
				//derivatives.push_back(Lzy);
				derivatives.push_back(blIAFeatures::Lzz);
			}
		}
		break;


		case blIAFeatures::LijLji:
		{
			derivatives.push_back(blIAFeatures::Lxx);
			derivatives.push_back(blIAFeatures::Lxy);		

			//derivatives.push_back(Lyx);
			derivatives.push_back(blIAFeatures::Lyy);
			
			if (OutputImageType::ImageDimension == 3)
			{
				derivatives.push_back(blIAFeatures::Lxz);
				derivatives.push_back(blIAFeatures::Lyz);

				//derivatives.push_back(Lzx);
				//derivatives.push_back(Lzy);
				derivatives.push_back(blIAFeatures::Lzz);
			}
		}
		break;

		default:			
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
			std::cout << "\t No derivative extracted from given feature ID" 
						<< std::endl;
#endif
		break;
	}

}

/**
 *	This method calculates a feature image given a feature ID 
 *	and a value for sigma. It inserts the new image into the map 
 *	and returns it.
 *
 *	\param featureID feature ID
 *	\param sigmaVal sigma value
 *	\return a pointer to the new feature image.
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
typename blIAFeaturesCalculator<TInputImage, TOutputImage>::OutputImagePointer 
blIAFeaturesCalculator<TInputImage, TOutputImage>::
CalculateFeatureImage(FeatureID featureID, double sigmaVal)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	std::cout << std::endl;
	std::cout << "--> blIAFeaturesCalculator::CalculateFeatureImage()" 
		<< std::endl;
#endif

	// output image
	typename OutputImageType::Pointer outputImage;
	typename OutputImageType::Pointer derivativeImage;


	// list of derivatives to be calculated for each feature
	FeatureIDVectorType derivativeList;

	// ------------------------------------------------------
	// 1. Extract derivatives from input feature expression
	// ------------------------------------------------------

	this->ExtractDerivativeIDs(featureID, derivativeList);

	// ------------------------------------------------------
	// 2. Compute required derivatives, if not already calculated
	// ------------------------------------------------------

	FeatureID currentDerivative;

	FeatureIDVectorType::iterator derIter = derivativeList.begin();

	while (derIter != derivativeList.end())
	{
		currentDerivative = *derIter;

		derivativeImage = 0; // null
		derivativeImage	= 
			this->GetDerivativeImage(currentDerivative, sigmaVal);

		// if derivative does not exist in the derivative map, generate it
		if (derivativeImage.IsNull())
		{
			// generate current feature image
			this->GenerateDerivativeImage(currentDerivative, sigmaVal);
		}

		derIter++;
	}


	derivativeList.clear(); // empty list of derivatives

	// ------------------------------------------------------
	// 3. Calculate current feature Image
	// ------------------------------------------------------

	outputImage = this->GenerateFeatureImage(featureID, sigmaVal);

	return outputImage;
}



/**
 *	Creates a feature image. 
 *	All derivatives needed for this feature image MUST be previously 
 *	calculated in the map.
 *
 *	\param featureID feature ID
 *	\param sigmaVal value for sigma
 *	\return a pointer to the new feature image
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
typename blIAFeaturesCalculator<TInputImage, TOutputImage>::OutputImagePointer
blIAFeaturesCalculator<TInputImage, TOutputImage>::
GenerateFeatureImage(FeatureID featureID, double sigmaVal)
//--------------------------------------------------
{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
	std::cout << "--> blIAFeaturesCalculator::GenerateFeatureImage()" 
				<< std::endl;
#endif

	typename OutputImageType::Pointer outputImage;

	switch(featureID)
	{

		// without equivalences
		// ----------------------------------
		case blIAFeatures::L:

		case blIAFeatures::Lx:
		case blIAFeatures::Lxx:
		
		case blIAFeatures::Ly:
		case blIAFeatures::Lyy:
		
		case blIAFeatures::Lz:				
		case blIAFeatures::Lzz:
		{
			// get it from the map
			outputImage = GetDerivativeImage(featureID, sigmaVal);
		}
		break;


		// equivalent derivatives
		// --------------------------------
		case blIAFeatures::Lxy:
		case blIAFeatures::Lyx:
		{
			outputImage = GetDerivativeImage(blIAFeatures::Lxy, sigmaVal);	
		}
		break;
		
		case blIAFeatures::Lxz:
		case blIAFeatures::Lzx:
		{
			outputImage = GetDerivativeImage(blIAFeatures::Lxz, sigmaVal);	
		}
		break;

		case blIAFeatures::Lyz:
		case blIAFeatures::Lzy:
		{
			outputImage = GetDerivativeImage(blIAFeatures::Lyz, sigmaVal);	
		}
		break;


		// if featureID is an invariant:
		// ----------------------------------

		// Einstein convention syntax
		case blIAFeatures::Lii:
		{
			// Lii = Lxx + Lyy + Lzz
#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
			std::cout << "    blIAFeaturesCalculator::GenerateFeatureImage():"
					<< "Generating Lii " << std::endl;
#endif

			typename OutputImageType::Pointer imageLxx = 
							GetDerivativeImage(blIAFeatures::Lxx, sigmaVal);

			typename OutputImageType::Pointer imageLyy = 
							GetDerivativeImage(blIAFeatures::Lyy, sigmaVal);

			typename OutputImageType::Pointer imageLzz;

			// for 3D images
			if (InputImageType::ImageDimension == 3)
			{
				imageLzz = GetDerivativeImage(blIAFeatures::Lzz, sigmaVal);
			}

			
			typedef itk::AddImageFilter<OutputImageType, OutputImageType, 
										OutputImageType> AddImageFilterType;

			typename AddImageFilterType::Pointer addFilter = AddImageFilterType::New();

			addFilter->SetInput1(imageLxx);
			addFilter->SetInput2(imageLyy);
			addFilter->Update();


			// for 3D images
			if (InputImageType::ImageDimension == 3)
			{
				typename OutputImageType::Pointer auxImage = addFilter->GetOutput();
				auxImage->DisconnectPipeline();

				addFilter->SetInput1(auxImage);
				addFilter->SetInput2(imageLzz);
				addFilter->Update();
			}

			outputImage = addFilter->GetOutput();
			outputImage->DisconnectPipeline();	

		}
		break;

		case blIAFeatures::LiLi:
		{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
			std::cout << "    blIAFeaturesCalculator::GenerateFeatureImage():"
						<< "Generating LiLi " << std::endl;
#endif

			// LiLi = LxLx + LyLy + LzLz

			// filters
			typename AddImageFilterType::Pointer addFilter = AddImageFilterType::New();

			typename SquareImageFilterType::Pointer squareFilter = 
												SquareImageFilterType::New();



			typename OutputImageType::Pointer imageLx = 
							GetDerivativeImage(blIAFeatures::Lx, sigmaVal);

			typename OutputImageType::Pointer imageLy = 
							GetDerivativeImage(blIAFeatures::Ly, sigmaVal);

			typename OutputImageType::Pointer imageLz;

			typename OutputImageType::Pointer imageLx2;
			typename OutputImageType::Pointer imageLy2;
			typename OutputImageType::Pointer imageLz2;
		
			typename OutputImageType::Pointer auxImage;

			// for 3D images
			if (InputImageType::ImageDimension == 3)
			{
				imageLz = GetDerivativeImage(blIAFeatures::Lz, sigmaVal);
			}

			// Generate LxLx, LyLy and LzLz
			// -------------------------------------

			// imageLx2 = LxLx		
			squareFilter->SetInput(imageLx);
			squareFilter->Update();
			imageLx2 = squareFilter->GetOutput();
			imageLx2->DisconnectPipeline();
			

			// imageLy2 = LyLy		
			squareFilter->SetInput(imageLy);
			squareFilter->Update();
			imageLy2 = squareFilter->GetOutput();
			imageLy2->DisconnectPipeline();

			
			// for 3D images
			if (InputImageType::ImageDimension == 3)
			{
				// imageLz2 = LzLz		
				squareFilter->SetInput(imageLz);
				squareFilter->Update();
				imageLz2 = squareFilter->GetOutput();
				imageLz2->DisconnectPipeline();
			}			
	

			// Sum LxLx + LyLy + LzLz = LiLi
			// -------------------------------------
			addFilter->SetInput1(imageLx2);
			addFilter->SetInput2(imageLy2);
			addFilter->Update();


			// for 3D images
			if (InputImageType::ImageDimension == 3)
			{
				auxImage = addFilter->GetOutput();
				auxImage->DisconnectPipeline();

				addFilter->SetInput1(auxImage);
				addFilter->SetInput2(imageLz2);
				addFilter->Update();
			}

			outputImage = addFilter->GetOutput();
			outputImage->DisconnectPipeline();	
		}
		break;

		case blIAFeatures::LiLijLj:
		{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
			std::cout << "    blIAFeaturesCalculator::GenerateFeatureImage():"
						<< "Generating LiLijLj " << std::endl;
#endif

			// LxLxxLx + LxLxyLy + LxLxzLz +
			// LyLyxLx + LyLyyLy + LyLyzLz +
			// LzLzxLx + LzLzyLy + LzLzzLz 

			// if Lij == Lji --> 
			// Lx^2Lxx + 2 * LxLxyLy + 2 * LxLxzLz +
			// Ly^2Lyy + 2 * LyLyzLz + Lz^2Lzz

			// Lx^2Lxx + 2 * LxLxyLy + Ly^2Lyy +		<-- x, y
			// [2 * LyLyzLz + Lz^2Lzz + 2 * LxLxzLz]	<-- z

			// derivatives: 
			// ----------------------------------------
			// Lx, Lxx, Lxy, Ly, Lyx, Lyy, 
			// [Lxz, Lyz, Lz, Lzx, Lzy, Lzz]



			// filters
			typename AddImageFilterType::Pointer addFilter = AddImageFilterType::New();

			typename MultiplyFilterType::Pointer multiplyFilter = MultiplyFilterType::New();


			typename OutputImageType::Pointer imageLx = 
								GetDerivativeImage(blIAFeatures::Lx, sigmaVal);
			typename OutputImageType::Pointer imageLxx = 
							GetDerivativeImage(blIAFeatures::Lxx, sigmaVal);

			typename OutputImageType::Pointer imageLxy = 
							GetDerivativeImage(blIAFeatures::Lxy, sigmaVal);

			typename OutputImageType::Pointer imageLxz;

			typename OutputImageType::Pointer imageLy = 
							GetDerivativeImage(blIAFeatures::Ly, sigmaVal);	

			typename OutputImageType::Pointer imageLyy = 
							GetDerivativeImage(blIAFeatures::Lyy, sigmaVal);

			typename OutputImageType::Pointer imageLyz;			

			typename OutputImageType::Pointer imageLz;
			typename OutputImageType::Pointer imageLzz;

			typename OutputImageType::Pointer auxImage; 

			typename OutputImageType::Pointer imageLxLx;
			typename OutputImageType::Pointer imageLxLy;
			typename OutputImageType::Pointer imageLyLy;
			typename OutputImageType::Pointer imageLxLxLxx;
			typename OutputImageType::Pointer imageLyLyLyy;
			typename OutputImageType::Pointer imageLxLyLxy;
			typename OutputImageType::Pointer image2LxLyLxy;



			// if 3D images
			// -----------------------------------
			if (InputImageType::ImageDimension == 3)
			{
				imageLxz = GetDerivativeImage(blIAFeatures::Lxz, sigmaVal);
				imageLyz = GetDerivativeImage(blIAFeatures::Lyz, sigmaVal);			

				imageLz = GetDerivativeImage(blIAFeatures::Lz, sigmaVal);
				imageLzz = GetDerivativeImage(blIAFeatures::Lzz, sigmaVal);
			}

			
			// -----------------------
			// LxLxLxx
			// -----------------------

			// LxLx = Lx * Lx
			multiplyFilter->SetInput1(imageLx);
			multiplyFilter->SetInput2(imageLx);
			multiplyFilter->Update();

			imageLxLx = multiplyFilter->GetOutput();
			imageLxLx->DisconnectPipeline();
						
			
			// LxLxLxx = LxLx * Lxx
			multiplyFilter->SetInput1(imageLxLx);
			multiplyFilter->SetInput2(imageLxx);
			multiplyFilter->Update();

			imageLxLxLxx = multiplyFilter->GetOutput();
			imageLxLxLxx->DisconnectPipeline();
						

			// -----------------------
			// LyLyLyy
			// -----------------------

			// LyLy = Ly * Ly
			multiplyFilter->SetInput1(imageLy);
			multiplyFilter->SetInput2(imageLy);
			multiplyFilter->Update();

			imageLyLy = multiplyFilter->GetOutput();
			imageLyLy->DisconnectPipeline();
						
			
			// LyLyLyy = LyLy * Lyy
			multiplyFilter->SetInput1(imageLyLy);
			multiplyFilter->SetInput2(imageLyy);
			multiplyFilter->Update();

			imageLyLyLyy = multiplyFilter->GetOutput();
			imageLyLyLyy->DisconnectPipeline();
						

			// -----------------------
			// 2 * LxLxyLy
			// -----------------------

			// LxLy = Lx * Ly
			multiplyFilter->SetInput1(imageLx);
			multiplyFilter->SetInput2(imageLy);
			multiplyFilter->Update();

			imageLxLy = multiplyFilter->GetOutput();
			imageLxLy->DisconnectPipeline();
						

			// LxLyLxy = LxLy * Lxy
			multiplyFilter->SetInput1(imageLxLy);
			multiplyFilter->SetInput2(imageLxy);
			multiplyFilter->Update();

			imageLxLyLxy = multiplyFilter->GetOutput();
			imageLxLyLxy->DisconnectPipeline();
						

			// 2LxLxyLy = 2 * LxLxyLy
			addFilter->SetInput1(imageLxLyLxy);
			addFilter->SetInput2(imageLxLyLxy);
			addFilter->Update();

			image2LxLyLxy = addFilter->GetOutput();
			image2LxLyLxy->DisconnectPipeline();
			
			
	
			// if 3D images
			// -------------------------------------------
			typename OutputImageType::Pointer image2LxLzLxz;
			typename OutputImageType::Pointer image2LyLzLyz;
			typename OutputImageType::Pointer imageLzLzLzz;


			if (InputImageType::ImageDimension == 3)
			{
				
				// [2 * LxLxzLz + 2 * LyLyzLz + Lz^2Lzz + ]	<-- z

				// -----------------------
				// 2 * LxLxzLz
				// -----------------------
				typename OutputImageType::Pointer imageLxLz;
				typename OutputImageType::Pointer imageLxLzLxz;
				

				// LxLz = Lx * Lz
				multiplyFilter->SetInput1(imageLx);
				multiplyFilter->SetInput2(imageLz);
				multiplyFilter->Update();
				imageLxLz = multiplyFilter->GetOutput();
				imageLxLz->DisconnectPipeline();

				// LxLzLxz = LxLz * Lxz
				multiplyFilter->SetInput1(imageLxLz);
				multiplyFilter->SetInput2(imageLxz);
				multiplyFilter->Update();
				imageLxLzLxz = multiplyFilter->GetOutput();
				imageLxLzLxz->DisconnectPipeline();

				// 2LxLxzLz = 2 * LxLxzLz
				addFilter->SetInput1(imageLxLzLxz);
				addFilter->SetInput2(imageLxLzLxz);
				addFilter->Update();
				image2LxLzLxz = addFilter->GetOutput();
				image2LxLzLxz->DisconnectPipeline();


				// -----------------------
				// 2 * LyLzLyz
				// -----------------------
				typename OutputImageType::Pointer imageLyLz;
				typename OutputImageType::Pointer imageLyLzLyz;
				

				// LyLz = Ly * Lz
				multiplyFilter->SetInput1(imageLy);
				multiplyFilter->SetInput2(imageLz);
				multiplyFilter->Update();
				imageLyLz = multiplyFilter->GetOutput();
				imageLyLz->DisconnectPipeline();

				// LyLzLyz = LyLz * Lyz
				multiplyFilter->SetInput1(imageLyLz);
				multiplyFilter->SetInput2(imageLyz);
				multiplyFilter->Update();
				imageLyLzLyz = multiplyFilter->GetOutput();
				imageLyLzLyz->DisconnectPipeline();


				// 2LyLyzLz = 2 * LyLyzLz
				addFilter->SetInput1(imageLyLzLyz);
				addFilter->SetInput2(imageLyLzLyz);
				addFilter->Update();
				image2LyLzLyz = addFilter->GetOutput();
				image2LyLzLyz->DisconnectPipeline();


				// -----------------------
				// LzLzLzz
				// -----------------------
				typename OutputImageType::Pointer imageLzLz;				
				
				// LzLz = Lz * Lz
				multiplyFilter->SetInput1(imageLz);
				multiplyFilter->SetInput2(imageLz);
				multiplyFilter->Update();

				imageLzLz = multiplyFilter->GetOutput();
				imageLzLz->DisconnectPipeline();

				// LzLzLzz = LzLz * Lzz
				multiplyFilter->SetInput1(imageLzLz);
				multiplyFilter->SetInput2(imageLzz);
				multiplyFilter->Update();
				imageLzLzLzz = multiplyFilter->GetOutput();
				imageLzLzLzz->DisconnectPipeline();
			}


			// Lx^2Lxx + 2 * LxLxyLy + Ly^2Lyy +		<-- x, y
			// [2 * LyLyzLz + Lz^2Lzz + 2 * LxLxzLz]	<-- z
			

			// auxImage = LxLxLxx + 2LxLxyLy
			addFilter->SetInput1(imageLxLxLxx);
			addFilter->SetInput2(image2LxLyLxy); 
			addFilter->Update();

			auxImage = addFilter->GetOutput();
			auxImage->DisconnectPipeline();
	

			// output = LxLxLxx + 2LxLxyLy + LyLyLyy
			addFilter->SetInput1(auxImage);
			addFilter->SetInput2(imageLyLyLyy);
			addFilter->Update();

			outputImage = addFilter->GetOutput();
			outputImage->DisconnectPipeline();
			

			if (InputImageType::ImageDimension == 3)
			{
				// output = LxLxLxx + 2LxLxyLy + LyLyLyy + 2LxLxzLz
				addFilter->SetInput1(outputImage);
				addFilter->SetInput2(image2LxLzLxz);
				addFilter->Update();
				outputImage = addFilter->GetOutput();
				outputImage->DisconnectPipeline();

				// output = LxLxLxx + 2LxLxyLy + LyLyLyy + 2LxLxzLz + 2LyLyzLz
				addFilter->SetInput1(outputImage);
				addFilter->SetInput2(image2LyLzLyz);
				addFilter->Update();
				outputImage = addFilter->GetOutput();
				outputImage->DisconnectPipeline();

				// output = LxLxLxx + 2LxLxyLy + LyLyLyy + 2LxLxzLz + 2LyLyzLz + LzLzLzz
				addFilter->SetInput1(outputImage);
				addFilter->SetInput2(imageLzLzLzz);
				addFilter->Update();
				outputImage = addFilter->GetOutput();
				outputImage->DisconnectPipeline();
			}
		}
		break;


		case blIAFeatures::LijLji:
		{

#ifdef DEBUG_MESSAGES_IAFEATURESCALCULATOR
			std::cout << "    blIAFeaturesCalculator::GenerateFeatureImage():" 
						<< "Generating LijLji " << std::endl;
#endif

			//	LxxLxx + LxyLyx + LxzLzx + 
			//	LyxLxy + LyyLyy + LyzLzy + 
			//	LzxLxz + LzyLyz + LzzLzz

			// if Lij == Lji -->
			//
			//	LxxLxx + 2LxyLxy + LyyLyy
			//	[2LxzLxz + 2LyzLyz + LzzLzz]


			// Derivatives: Lxx, Lxy, Lyy, Lxz, Lyz, Lzz


			// filters
			typename AddImageFilterType::Pointer addFilter = AddImageFilterType::New();

			typename MultiplyFilterType::Pointer multiplyFilter = MultiplyFilterType::New();

			typename SquareImageFilterType::Pointer squareFilter = 
												SquareImageFilterType::New();

			
			// get images
			typename OutputImageType::Pointer imageLxx = 
							GetDerivativeImage(blIAFeatures::Lxx, sigmaVal);
			typename OutputImageType::Pointer imageLxy = 
							GetDerivativeImage(blIAFeatures::Lxy, sigmaVal);
			typename OutputImageType::Pointer imageLxz;

			typename OutputImageType::Pointer imageLyy = 
							GetDerivativeImage(blIAFeatures::Lyy, sigmaVal);			

			typename OutputImageType::Pointer imageLyz;			
			
			typename OutputImageType::Pointer imageLzz;


				// for 3D images
			if (InputImageType::ImageDimension == 3)
			{
				imageLxz = GetDerivativeImage(blIAFeatures::Lxz, sigmaVal);
				imageLyz = GetDerivativeImage(blIAFeatures::Lyz, sigmaVal);
				imageLzz = GetDerivativeImage(blIAFeatures::Lzz, sigmaVal);
			}

			// -----------------
			// LxxLxx
			// -----------------

			typename OutputImageType::Pointer imageLxxLxx;

			squareFilter->SetInput(imageLxx);
			squareFilter->Update();
			imageLxxLxx = squareFilter->GetOutput();
			imageLxxLxx->DisconnectPipeline();


			// -----------------
			// 2LxyLxy
			// -----------------

			typename OutputImageType::Pointer imageLxyLxy;
			typename OutputImageType::Pointer image2LxyLxy;

			// LxyLxy = Lxy^2
			squareFilter->SetInput(imageLxy);
			squareFilter->Update();
			imageLxyLxy = squareFilter->GetOutput();
			imageLxyLxy->DisconnectPipeline();

			// 2LxyLxy = 2 * LxyLxy
			addFilter->SetInput1(imageLxyLxy);
			addFilter->SetInput2(imageLxyLxy);
			addFilter->Update();
			image2LxyLxy = addFilter->GetOutput();
			image2LxyLxy->DisconnectPipeline();
			
			// -----------------
			// LyyLyy
			// -----------------

			typename OutputImageType::Pointer imageLyyLyy;

			squareFilter->SetInput(imageLyy);
			squareFilter->Update();
			imageLyyLyy = squareFilter->GetOutput();
			imageLyyLyy->DisconnectPipeline();


			// for 3D images

			typename OutputImageType::Pointer image2LxzLxz;
			typename OutputImageType::Pointer image2LyzLyz;
			typename OutputImageType::Pointer imageLzzLzz;


			if (InputImageType::ImageDimension == 3)
			{
				//	[2LxzLxz + 2LyzLyz + LzzLzz]				

				// -----------------
				// 2LxzLxz
				// -----------------
				typename OutputImageType::Pointer imageLxzLxz;
				

				// LxzLxz = Lxz^2
				squareFilter->SetInput(imageLxz);
				squareFilter->Update();
				imageLxzLxz = squareFilter->GetOutput();
				imageLxzLxz->DisconnectPipeline();

				// 2LxzLxz = 2 * LxzLxz
				addFilter->SetInput1(imageLxzLxz);
				addFilter->SetInput2(imageLxzLxz);
				addFilter->Update();
				image2LxzLxz = addFilter->GetOutput();
				image2LxzLxz->DisconnectPipeline();
			

				// -----------------
				// 2LyzLyz
				// -----------------
				typename OutputImageType::Pointer imageLyzLyz;				

				// LxzLxz = Lxz^2
				squareFilter->SetInput(imageLyz);
				squareFilter->Update();
				imageLyzLyz = squareFilter->GetOutput();
				imageLyzLyz->DisconnectPipeline();

				// 2LyzLyz = 2 * LyzLyz
				addFilter->SetInput1(imageLyzLyz);
				addFilter->SetInput2(imageLyzLyz);
				addFilter->Update();
				image2LyzLyz = addFilter->GetOutput();
				image2LyzLyz->DisconnectPipeline();


				// -----------------
				// LzzLzz
				// -----------------				
				squareFilter->SetInput(imageLzz);
				squareFilter->Update();
				imageLzzLzz = squareFilter->GetOutput();
				imageLzzLzz->DisconnectPipeline();
			}


			//	LxxLxx + 2LxyLxy + LyyLyy
			//	[2LxzLxz + 2LyzLyz + LzzLzz]


			// output = LxxLxx + 2LxyLxy
			addFilter->SetInput1(imageLxxLxx);
			addFilter->SetInput2(image2LxyLxy);
			addFilter->Update();
			outputImage = addFilter->GetOutput();
			outputImage->DisconnectPipeline();

			// output = LxxLxx + 2LxyLxy + LyyLyy
			addFilter->SetInput1(outputImage);
			addFilter->SetInput2(imageLyyLyy);
			addFilter->Update();
			outputImage = addFilter->GetOutput();
			outputImage->DisconnectPipeline();


			if (InputImageType::ImageDimension == 3)
			{
				// output = LxxLxx + 2LxyLxy + LyyLyy + 2LxzLxz
				addFilter->SetInput1(outputImage);
				addFilter->SetInput2(image2LxzLxz);
				addFilter->Update();
				outputImage = addFilter->GetOutput();
				outputImage->DisconnectPipeline();


				// output = LxxLxx + 2LxyLxy + LyyLyy + 2LxzLxz + 2LyzLyz
				addFilter->SetInput1(outputImage);
				addFilter->SetInput2(image2LyzLyz);
				addFilter->Update();
				outputImage = addFilter->GetOutput();
				outputImage->DisconnectPipeline();

				// output = LxxLxx + 2LxyLxy + LyyLyy + 2LxzLxz + 2LyzLyz + LzzLzz
				addFilter->SetInput1(outputImage);
				addFilter->SetInput2(imageLzzLzz);
				addFilter->Update();
				outputImage = addFilter->GetOutput();
				outputImage->DisconnectPipeline();
			}
		}
		break;
	}

	return outputImage;
}



/**
 *	Writes a feature ID in standart output stream
 */
//--------------------------------------------------
template <class TInputImage, class TOutputImage>
void blIAFeaturesCalculator<TInputImage, TOutputImage>::
PrintFeatureID(FeatureID featureID)
//--------------------------------------------------
{

	switch(featureID)
	{

		case blIAFeatures::L: std::cout << "L" ; 	break;

		case blIAFeatures::Lx: std::cout << "Lx" ; break;
		case blIAFeatures::Lxx: std::cout << "Lxx" ; break;
		case blIAFeatures::Lxy: std::cout << "Lxy" ; break;
		case blIAFeatures::Lxz:

		case blIAFeatures::Ly: std::cout << "Ly" ; break;
		case blIAFeatures::Lyx: std::cout << "Lyx" ; break;
		case blIAFeatures::Lyy: std::cout << "Lyy" ; break;
		case blIAFeatures::Lyz: std::cout << "Lyz" ; break;

		case blIAFeatures::Lz:  std::cout << "Lz" ; break;
		case blIAFeatures::Lzx: std::cout << "Lzx" ; break;
		case blIAFeatures::Lzy: std::cout << "Lzy" ; break;
		case blIAFeatures::Lzz: std::cout << "Lzz" ; break;

		case blIAFeatures::LiLi:  std::cout << "LiLi" ; break;
		case blIAFeatures::Lii: std::cout << "Lii" ; break;
		case blIAFeatures::LiLijLj: std::cout << "LiLijLj" ; break;
		case blIAFeatures::LijLji: std::cout << "LijLji" ; break;

		default: std::cout << "Unknown" ; break;
	}
}



#endif
