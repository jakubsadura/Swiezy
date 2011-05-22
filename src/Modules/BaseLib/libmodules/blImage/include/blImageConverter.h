/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blimageconverter_h
#define __blimageconverter_h


/** -----------------------------------------------------------------------
 *		Includes														   
 * ------------------------------------------------------------------------
 */

#  ifdef WIN32
#    include <windows.h>
#    include <wingdi.h>
#  endif /* WIN32 */

#include <itkImage.h>	// For blImage.
#include <iostream>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkRGBPixel.h>

// BMP structures
/**
 * \ingroup blImage 
 * Bitmap file data structures (these are defined in <wingdi.h> under
 * Windows...)
 *
 * Note that most Windows compilers will pack the following structures, 
 * so when reading them under MacOS or UNIX we need to read individual 
 * fields to avoid differences in alignment...
 */

#  ifndef WIN32

typedef struct                       /**** BMP file header structure ****/
    {
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER;

#  define BF_TYPE 0x4D42             /* "MB" */

typedef struct                       /**** BMP file info structure ****/
    {
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER;

/*
 * Constants for the biCompression field...
 */

#  define BI_RGB       0             /* No compression - straight BGR data */
#  define BI_RLE8      1             /* 8-bit run-length compression */
#  define BI_RLE4      2             /* 4-bit run-length compression */
#  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

typedef struct                       /**** Colormap entry structure ****/
    {
    unsigned char  rgbBlue;          /* Blue value */
    unsigned char  rgbGreen;         /* Green value */
    unsigned char  rgbRed;           /* Red value */
    unsigned char  rgbReserved;      /* Reserved */
    } RGBQUAD;

typedef struct                       /**** Bitmap information structure ****/
    {
    BITMAPINFOHEADER bmiHeader;      /* Image header */
    RGBQUAD          bmiColors[256]; /* Image colormap */
    } BITMAPINFO;
#  endif /* !WIN32 */


// ___________________________________________________
// ---------------------------------------------------
/**
 * \def ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
 * \brief If defined, prints debug messages
 */
#define ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR

/**
 * \def ENABLE_BL_IMAGE_CONVERTERS_EXAMPLES
 * \brief If defined enables examples for image converters.
 */
#define ENABLE_BL_IMAGE_CONVERTERS_EXAMPLES
// ___________________________________________________
// ---------------------------------------------------

// Image types and Image pointers

typedef itk::Image<unsigned char, 2> ImageType;
typedef itk::Image<unsigned char, 2>::Pointer ImagePointerType;
typedef std::list<ImagePointerType> ImagePointerListType;
typedef itk::ImageFileWriter< ImageType > WriterType;
typedef itk::ImageFileReader< ImageType > ReaderType;


typedef unsigned char ComponentType;
typedef itk::RGBPixel<ComponentType> RGBPixelType;
typedef itk::Image<RGBPixelType, 2> RGBImageType;
typedef RGBImageType::Pointer RGBImagePointerType;
typedef itk::ImageFileReader<RGBImageType> RGBImageReaderType;


/** -----------------------------------------------------------------------
 * \brief This class can be used to convert standard image formats (BMP, PNG, JPG, JP2) 
 * to itk::Image format.
 * \ingroup blImage 
 *
 * This class was generated with the help of code written by Michael Sweet
 * Reference: http://astronomy.swin.edu.au/~pbourke/dataformats/bmp/
 * ------------------------------------------------------------------------
 */
// ___________________________________________________
// ---------------------------------------------------
class blImageConverter
// ___________________________________________________
// ---------------------------------------------------
{

public:


	/**	
	* \brief converts a BMP image to itk::Image
	*
	*	we'll try to take advantage from the itkImportImageFilter
	*
	*	\param setRegion (if true sets the returned image regions to these from the biBMPImage)	
	*	\param setRegion (if false, region is largest possible)
	*  \todo Check for setRegion condition.
	*  \return True, if process successful, else False.
	*/
	static bool
	BMPImageToBlImage(const char* filename, 
					  ImagePointerType& image,
				      bool setRegion);

	/**	
	 * \brief converts a JPG/PNG image to itk::Image (ONLY DCT based JPEG)
	 *
	 *	\param setRegion (if true sets the returned image regions to 
	 * these from the biBMPImage)	
	 *	\param setRegion (if false, region is largest possible)
	 *  \todo Check for setRegion condition.
	 *  \return True, if process successful, else False.
	 */
	static bool
	JPG_PNGImageToBlImage(const char* filename, 
						  ImagePointerType &image,
						  bool setRegion);




private:

	/**
	 *\brief This function reads a BMP image.
	 *\param filename The BMP file to be read
	 *\param numberOfBytes The buffer size. Size of data.
	 *\param info BITMAPINFOHEADER
	 *\param turnTo8bpp A boolean, if set TRUE, converts a color image to B&W.
	 */
	static
	unsigned char* readBMP(const char* filename, 
						   BITMAPINFOHEADER &info, 
						   int &numberOfBytes, 
						   bool turnTo8bpp);

	

	/** 
	 *\brief FLIPPING MEMORY BUFFERS SINCE THEY HAVE THE 0,0 IN THE UP,RIGHT
	 *
	 * Filps a frame upside down 
	 *	from fsukno
	 *
	 *	LineSize must be in bytes
	 */
	static
	void FrameFlip (const unsigned char *InputFrame, 
				    unsigned char *OutputFrame, 
				    const unsigned long LineSize, 
				    const unsigned long nLines);



};

#ifdef ENABLE_BL_IMAGE_CONVERTERS_EXAMPLES
	namespace blClassesTests
	{
		void testImageConverters();
	}
#endif // ENABLE_UZ_RSIMAGE_CONVERTERS_EXAMPLES

#endif // __blimageconverter_h


