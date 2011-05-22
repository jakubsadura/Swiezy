/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BL4DIMAGETOVECTOROF3DIMAGEFILTER_H
#define BL4DIMAGETOVECTOROF3DIMAGEFILTER_H

#include "itkImage.h"
#include "itkProcessObject.h"
#include "itkDataObject.h"
#include <vector>

/**
\brief This class is an ITK container for a vector of images. It is used in blImage4DToVectorOfImage3DFilter.
\ingroup blImageUtilities

\author Maarten Nieber
\date 12 Nov 2007
*/

template< class Image_  >
class blImageCollection : public itk::DataObject
{
public:
	//! Standard itk typedefs
	typedef blImageCollection Self;
	typedef itk::DataObject ParentType;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;

	/** Run-time type information (and related methods). */
	itkTypeMacro(blImageCollection,ParentType);
	itkFactorylessNewMacro(blImageCollection);

	//! Holds the images
	std::vector<typename Image_::Pointer> images;
};

/**
\brief This class splits a x,y,z,t image into a vector of n (x,y,z) images,
where n is the number of time-points.
\ingroup blImageUtilities

The output of the filter is of type blImageCollection.

\todo Add concept check for template arguments.
*/
template< class Image4D_, class Image3D_  >
class blImage4DToVectorOfImage3DFilter : public itk::ProcessObject
{
public:
	typedef blImage4DToVectorOfImage3DFilter Self;
	typedef itk::ProcessObject ParentType;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;
	typedef Image4D_ InputImageType;
	typedef Image3D_ OutputImageType;
	typedef blImageCollection<Image3D_> ImageCollectionType;

	/** Run-time type information (and related methods). */
	itkTypeMacro(blImage4DToVectorOfImage3DFilter,ParentType);
	itkFactorylessNewMacro(blImage4DToVectorOfImage3DFilter);

	//!
	blImage4DToVectorOfImage3DFilter()
	{
		this->SetNumberOfRequiredInputs(1);
		ImageCollectionType::Pointer output = ImageCollectionType::New();
		this->SetNthOutput(0, output);
	}

	//!
	virtual void SetInput( const InputImageType *input)
	{
		// Process object is not const-correct so the const_cast is required here
		this->ProcessObject::SetNthInput(0, 
			const_cast< InputImageType * >( input ) );
	}

	//!
	ImageCollectionType* GetOutput()
	{
		this->Update();
		return dynamic_cast<ImageCollectionType *>(this->ProcessObject::GetOutput(0));
	}

	//!
	void GenerateData()
	{
		InputImageType* input = dynamic_cast<InputImageType *>(this->ProcessObject::GetInput(0));
		ImageCollectionType *output = dynamic_cast<ImageCollectionType *>(this->ProcessObject::GetOutput(0));
		if(input && output)
		{
			InputImageType::PixelContainer* inputPixelContainer = input->GetPixelContainer();
			output->images.clear();
			InputImageType::SizeType size = input->GetRequestedRegion().GetSize();
			int volumeSize = size[0] * size[1] * size[2];
			int nrTimePoints = size[3];

			for( int iTimePoint = 0; iTimePoint < nrTimePoints; ++iTimePoint )
			{
				OutputImageType::Pointer outputImage = OutputImageType::New();
				OutputImageType::SizeType size3D = {size[0], size[1], size[2]};
				OutputImageType::IndexType start = {0, 0, 0};
				OutputImageType::RegionType region;
				region.SetSize(size3D);
				region.SetIndex(start);
				outputImage->SetRegions(region);

				OutputImageType::PixelContainer::Pointer container = OutputImageType::PixelContainer::New();
				int pos = iTimePoint * volumeSize;
				container->SetImportPointer( &((*inputPixelContainer)[pos]), volumeSize );
				outputImage->SetPixelContainer(container);
				output->images.push_back(outputImage);
			}
		}
	}
};

#endif //BL4DIMAGETOVECTOROF3DIMAGEFILTER_H
