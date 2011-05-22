/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include <blImageConverter.h>
#include <fstream>
#include <stdio.h>


/**	
 * \brief converts a JPG/PNG image to itk::Image
 *  \param image itk::Image  type object
 *	\param setRegion (if true sets the returned image regions to 
 *   these from the biBMPImage)	
 *	\param setRegion (if false, region is largest possible)
 *  \todo Check for setRegion condition.
 */
bool
blImageConverter::JPG_PNGImageToBlImage(const char* filename, 
										  ImagePointerType &image,
										  bool setRegion)
{

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(filename);
	try
	{
		reader->Update();
	}
	catch( itk::ExceptionObject & excp )
	{
		std::cerr << "error " << excp << std::endl;
		return false;
	}

	image = reader->GetOutput();
	return true;

}



/**	
 * \brief converts a BMP image to itk::Image
 *  \param image blImage  type object
 *	\param setRegion (if true sets the returned image regions to these from the biBMPImage)	
 *	\param setRegion (if false, region is largest possible)
 *  \todo Check for setRegion condition.
 */
bool
blImageConverter::BMPImageToBlImage(const char* filename, 
    				      ImagePointerType& image,
				      bool setRegion)
{


	//-------------------------------------------------------
	// This filter requires the size of the image to be produced as output. 
	// SetRegion() method is used to this end.
	ImageType::RegionType region;
	ImageType::SizeType size;
	ImageType::IndexType start;
	//--------------------------------------------------------



	// Now reading a BMP file...
	//--------------------------------------------------------
	unsigned char* bits;
	int numberOfBytes;
	BITMAPINFOHEADER info;

	bits = readBMP(filename, 
			info, 
			numberOfBytes, 
			true);
	//--------------------------------------------------------

	// Reading BMP parameters
	BITMAPINFOHEADER* bmih = (BITMAPINFOHEADER*)bits;

	int width = bmih->biWidth;
	int height = bmih->biHeight;
	int bpp = bmih->biBitCount;


	// Checking bpp, it has to be 8, since readBMP was
	// configured to convert color to B/W images
	if (bpp != 8)
	{
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->BMPImageToUzImage: bpp = " 
					  << bpp << " and != 8" << std::endl;
		#endif
		return false;
	}

	// acceso a los datos de la imagen.
	unsigned char* imageData = bits + sizeof(BITMAPINFOHEADER);

	// tamao de la imagen: UNUSED VARIABLE
	//unsigned long imageSize = numberOfBytes - sizeof(BITMAPINFOHEADER);


	// setRegion not implemented yet.
	//---------------------------------------	
	if (!setRegion)
	{
		size[0] = info.biWidth; // size along X
		size[1] = info.biHeight; // size along Y

		start[0] = 0;
		start[1] = 0;
	}
	else	// setting region to these from the struct
	{
		size[0] = info.biWidth;; // size along X
		size[1] = info.biHeight; // size along Y

		start[0] = 0;
		start[1] = 0;	
	}

	// Set region information
	region.SetIndex( start );
	// Set Size information
	region.SetSize( size );

	// Set this data to ImageType::Pointer
	image->SetRegions( region );


	// The origin of the output image is specified with the SetOrigin() method.
	double origin[ 2 ];
	origin[0] = 0; // X coordinate
	origin[1] = 0; // Y coordinate
	image->SetOrigin( origin );


	double spacing[ 2 ];
	spacing[0] = 1; // along X direction
	spacing[1] = 1; // along Y direction

	image->SetSpacing( spacing );
	

	// 20040504 - Adolfo
	unsigned char * invertedImageData = 
		new unsigned char[width * height * sizeof(unsigned char)];


	FrameFlip ( imageData,							// input frame
					  invertedImageData,					// output frame
					  (width * sizeof(unsigned char)),		// line size
					  height);								// number of lines


	memcpy(imageData, invertedImageData, (width*height*sizeof(unsigned char)) );

	
	delete bits;


	// copying data from the buffer of the bmp into the ImageType
	//---------------------------------------

	ImageType::PixelContainer::Pointer containerPointer = 
												image->GetPixelContainer();

	

	containerPointer->Reserve(size[0] * size[1] * sizeof(unsigned char));
	// If any error, it has to be here.
	containerPointer->SetImportPointer (invertedImageData, 
										size[0] * size[1], 
										true );

	

	image->SetPixelContainer( containerPointer );

	return true;

}


/**
 *\brief This function reads a BMP image.
 *\param filename The BMP file to be read
 *\param numberOfBytes The buffer size. Size of data.
 *\param info BITMAPINFOHEADER
 *\param turnTo8bpp A boolean, if set TRUE, converts a color image to B/W.
 */

unsigned char* blImageConverter::readBMP(const char* filename, 
					  BITMAPINFOHEADER &info, 
					  int &numberOfBytes, 
					  bool turnTo8bpp) 

{
	FILE*			fp;				// Open file pointer 
    BITMAPFILEHEADER header;		// File header 
	


    // Try opening the file; use "rb" mode to read this *binary* file.
	//---------------------------------------------
	if ((fp = fopen(filename, "rb")) == NULL)
    {
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->readBMP: "
					  << "error on reading" << std::endl;
		#endif
		return 0;
	}



    // Read the file header and any following bitmap information... 
	//---------------------------------------------
    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
    {
        // Couldn't read the file header - return NULL...
		fclose(fp);
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->readBMP: "
					  << "error on reading" << std::endl;
		#endif
		return 0;
    }


	#ifndef BF_TYPE 
	#define BF_TYPE 0x4D42             /* "MB" */
	#endif


    if (header.bfType != BF_TYPE)	/* Check for BM reversed... */
    {
        /* Not a bitmap file - return NULL... */
        fclose(fp);
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->readBMP: "
					  << "error on reading" << std::endl;
		#endif
		return 0;
    }
    
	
    if (fread(&info, 1, sizeof(info), fp) < sizeof(info))
    {
        // Couldn't read the bitmap header - return NULL...
        fclose(fp);
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->readBMP: "
					  << "error on reading" << std::endl;
		#endif
		return 0;
    }



    // Now that we have all the header info read in, allocate memory for the bitmap
	//--------------------------------------------
    if (info.biSizeImage == 0)
    {
		info.biSizeImage = info.biWidth * info.biHeight * (info.biBitCount >> 3);
	}



	
	// filling the structure
	//----------------------------------------------

	// we have header size into infosize and datasize into bitsize
	// we must reopen the file, read the file header and the read the bitsize+infosize following bits
	// into the buffer of the biBMPImage struct
	numberOfBytes = info.biSize + info.biSizeImage;
	unsigned char *bits = new unsigned char[numberOfBytes];

	// setting the reading position on the start of the info header
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	
    if (fread(bits, 1, sizeof(BITMAPINFOHEADER), fp) < sizeof(BITMAPINFOHEADER))
    {
        // Couldn't read bitmap - free memory and return NULL!
        free(bits);
        fclose(fp);
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->readBMP: "
					  << "error on reading" << std::endl;
		#endif
		return 0;
    }

	// we set the reader after the end of the palette
	//------------------------------------------------
	fseek(fp, 
			header.bfOffBits,
			SEEK_SET);

    if (fread(bits + sizeof(BITMAPINFOHEADER), 1, info.biSizeImage, fp) < info.biSizeImage)
    {
        // Couldn't read bitmap - free memory and return NULL!
        free(bits);
        fclose(fp);
		#ifdef ENABLE_DEBUG_MESSAGES_BL_IMAGE_CONVERTOR
			std::cout << "blImageConverter->readBMP: "
					  << "error on reading" << std::endl;
		#endif
		return 0;
    }

	/* OK, everything went fine - return the allocated bitmap... */
	fclose(fp);

	// Change the image to gray-scale if requested
	if (turnTo8bpp)
	{
		if (info.biBitCount == 24)
		{
			unsigned char *bits1;
			BITMAPINFOHEADER info1 = info;
			int ii, iiaux, jjaux;

			info.biSizeImage = info1.biHeight * info1.biWidth;
			bits1 = new unsigned char[info.biSizeImage + sizeof(BITMAPINFOHEADER)];
			info.biBitCount = 8;
			memcpy(bits1, &info, sizeof(BITMAPINFOHEADER));
			iiaux = sizeof(BITMAPINFOHEADER);
			jjaux = iiaux;

		//	std::ofstream ROut("R_Channel.txt");
		//	std::ofstream GOut("G_Channel.txt");
		//	std::ofstream BOut("B_Channel.txt");
		//	std::ofstream GreyOut("Gray_Channel.txt");

			for (ii = 0; ii < info.biSizeImage ; ii++)
			{

			//	BOut << (unsigned int)bits[jjaux + 0] << "\t";
			//	GOut << (unsigned int)bits[jjaux + 1] << "\t";
			//	ROut << (unsigned int)bits[jjaux + 2] << "\t";

				// The bytes are read in this order B, G and R, then intensity values are 
				// extracted from RGB components.

				double GrayValue = ( 0.0721 * (double)bits[jjaux + 0] + 
									 0.7154 * (double)bits[jjaux + 1] +  
									 0.2125 * (double)bits[jjaux + 2]);

				jjaux += 3;

			//	GreyOut << GrayValue << "\t";

				bits1[iiaux++] = (unsigned char)(GrayValue);


				/* old implementation - for reference sake
				bits1[iiaux++] = (unsigned char)(( 721 * (double)bits[jjaux++] +
					7154 * (double)bits[jjaux++] +  2125 * (double)bits[jjaux++]) / 10000);
				*/

				

			}

		//	ROut.close();
		//	GOut.close();
		//	BOut.close();
		//	GreyOut.close();
			delete bits;
			
			// since each three bytes have been turned into 1 (except the header)
			// so final numberOfBytes is header plus total - header / 3 
			numberOfBytes = ( (numberOfBytes - sizeof(BITMAPINFOHEADER) ) / 3) + sizeof(BITMAPINFOHEADER); 

			return bits1;
		}
	}
	


	return bits;
}


/** FLIPPING MEMORY BUFFERS SINCE THEY HAVE THE 0,0 IN THE UP,RIGHT
 *
 * Filps a frame upside down 
 *	from fsukno
 *
 *	LineSize must be in bytes
 */
//-----------------------------------------------------
void blImageConverter::FrameFlip (const unsigned char *InputFrame, 
				unsigned char *OutputFrame, 
				const unsigned long LineSize, 
				const unsigned long nLines) 
//-----------------------------------------------------
{ 
	const /*register*/ unsigned char *iFrame = InputFrame; 
	/*register*/ unsigned char *oFrame = OutputFrame; 

	iFrame = InputFrame + (nLines - 1) * LineSize; 

	for (unsigned long i1 = 0; i1 < nLines; i1++) 
	{ 
		memcpy (oFrame, iFrame, LineSize); 
		oFrame += LineSize; 
		iFrame -= LineSize; 
	} 
} 







#ifdef ENABLE_BL_IMAGE_CONVERTERS_EXAMPLES
	namespace blClassesTests
	{
		void testImageConverters()
		{
			// In this example we will read three types of images, and write the pixel data onto a 
			// text file.

			ImagePointerType BmpImagePointer = ImageType::New();
			ImagePointerType PngImagePointer = ImageType::New();
//			ImagePointerType JpgImagePointer = ImageType::New();
			ImagePointerType Jpg_Memory_ImagePointer = ImageType::New();
 
			blImageConverter::BMPImageToBlImage("C:/src/AHRED/BmpImages/m-001-1.bmp", BmpImagePointer, true);
			blImageConverter::JPG_PNGImageToBlImage("C:/src/AHRED/PngImages/m-001-1.png", PngImagePointer, true);
//			blImageConverter::JPG_PNGImageToBlImage("C:/m-001-1.jpg", JpgImagePointer, true);


			// Binary JPEG data will be read from memory, and stored in a buffer, 
			// to simulate the ITA application.
			const char * filename = "C:/m-001-1.jpg";

			std::ifstream file (filename, 
		     					std::ios::in | std::ios::binary | std::ios::ate);
			if (!file.is_open())
			{
				std::cout << "return yeah" << std::endl;
				return ;
			}

			// Getting the number of bytes in the file.
			//int size = file.tellg(); //UNSUDED VARIABLE

			// Getting back to the beginning of the file.
			file.seekg(0);

			// Checking if the filer pointer is at the beginning..
			if (int(file.tellg()) != 0)
			{
				std::cerr << "The file pointer is not at the beginning" 
					  << std::endl;
				return;
			}
/*
			// Reading the file to a buffer of size specified.
			char* readBuffer = new char [size];
			if (readBuffer == NULL)
			{
				std::cerr << "The buffer was not created." << std::endl;
				return;
			}

			file.read ( readBuffer, size);
			

			blImageConverter::JPG_ImageInMemoryToBlImage ((unsigned char*)readBuffer,
															size,
															Jpg_Memory_ImagePointer);

			delete [] readBuffer;
			*/

			// Writing BMP Image.
			ImageType::RegionType regionBMP = BmpImagePointer->GetBufferedRegion ();
			ImageType::SizeType sizeBMP = regionBMP.GetSize ();
	
			int sizeXBMP = sizeBMP [0];
			int sizeYBMP = sizeBMP [1];

			std::ofstream outBMP("bmpFile.txt");

			ImageType::IndexType index;

			for (int iter1 = 0; iter1 < sizeXBMP; iter1++)
			{
				for (int iter2 = 0; iter2 < sizeYBMP; iter2++)
				{
					index[0] = iter1;
					index[1] = iter2;
					outBMP << (unsigned int)BmpImagePointer->GetPixel(index) << "\t";
				}
				outBMP << std::endl;
			}

			outBMP.close();

			std::cout << "outBMP written" << std::endl;


			// Writing PNG Image.
			ImageType::RegionType regionPNG = PngImagePointer->GetBufferedRegion ();
			ImageType::SizeType sizePNG = regionPNG.GetSize ();
	
			int sizeXPNG = sizePNG [0];
			int sizeYPNG = sizePNG [1];

			std::ofstream outPNG("pngFile.txt");

			for (int iter1 = 0; iter1 < sizeXPNG; iter1++)
			{
				for (int iter2 = 0; iter2 < sizeYPNG; iter2++)
				{
					index[0] = iter1;
					index[1] = iter2;
					double h = (unsigned int)PngImagePointer->GetPixel(index);
					outPNG << h << "\t";
				}
				outPNG << std::endl;
			}

			outPNG.close();

			std::cout << "outPNG written" << std::endl;

			// Writing JPG Image.
			ImageType::RegionType regionJPG = Jpg_Memory_ImagePointer->GetBufferedRegion ();
			ImageType::SizeType sizeJPG = regionJPG.GetSize ();
	
			int sizeXJPG = sizeJPG[0];
			int sizeYJPG = sizeJPG[1];

			std::ofstream outJPG("jpgFile.txt");

			for (int iter1 = 0; iter1 < sizeXJPG; iter1++)
			{
				for (int iter2 = 0; iter2 < sizeYJPG; iter2++)
				{
					index[0] = iter1;
					index[1] = iter2;
					outJPG << (unsigned int)Jpg_Memory_ImagePointer->GetPixel(index) << "\t";
				}
				outJPG << std::endl;
			}

			outJPG.close();

			std::cout << "outJPG written" << std::endl;
				
		}
	}
#endif // ENABLE_BL_IMAGE_CONVERTERS_EXAMPLES

