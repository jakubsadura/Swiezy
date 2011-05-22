/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blIAFeaturesCalculator_h
#define __blIAFeaturesCalculator_h

#include <map>
#include <utility>
#include <vector>

#include <itkAddImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkSquareImageFilter.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"


// for compilation with/without debug messages

//#define DEBUG_MESSAGES_IAFEATURESCALCULATOR


const int MAX_DERIVATIVE_ORDER = 2;

/**
 * \brief A node for GaussianImageContainer
 * \ingroup blImageProperties
 *
 *	It stores a pointer to a Gaussian image and 
 *	a vector of pointers to its child nodes.
 */
template <class TImage, unsigned int MaxDerivativeOrder>
class blGaussianImageContainerNode
{
	public:

		blGaussianImageContainerNode();
		~blGaussianImageContainerNode();

		blGaussianImageContainerNode * childNodes[MaxDerivativeOrder + 1];
		typename TImage::Pointer imagePointer;
};




/**
 * \brief Container for gaussian images 
 * \ingroup blImageProperties
 *
 *	This class is a map of pairs<sigmaValue, Tree>
 *
 *	A Tree starts with a root node containing only pointers to childNodes.
 *
 *	Each node contains:
 *		- pointers to child nodes (as many as number of maxDerivativeOrder + 1)
 *		- pointer to gaussian image stored in it
 *
 *	So, each pointer defines a different order of derivative.
 */
template <class TImage, unsigned int MaxDerivativeOrder>
class blGaussianImageContainer
{

	public:

		typedef TImage ImageType;
		typedef typename ImageType::Pointer ImagePointerType;


		blGaussianImageContainer();
		~blGaussianImageContainer();

		/**\brief Returns an image corresponding to given parameters*/
		ImagePointerType GetImage(double sigmaValue, 
							const std::vector<unsigned int> & orderVector,
							unsigned int numberOfDirections);

		/**\brief Inserts an image in the container*/
		void InsertImage(double sigmaValue,  
							const std::vector<unsigned int> & orderVector,
							unsigned int numberOfDirections,
							ImagePointerType image);

		/**\brief removes all elements from this container*/
		void Clear();

	private:

		typedef blGaussianImageContainerNode<ImageType, MaxDerivativeOrder> NodeType;
		typedef NodeType* NodePointerType;

		typedef std::map<double, NodePointerType> MapType;


		MapType mapContainer;		// map <sigmaValues, NodePointer>

};



/**
 *	Namespace where FeatureID enum type is defined.
 */
namespace blIAFeatures
{
	/**\brief Features IDs */
	enum FeatureID {Lx, Ly, Lz, 
						Lxx, Lxy, Lxz, 
							Lyx, Lyy, Lyz, 
							Lzx, Lzy, Lzz,

							L, Lii, LiLi, LiLijLj, LijLji};

	/**\brief Type of vector of feature IDs*/
	typedef std::vector<FeatureID> FeatureIDVectorType;
}	

///using namespace blIAFeatures;



/**
 * \brief Computes features images for OF-ASM algorithm
 * \ingroup blImageProperties
 *
 *	This class can calculate some features from a given input image:
 *
 *	These features are derivatives or invariants up to 2nd order, for 2D/3D:
 *	- Derivatives : 
 *		Lx, Ly, Lz, Lxx, Lxy, Lxz, Lyx, Lyy, Lyz, Lzx, Lzy, Lzz
 *
 *	- Invariants :
 *		L, Lii, LiLi, LiLijLj, LijLji		
 *
 *
 *	All features are calculated with a gaussian filter, 
 *	so it is necessary to choose a value for sigma, for each feature.
 *
 *	The features to be calculated can be specified in two different ways:
 *
 *	- a vector of features (IDs) and a vector of sigma values, or
 *	- a vector of pairs, [featureID, sigmaValue]
 *
 *	In the first case, each feature image is generated for each sigma value.
 *	On the other hand, for the second case features are generated with its 
 *	corresponding sigma value in each pair.
 *
 *	The output images are returned in a vector of pointers.
 *
 *		- if a vector of pairs was set as input, the vector of output images 
 *		keeps the same ordering.
 *	
 *		- if two vectors (features and sigmas), the vector of output images 
 *		will contain all images for first feature with different sigma 
 *		values, then next feature with all sigma values.. and so.
 */
template <class TInputImage, class TOutputImage>
class blIAFeaturesCalculator: public blLightObject
{

	public:

		// ---------------------------------
		// Typedefs
		// ---------------------------------

		typedef blIAFeaturesCalculator Self;
		typedef blSmartPointer<Self> Pointer;



		/**\brief type of feature ID*/
		typedef blIAFeatures::FeatureID FeatureID;

		/**\brief type of vector of feature IDs*/
		typedef blIAFeatures::FeatureIDVectorType FeatureIDVectorType;


		/**\brief Type of input image*/
		typedef TInputImage InputImageType;

		/**\brief Type of output image*/
		typedef TOutputImage OutputImageType;

		/**\brief Type of pointer to input image*/
		typedef typename InputImageType::Pointer InputImagePointer;

		/**\brief Type of pointer to output image*/
		typedef typename OutputImageType::Pointer OutputImagePointer;

		/**\brief Type of vector of output images*/
		typedef std::vector<OutputImagePointer> OutputImageVectorType;

		/**\brief Type of vector of feature IDs*/
//		typedef std::vector<FeatureID> FeaturesVectorType;

		/**\brief Type of vector of sigma values*/
		typedef std::vector<double> SigmaVectorType;

		/**\brief Type of pair [FeatureID, sigmaValue]*/
		typedef std::pair<FeatureID, double> FeatureSigmaPairType;

		/**\brief Type of vector of pairs [FeatureID, sigmaValue]*/
		typedef std::vector<FeatureSigmaPairType> FeatureSigmaPairsVectorType;


		// --------------------------------
		// Methods
		// --------------------------------
		
		/**\brief Static constructor */
		blNewMacro(Self);


		/**\brief returns the input image
		 *	This methods returns the input image previously set
		 *
		 *	\return input image pointer
		 */
		InputImageType * GetInputImage()
		{
			return this->inputImage;
		};
		

		/**\brief returns the output of the filter
		 *	This method returns all output feature images in a vector
		 *
		 *	\return vector with feature images
		 */
		OutputImageVectorType & GetOutput()
		{
			return this->outputImages;
		};


		/**\brief returns the vectors of feature IDs 
		 *	This method returns all the feature IDs
		 *
		 *	\return a vector of feature IDs
		 */
		const FeatureIDVectorType & GetFeatures()
		{
			return this->featuresVector;
		};


		/**\brief returns the pairs [featureID, sigma] 
		 *
		 *	This method returns all the pairs [featureID, sigmaValue]
		 *
		 *	\return a vector of pairs
		 */
		const FeatureSigmaPairsVectorType & GetFeatureSigmaPairs()
		{
			return this->featureSigmaPairs;
		};


		/**\brief returns the vector of values for sigma
		 *
		 *	This method returns all values for sigma. 
		 *	These values are used for gaussian blurring.
		 *
		 *	\return a vector of values for sigma
		 */
		const SigmaVectorType & GetSigmaValues()
		{
			return this->sigmaVector;
		};
		

		/**\brief sets the input image
		 *	This method sets the input image
		 *
		 *	\param inputImage pointer to the input image
		 */
		void SetInputImage(InputImageType * inputImage)
		{
			this->inputImage = inputImage;
		};


		/**\brief sets the pairs [feature, sigma] to be calculated */
		void SetFeatureSigmaPairs(const FeatureSigmaPairsVectorType & pairs);

		/**\brief sets the features to be calculates */
		void SetFeatures(const FeatureIDVectorType & features,
							const SigmaVectorType & sigmaValues);


		/**\brief generates the output images */
		void Update();

			

	protected:

		/** \brief Empty Constructor */
		blIAFeaturesCalculator();   


		/** \brief Destructor */
		virtual ~blIAFeaturesCalculator();  

    private:        

		// ---------------------------------
		// Typedefs
		// ---------------------------------

		// itk filters for invariants
		typedef itk::AddImageFilter<OutputImageType, OutputImageType, 
										OutputImageType> AddImageFilterType;

		typedef itk::MultiplyImageFilter<OutputImageType, OutputImageType, 
										OutputImageType> MultiplyFilterType;
									
		typedef itk::SquareImageFilter<OutputImageType, OutputImageType>
														SquareImageFilterType;


		// gaussian filters
		typedef itk::RecursiveGaussianImageFilter<InputImageType, 
													OutputImageType>
															PrimaryFilterType;

		typedef itk::RecursiveGaussianImageFilter<OutputImageType, 
													OutputImageType>
														SecondaryFilterType;
		

		// ---------------------------------
		// Member vars.
		// ---------------------------------		

		// input data
		// ---------------------------------
		bool inputFeaturesInPairs;	
			//!< are input features in a vector of pairs<featureID, sigma>

		bool inputFeaturesInVectors;
			//!< are input features in a two vectors features and sigmas
		
		FeatureSigmaPairsVectorType featureSigmaPairs;	
				// vector of pairs feature/sigma

		FeatureIDVectorType featuresVector;	// vector of feature IDs 
		SigmaVectorType sigmaVector;		// vector ot sigma values

		
		typename InputImageType::Pointer inputImage; //!< input image


		// output data
		// ---------------------------------
		std::vector<typename OutputImageType::Pointer> outputImages;	//!< output images


		// temporary data
		// ---------------------------------
		
		// maps for creating a table sigma x features(derivatives) 
		// containing Image Pointers
		typedef std::map<FeatureID, typename OutputImageType::Pointer> FeaturesMapType;
		typedef std::map<double, FeaturesMapType> SigmaFeaturesMapType;

	
		SigmaFeaturesMapType derivativeMap;	
				// map of pairs <sigma, featuresMap> 
				// featuresMap of pairs <featureID, ImagePointer>


		typedef blGaussianImageContainer<OutputImageType, 
						MAX_DERIVATIVE_ORDER> GaussianContainerType;

		GaussianContainerType gaussianContainer;
			// container of temporary gaussian image pointers 


		// filters
		// --------------------------
		typename PrimaryFilterType::Pointer filterX;		// filter for direction 0
		std::vector<typename SecondaryFilterType::Pointer> filterVector;	// filters for direction > 0

		

		// ---------------------------------
		// Methods
		// ---------------------------------
		
		blIAFeaturesCalculator(const Self&); //purposely not implemented
		void operator=(const Self&); //purposely not implemented

	
		/**\brief Calculates a feature image */
		OutputImagePointer CalculateFeatureImage(FeatureID featureID, double sigmaVal);

		/**\brief Extracts derivatives to be calculated for this feature ID*/
		void ExtractDerivativeIDs(FeatureID featureID, 
									FeatureIDVectorType & derivatives);

		/**\brief Generates a derivative image with given parameters*/
		OutputImagePointer GenerateDerivativeImage(FeatureID featureID, double sigmaVal);


		/**\brief Generates a feature image with given parameters*/
		OutputImagePointer GenerateFeatureImage(FeatureID featureID, double sigmaVal);

		/**\brief Returns a derivative image from the map*/
		OutputImagePointer GetDerivativeImage(FeatureID featureID, double sigmaVal);



		/**\brief Returns params that define a given feature ID*/
		void GetParamsForDerivative(FeatureID featureID, 
									unsigned int & order, 
									std::vector<unsigned int> &partialOrder);

		/**\brief Inserts a derivative image into the map*/
		void InsertDerivativeImage(FeatureID featureID, double sigmaVal, 
											OutputImageType * imagePt);


		
		/**\brief Writes a feature ID in std::cout*/
		void PrintFeatureID(FeatureID featureID);

		

};

#include "blIAFeaturesCalculator.txx"
#endif
