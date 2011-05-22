// Predefined paths
#include "CISTIBToolkit.h"

#include "cxxtest/TestSuite.h"

#include "coreVTKImageDataHolder.h"
#include "coreVTKPolyDataHolder.h"
#include "coreDataEntityReader.h"
#include "coreDataEntityIORegistration.h"
#include "coreDataEntityBuildersRegistration.h"

// CLASS DEFINITION

/**
*\brief Tests for DataEntityReader
*\ingroup gmIO
*\author Jakub Lyko
*\date 19-10-09
*/

class coreDataEntityReaderTest : public CxxTest::TestSuite 
{
public:
	void TestNIfTI2dData()
	{
		try
		{		
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/2D/IM816_MRI.nii";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/2D/IM848_MRI.nii";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/2D/IM880_MRI.nii";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);

			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();

			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			const double delta = 0.01; // delta for double comparison
			const int dimmension = 2;
			const int numberOfTimesteps = 3;
			const int dimX = 256;
			const int dimY = 256;
			const double spacingX = 1.8;
			const double spacingY = spacingX;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);

			//check processing data
			Core::vtkImageDataPtr image;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(image, i);
				TS_ASSERT( image.GetPointer() )
				TS_ASSERT_EQUALS( dimmension, image->GetDataDimension() );

				int dim[3]; //has to be 3 otherwise there is variable corruption
				image->GetDimensions(dim);
				TS_ASSERT_EQUALS( dim[0], dimX );
				TS_ASSERT_EQUALS( dim[1], dimY );

				double spacing[3]; //has to be 3 otherwise there is varibale corruption
				image->GetSpacing(spacing);
				TS_ASSERT_DELTA( spacing[0], spacingX, delta );
				TS_ASSERT_DELTA( spacing[1], spacingY, delta );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadNIfTI2dData failed: exception was thrown");
		}
		TS_TRACE("TestReadNIfTI2dData is finished");
	}

	void TestNIfTI3dData()
	{
		try
		{		
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/3D/timepoint1_MRI.nii";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/3D/timepoint3_MRI.nii";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/3D/timepoint5_MRI.nii";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);
	
			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();
		
			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			// delta for double comparison
			const int numberOfTimesteps = 3;
			const int dimmension = 3;
			const double delta = 0.01;
			const int dimX = 256;
			const int dimY = 256;
			const int dimZ = 16;
			const double spacingX = 1.8;
			const double spacingY = spacingX;
			const double spacingZ = 10;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);
			
			//check processing data
			Core::vtkImageDataPtr image;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(image, i);
				TS_ASSERT( image.GetPointer() )
				TS_ASSERT_EQUALS( dimmension, image->GetDataDimension() );

				int dim[3];
				image->GetDimensions(dim);
				TS_ASSERT_EQUALS( dim[0], dimX );
				TS_ASSERT_EQUALS( dim[1], dimY );
				TS_ASSERT_EQUALS( dim[2], dimZ );

				double spacing[3];
				image->GetSpacing(spacing);
				TS_ASSERT_DELTA( spacing[0], spacingX, delta );
				TS_ASSERT_DELTA( spacing[1], spacingY, delta );
				TS_ASSERT_DELTA( spacing[2], spacingZ, delta );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadNIfTIData failed: exception was thrown");
		}
		TS_TRACE("TestReadNIfTI3dData is finished");
	}
	
	void TestAnalyze2dData()
	{	
		try
		{		
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/2D/IM816_MRI.hdr";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/2D/IM848_MRI.hdr";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/NIfTI/2D/IM880_MRI.hdr";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);

			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();

			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			const double delta = 0.01; // delta for double comparison
			const int dimmension = 2;
			const int numberOfTimesteps = 3;
			const int dimX = 256;
			const int dimY = 256;
			const double spacingX = 1.8;
			const double spacingY = spacingX;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);

			//check processing data
			Core::vtkImageDataPtr image;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(image, i);
				TS_ASSERT( image.GetPointer() )
				TS_ASSERT_EQUALS( dimmension, image->GetDataDimension() );

				int dim[3];
				image->GetDimensions(dim);
				TS_ASSERT_EQUALS( dim[0], dimX );
				TS_ASSERT_EQUALS( dim[1], dimY );

				double spacing[3];
				image->GetSpacing(spacing);
				TS_ASSERT_DELTA( spacing[0], spacingX, delta );
				TS_ASSERT_DELTA( spacing[1], spacingY, delta );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadAnalzye2dData failed: exception was thrown");
		}
		TS_TRACE("TestReadAnalyze2dData is finished");
	}
  
	void TestAnalyze3dData()
	{
		try
		{		
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/Analyze/3D/IMAFUNE216.hdr";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/Analyze/3D/IMAFUNE217.hdr";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/Analyze/3D/IMAFUNE218.hdr";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);

			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();

			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			const double delta = 0.01; // delta for double comparison
			const int dimmension = 3;
			const int numberOfTimesteps = 3;
			const int dimX = 64;
			const int dimY = 64;
			const int dimZ = 64;
			const double spacingX = 4.42;
			const double spacingY = 4.42;
			const double spacingZ = 4.42;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);

			//check processing data
			Core::vtkImageDataPtr image;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(image, i);
				TS_ASSERT( image.GetPointer() )
					TS_ASSERT_EQUALS( dimmension, image->GetDataDimension() );

				int dim[3];
				image->GetDimensions(dim);
				TS_ASSERT_EQUALS( dim[0], dimX );
				TS_ASSERT_EQUALS( dim[1], dimY );
				TS_ASSERT_EQUALS( dim[2], dimZ );

				double spacing[3];
				image->GetSpacing(spacing);
				TS_ASSERT_DELTA( spacing[0], spacingX, delta );
				TS_ASSERT_DELTA( spacing[1], spacingY, delta );
				TS_ASSERT_DELTA( spacing[2], spacingZ, delta );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadAnalzye3dData failed: exception was thrown");
		}
		TS_TRACE("TestReadAnalyze3dData is finished");
	}
  
	void TestVtkImage2dData()
	{
		try
		{		
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkImage/2D/IM816_MRI.vtk";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkImage/2D/IM848_MRI.vtk";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkImage/2D/IM880_MRI.vtk";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);

			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();

			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			const double delta = 0.01; // delta for double comparison
			const int dimmension = 2;
			const int numberOfTimesteps = 3;
			const int dimX = 256;
			const int dimY = 256;
			const double spacingX = 1.8;
			const double spacingY = spacingX;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);

			//check processing data
			Core::vtkImageDataPtr image;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(image, i);
				TS_ASSERT( image.GetPointer() )
					TS_ASSERT_EQUALS( dimmension, image->GetDataDimension() );

				int dim[3]; //has to be 3 otherwise there is variable corruption
				image->GetDimensions(dim);
				TS_ASSERT_EQUALS( dim[0], dimX );
				TS_ASSERT_EQUALS( dim[1], dimY );

				double spacing[3]; //has to be 3 otherwise there is varibale corruption
				image->GetSpacing(spacing);
				TS_ASSERT_DELTA( spacing[0], spacingX, delta );
				TS_ASSERT_DELTA( spacing[1], spacingY, delta );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadVtkImage2dData failed: exception was thrown");
		}
		TS_TRACE("TestReadVtkImage2dData is finished");
	}
  
	void TestVtkImage3dData()
	{
		try
		{		
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkImage/3D/timepoint1_US.vtk";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkImage/3D/timepoint2_US.vtk";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkImage/3D/timepoint3_US.vtk";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);

			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();

			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			const double delta = 0.01; // delta for double comparison
			const int dimmension = 3;
			const int numberOfTimesteps = 3;
			const int dimX = 148;
			const int dimY = 204;
			const int dimZ = 148;
			const double spacingX = 0.92;
			const double spacingY = 0.64;
			const double spacingZ = 0.92;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);

			//check processing data
			Core::vtkImageDataPtr image;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(image, i);
				TS_ASSERT( image.GetPointer() )
				TS_ASSERT_EQUALS( dimmension, image->GetDataDimension() );

				int dim[3];
				image->GetDimensions(dim);
				TS_ASSERT_EQUALS( dim[0], dimX );
				TS_ASSERT_EQUALS( dim[1], dimY );
				TS_ASSERT_EQUALS( dim[2], dimZ );

				double spacing[3];
				image->GetSpacing(spacing);
				TS_ASSERT_DELTA( spacing[0], spacingX, delta );
				TS_ASSERT_DELTA( spacing[1], spacingY, delta );
				TS_ASSERT_DELTA( spacing[2], spacingZ, delta );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadVtkImage3dData failed: exception was thrown");
		}
		TS_TRACE("TestReadVtkImage3dData is finished");
	}
  
	void TestVtkPolyData()
	{
		try
		{
			std::string filePath1 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkPolyData/smlTestCardiacAnalysis.T04..InputImage_3D_US_00.Out.vtk";
			std::string filePath2 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkPolyData/smlTestCardiacAnalysis.T05..InputImage_3D_US_00.Out.vtk";
			std::string filePath3 = std::string(CISTIB_TOOLKIT_FOLDER) + "/Data/Tests/DataFormats/VtkPolyData/smlTestCardiacAnalysis.T06..InputImage_3D_US_00.Out.vtk";

			std::vector<std::string> fileList;
			fileList.push_back(filePath1);
			fileList.push_back(filePath2);
			fileList.push_back(filePath3);

			// register required data entity readers and builders
			Core::DataEntityBuildersRegistration::RegisterDataEntityBuilders();
			Core::IO::DataEntityIORegistration::RegisterDefaultFormats();

			Core::IO::DataEntityReader::Pointer der = Core::IO::DataEntityReader::New();
			der->SetFileNames(fileList);
			der->Update();

			Core::DataEntity::Pointer dataEntity = der->GetOutputDataEntity();
			TS_ASSERT(dataEntity.IsNotNull());

			// Check content
			const int numberOfTimesteps = 3;
			const int numberOfCells = 5350;
			const int numberOfPoints = 2677;

			//check number of timesteps
			TS_ASSERT_EQUALS(dataEntity->GetNumberOfTimeSteps(), numberOfTimesteps);

			//check processing data
			Core::vtkPolyDataPtr polydata;
			for (int i=0; i<dataEntity->GetNumberOfTimeSteps(); i++)
			{
				dataEntity->GetProcessingData(polydata, i);
				TS_ASSERT( polydata.GetPointer() )

				TS_ASSERT_EQUALS( polydata->GetNumberOfCells(), numberOfCells );
				TS_ASSERT_EQUALS( polydata->GetNumberOfPoints(), numberOfPoints );
			}

		}
		catch(...)
		{
			TS_FAIL("TestReadVtkPolyData failed: exception was thrown");
		}
		TS_TRACE("TestReadVtkPolyData is finished");
	}
};
