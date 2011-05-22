/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blShapeUtils.h"
#include "vtkPointData.h"
#include "vtkGlyph3D.h"
#include "vtkGlyphSource2D.h"
#include "vtkTubeFilter.h"
#include "vtkFeatureEdges.h"
#include "vtkDelaunay2D.h"
#include "vtkTriangleFilter.h"
#include "vtkAppendPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkThreshold.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkMassProperties.h"
#include "vtkCellLocator.h"
#include "vtkMath.h"
#include "vtkFloatArray.h"
#include "vtkPointLocator.h"
#include "vtkCellArray.h"
#include "vtkCardinalSpline.h"
#include "vtkSTLWriter.h"
#include "vtkSTLReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataNormals.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkIVWriter.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkLandmarkTransform.h"
#include "vtkLoopSubdivisionFilter.h"
#include "vtkCellArray.h"
#include "vtkIdList.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkMaskPoints.h"
#include "vtkSmartPointer.h"
#include "vtkCellData.h"
#include "vtkPLYReader.h"
#include "vtkPLYWriter.h"
#include <blVTKHelperTools.h>
#include <blVector.h>
#include <blMatrix.h>
#include <blMath.h>
#include <vnl/vnl_vector.h>
#include "itksys/SystemTools.hxx"

#include <vnl/vnl_math.h> // for _isnan

using namespace blShapeUtils;

///**
// *	This method creates a new vtkImageData from a given itk image 
// *	with pixel type Unsigned Short.
// */
////-----------------------------------------------------------------
//vtkImageData * ShapeUtils::itk2vtk(itkImage * itkImagePt)
////-----------------------------------------------------------------
//{
//	if (!itkImagePt) return 0;	// return if null pointer
//	
//	// region
//	Short2ImageType::RegionType region = 
//							itkImagePt->GetLargestPossibleRegion();
//
//	// define origin, spacing and dimensions for VTK image
//	double origin[3];
//	origin[0] = itkImagePt->GetOrigin()[0];
//	origin[1] = itkImagePt->GetOrigin()[1];
//	origin[2] = itkImagePt->GetOrigin()[2];
//
//	double spacing[3];
//	spacing[0] = itkImagePt->GetSpacing()[0];
//	spacing[1] = itkImagePt->GetSpacing()[1];
//	spacing[2] = itkImagePt->GetSpacing()[2];
//
//	unsigned int dim[3];
//	dim[0] = region.GetSize()[0];
//	dim[1] = region.GetSize()[1];
//	dim[2] = region.GetSize()[2];
//	
//	// new vtk image	
//	//--------------------------
//	vtkImageData * vtkImage = vtkImageData::New();	
//	
/////	vtkImage->SetScalarTypeToUnsignedShort();
//	vtkImage->SetScalarTypeToShort();
//	vtkImage->SetNumberOfScalarComponents(1);
//	vtkImage->SetDimensions(dim[0], dim[1], dim[2]);	
//	vtkImage->SetOrigin(origin[0], origin[1], origin[2]);
//	vtkImage->SetSpacing(spacing[0], spacing[1], spacing[2]);
//
//	// get buffer from itk image
//	Short2ImageType::PixelContainer * pixelContainer = 
//											itkImagePt->GetPixelContainer();
//
//	// this does NOT copy the buffer, just share it
/////	vtkUnsignedShortArray * dataBuffer = vtkUnsignedShortArray::New();
//	vtkShortArray * dataBuffer = vtkShortArray::New();
//	dataBuffer->SetArray(pixelContainer->GetBufferPointer(), 
//										pixelContainer->Size(), 1);
//
//	// set buffer to vtk image
//	vtkPointData * pointData = vtkImage->GetPointData();
//	pointData->SetScalars(dataBuffer);
//
//	return vtkImage;
//}

/*
//------------------------------------------------------------------
void ShapeUtils::GrowOrShrink(vtkPolyData * paramShape, float paramThickness)
//-----------------------------------------------------------------
{
	//vtkPolyDataNormals *normals = vtkPolyDataNormals::New();
	//normals -> SetInput(paramShape);
	//normals -> SetFeatureAngle (30);
	//normals -> ConsistencyOn ();
	//normals -> SplittingOff ();
	//normals -> ComputeCellNormalsOff ();
	//normals -> NonManifoldTraversalOn ();
	//normals -> AutoOrientNormalsOn();
	//normals -> Update();

	//paramShape->DeepCopy(normals ->GetOutput());
	//normals -> Delete();

	unsigned int numberOfPoints = paramShape ->GetNumberOfPoints();
	vtkPoints * shapePoints = paramShape->GetPoints();
	vtkDataArray * shapeNormals = ShapeUtils::GetNormals(paramShape, false);//paramShape->GetPointData()->GetNormals();

	unsigned int pointIndex;
    for (pointIndex = 0; pointIndex<numberOfPoints; pointIndex++)
	{
		double *p1 = shapePoints->GetPoint(pointIndex);
		double *n1 = shapeNormals->GetTuple(pointIndex);

		p1[0] += n1[0] * paramThickness;
		p1[1] += n1[1] * paramThickness;
		p1[2] += n1[2] * paramThickness;

		shapePoints->SetPoint(pointIndex,p1);
   }

	paramShape -> Update();
}
*/

//-----------------------------------------------------------------
void ShapeUtils::ScaleShape(vtkPolyData* paramInputShape, 
							vtkPolyData* paramOutputShape, 
							const double * paramScale)
//-----------------------------------------------------------------
{
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Scale(paramScale);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInput(paramInputShape);
	transformFilter->SetTransform(transform);
	transformFilter->Update();

	paramOutputShape->DeepCopy(transformFilter->GetOutput());
}
//-----------------------------------------------------------------
int ShapeUtils::SubdivideMesh(vtkPolyData * paramInputShape, 
							  vtkPolyData * paramOutputShape, 
							  unsigned int paramTimes)
//-----------------------------------------------------------------
{
	if (!paramInputShape)
	{
		std::cerr << "ShapeUtils::SubdivideMesh ERROR#1" << std::endl;
		return EXIT_FAILURE;
	}

	vtkSmartPointer<vtkLoopSubdivisionFilter> subdivider = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	subdivider->SetInput(paramInputShape);
	subdivider->SetNumberOfSubdivisions(paramTimes);
	subdivider->Update();

	if (!paramOutputShape)
	{
		std::cerr << "ShapeUtils::SubdivideMesh ERROR#2" << std::endl;
		return EXIT_FAILURE;
	}

	paramOutputShape->DeepCopy(subdivider->GetOutput());

	return EXIT_SUCCESS;
}
//-----------------------------------------------------------------
void ShapeUtils::AdjustShapeToLandmarks(vtkPolyData* paramInputShape, vtkPolyData* paramOutputShape, 
										vtkPoints *paramSourcePoints, vtkPoints *paramTargetPoints, 
										int TrType)  // default: TrType=1 
//-----------------------------------------------------------------
{
	vtkSmartPointer<vtkLandmarkTransform> transform = vtkSmartPointer<vtkLandmarkTransform>::New();
	transform -> SetSourceLandmarks (paramSourcePoints);
	transform -> SetTargetLandmarks (paramTargetPoints);
	switch (TrType) {
		case 0: transform ->SetModeToRigidBody(); break;
		case 1: transform -> SetModeToSimilarity(); break;
		case 2: transform -> SetModeToAffine(); break;
		default: transform -> SetModeToSimilarity();
	}
	transform -> Modified();

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInput(paramInputShape);
	transformFilter->SetTransform(transform);
	transformFilter->Update();

	paramOutputShape->DeepCopy(transformFilter->GetOutput());
}


//
////-----------------------------------------------------------------
//void ShapeUtils::SaveVTKImageData(vtkImageData* paramImage, const char * paramFileName, bool paramSaveAsBinary = true)
////-----------------------------------------------------------------
//{
//	vtkStructuredPointsWriter *writer = vtkStructuredPointsWriter::New();
//	writer->SetFileName(paramFileName);
//	writer->SetInput(paramImage);
//	if (paramSaveAsBinary)
//		writer->SetFileTypeToBinary();
//	else
//		writer->SetFileTypeToASCII();
//	writer->Update();
//	writer->Delete();
//}

//-----------------------------------------------------------------
void ShapeUtils::BubbleSort(vtkFloatArray *numbers, 
							int *indexes, 
							unsigned int array_size)
//------------------------------------------------------------------
{
  int i,j;
  double temp;
  unsigned int tempIdx;

  for (i = (array_size - 1); i >= 0; i--)
  {
    for (j = 1; j <= i; j++)
    {
      if (numbers->GetValue(j-1) > numbers->GetValue(j))
      {
        temp = numbers->GetValue(j-1); tempIdx = indexes[j-1];
        numbers->SetValue(j-1,numbers->GetValue(j)); indexes[j-1] = indexes[j];
        numbers->SetValue(j,temp); indexes[j] = tempIdx;
      }
    }
  }
}
//-----------------------------------------------------------------
unsigned int ShapeUtils::GetNumberOfDifferentRegions(vtkPolyData* shapePt, 
													 ShapeUtils::RegionMapType &scalar_map, 
													 const char* property_name)
//-----------------------------------------------------------------
{
	vtkShortArray* scalars = (vtkShortArray*) shapePt->GetPointData()->GetArray(property_name);
	
	if (!scalars) 
	{
		std::cout << " GetNumberOfDifferentRegions: input shape has not got data array " << property_name << std::endl;
		return 0;
	}

	unsigned int npoints = shapePt -> GetNumberOfPoints();

	for (unsigned int i=0; i<npoints; i++) 
	{
		scalar_map[scalars->GetValue(i)]++;
	}

	return scalar_map.size();
}

//-----------------------------------------------------------------
double ShapeUtils::GetMaxPropertyValue(vtkPolyData * shapePt, 
									   const char* property_name)
//-----------------------------------------------------------------
{
	double range[2];
	shapePt -> GetPointData()->SetActiveScalars(property_name);
	shapePt -> GetScalarRange(range);
	return range[1];
}

//-----------------------------------------------------------------
double ShapeUtils::GetMinPropertyValue(vtkPolyData * shapePt, 
									   const char* property_name)
//-----------------------------------------------------------------
{
	double range[2];
	shapePt -> GetPointData()->SetActiveScalars(property_name);
	shapePt -> GetScalarRange(range);
	return range[0];
}
//------------------------------------------------------------------
bool ShapeUtils::CheckSaveFileExtension(const char *shapeFileName)
//------------------------------------------------------------------
{
	bool bRes = false;

	if ( strlen(shapeFileName) < 4 )
	{
		return false;
	}

	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-4,".stl") == 0;
	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-4,".vtk") == 0;
	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-4,".vtp") == 0;
	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-3,".iv") == 0;

	return bRes;
}

//------------------------------------------------------------------
void ShapeUtils::SaveShapeToFile(
						vtkPolyData *shapePt, 
						const char *shapeFileName,
						const char *header )
//------------------------------------------------------------------
{
	std::string ext = itksys::SystemTools::GetFilenameLastExtension(shapeFileName);
	ext = itksys::SystemTools::LowerCase(ext);


	if ( ext == ".ply" )
	{
		vtkSmartPointer<vtkPLYWriter> writer = vtkSmartPointer<vtkPLYWriter>::New();
		writer -> SetFileName (shapeFileName);
		writer -> SetInput (shapePt);
		if ( header != NULL )
		{
			writer->SetHeader( header );
		}
		writer -> Write();
	}
	if ( ext == ".stl" )
	{
		vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
		writer -> SetFileName (shapeFileName);
		writer -> SetInput (shapePt);
		if ( header != NULL )
		{
			writer->SetHeader( header );
		}
		writer -> Write();
	}
	if ( ext == ".vtk" )
	{
		vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
		writer -> SetFileName (shapeFileName);
		writer -> SetInput (shapePt);
		if ( header != NULL )
		{
			writer->SetHeader( header );
		}
		writer -> Write();
	}
	if ( ext == ".vtp" )
	{
		vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
		writer -> SetFileName (shapeFileName);
		writer -> SetInput (shapePt);
		writer -> SetDataModeToAscii();
		writer -> Write();
	}
	if ( ext == ".iv" )
	{
		vtkSmartPointer<vtkIVWriter> writer = vtkSmartPointer<vtkIVWriter>::New();
		writer -> SetFileName (shapeFileName);
		writer -> SetInput (shapePt);
		if ( header != NULL )
		{
			writer->SetHeader( header );
		}
		writer -> Write();
	}
	
}

//------------------------------------------------------------------
void ShapeUtils::SaveVolumeToFile(
								 vtkUnstructuredGrid *volumePt, 
								 const char *volumeFileName,
								 const char *header)
//------------------------------------------------------------------
{
	std::string ext = itksys::SystemTools::GetFilenameLastExtension(volumeFileName);
	ext = itksys::SystemTools::LowerCase(ext);
	if ( ext == ".vtk" )
	{
		vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
		writer -> SetFileTypeToBinary();
		writer -> SetFileName (volumeFileName);
		writer -> SetInput (volumePt);
		if ( header != NULL )
		{
			writer->SetHeader( header );
		}
		writer -> Write();
	}
}
//
//
////------------------------------------------------------------------
//bool ShapeUtils::CheckLoadFileExtension(const char *shapeFileName)
////------------------------------------------------------------------
//{
//	bool bRes = false;
//
//	if ( strlen(shapeFileName) < 4 )
//	{
//		return false;
//	}
//
//	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-4,".stl") == 0;
//	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-4,".vtk") == 0;
//	bRes |= strcmp(shapeFileName+strlen(shapeFileName)-4,".vtp") == 0;
//
//	return bRes;
//}

//-------------------------------------------------------------------------------
ShapeUtils::VTKSurfaceMeshFormats ShapeUtils::GetTypeOfVTKData(const char *shapeFileName)
//-------------------------------------------------------------------------------
{
	ShapeUtils::VTKSurfaceMeshFormats type = ShapeUtils::UnknownType;

	std::string ext = itksys::SystemTools::GetFilenameLastExtension(shapeFileName);
	ext = itksys::SystemTools::LowerCase(ext);

	if ( ext == ".stl" )
	{
		vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName (shapeFileName);
		reader->Update();
		if(reader->GetOutput() != NULL)
			type = ShapeUtils::STLType;
	}
	if ( ext == ".vtk" )
	{
		vtkSmartPointer<vtkDataReader> reader = vtkSmartPointer<vtkDataReader>::New();
		reader->SetFileName (shapeFileName);
		if(reader->IsFilePolyData())
			type = ShapeUtils::VTKPolyDataType;
		//if(reader->IsFileUnstructuredGrid())
		  //  type = ShapeUtils::VTKUnstructuredGridType;
	}
	if ( ext == ".ply" )
	{
		//vtkSmartPointer<vtkDataReader> reader = vtkSmartPointer<vtkDataReader>::New();
		//reader->SetFileName (shapeFileName);
		//if(reader->IsFilePolyData())
			type = ShapeUtils::PLYType;
		//if(reader->IsFileUnstructuredGrid())
		//  type = ShapeUtils::VTKUnstructuredGridType;
	}
	if ( ext == ".vtp" )
	{
		vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
		reader->SetFileName (shapeFileName);
		if(reader->GetOutput())
			type = ShapeUtils::VTKXMLPolyDataType;
	}

	return type;
}

//-------------------------------------------------------------------------------
ShapeUtils::VTKVolumeMeshFormats ShapeUtils::GetTypeOfVTKVolumeData(const char *volumeFileName)
//-------------------------------------------------------------------------------
{
	ShapeUtils::VTKVolumeMeshFormats type = ShapeUtils::UnknownVolumeType;

	std::string ext = itksys::SystemTools::GetFilenameLastExtension(volumeFileName);
	ext = itksys::SystemTools::LowerCase(ext);

	if ( ext == ".vtk" )
	{
		vtkSmartPointer<vtkDataReader> reader = vtkSmartPointer<vtkDataReader>::New();
		reader->SetFileName (volumeFileName);
		if(reader->IsFileUnstructuredGrid())
			type = ShapeUtils::VTKUnstructuredGridType;
	}
	
	return type;
}

//------------------------------------------------------------------
vtkPolyData* ShapeUtils::LoadShapeFromFile(const char *shapeFileName)
//------------------------------------------------------------------
{
	vtkPolyData* shapePt = NULL;
	ShapeUtils::VTKSurfaceMeshFormats	vtkSurfaceFormat;
	vtkSurfaceFormat = GetTypeOfVTKData( shapeFileName );

	switch( vtkSurfaceFormat )
	{
	case ShapeUtils::PLYType:
		{
			vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
			reader -> SetFileName (shapeFileName);
			reader -> Update();
			if (reader->GetOutput())
			{
				shapePt = vtkPolyData::New();
				shapePt -> ShallowCopy(reader->GetOutput());			
			}
		}
		break;
	case ShapeUtils::STLType:
		{
			vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
			reader -> SetFileName (shapeFileName);
			reader -> Update();
			if (reader->GetOutput())
			{
				shapePt = vtkPolyData::New();
				shapePt -> ShallowCopy(reader->GetOutput());			
			}
		}
		break;
	case ShapeUtils::VTKPolyDataType:
		{
			vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
			reader -> SetFileName (shapeFileName);
			reader -> Update();
			if (reader->GetOutput())
			{
				shapePt = vtkPolyData::New();
				shapePt -> ShallowCopy(reader->GetOutput());			
			}
		}
		break;
	case ShapeUtils::VTKXMLPolyDataType:
		{
			vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
			reader -> SetFileName (shapeFileName);
			reader -> Update();
			if (reader->GetOutput())
			{
				shapePt = vtkPolyData::New();
				shapePt -> ShallowCopy(reader->GetOutput());			
			}
		}
	    break;	
	
	default:
	    break;
	}

	return shapePt;
}
//------------------------------------------------------------------
vtkUnstructuredGrid* ShapeUtils::LoadVolumeFromFile(const char *volumeFileName)
//------------------------------------------------------------------
{
	vtkUnstructuredGrid* VolPt = NULL;
	ShapeUtils::VTKVolumeMeshFormats	vtkVolumeFormat;
	vtkVolumeFormat = GetTypeOfVTKVolumeData( volumeFileName );

	switch( vtkVolumeFormat )
	{
		case ShapeUtils::VTKUnstructuredGridType:
		{
			vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
			reader -> SetFileName (volumeFileName);
			reader->ReadAllScalarsOn();
			reader->ReadAllVectorsOn();
			reader -> Update();
			if (reader->GetOutput()->GetNumberOfPoints())
			{
				VolPt = vtkUnstructuredGrid::New();
				VolPt -> ShallowCopy(reader->GetOutput());			
			}
		}
		break;
		default:
	    break;
	}

	return VolPt;
}


/*
//------------------------------------------------------------------
vtkDataArray* ShapeUtils::GetNormals(vtkPolyData* shapePt, bool b_flip)
//------------------------------------------------------------------
{
		vtkPolyData *shapeWithNormalsPd = vtkPolyData::New();
		vtkPolyDataNormals *polydataNormalsFilter = vtkPolyDataNormals::New();
		polydataNormalsFilter->SetInput(shapePt);
		polydataNormalsFilter->SetOutput(shapeWithNormalsPd);
		polydataNormalsFilter->ComputePointNormalsOn();
		polydataNormalsFilter->SplittingOff();
		if (b_flip)	polydataNormalsFilter->FlipNormalsOn();
		polydataNormalsFilter->ConsistencyOn();
		polydataNormalsFilter->Update();
		polydataNormalsFilter->Delete();
		
		vtkFloatArray* normals = vtkFloatArray::New();
		normals->SetNumberOfComponents(3);
		normals->DeepCopy(shapeWithNormalsPd->GetPointData()->GetNormals());

		shapeWithNormalsPd->Delete();

		return normals;
}
*/


//------------------------------------------------------------------
vtkPolyData* ShapeUtils::GetShapeWithNormals(vtkPolyData* paramShapePt)
//------------------------------------------------------------------
{
		if (paramShapePt == NULL)
		{
			std::cout << " ShapeUtils::GetShapeWithNormals input shape is NULL" << std::endl;
			return NULL;
		}

		vtkPolyData* shapeWithNormalsPt = vtkPolyData::New();
		vtkSmartPointer<vtkPolyDataNormals> polydataNormalsFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
		polydataNormalsFilter->SetInput(paramShapePt);
		polydataNormalsFilter->SetOutput(shapeWithNormalsPt);
		polydataNormalsFilter->ComputePointNormalsOn();	
		polydataNormalsFilter->SplittingOff();
		polydataNormalsFilter->ConsistencyOn();
		try 
		{
			polydataNormalsFilter->Update();
		}
		catch( ... ) 
		{ 
			std::cerr << "ShapeUtils::GetShapeWithNormals exception caught!" << std::endl; 
			return NULL;
		}

		return shapeWithNormalsPt;
}

/*
//------------------------------------------------------------------
vtkPolyData* ShapeUtils::GetTubesFromSplines(vtkPolyData *shapePt, float start_rad, float end_rad, int nsides)
//------------------------------------------------------------------
{
		vtkAppendPolyData *spline_appender = vtkAppendPolyData::New();
		vtkAppendPolyData *tube_appender = vtkAppendPolyData::New();

		int counter = 0;

		vtkPolyData *subpart = ShapeUtils::GetShapeSubpart(shapePt, counter);
		//vtkShortArray* subpart_regionID = subpart->GetPointData()->GetArray("regionID");

		while (subpart!=NULL)
		{
			vtkAppendPolyData *appender = vtkAppendPolyData::New();
			int npoints = subpart->GetNumberOfPoints();

			vtkFloatArray *radius = vtkFloatArray::New();
			radius->SetName("radius");

			radius->InsertNextValue(start_rad);

			for (int i = 1; i < npoints; i++) {
				float rad = end_rad + (start_rad - end_rad) * (npoints-i)/npoints;
				radius->InsertNextValue(rad);
			}

			subpart -> GetPointData() -> AddArray(radius);
			subpart -> Update();

			radius->Delete();
			
			vtkPointLocator *locator;

			if (counter) {
				locator = vtkPointLocator::New();
				locator->SetDataSet(spline_appender->GetOutput());
				locator->BuildLocator();
			}

			for (int i = 1; i < npoints; i++) {

				// actions on tube
				vtkPolyData *seg_pd = vtkPolyData::New();
				vtkPoints *seg_pts = vtkPoints::New();
				vtkCellArray *seg_cells = vtkCellArray::New();

				seg_cells -> InsertNextCell(2);

				seg_pts -> InsertNextPoint(subpart->GetPoint(i-1));
				seg_cells -> InsertCellPoint(0);	

				seg_pts -> InsertNextPoint(subpart->GetPoint(i));
				seg_cells -> InsertCellPoint(1);

				//short value = subpart_regionID->GetValue(0);

				seg_pd -> SetPoints(seg_pts);
				seg_pd -> SetLines(seg_cells);
				seg_pd -> Modified();

				if (counter) {
					if (i==1) {
						vtkIdType closest_id;
						vtkFloatArray* spline_appender_scalars = 
							(vtkFloatArray*) spline_appender->GetOutput()->GetPointData()->GetScalars("radius");
						closest_id = locator -> FindClosestPoint(subpart->GetPoint(0));
						//big_appender->GetOutput()->GetPointData()->SetActiveScalars("radius");
						start_rad = spline_appender_scalars->GetValue(closest_id);
						end_rad = 0.1;
					}
				}
				
				float rad = end_rad + (start_rad - end_rad) * (npoints-i)/npoints;
					
				vtkTubeFilter *streamTube = vtkTubeFilter::New();
				streamTube -> SetInput (seg_pd);
				streamTube -> SetRadius (rad);
				
				streamTube -> SetNumberOfSides (nsides);
				streamTube -> Modified();

				appender->AddInput(streamTube->GetOutput());
				appender->Update();

				seg_pd -> Delete();
				seg_pts -> Delete();
				seg_cells -> Delete();
				streamTube -> Delete();
			}

			if (counter)
				locator->Delete();

			spline_appender->AddInput(subpart);
			spline_appender->Update();

			vtkTriangleFilter* triangulator = vtkTriangleFilter::New();
			triangulator -> SetInput(appender->GetOutput());
			triangulator -> PassVertsOn ();
			triangulator -> PassLinesOn ();
			triangulator -> Update();		

			tube_appender->AddInput(triangulator->GetOutput());
			tube_appender->Update();

			appender->Delete();
			triangulator->Delete();
			subpart->Delete();

			counter ++;
			subpart = ShapeUtils::GetShapeSubpart(shapePt, counter);
		}
		
		vtkPolyData* output = vtkPolyData::New();
		output ->DeepCopy(tube_appender->GetOutput());

		tube_appender->Delete();
		spline_appender->Delete();

		if (subpart != NULL)
			subpart -> Delete();

		return output;
}
*/


//------------------------------------------------------------------
vtkPolyData* ShapeUtils::GetShapeRegion(
						vtkPolyData* shapePt, 
						unsigned int start, 
						unsigned int end, 
						const char *name)
//------------------------------------------------------------------------
{
  vtkPolyData* output = NULL;

  if ( shapePt->GetPointData() == NULL )
  {
	  return NULL;
  }

  int res = shapePt->GetPointData()->SetActiveScalars(name);
  if (res < 0 && shapePt->GetCellData() != NULL )
  {
		shapePt->GetCellData()->SetActiveScalars(name);
  }

  // Select the points
  vtkSmartPointer<vtkThreshold> threshold = vtkSmartPointer<vtkThreshold>::New();
  threshold -> SetInput(shapePt);
  threshold -> ThresholdBetween(start,end);
  threshold -> Update();

  vtkSmartPointer<vtkDataSetSurfaceFilter> surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
  surface -> SetInput((vtkDataObject*) threshold->GetOutput());
  surface -> Update();

  //vtkCleanPolyData *cleaner = vtkCleanPolyData::New();
  //cleaner -> SetInput(surface->GetOutput());
  //cleaner -> SetTolerance (0.0);
  //cleaner -> SetAbsoluteTolerance (1.0);
  ////pdCleaner -> ToleranceIsAbsoluteOn ();
  //cleaner -> ConvertStripsToPolysOn ();
  //cleaner -> ConvertPolysToLinesOn ();
  //cleaner -> PointMergingOn ();
  //cleaner -> Update();
  
  if (surface->GetOutput()->GetNumberOfPoints())
  {
	  output = vtkPolyData::New();
	  output->DeepCopy(surface -> GetOutput());
  }

  return output;
}

//------------------------------------------------------------------
vtkPolyData* ShapeUtils::GetVolumeRegion(
										vtkUnstructuredGrid* volumePt, 
										unsigned int start, 
										unsigned int end, 
										const char *name)
//------------------------------------------------------------------------
{
	vtkPolyData* output = NULL;

	volumePt->GetPointData()->SetActiveScalars(name);

	// Select the points
	vtkSmartPointer<vtkThreshold> threshold = vtkSmartPointer<vtkThreshold>::New();
	threshold -> SetInput(volumePt);
	threshold -> ThresholdBetween(start,end);
	threshold -> Update();

	vtkSmartPointer<vtkDataSetSurfaceFilter> surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	surface -> SetInput((vtkDataObject*) threshold->GetOutput());
	surface -> Update();

	//vtkCleanPolyData *cleaner = vtkCleanPolyData::New();
	//cleaner -> SetInput(surface->GetOutput());
	//cleaner -> SetTolerance (0.0);
	//cleaner -> SetAbsoluteTolerance (1.0);
	////pdCleaner -> ToleranceIsAbsoluteOn ();
	//cleaner -> ConvertStripsToPolysOn ();
	//cleaner -> ConvertPolysToLinesOn ();
	//cleaner -> PointMergingOn ();
	//cleaner -> Update();

	if (surface->GetOutput()->GetNumberOfPoints())
	{
		output = vtkPolyData::New();
		//when it will be possible to render the unstuctured grid there will be no need to do that!
		output->DeepCopy(surface -> GetOutput());
	}

	return output;
}
/*
//------------------------------------------------------------------
vtkPolyData* ShapeUtils::GetGlyphGeometry(
										vtkPolyData* shapePt, 
										unsigned int start, 
										unsigned int end, 
										const char *name)
//------------------------------------------------------------------------
{
	return shapePt;
}
*/
/*
//------------------------------------------------------------------
vtkPolyData* ShapeUtils::GetShapeSubpart(vtkPolyData * shapePt, unsigned int subpart)
//------------------------------------------------------------------
{
	vtkPolyDataConnectivityFilter * connector = vtkPolyDataConnectivityFilter::New();
	connector -> SetInput(shapePt);
	connector -> SetScalarRange(shapePt->GetScalarRange());
	connector -> ScalarConnectivityOn();
	connector -> AddSpecifiedRegion(subpart);
	connector -> SetExtractionModeToSpecifiedRegions();
	connector -> Update();

	vtkCleanPolyData * cleaner = vtkCleanPolyData::New();
	cleaner -> SetInput(connector->GetOutput());
	//cleaner -> SetTolerance (0.0);
	//cleaner -> SetAbsoluteTolerance (1.0);
	cleaner -> ConvertStripsToPolysOn();
	cleaner -> ConvertPolysToLinesOn();
	cleaner -> PointMergingOn();
	cleaner -> Update();
	if(cleaner->GetOutput()->GetNumberOfPoints() == 0)
		return NULL;

	vtkPolyData * output = vtkPolyData::New();
	output -> DeepCopy(cleaner->GetOutput());

	connector -> Delete();
	cleaner -> Delete();

	return output;

}
*/



//------------------------------------------------------------------
double ShapeUtils::GetClosedSurfaceVolume(
						vtkPolyData* paramShape, 
						bool return_projected,
						bool convertToMl /* = true*/)
//------------------------------------------------------------------
{

	double volume;

	if (paramShape == NULL)
	{
		throw std::runtime_error( "Input shape is NULL" );
	}

	vtkSmartPointer<vtkPolyDataNormals> norms = vtkSmartPointer<vtkPolyDataNormals>::New();
	norms->SetInput(paramShape);
	norms->ConsistencyOn();
	norms->NonManifoldTraversalOn();
	norms->SplittingOff();
	norms->Update();


	vtkSmartPointer<vtkMassProperties> massFilter = vtkSmartPointer<vtkMassProperties>::New();
	massFilter->SetInput( norms->GetOutput() );
	massFilter->Update( );

	double vol = massFilter->GetVolume();
	double volp = massFilter->GetVolumeProjected();
	int conversionFactor  = 1;
	if ( convertToMl ) conversionFactor = 1000;

	//1 cubic millimeter = 0.001 milliliter
	// Convert to ml
	if ( return_projected )
	{
		volume = fabs(volp) / conversionFactor; 
	}
	else
	{
		volume = vol / conversionFactor; 
	}

	return volume;
}




//------------------------------------------------------------------
// \todo Remove blVector and blMatrix
int ShapeUtils::GetClosedSurfaceVolumeUsingProjection( vtkPolyData* paramShape, 
													  double &paramVolume )
//------------------------------------------------------------------
{
	// Steps:
	// - File opening/reading
	// - Defining required translation in the z-direction
	// - Defining positive and negative contributions for each face
	// - Computing volume underneath each face

	// ------ BEGIN DEFINING REQUIRED TRANSLATION IN THE z-DIRECTION
	// Define zlift as the amount the volume will be raised to ensure its
	// position above the z=0 plane.
	double bds[6];
	paramShape->GetBounds( bds );
	double zlift;
	if( bds[4] <= 0 )
	{
		zlift = abs(bds[4])+1;
	} else {
		zlift = 0;
	}
	// ------ END DEFINING REQUIRED TRANSLATION IN THE z-DIRECTION

	// ------ BEGIN DEFINING POSITIVE AND NEGATIVE CONTRIBUTIONS FOR EACH FACE (expensive!)
	vtkSmartPointer<vtkIdList> pointsi = vtkSmartPointer<vtkIdList>::New();
	vtkSmartPointer<vtkIdList> pointsj = vtkSmartPointer<vtkIdList>::New();
	int cnum = paramShape->GetNumberOfCells();
	blVector<int>::Pointer contributions = blVector<int>::New(cnum,0);
	blMatrix<double>::Pointer blMOrd = blMatrix<double>::New();
	for( int i = 0; i < cnum; ++i ) // for each face i
	{
		if( contributions->get(i) != 0 )
		{
			// If a face was already classified, there is no need to
			// reclassify it.
			continue; // Ugly, but it avoids yet another nesting.
		}

		//if( i % 100 == 0 )
		//	std::cout << i << std::endl;

		paramShape->GetCellPoints(i,pointsi);
		blVector<double>::Pointer blVPtsi[3] = {blVector<double>::New(3), blVector<double>::New(3), blVector<double>::New(3)};
		double pts[3];
		paramShape->GetPoint(pointsi->GetId(0),pts);
		blVPtsi[0]->set(pts);
		paramShape->GetPoint(pointsi->GetId(1),pts);
		blVPtsi[1]->set(pts);
		paramShape->GetPoint(pointsi->GetId(2),pts);
		blVPtsi[2]->set(pts);

		// Compute center point c of face i.
		blVector<double>::Pointer blVCPt = blVector<double>::New(3);
		blVCPt->update( (*blVPtsi[0] + *blVPtsi[1] + *blVPtsi[2]) / 3 );
		double z1 = bllao::Z(*blVCPt);

		blMOrd->set_size(1,3);
		// blMOrd:
		// - first column has the face indexes
		// - second column has the z coordinate
		// - third column tells whether the currently known contribution
		//   is negative or positive
		blMOrd->put(0,0,i);blMOrd->put(0,1,z1);blMOrd->put(0,2,-1);
		int listlen = 1; // How many faces were found; face i was first

		for( int j = 0; j < cnum; ++j ) // for each face j
		{
			if( j == i )
				continue; // Again ugly, but avoids yet another nesting.

			paramShape->GetCellBounds(j,bds);
			// Check whether the xy projection of the center point is
			// within the bounding box of the xy projection of face j.
			if( bllao::X(*blVCPt) >= bds[0] && bllao::X(*blVCPt) <= bds[1] && bllao::Y(*blVCPt) >= bds[2] && bllao::Y(*blVCPt) <= bds[3] )
			{
				// If so, compute the barycentric coordinates of the z=0
				// projection of the center point on the z=0 projection of
				// face j.
				paramShape->GetCellPoints(j,pointsj);
				blVector<double>::Pointer blVPtsj[3] = {blVector<double>::New(3), blVector<double>::New(3), blVector<double>::New(3)};
				double pts[3];
				paramShape->GetPoint(pointsj->GetId(0),pts);
				blVPtsj[0]->set(pts);
				paramShape->GetPoint(pointsj->GetId(1),pts);
				blVPtsj[1]->set(pts);
				paramShape->GetPoint(pointsj->GetId(2),pts);
				blVPtsj[2]->set(pts);
				blVector<double>::Pointer blVEdge1 = blVector<double>::New(3);
				blVector<double>::Pointer blVEdge2 = blVector<double>::New(3);
				blVector<double>::Pointer blVEdge3 = blVector<double>::New(3);

				blVEdge1->update( *blVPtsj[2] - *blVPtsj[0] );
				blVEdge2->update( *blVPtsj[1] - *blVPtsj[0] );
				blVEdge3->update( *blVCPt     - *blVPtsj[0] );

				blVEdge1->put(2,0); blVEdge2->put(2,0); blVEdge3->put(2,0); // Project the face onto z=0

				double d11 = dot_product( *blVEdge1, *blVEdge1 );
				double d12 = dot_product( *blVEdge1, *blVEdge2 );
				double d13 = dot_product( *blVEdge1, *blVEdge3 );
				double d22 = dot_product( *blVEdge2, *blVEdge2 );
				double d23 = dot_product( *blVEdge2, *blVEdge3 );

				double invdenom = 1 / (d11 * d22 - d12 * d12);
				double u = (d22 * d13 - d12 * d23) * invdenom; // Barycentric u
				double v = (d11 * d23 - d12 * d13) * invdenom; // Barycentric v

				// Check whether the projection of the center point lies
				// within the projection of face j.
				if( u >= 0 && v >= 0 && (u+v) <= 1 )
				{
					// Get the original faces back (with the original z
					// coordinates).
					blVEdge1->update( *blVPtsj[2] - *blVPtsj[0] );
					blVEdge2->update( *blVPtsj[1] - *blVPtsj[0] );
					blVEdge3->update( *blVCPt     - *blVPtsj[0] );
					// We can use u and v to find the z coordinate of the
					// intersection on the real face j.
					double zcoord = bllao::Z(*blVPtsj[0]) + u * bllao::Z(*blVEdge1) + v * bllao::Z(*blVEdge2);
					// Update the list of found faces (no need to sort,
					// just change the contribution factor) and add the
					// new face.
					int order = -1;
					for( int k = 0; k < listlen; ++k )
					{
						if( zcoord < blMOrd->get(k,1) )
						{
							double curOr = blMOrd->get(k,2);
							blMOrd->put( k, 2, -curOr );
						} else {
							order *= -1;
						}
					}
					// Resizing the matrix results in a loss of its
					// contents, so we use a temporary storage.
					blMatrix<double>::Pointer tempmat = blMatrix<double>::New( blMOrd->data_block(), blMOrd->rows(), blMOrd->cols() );
					blMOrd->set_size( listlen+1, 3 );
					for( int k = 0; k < listlen; ++k )
						blMOrd->set_row(k,tempmat->get_row(k));
					blMOrd->put( listlen, 0, j );
					blMOrd->put( listlen, 1, zcoord );
					blMOrd->put( listlen, 2, order );
					++listlen;
				}
			}
		}

		// For face i and all the faces above and below its center point
		// we now know the contribution factors. Time to update the big
		// list of contribution factors that we will use later on.
		for( int j = 0; j < listlen; ++j )
		{
			// If the contribution was already set, don't set it again
			// (not sure if this check is cheaper than setting the same
			// value again and again).
			if( contributions->get( blMOrd->get(j,0) ) == 0 )
			{
				contributions->put( blMOrd->get(j,0), blMOrd->get(j,2) );
			}
		}
	}
	// ------ END DEFINING POSITIVE AND NEGATIVE CONTRIBUTIONS FOR EACH FACE (expensive!)


	// ------ BEGIN COMPUTING VOLUME UNDERNEATH EACH FACE
	double totalvolume(0);
	//vtkIdList *points = vtkIdList::New();
	for( int i = 0; i < cnum; ++i ) // for each face i
	{
		// Extract the points for the current face
		paramShape->GetCellPoints(i, pointsi);
		blVector<double>::Pointer blVPts[3] = {blVector<double>::New(3), blVector<double>::New(3), blVector<double>::New(3)};
		double pts[3];
		paramShape->GetPoint(pointsi->GetId(0),pts);
		blVPts[0]->set(pts);
		paramShape->GetPoint(pointsi->GetId(1),pts);
		blVPts[1]->set(pts);
		paramShape->GetPoint(pointsi->GetId(2),pts);
		blVPts[2]->set(pts);

		// Extract 'random' edges from the face at hand
		blVector<double>::Pointer blVEdge1 = blVector<double>::New(3);
		blVector<double>::Pointer blVEdge2 = blVector<double>::New(3);
		blVEdge1->update( *blVPts[0] - *blVPts[1] );
		blVEdge2->update( *blVPts[0] - *blVPts[2] );

		// Compute the area of the projection of the face onto the
		// xy-plane; this is half of the z-component of the cross product
		// of two of the edges
		double columnbasearea = (bllao::X(*blVEdge1) * bllao::Y(*blVEdge2) - bllao::Y(*blVEdge1) * bllao::X(*blVEdge2)) /2;
		columnbasearea = abs(columnbasearea); // Correct for negative values

		// Determine which point has the lowest, middle and highest z-coordinates.
		int minind; int midind; int maxind;
		if( bllao::Z(*blVPts[0]) < bllao::Z(*blVPts[1]) )
		{
			if( bllao::Z(*blVPts[0]) < bllao::Z(*blVPts[2]) )
			{
				minind = 0;
				if( bllao::Z(*blVPts[1]) < bllao::Z(*blVPts[2]) )
				{
					midind = 1; maxind = 2;
				} else {
					midind = 2; maxind = 1;
				}
			} else {
				minind = 2; midind = 0; maxind = 1;
			}
		}
		else if( bllao::Z(*blVPts[1]) < bllao::Z(*blVPts[2]) )
		{
			minind = 1;
			if( bllao::Z(*blVPts[0]) < bllao::Z(*blVPts[2]) )
			{
				midind = 0; maxind = 2;
			} else {
				midind = 2; maxind = 0;
			}
		} else {
			minind = 2; midind = 1; maxind = 0;
		}


		// Edge1 is the projection of the edge between top and middle face
		// vertex; edge2 is the projection of the edge between top and
		// bottom face vertex
		blVEdge1->update( *blVPts[midind] - *blVPts[maxind] );
		blVEdge2->update( *blVPts[maxind] - *blVPts[minind] );
		blVEdge1->put(2,0);
		blVEdge2->put(2,0);

		// The pyramid base is defined by the top and middle vertices of
		// the face and their projections on the plane where z equals the
		// height of the bottom vertex of the face; this is an irregular
		// tetragon with at least two square angles. Thus it can be
		// decomposed into a rectangle and a triangle.
		double pyramidbasearea = sqrt( blVEdge1->two_norm() ) *
			( bllao::Z(*blVPts[midind]) - bllao::Z(*blVPts[minind]) + 
			( bllao::Z(*blVPts[maxind]) - bllao::Z(*blVPts[midind])
			) /2
			);

		// The pyramid height is the point-to-line distance from the
		// bottom face vertex to the projection of the edge between the
		// top and middle face vertices.
		blVector<double>::Pointer blVNorm = blVector<double>::New( 3, 0 );
		blVNorm->put(0,bllao::Y(*blVEdge1) * bllao::Z(*blVEdge2) - bllao::Z(*blVEdge1)*bllao::Y(*blVEdge2));
		blVNorm->put(1,bllao::Z(*blVEdge1)*bllao::X(*blVEdge2)-bllao::X(*blVEdge1)*bllao::Z(*blVEdge2));
		blVNorm->put(2,bllao::X(*blVEdge1)*bllao::Y(*blVEdge2)-bllao::Y(*blVEdge1)*bllao::X(*blVEdge2));
		double pyramidheight = sqrt(blVNorm->two_norm()) / sqrt(blVEdge1->two_norm());

		double pyramidvolume = pyramidbasearea * pyramidheight / 3;
		double columnvolume = columnbasearea * (bllao::Z(*blVPts[minind]) + zlift);
		double facevolume = (columnvolume + pyramidvolume) * contributions->get(i);
		totalvolume = totalvolume + facevolume;
	}
	// ------ END COMPUTING VOLUME UNDERNEATH EACH FACE

	// We will probably need some proper outputting here...
	//	std::cout << totalvolume << std::endl;

	paramVolume = totalvolume;	
	return EXIT_SUCCESS;
}


//------------------------------------------------------------------
void ShapeUtils::GetP2S(vtkPolyData * manualPt, vtkPolyData *segmentedPt, 
						double& mean, double& std_dev, double& max, 
						double& last, bool b_array)
//------------------------------------------------------------------
{
	
	unsigned int n_manual_points = manualPt -> GetNumberOfPoints();

	vtkSmartPointer<vtkFloatArray> error_array;
	if (b_array) 
	{
		error_array = vtkSmartPointer<vtkFloatArray>::New();
		error_array -> SetName("p2s");
		error_array -> SetNumberOfValues(n_manual_points);
		manualPt -> GetPointData()-> AddArray(error_array);
	}

	// fill the CellLocator with the fitted shape
	vtkSmartPointer<vtkCellLocator> cell_locator = vtkSmartPointer<vtkCellLocator>::New();
	cell_locator->SetDataSet(segmentedPt);
	cell_locator->BuildLocator();
	
	vnl_vector<float> stdaux(n_manual_points);
	
	for (size_t i=0; i<n_manual_points; i++) 
	{
		double *p1 = manualPt -> GetPoint(i);
		double p2[] = {0.0, 0.0, 0.0};
		int subId;
		vtkIdType cellId;
		double dist;
		cell_locator->FindClosestPoint(p1, p2, cellId, subId, dist);
		stdaux[i] = sqrt(dist);
		error_array->SetValue(i,stdaux[i]);
	}

	max = stdaux.max_value();
	//min = stdaux.min_value();
	mean = stdaux.mean();
	
	double sum=0;
	double sum2=0;

	for (size_t i = 0; i < n_manual_points; i++)
	{
		sum2=sum2+(stdaux[i]-mean)*(stdaux[i]-mean);
		sum=sum+(stdaux[i]-mean);
	}

	std_dev = sqrt((sum2-sum/n_manual_points)/(n_manual_points-1));

	last = stdaux[n_manual_points-1];
}

//------------------------------------------------------------------
void ShapeUtils::GetP2P(vtkPolyData * manualPt, vtkPolyData *segmentedPt, 
						double& mean, double& std_dev, double& max, 
						double& last, bool b_array)
//------------------------------------------------------------------
{
	unsigned int n_manual_points = manualPt -> GetNumberOfPoints();

	vtkSmartPointer<vtkFloatArray> error_array;

	if (b_array) 
	{
		error_array = vtkSmartPointer<vtkFloatArray>::New();
		error_array -> SetName("p2p");
		error_array -> SetNumberOfValues(n_manual_points);
		manualPt -> GetPointData()-> AddArray(error_array);
	}
	
	vnl_vector<float> stdaux(n_manual_points);
	vnl_vector<double> p1(3);
	vnl_vector<double> p2(3);
	vnl_vector<double> dist(3);
	
	for (size_t i=0; i<n_manual_points; i++) 
	{
		manualPt -> GetPoint(i,p1.data_block());
		segmentedPt -> GetPoint(i,p2.data_block());
		dist = p1 - p2;
		stdaux[i] = dist.magnitude();
		error_array->SetValue(i,stdaux[i]);
	}

	max = stdaux.max_value();
	mean = stdaux.mean();
	
	double sum=0;
	double sum2=0;

	for (size_t i = 0; i < n_manual_points; i++)
	{
		sum2=sum2+(stdaux[i]-mean)*(stdaux[i]-mean);
		sum=sum+(stdaux[i]-mean);
	}

	std_dev = sqrt((sum2-sum/n_manual_points)/(n_manual_points-1));

	last = stdaux[n_manual_points-1];
}


/*
//------------------------------------------------------------------
vtkPlane *ShapeUtils::FitPlaneToPoints(vtkPolyData* shapePt)
//------------------------------------------------------------------
{
	int nPoints = shapePt->GetNumberOfPoints();
	vtkPoints *points = shapePt->GetPoints();
	
	vnl_matrix<double> W(3,3);
	W.fill(0.0);

	vnl_vector<double>center(3);
	shapePt->GetCenter(center.data_block()); 

	for (int i = 0; i<nPoints; ++i)
	{
		double *p = points->GetPoint(i);
		W(0,0) += (p[0]-center[0])*(p[0]-center[0]);	
		W(0,1) += (p[0]-center[0])*(p[1]-center[1]); 
		W(0,2) += (p[0]-center[0])*(p[2]-center[2]);
		W(1,0) += (p[0]-center[0])*(p[1]-center[1]);	
		W(1,1) += (p[1]-center[1])*(p[1]-center[1]); 
		W(1,2) += (p[1]-center[1])*(p[2]-center[2]);
		W(2,0) += (p[0]-center[0])*(p[2]-center[2]);	
		W(2,1) += (p[1]-center[1])*(p[2]-center[2]); 
		W(2,2) += (p[2]-center[2])*(p[2]-center[2]);
	}

	vnl_symmetric_eigensystem<double> eigen( W.as_ref() );
	vnl_vector<double> lambda(3);
	vnl_vector<double> abslambda(3);
	vnl_vector<double> normal(3);
	vnl_vector<double> p1(3);
	vnl_vector<double> p2(3);

	lambda[0] = eigen.get_eigenvalue(0);
	lambda[1] = eigen.get_eigenvalue(1);
	lambda[2] = eigen.get_eigenvalue(2);

	abslambda[0] = fabs(lambda[0]);
	abslambda[1] = fabs(lambda[1]);
	abslambda[2] = fabs(lambda[2]);

	int idx = 0 ; // index of the smallest eigenvalue
	if ((abslambda[1] >= abslambda[0]) && (abslambda[2] >= abslambda[0])) idx = 0 ;
	if ((abslambda[2] >= abslambda[1]) && (abslambda[0] >= abslambda[1])) idx = 1 ;
	if ((abslambda[0] >= abslambda[2]) && (abslambda[1] >= abslambda[2])) idx = 2 ;

	vnl_vector<double> np;
	np = eigen.get_eigenvector(idx);
	np.normalize();

	vtkPlane* plane = vtkPlane::New();
	plane->SetOrigin(center.data_block());
	plane->SetNormal(np.data_block());

	return plane;
}
*/


//------------------------------------------------------------------
VtkPolyDataPtr ShapeUtils::GetTubesFromPointData(VtkPolyDataPtr shapePt, 
											   const char* name, 
											   int max_points, 
											   float scale, 
											   float radius,
											   ShapeUtils::Glyph2DShapes Glyphtype,
											   int sides,
											   const char* scalar)
//------------------------------------------------------------------
{
	VtkPolyDataPtr output;

	// The following line makes all the glyph to point to the same direction, might be because the given name doesn't exist. 
	shapePt->GetPointData()->SetActiveVectors(name); 
	
	if (scalar != NULL)
		shapePt->GetPointData()->SetActiveScalars(scalar);

	vtkSmartPointer<vtkGlyphSource2D> glyphSource = vtkSmartPointer<vtkGlyphSource2D>::New();
	switch(Glyphtype)
	{
	case ShapeUtils::ArrowType :
		glyphSource->SetGlyphTypeToArrow();
		break;
	case ShapeUtils::DashType :
	    glyphSource->SetGlyphTypeToDash();
		break;
	default:
		break;
	}
	glyphSource->FilledOff();

	vtkSmartPointer<vtkMaskPoints> mask = vtkSmartPointer<vtkMaskPoints>::New();
	mask->SetInput(shapePt);
	mask->SetMaximumNumberOfPoints(max_points);
	mask->RandomModeOn();

	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInput(mask->GetOutput()); 
	glyph->SetSource(glyphSource->GetOutput());
	glyph->OrientOn();
	glyph->SetVectorModeToUseVector();   
	glyph->SetColorModeToColorByScalar();
	glyph->SetScaleModeToScaleByVector();
	glyph->SetScaleFactor(10/*scale*/);

	vtkSmartPointer<vtkTubeFilter> fibers = vtkSmartPointer<vtkTubeFilter>::New();
	fibers->SetInput(glyph->GetOutput());
	fibers->SetRadius(radius);
	fibers->SetNumberOfSides(sides);
	fibers->Update();

	output = fibers->GetOutput();

	return output;
}


/**
 */
bool ShapeUtils::CompareVtkCellArray( 
						vtkCellArray*		ptrArray1,
						vtkCellArray*		ptrArray2 )
{

	bool bRes = true;

	bRes &= ptrArray1->GetNumberOfCells( ) == ptrArray2->GetNumberOfCells( );

	vtkIdType		npts1, npts2;
	vtkIdType		*pts1, *pts2;
	ptrArray1->InitTraversal( );
	ptrArray2->InitTraversal( );
	while ( bRes && ptrArray1->GetNextCell( npts1, pts1 ) )
	{
		ptrArray2->GetNextCell( npts2, pts2 );

		if ( npts1 == npts2 )
		{
			for ( int i = 0 ; i < npts1 ; i++ )
			{
				bRes &= pts1[ i ] == pts2[ i ];
			}
		}
		else
		{
			bRes = false;
		}
	}

	return bRes;
}



/**
 */
bool ShapeUtils::CompareShapes( 
						vtkPolyData*		ptrShape1,
						vtkPolyData*		ptrShape2,
						float &fMaxDifference,
						float fEpsilon )
{
	bool bRes = true;

	if ( ptrShape1 == NULL || ptrShape2 == NULL )
	{
		return false;
	}

	bRes &=	ptrShape1->GetNumberOfPoints( ) == 
			ptrShape2->GetNumberOfPoints( );
	if ( !bRes )
	{
		std::cout << "[ShapeUtils::CompareShapes] " 
			<< "GetNumberOfPoints( ): " 
			<< ptrShape1->GetNumberOfPoints( ) << " vs. " 
			<< ptrShape2->GetNumberOfPoints( ) << std::endl;
		return false;
	}
	
	bRes &=	ptrShape1->GetNumberOfLines( ) == 
			ptrShape2->GetNumberOfLines( );
	if ( !bRes )
	{
		std::cout << "[ShapeUtils::CompareShapes] " 
			<< "GetNumberOfLines( ): " 
			<< ptrShape1->GetNumberOfLines( ) << " vs. " 
			<< ptrShape2->GetNumberOfLines( ) << std::endl;
		return false;
	}

	bRes &=	ptrShape1->GetNumberOfPolys( ) == 
			ptrShape2->GetNumberOfPolys( );
	if ( !bRes )
	{
		std::cout << "[ShapeUtils::CompareShapes] " 
			<< "GetNumberOfPolys( ): " 
			<< ptrShape1->GetNumberOfPolys( ) << " vs. " 
			<< ptrShape2->GetNumberOfPolys( ) << std::endl;
		return false;
	}


	// Points
	fMaxDifference = 0;
	int iMaxPointPosition = -1;
	double point1[ 3 ];
	double point2[ 3 ];
	for ( int i = 0 ; i < ptrShape1->GetNumberOfPoints( ) ; i++ )
	{
		ptrShape1->GetPoint( i, point1 );
		ptrShape2->GetPoint( i, point2 );

		for ( int iCoord = 0 ; iCoord < 3 ; iCoord++ )
		{
			bRes = bRes && FLOAT_EQ_INCLUSIVE( point1[ iCoord ], point2[ iCoord ], fEpsilon );
			if ( fMaxDifference < fabs( point1[ iCoord ] - point2[ iCoord ] ) )
			{
				fMaxDifference = fabs( point1[ iCoord ] - point2[ iCoord ] );
				iMaxPointPosition = i;
			}
		}
	}

	if (!bRes)
	{
		std::cout << "[ShapeUtils::CompareShapes] " << std::endl;
		std::cout << "Different points " << iMaxPointPosition;
		std::cout << " difference in: " << fMaxDifference << std::endl;
		std::cout.precision( 10 );
		ptrShape1->GetPoint( iMaxPointPosition, point1 );
		ptrShape2->GetPoint( iMaxPointPosition, point2 );
		std::cout << point1[ 0 ] << ", " << point1[ 1 ] << ", " << point1[ 2 ] << std::endl;
		std::cout << point2[ 0 ] << ", " << point2[ 1 ] << ", " << point2[ 2 ] << std::endl;
	}


	// Lines
	if ( bRes )
	{
		bRes &= CompareVtkCellArray( ptrShape1->GetLines( ), ptrShape2->GetLines( ) );
	}


	// Polygons
	if ( bRes )
	{
		bRes &= CompareVtkCellArray( ptrShape1->GetPolys( ), ptrShape2->GetPolys( ) );
	}


	return bRes;
}



/**
 */
void ShapeUtils::Convert( 
						vtkCellArray*		vtkCells,
						blPDShapeInterface::ShapeCellsContainer &pdCells )
{
	vtkIdType		npts1;
	vtkIdType		*pts1;

	pdCells.resize( vtkCells->GetNumberOfCells( ) );

	long			lCellCounter = 0;
	vtkCells->InitTraversal( );
	while ( vtkCells->GetNextCell( npts1, pts1 ) )
	{
		pdCells[ lCellCounter ].resize( npts1 );
		for ( int i = 0 ; i < npts1 ; i++ )
		{
			pdCells[ lCellCounter ][ i ] = pts1[ i ];
		}

		lCellCounter++;
	}
}



/**
 */
void ShapeUtils::Convert( 
						blPDShapeInterface::ShapeCellsContainer &pdCells,
						vtkCellArray*		vtkCells )
{
	vtkIdType		npts1;
	vtkIdType		*pts1;

//	vtkCells->SetNumberOfCells( pdCells.size( ) );

	for ( size_t iCell = 0 ; iCell < pdCells.size( ) ; iCell++ )
	{
		npts1 = pdCells[ iCell ].size();
		pts1 = new vtkIdType[ npts1 ];
		for ( int i = 0 ; i < npts1 ; i++ )
		{
			pts1[ i ] = pdCells[ iCell ][ i ];
		}
		vtkCells->InsertNextCell( npts1, pts1 );
		delete [] pts1;
	}
}



/** 
 */
bool ShapeUtils::CompareShapes( const char *shapeFileName1, const char *shapeFileName2 )
{
	bool bRes = false;

	// Read shape 1
	vtkPolyData* shape1 = LoadShapeFromFile( shapeFileName1 );
	vtkPolyData* shape2 = LoadShapeFromFile( shapeFileName2 );

	// Perform comparison
	float fMaxDifference;
	bRes = ShapeUtils::CompareShapes( shape1, shape2, fMaxDifference );

	if ( shape1 )
	{
		shape1->Delete();
	}
	if ( shape2 )
	{
		shape2->Delete();
	}

	if ( bRes )
	{
		std::cout << "[ShapeUtils::CompareShapes] Shapes are similar."
			<< " Max difference: " << fMaxDifference << std::endl;
	}

	return bRes;
}



/** 
 */
void ShapeUtils::GetNeighborPoints( 
							  vtkPolyData* polyData, 
							  int iSelectedPoint,
							  vtkIdList	*neighborPoints )
{
	neighborPoints->Reset();

	//--------------------------------------------------
	// This will be the neighbor points ordered by the first we found
	vtkCellArray	*linesArray = polyData->GetLines();
	int iFoundPoint = 0;
	vtkIdType npts;
	vtkIdType* pts;
	bool bRes = true;


	//--------------------------------------------------
	// Add the neighbor to the list
	linesArray->InitTraversal( );
	while ( linesArray->GetNextCell( npts, pts ) )
	{
		// For each point inside the line
		for ( int iPoint = 0 ; iPoint < npts ; iPoint++ )
		{
			// If the selected point is inside this line
			if ( pts[ iPoint ] == iSelectedPoint )
			{
				// Get the left point ID
				if ( iPoint - 1 >= 0 )
				{
					neighborPoints->InsertNextId( pts[ iPoint - 1 ] );
				}

				// Get the right point ID
				if ( iPoint + 1 < npts )
				{
					neighborPoints->InsertNextId( pts[ iPoint + 1 ] );
				}
			}
		}
	}

}



/** 
 */
void ShapeUtils::GetTangentVector( 
					vtkPolyData* polyData, 
					int iPoint1,
					int iPoint2,
					double tangent[ 3 ] )
{
	if ( iPoint1 >= 0 && iPoint1 < polyData->GetNumberOfPoints() &&
		 iPoint2 >= 0 && iPoint2 < polyData->GetNumberOfPoints() )
	{
		double point1[ 3 ];
		double point2[ 3 ];

		polyData->GetPoint( iPoint1, point1 );
		polyData->GetPoint( iPoint2, point2 );

		for(int i=0; i<3; i++)
		{
			double val1 = point1[i];
			double val2 = point2[i];
			tangent[ i ] = val1 - val2;
		}
	}

}

void blShapeUtils::ShapeUtils::GetScalarsVector( 
	vtkDataSet* data, 
	std::vector<VTKScalarType> &scalarnames )
{
	if ( data == NULL )
	{
		return;
	}

	vtkSmartPointer<vtkDataSetAttributes> target = vtkSmartPointer<vtkDataSetAttributes>::New();
	VTKScalarType temp;

	if (data->GetCellData())
	{
		target = data->GetCellData();
		for (int ic = 0; ic < target->GetNumberOfArrays();ic++)
		{
			if ( target->GetArrayName(ic) == NULL ||
				 strcmp( target->GetArrayName(ic), "" ) == 0 )
			{
				// Error en VTK
				int i = 0;
			}
			else 
			{
				temp.name = target->GetArrayName(ic);
				temp.mode = SCALAR_ARRAY_CELL_DATA;
				scalarnames.push_back(temp);
			}
		}
	}
	if (data->GetPointData())
	{
		target = data->GetPointData();
		for (int ip = 0; ip < target->GetNumberOfArrays();ip++)
		{ 
			if ( target->GetArrayName(ip) == NULL ||
				strcmp( target->GetArrayName(ip), "" ) == 0 )
			{
				// Error en VTK
			}
			else 
			{
				temp.name = target->GetArrayName(ip);
				temp.mode = SCALAR_ARRAY_POINT_DATA;
				scalarnames.push_back(temp);
			}
		}
	}
}



vtkPolyData* blShapeUtils::ShapeUtils::GetP2S(vtkPolyData *shapePt1, vtkPolyData *shapePt2, std::vector< vnl_vector<double> >& distances)
{
	const double* scalar_range = shapePt1->GetScalarRange();


	vtkSmartPointer<vtkAppendPolyData> appender = vtkSmartPointer<vtkAppendPolyData>::New();

	int c=0;
	for( int region = floor(scalar_range[0]); region<=floor(scalar_range[1]); region++ )
	{
		vnl_vector<double> dist;

		vtkSmartPointer<vtkPolyData> shape1 = vtkSmartPointer<vtkPolyData>::Take(
			blVTKHelperTools::GetShapeSubSurface(shapePt1,region) );
		vtkSmartPointer<vtkPolyData> shape2 = vtkSmartPointer<vtkPolyData>::Take(
			blVTKHelperTools::GetShapeSubSurface(shapePt2,region) );

		vtkSmartPointer<vtkPolyData> shape = vtkSmartPointer<vtkPolyData>::Take(
			GetP2S( shape1, shape2, dist ) );

		if( shape.GetPointer()!=NULL )
		{
			vnl_vector<double> regionID(1);
			regionID[0] = region;
			appender->AddInput(shape);
			distances.push_back( regionID );
			distances.push_back( dist );
		}
	}

	appender->Update();

	vtkPolyData* result = vtkPolyData::New();
	result->DeepCopy( appender->GetOutput() );

	return result;
}


vtkPolyData* blShapeUtils::ShapeUtils::GetP2S(vtkPolyData *shapePt1, vtkPolyData *shapePt2, vnl_vector<double>& distances)
{
	if( shapePt1->GetNumberOfPoints()==0 || shapePt2->GetNumberOfPoints()==0 )
	{
		return NULL;
	}


	vtkPolyData* shape1 = vtkPolyData::New();
	shape1->DeepCopy( shapePt1 );



	vtkSmartPointer<vtkFloatArray> scalars1 = vtkSmartPointer<vtkFloatArray>::New();
	scalars1->SetName("Displacement");
	scalars1->SetNumberOfValues( shape1->GetNumberOfPoints() );

	vtkPoints *points1 = shape1 -> GetPoints();

	vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	cellLocator->SetDataSet(shapePt2);
	cellLocator->BuildLocator();

	const unsigned int nPoints1 = shape1 -> GetNumberOfPoints();

	vtkSmartPointer<vtkPoints> intersectionPoints2 = vtkSmartPointer<vtkPoints>::New();

	distances.set_size(nPoints1);


	for (unsigned int i=0; i<nPoints1; i++) {
		double *p1 = shape1 -> GetPoint(i);
		double p2[] = {0.0, 0.0, 0.0};
		int subId;
		vtkIdType cellId;
		double dist;
		cellLocator->FindClosestPoint(p1, p2, cellId, subId, dist);
		dist = sqrt(dist);
		distances[i] = dist;
		scalars1 -> SetValue(i,dist);
		intersectionPoints2->InsertNextPoint(p2);
	}


	shape1->GetPointData()->AddArray( scalars1 );


	return shape1;
}


void blShapeUtils::ShapeUtils::GetS2S(vtkPolyData *shapePt1, vtkPolyData *shapePt2, std::vector< vnl_vector<double> >& distances )
{
	std::vector< vnl_vector<double> > errors;
	std::vector< vnl_vector<double> > errors1;
	vtkPolyData* deformation = GetP2S( shapePt1, shapePt2, errors );
	vtkPolyData* deformation1 = GetP2S( shapePt2, shapePt1, errors1 );
	deformation1->Delete();
	deformation->Delete();

	distances.resize(errors.size());

	for( unsigned int i=0; i<errors.size(); i+=2 )
	{
		distances[i] = errors[i];
		distances[i+1] = (errors[i+1] + errors1[i+1])/2.0; //get symmetric S2S error
	}

}

bool blShapeUtils::ShapeUtils::IsSkeleton( vtkPolyData* processingData )
{
	if ( processingData->GetNumberOfVerts() == 0 &&
		processingData->GetNumberOfStrips() == 0 &&
		processingData->GetNumberOfPoints() != 0 &&
		processingData->GetNumberOfLines() != 0  &&
		processingData->GetNumberOfPolys() == 0 
		)
	{
		return true;
	}

	return false;
}


/**
*/
std::vector<float> blShapeUtils::ShapeUtils::GetDataArrayValues(
	const std::vector<vtkPolyData* > &meshes,
	int cellID,
	const std::string &arrayName )
{
	std::vector<float> outputVector;

	if ( meshes.size() == 0 )
	{
		return outputVector;
	}

	outputVector.resize( (int)meshes.size() );

	for (unsigned int ts=0; ts < meshes.size(); ts++)	
	{
		if ( meshes[ts] == NULL || meshes[ts]->GetCellData() == NULL ||
			meshes[ts]->GetNumberOfPoints() == 0)
		{
			return outputVector;
		}

		vtkDataArray* circularArray = meshes[ts]->GetCellData()->GetArray( arrayName.c_str() );
		if ( circularArray == NULL)
		{
			return outputVector;
		}

		outputVector[ts] = circularArray->GetTuple1(cellID);
	}

	return outputVector;
}

unsigned int blShapeUtils::ShapeUtils::ComputeNumberOfHoles(
								vtkPolyData* inputShape)
{
    // throw exception if nan values
    vtkCellArray *inPolys  = inputShape->GetPolys();
    vtkPoints   *inPts = inputShape->GetPoints();
    double x[3];
    int numVerts = inputShape->GetNumberOfPoints();
    for(int i=0; i<numVerts; i++)
    {
        inPts->GetPoint(i, x);
        if(vnl_math_isnan(x[0]) || vnl_math_isnan(x[1]) || vnl_math_isnan(x[2]))
        {
            std::cout << "there are invalid points in the mesh starting from vertex: " << i << std::endl;
            throw std::runtime_error("The mesh contains invalid vertex points. Extract main surface or clean the mesh manually!");
        }
    }

	unsigned int numHoles = 0;

	vtkSmartPointer<vtkCleanPolyData> clean = vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInput(inputShape);
	clean->Update();

	// Extract edges, but only boundary edges
	vtkSmartPointer<vtkFeatureEdges> extractEdges = vtkSmartPointer<vtkFeatureEdges>::New();
	extractEdges->SetInput( clean->GetOutput() );
	extractEdges->BoundaryEdgesOn();
	extractEdges->FeatureEdgesOff();
	extractEdges->ManifoldEdgesOff();
	extractEdges->NonManifoldEdgesOff();

	// Split output, to get boundaries one by one
	vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivity = 
		vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connectivity->SetInput( extractEdges->GetOutput() );
	connectivity->SetExtractionModeToSpecifiedRegions();
	connectivity->Update();

	numHoles = connectivity->GetNumberOfExtractedRegions();

	return numHoles;

}
