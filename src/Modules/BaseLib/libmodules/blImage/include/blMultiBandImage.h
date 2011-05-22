/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blMultiBandImage_h
#define _blMultiBandImage_h


#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

#include <vector>


/**
 * \brief Multi-Band image base class
 * \ingroup blImage 
 */
template <class TImage, unsigned int NBands>
class blMultiBandImage: public blLightObject 
{
	public:
		
		typedef blMultiBandImage Self;
		typedef blSmartPointer<Self>  Pointer;
		typedef blSmartPointer<const Self>  ConstPointer;

		typedef TImage ImageType;
		typedef typename ImageType::SizeType SizeType;
		typedef typename ImageType::IndexType IndexType;
		typedef typename ImageType::RegionType RegionType;
		typedef typename ImageType::PixelType PixelType;

		static const int NumberOfBands = NBands;

		/** \brief Static constructor */
		blNewMacro(Self);

		void SetBand( unsigned index, ImageType* image, bool sampling = true, bool automatic = false )
		{
			_bands[index] = image;
			_sampling_bands[index] = sampling;
			_auto_bands[index] = automatic;
		}

		
		//to simplify referencing
		typename ImageType::Pointer operator[]( unsigned index )
		{
			return GetBand( index );
		}

		typename ImageType::Pointer GetBand( unsigned index )
		{
			return _bands[index];
		}

		bool CanSample( unsigned index ) const
		{
			return _sampling_bands[index];
		}

		bool IsAutoBand( unsigned index )
		{
			return _auto_bands[index];
		}
		
		unsigned GetNumberOfBands() const
		{
			return _bands.size();
		}

		unsigned GetNumberOfSampledBands() const
		{
			unsigned count = 0;
			for( int i=0; i<GetNumberOfBands(); i++ )
			{
				if( CanSample(i) ) count++;
			}

			return count;
		}

		virtual void LoadImageFromFilename( const char* filename ) = 0;

		virtual void SaveImage( const char* filename ) = 0;

		void SetResolution( unsigned resolution ) //default is 1
		{
			_resolution = resolution;
		}
		
		unsigned GetResolution() const
		{
			return _resolution;
		}


		virtual void RecomputeBands()
		{
		}

		void SetRegions( RegionType region );

	protected:
		blMultiBandImage();
		virtual ~blMultiBandImage();

	private:
		blMultiBandImage(const Self&); //purposedly not implemented (see itk::Image)
		void operator= (const Self&); //purposedly not implemented (see itk::Image)

		std::vector<typename ImageType::Pointer> _bands;
		std::vector<bool> _sampling_bands; //true for bands from which information will be sampled by AAM
		std::vector<bool> _auto_bands; //true for bands that will be generated automatically (derivatives for example)

		unsigned _resolution; //resolution for loading


};


#include "blMultiBandImage.txx"

#endif
