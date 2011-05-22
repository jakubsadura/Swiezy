/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blImageWriter_h
#define _blImageWriter_h


#include <itkImageFileWriter.h>

#include "blLightObject.h"
#include "blSmartPointer.h"
#include "blMacro.h"

/**
 * \ingroup blImage 
 *
 */
class AccessorType
{
public :

	typedef unsigned char ExternalType;

	typedef double InternalType;

	inline void Set(InternalType & output, const ExternalType & input) const
		{output = (InternalType) input;}

	inline ExternalType Get( const InternalType & input ) const
	{
		if (input < 0) return (ExternalType) 0;
		if (input > 255) return (ExternalType) 255;
		return (ExternalType) (input + 0.5);
	}

};



/** \brief Writer for blImages
 * \ingroup blImage 
 *
 *  It is derived from itk::ImageFileWriter<TInputImage>

 *
 */
template <class TInputImage>
class blImageWriter: public itk::ImageFileWriter<TInputImage>
{
	public:
		
		typedef blImageWriter Self;
		typedef blSmartPointer<Self>  Pointer;
		typedef blSmartPointer<const Self>  ConstPointer;

		/** \brief Static constructor */
		blNewMacro(Self);


	protected:
		blImageWriter();
		virtual ~blImageWriter();

	private:
		blImageWriter(const Self&); 
			//purposedly not implemented (see itk::Image)
		void operator= (const Self&); 
			//purposedly not implemented (see itk::Image)
};


/**
Helper function that writes _image to _filename.
\author Maarten Nieber
\date 15 Nov 2007
*/

template< class ImageType_ >
void blWriteImageToFile(const ImageType_* _image, std::string& _filename)
{
	typedef itk::ImageFileWriter< ImageType_ > WriterType;
	typename WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( _filename.c_str() );
	writer->SetInput( _image );
	writer->Update();
}

#include "blImageWriter.txx"

#endif
