/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blV3DImageFileReader.h"


//--------------------------------------------------
blV3DImageFileReader::blV3DImageFileReader()
//--------------------------------------------------
{
	// allocation and initialization of vectors representing Origin, Spacing and Dimensions
	this->v3Origin.resize(3);
	this->SetOrigin(0.0f, 0.0f, 0.0f);
	this->v3Spacing.resize(3);
	this->SetSpacing(0.0f, 0.0f, 0.0f);
	this->v3Dimensions.resize(3);
	this->SetDimensions(0.0f, 0.0f, 0.0f);
	// initialization of scale factor
	this->SetScale(1.0f);
	// manual selection of origin, spacing, dimensions and scaling are disabled
	this->SetOriginOff();
	this->SetSpacingOff();
	this->SetDimensionsOff();
	this->SetScaleDataOff();
	// initialization of fileName
	this->SetFileName("");
	// initialization of image data object to NULL
	this->SetVTKDataObject(NULL);
	// default value: no byte swapping is necessary
	this->SetSwapOff();
	// default value: unsigned short data type
	this->SetType(BLV3Dushort);
	// default value: no unit conversion is necessary
	this->SetUnitConversion(BLV3DnoConv);
	// default value: no verbose
	this->SetVerboseOff();
	// default value: initialization to 0 of the header size
	this->SetHeaderSize(0);
	// initialize the image object to NULL
	this->SetVTKDataObject(NULL);
	// initialize the image object to NULL
	this->SetITKDataObject(NULL);
}

//--------------------------------------------------
blV3DImageFileReader::~blV3DImageFileReader() 
//--------------------------------------------------
{
	// restoring the default values to the object before destroying
	this->SetOrigin(0.0f, 0.0f, 0.0f);
	this->SetSpacing(0.0f, 0.0f, 0.0f);
	this->SetDimensions(0.0f, 0.0f, 0.0f);
	this->SetOriginOff();
	this->SetSpacingOff();
	this->SetDimensionsOff();
	this->SetScaleDataOff();
	this->SetFileName("");
	this->SetVTKDataObject(NULL);
	this->SetSwapOff();
	this->SetType(BLV3Dushort);
	this->SetUnitConversion(BLV3DnoConv);
	this->SetScale(1.0f);
	this->SetVerboseOff();
	this->SetHeaderSize(0);
	if (this->vtkImageObj != NULL) this->vtkImageObj->Delete();
	this->SetVTKDataObject(NULL);
}

//--------------------------------------------------
void blV3DImageFileReader::Update()
//--------------------------------------------------
{
	// read the data from the v3d file and update the member variables with the data describing its contents
	this->ReadDataFromV3D();
	// using the data read from the v3d file create a VTK object with the same characteristics and contents
	this->CreateVTKDataObject();
	this->CreateITKDataObject();
}
//--------------------------------------------------
void blV3DImageFileReader::ReadDataFromV3D()
//--------------------------------------------------
{
	this->SetHeaderSize(40 + 6*8 + 6*4);
	// open the header file
	FILE * fp;
	fp = fopen(this->fileName.c_str(), "r");
	
	// read version
	char buf[128];
	ReadBytes( buf, sizeof(char), 10, fp);

	buf[11] = '\0';
	rewind(fp);
	//int ver = ( buf[7] == '4' ) ? 4 : 3;
	//int ver = atof( (const char*)buf[7] );	
	std::cout << buf << std::endl;
	int ver = 4;
	if (buf[7]=='3')
		ver = 3;
	else if (buf[7]=='4')
		ver = 4;
	else if (buf[7]=='6')
		ver = 6;

	// The rewind function positions the stream at the beginning of the file. It is
	// equivalent to calling fseek or fseeko on the stream with an offset argument of 0L and
	// a whence argument of SEEK_SET, except that the return value is discarded and the error
	// indicator for the stream is reset.

	// The fseek function is used to change the file position of the stream. The value of whence
	// must be one of the constants SEEK_SET, SEEK_CUR, or SEEK_END, to indicate whether the
	// offset is relative to the beginning of the file, the current file position, or the end 
	// of the file, respectively.

	// Reads v3d versions 3 or 4
	//if (ver == 3)
	//	ReadV3Dversion3(fp);
	//else 
	//	ReadV3Dversion4(fp);

	switch ( ver ) 
	{
		case 3 : 
			// Process for ver = 3
			ReadV3Dversion3(fp);
			break;
		case 4 : 
			// Process for test = 4
			ReadV3Dversion4(fp);
			break;
		case 6 : 
			// Process for test = 4
			ReadV3Dversion6(fp);
			break;
		default : 
			// Process for all other cases.
			ReadV3Dversion4(fp);
	}
	
	fclose(fp);
}

//--------------------------------------------------
void blV3DImageFileReader::ReadV3Dversion3(FILE * fp)
//--------------------------------------------------
{
	// read header
	int nx, ny, nz;		//number of voxel in X, Y, and Z
	double x0, y0, z0; //origin
	double x1, y1, z1; 
	double dx, dy, dz; //spacing (voxels size)

	//Sets the position indicator associated with the stream at the beginning of file.
	fseek(fp, 84, SEEK_SET);

	ReadBytes( (void*)&x0, sizeof(double), 1, fp );
	ReadBytes( (void*)&y0, sizeof(double), 1, fp );
	ReadBytes( (void*)&z0, sizeof(double), 1, fp );
	ReadBytes( (void*)&x1, sizeof(double), 1, fp );
	ReadBytes( (void*)&y1, sizeof(double), 1, fp );
	ReadBytes( (void*)&z1, sizeof(double), 1, fp );
	ReadBytes( (void*)&nx, sizeof(int), 1, fp );		//Number Of Voxels in X direction
	ReadBytes( (void*)&ny, sizeof(int), 1, fp );		//Number Of Voxels in Y direction
	ReadBytes( (void*)&nz, sizeof(int), 1, fp );		//Number Of Voxels in Z direction
	dx = (x1-x0)/((double)nx);				//Spacing in X
	dy = (y1-y0)/((double)ny);				//Spacing in Y
	dz = (z1-z0)/((double)nz);				//Spacing in Z
	x0 = 0.5*(x1 + x0);
	y0 = 0.5*(y1 + y0);
	z0 = 0.5*(z1 + z0);

	this->SetHeaderSize(84 + 6 * 8 + 3 * 4);

	if (!this->bDimensions) this->SetDimensions(nx, ny, nz);
	if (!this->bSpacing) this->SetSpacing(dx, dy, dz);
	if (!this->bOrigin) this->SetOrigin(x0, y0, z0);
	
	if (this->blV3DUnitConvType == BLV3Dmm2cm) this->SetSpacing(dx/10.0f, dy/10.0f, dz/10.0f);
}

//--------------------------------------------------
void blV3DImageFileReader::ReadV3Dversion4(FILE * fp)
//--------------------------------------------------
{
	// read header
	int nx, ny, nz;		//number of voxel in X, Y, and Z
	int nd, np, nf;
	double x0, y0, z0; //origin
	double dx, dy, dz; //spacing (voxels size)

	fseek(fp, 40, SEEK_SET);
	ReadBytes( (void*)&nx, sizeof(int), 1, fp );
	ReadBytes( (void*)&ny, sizeof(int), 1, fp );
	ReadBytes( (void*)&nz, sizeof(int), 1, fp );

	ReadBytes( (void*)&dx, sizeof(double), 1, fp );
	ReadBytes( (void*)&dy, sizeof(double), 1, fp );
	ReadBytes( (void*)&dz, sizeof(double), 1, fp );
	ReadBytes( (void*)&x0, sizeof(double), 1, fp );
	ReadBytes( (void*)&y0, sizeof(double), 1, fp );
	ReadBytes( (void*)&z0, sizeof(double), 1, fp );

	ReadBytes( (void*)&nd, sizeof(int), 1, fp );
	ReadBytes( (void*)&np, sizeof(int), 1, fp );
	ReadBytes( (void*)&nf, sizeof(int), 1, fp );
	this->SetHeaderSize(40 + 3 * 4 + 6 * 8 + 3 * 4);

	if (!this->bDimensions) this->SetDimensions(nx, ny, nz);
	if (!this->bSpacing) this->SetSpacing(dx, dy, dz);
	if (!this->bOrigin) this->SetOrigin(x0, y0, z0);
	
	if (this->blV3DUnitConvType == BLV3Dmm2cm) this->SetSpacing(dx/10.0f, dy/10.0f, dz/10.0f);

}

//--------------------------------------------------
void blV3DImageFileReader::ReadV3Dversion6(FILE * fp)
//--------------------------------------------------
{
	// read header
	int nx, ny, nz;		//number of voxel in X, Y, and Z
	int nd, np, nf;
	double x0, y0, z0; //origin
	double dx, dy, dz; //spacing (voxels size)

	fseek(fp, 40, SEEK_SET);
	ReadBytes( (void*)&nx, sizeof(int), 1, fp );
	ReadBytes( (void*)&ny, sizeof(int), 1, fp );
	ReadBytes( (void*)&nz, sizeof(int), 1, fp );

	ReadBytes( (void*)&dx, sizeof(double), 1, fp );
	ReadBytes( (void*)&dy, sizeof(double), 1, fp );
	ReadBytes( (void*)&dz, sizeof(double), 1, fp );
	ReadBytes( (void*)&x0, sizeof(double), 1, fp );
	ReadBytes( (void*)&y0, sizeof(double), 1, fp );
	ReadBytes( (void*)&z0, sizeof(double), 1, fp );

	ReadBytes( (void*)&nd, sizeof(int), 1, fp );
	ReadBytes( (void*)&np, sizeof(int), 1, fp );
	ReadBytes( (void*)&nf, sizeof(int), 1, fp );
	this->SetHeaderSize(40 + 3 * 4 + 6 * 8 + 3 * 4 + 92);	//in total 204 
															//not sure if the headers are like this!
	if (!this->bDimensions) this->SetDimensions(nx, ny, nz);
	if (!this->bSpacing) this->SetSpacing(dx, dy, dz);
	if (!this->bOrigin) this->SetOrigin(x0, y0, z0);
	
	if (this->blV3DUnitConvType == BLV3Dmm2cm) this->SetSpacing(dx/10.0f, dy/10.0f, dz/10.0f);
}

//--------------------------------------------------
void blV3DImageFileReader::ReadBytes(void *ptr, int size, int num, FILE * fp)
//--------------------------------------------------
{
	fread( (void*)ptr, size, num, fp);
	if(this->bSwapBytes == 1)
		SwapBytes( (void*)ptr, size, num);
}

//--------------------------------------------------
void blV3DImageFileReader::SwapBytes(void *ptr, int size, int num)
//--------------------------------------------------
{
	//For swaping header bytes 
	register int i, ii;
	char c;
	char *buf = (char*)ptr;

#define ZSWAP(A,B) {c = A; A = B; B = c;}

	switch( size )
	{
		case 1: break;
		case 2:
			for(i = 0; i < num; i++)
			{
				ii = i + i;
				ZSWAP( buf[ii+0], buf[ii+1] );
			}
			break;
		case 4:
			for(i = 0; i < num; i++)
			{
				ii = i + i + i + i;
				ZSWAP( buf[ii+0], buf[ii+3] );
				ZSWAP( buf[ii+1], buf[ii+2] );
			}
			break;
		case 8:
			for(i = 0; i < num; i++)
			{
				ii = i + i + i + i + i + i + i + i;
				ZSWAP( buf[ii+0], buf[ii+7] );
				ZSWAP( buf[ii+1], buf[ii+6] );
				ZSWAP( buf[ii+2], buf[ii+5] );
				ZSWAP( buf[ii+3], buf[ii+4] );
			}
			break;
	}
#undef ZSWAP
}


//--------------------------------------------------
void blV3DImageFileReader::CreateVTKDataObject()
//--------------------------------------------------
{
	vtkImageReader *reader = vtkImageReader::New();
	
	// allocate memory for vtkImageData
	this->vtkImageObj = vtkImageData::New();
	
	reader->SetFileName(this->GetFileName().c_str());
	reader->SetHeaderSize(this->GetHeaderSize());
	reader->SetDataScalarType(this->blV3DType);
	reader->SetSwapBytes(this->bSwapBytes);
	reader->SetFileDimensionality(3);
	reader->SetDataOrigin(this->v3Origin[0], this->v3Origin[1], this->v3Origin[2]);
	reader->SetDataSpacing(this->v3Spacing[0], this->v3Spacing[1], this->v3Spacing[2]);
	reader->SetDataExtent(0, this->v3Dimensions[0]-1, 0, this->v3Dimensions[1]-1, 0, this->v3Dimensions[2]-1);
	reader->SetNumberOfScalarComponents(1);
 	reader->SetScalarArrayName("ImageFile");
	reader->FileLowerLeftOn();
	reader->Update();

	if (this->bVerbose)
 	{
 		cout<< "v3d image data has:\n\t" 
 			<< reader->GetOutput()->GetNumberOfPoints()  << " Points\n\t"
 			<< reader->GetOutput()->GetNumberOfCells ()  << " Cells	 " 
			<< endl;

		cerr << "Original image has " 
			<< "\nrange : "	<<"\t" << reader->GetOutput()->GetScalarRange()[0]
							<<"\t" << reader->GetOutput()->GetScalarRange()[1]
			<<"\nData Object Type : " << reader->GetOutput()->GetDataObjectType()
			<< endl;
 	}

	if ( !this->bScaleData) 
	{
		//this->SetVTKDataObject(reader->GetOutput());
		vtkImageData* tmpImageData; //= vtkImageData::New();
		tmpImageData = reader->GetOutput();
		this->vtkImageObj->DeepCopy(tmpImageData);
	} 
	else {
		float rangeMin = (double)reader->GetOutput()->GetScalarRange()[0];
		float rangeMax = (double)reader->GetOutput()->GetScalarRange()[1];
		float scale = 1.0/(rangeMax - rangeMin);

		// load voxels
		// loading voxels into buffer
		this->vtkImageObj->SetSpacing(this->v3Spacing[0], this->v3Spacing[1], this->v3Spacing[2]);
		this->vtkImageObj->SetOrigin(this->v3Origin[0], this->v3Origin[1], this->v3Origin[2]);
		this->vtkImageObj->SetDimensions(this->v3Dimensions[0],this->v3Dimensions[1],this->v3Dimensions[2]);
		this->vtkImageObj->SetExtent(0, this->v3Dimensions[0]-1, 0, this->v3Dimensions[1]-1, 0, this->v3Dimensions[2]-1);
		this->vtkImageObj->SetScalarTypeToFloat();
		this->vtkImageObj->AllocateScalars();
		this->vtkImageObj->Update();

		vtkDataArray * scalars = reader->GetOutput()->GetPointData()->GetScalars();
		float * scalars1 = (float *)this->vtkImageObj->GetScalarPointer();

		unsigned int nVoxels = reader->GetOutput()->GetNumberOfPoints();
		register unsigned int ptId;
		for( ptId = 0; ptId < nVoxels; ptId++ )
		{
			scalars1[ptId] = scale * ( (float)scalars->GetComponent(ptId,0) - rangeMin);
		}
	}

	if(this->bVerbose)
 	{
 		cout<< "v3d vtkImageObj data has:\n\t" 
 			<< "Origin = ["		<< reader->GetOutput()->GetOrigin()[0]	<< " " 
								<< reader->GetOutput()->GetOrigin()[1]	<< " " 
								<< reader->GetOutput()->GetOrigin()[2]	<< " ]\n\t"
 			<< "Spacing = ["	<< reader->GetOutput()->GetSpacing()[0] << " " 
								<< reader->GetOutput()->GetSpacing()[1] << " " 
								<< reader->GetOutput()->GetSpacing()[2] << " ]\n\t"
 			<< "Dimension = ["	<< reader->GetOutput()->GetDimensions()[0] << " "
								<< reader->GetOutput()->GetDimensions()[1] << " "
								<< reader->GetOutput()->GetDimensions()[2] << " ]"
			<< endl;


	}
	reader->Delete();
}

//--------------------------------------------------
void blV3DImageFileReader::CreateITKDataObject()
//--------------------------------------------------
{
	typedef itk::VTKImageToImageFilter < base::UnsignedShortVolumeType > ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput((vtkImageData*)this->vtkImageObj);
	connector->Update();
	this->itkVolumeObj = connector->GetOutput();
}



#ifdef BASELIB_TESTING

namespace blTests
{

	int blV3DImageFileReaderTest( string fileName )
	{
		cout << fileName << endl;
		mvLib::blV3DImageFileReader::Pointer pV3DImageFileReader = mvLib::blV3DImageFileReader::New();
		pV3DImageFileReader->SetVerboseOn();
		pV3DImageFileReader->SetFileName(fileName);
		try
		{
			pV3DImageFileReader->Update();
		}
		catch ( itk::ExceptionObject &err)
		{
			std::cout << "blV3DImageFileReaderTest::ExceptionObject caught !" << std::endl;
			std::cout << err << std::endl;
			return -1;
		}
		return 0;
	}

} // blTests namespace

#endif // BASELIB_TESTING
