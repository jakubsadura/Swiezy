/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

	#include <vtkCleanPolyData.h>
	#include <vtkThreshold.h>
	#include <vtkDataSetSurfaceFilter.h>
	#include <vtkPolyDataWriter.h>
	#include <vtkTriangleFilter.h>
	#include <vtkAppendPolyData.h>
	#include <vtkDelaunay2D.h>
	#include <vtkCellArray.h>
	#include <vtkFloatArray.h>
	#include <vtkPointData.h>
	#include <vtkCellData.h>
	#include <vtkDataSetWriter.h>
	#include <vtkTransform.h>
	#include <vtkTransformPolyDataFilter.h>
	#include <vtkImageGaussianSmooth.h>
	#include <vtkImageShrink3D.h>
	#include <vtkImageData.h>
	#include <vtkShortArray.h>
	#include <vtkPolyData.h>
	#include <vtkDataSet.h>
	#include <vtkPoints.h>
	#include <vtkStripper.h>
	#include <vtkCutter.h>
	#include <vtkPointLocator.h>
	#include <vtkPlane.h>
	#include <vtkStringArray.h>
	#include <vtkSmartPointer.h>
	#include <vtkDataArray.h>
#include <vtkFeatureEdges.h>
#include <vtkTransformFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataConnectivityFilter.h>



	#include <blVTKHelperTools.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <fstream>
	#include <stdexcept>





	//---------------------------------------------------
	// OPERATIONS
	//---------------------------------------------------


void blVTKHelperTools::SaveVtkShortArray( const char *filename, vtkShortArray* the_array )
{
	FILE* f = fopen(filename,"wb");

	if(!f)
	{
		std::ostringstream strError;
		strError <<"Failed opening "<<filename<<std::endl;
		std::cout << strError;
		throw std::runtime_error( strError.str( ) );
	}

	char id[] = "RD10";
	fwrite(id, 4, 1, f );

	int size = the_array->GetSize();
	fwrite(&size, sizeof(size), 1, f);

	std::string name;
	if ( the_array->GetName() )
	{
		name = the_array->GetName();
	}
	fwrite(name.c_str(), name.length()+1, 1, f); //+1 to include eol



	short *scalars = new short[size];

	for( int i=0; i<size; i++ ) 
		scalars[i] = the_array->GetValue(i);

	fwrite(scalars, 1, size*sizeof(scalars[0]), f);


	fclose(f);

	delete[] scalars;
}

void blVTKHelperTools::LoadVtkShortArray( const char *filename, vtkShortArray* the_array )
{
	if( strlen(filename) == 0 )
	{
		std::ostringstream strError;
		strError <<"Failed opening empty file";
		std::cout << strError.str()<<std::endl;
		throw std::runtime_error( "Failed opening file" );
	}

	FILE* f = fopen(filename,"rb");
	if( !f )
	{
		std::ostringstream strError;
		strError <<"Failed opening "<<filename;
		std::cout << strError.str()<<std::endl;
		throw std::runtime_error( "Failed opening file" );
	}

	char id[]="1234\x0";
	fread(id, sizeof( id[0] ), 4, f ); //read four symbols
	if( strcmp(id, "RD10")!=0 )
	{
		fclose(f);
		std::ostringstream strError;
		strError <<"Region definitions has incorrect version number"<<std::endl;
		std::cout << strError;
		throw std::runtime_error( "Region definitions has incorrect version number" );
	}

	int size;
	fread(&size, sizeof(size), 1, f);

	std::string name="";
	char symbol = ' ';
	while( symbol!='\x0' )
	{
		fread(&symbol, sizeof(symbol), 1, f);
		name+=symbol;
	}

	the_array->SetNumberOfValues( size );
	the_array->SetName( name.c_str() );

	short *scalars = new short[size];
	int b = fread(scalars, 1,size*sizeof(scalars[0]),f);
	if( b!=size*sizeof(scalars[0]) )
	{
		fclose(f);
		std::ostringstream strError;
		strError << "Number of items read from region definition file is incorrect. "
			<<b<<" out of "<<size<<". Aborting."<<std::endl;
		std::cout << strError;
		throw std::runtime_error( "Number of items read from region definition file is incorrect" );
	}

	for( int i=0; i<size; i++) the_array->SetValue(i, scalars[i]);

	fclose(f);

	delete[] scalars;
}


//--------------------------------------------------------------------
// file is a text file with space separated ids of excluded regions
std::vector<unsigned int> blVTKHelperTools::LoadRegionExclusions( const char* filename )
{
	std::ifstream the_file( filename );

	std::vector<unsigned int> exclusions;

	while( !the_file.eof() ) 
	{
		char val[500];
		the_file >> val;
		if( val[0]>='0' && val[0]<='9' )
			exclusions.push_back( atoi(val) );
	}
	
	return exclusions;
}
//--------------------------------------------------------------------




vtkPolyData* blVTKHelperTools::GetShapeSubSurface(vtkPolyData * inputShape, double tholdLower, double tholdUpper)
{
	//extract the subpart
	vtkSmartPointer<vtkThreshold> thold = vtkSmartPointer<vtkThreshold>::New();
	thold->SetInput(inputShape);
	thold->ThresholdBetween(tholdLower, tholdUpper);
	thold->Update();

	//extract surface
	vtkSmartPointer<vtkDataSetSurfaceFilter> sur_filt = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
	sur_filt->SetInput( (vtkDataObject*)thold->GetOutput() );
	sur_filt->Update();

	vtkSmartPointer<vtkCleanPolyData> pdCleaner = vtkSmartPointer<vtkCleanPolyData>::New();
	pdCleaner -> SetInput(sur_filt->GetOutput());
	pdCleaner -> SetTolerance (0.0);
	pdCleaner -> SetAbsoluteTolerance (1.0);
	//pdCleaner -> ToleranceIsAbsoluteOn ();
	pdCleaner -> ConvertStripsToPolysOn ();
	pdCleaner -> ConvertPolysToLinesOn ();
	pdCleaner -> PointMergingOn ();
	pdCleaner -> Update();

	///
	vtkPolyData * outputShape = vtkPolyData::New();
	outputShape->DeepCopy(pdCleaner->GetOutput());

	return outputShape;
}	



//------------------------------------------------------------------
vtkPolyData* blVTKHelperTools::GetShapeSubSurface(vtkPolyData * inputShape, unsigned int nSubPart)
//------------------------------------------------------------------
{
	return blVTKHelperTools::GetShapeSubSurface(inputShape, nSubPart-0.1, nSubPart+0.1);
}	











void blVTKHelperTools::SavePolydata( vtkPolyData* poly, const char* filename, bool binary )
{
	//vtkSTLWriter * writer = vtkSTLWriter::New();
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	if ( binary ) writer->SetFileTypeToBinary();
	writer -> SetInput(poly);
	writer -> SetFileName (filename);
	writer -> Write();
}




void blVTKHelperTools::SaveImage( vtkDataSet* image, const char* filename )
{
	vtkSmartPointer<vtkDataSetWriter> writer = vtkSmartPointer<vtkDataSetWriter>::New();
	writer->SetFileTypeToBinary();
	writer->SetInput( (vtkDataObject*)image );
	writer->SetFileName( filename );
	writer->Write();
}



////-----------------------------------------------------------------
//vtkPolyData* blVTKHelperTools::GetAnnulus(vtkPolyData* shapePt)
////------------------------------------------------------------------
//{
//        vtkSmartPointer<vtkFeatureEdges> edges = vtkSmartPointer<vtkFeatureEdges>::New();
//        edges -> SetInput(shapePt);
//        edges -> BoundaryEdgesOn();
//        edges -> SetFeatureAngle (120);
//        edges -> FeatureEdgesOff();
//        edges -> NonManifoldEdgesOff();
//        edges -> ManifoldEdgesOff();
//        edges -> Update();
//
//		vtkPolyData* result = vtkPolyData::New();
//		result->DeepCopy(edges->GetOutput());
//
//        return result; //we have only one annulus
//}
//








vtkPolyData* blVTKHelperTools::CloseSurface( vtkPolyData* shape )
{
	vtkSmartPointer<vtkFeatureEdges> fe = vtkSmartPointer<vtkFeatureEdges>::New();
	fe->SetInput( shape );
	fe->BoundaryEdgesOn();
	fe->NonManifoldEdgesOff();
	fe->FeatureEdgesOff();
	fe->ManifoldEdgesOff();
	fe->Update();

	vtkSmartPointer<vtkPolyDataConnectivityFilter> connect = 
				vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	connect->SetInput(fe->GetOutput());
	connect->Update();

	const int ncontours = connect->GetNumberOfExtractedRegions();
		
	vtkSmartPointer<vtkAppendPolyData> append = vtkSmartPointer<vtkAppendPolyData>::New();
	append->AddInput(shape);


	for( int i =0; i<ncontours ; i++ )
	{
		connect->AddSpecifiedRegion(i);
		connect->SetExtractionModeToSpecifiedRegions();
		connect->Update();
		vtkPolyData *edges = connect->GetOutput();
		vtkPolyData* cover = GenerateHoleCover(edges);

		//blVTKHelperTools::SavePolydata(edges,"edges.vtk");
		//blVTKHelperTools::SavePolydata(cover,"cover.vtk");

		append->AddInput(cover);

		cover->Delete();
		connect->DeleteSpecifiedRegion(i);
	}

	append->Update();


	vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
	cleaner->SetInput(append->GetOutput());
	cleaner->Update();

	vtkPolyData *result = vtkPolyData::New();
	result->DeepCopy(cleaner->GetOutput());

	return result;
}


vtkPolyData* blVTKHelperTools::GenerateHoleCover(vtkPolyData* edges)
{
	// We'll create the building blocks of polydata including data attributes.
	vtkPolyData* cover = vtkPolyData::New();
	vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

	vtkSmartPointer<vtkCleanPolyData> sur_filt = vtkSmartPointer<vtkCleanPolyData>::New();
	sur_filt->SetInput( edges );
	sur_filt->Update();

	points->DeepCopy(sur_filt->GetOutput()->GetPoints());

	//add centroid
	double centr[3] = {0,0,0};
	for(int i=0; i<points->GetNumberOfPoints(); i++)
	{
		for(int j=0; j<3; j++)
			centr[j] += points->GetPoint(i)[j];
	}
	for(int j=0; j<3; j++)
		centr[j] /= points->GetNumberOfPoints();

	vtkIdType cnt_pt = points->InsertNextPoint(centr);

	//add cells
	for (int i=0; i<sur_filt->GetOutput()->GetNumberOfCells(); i++)
	{
		vtkCell *cell = sur_filt->GetOutput()->GetCell(i);

		vtkIdType pts3[3];
		pts3[0] = cell->GetPointId(0);
		pts3[1] = cell->GetPointId(1);
		pts3[2] = cnt_pt;
		polys->InsertNextCell(3,pts3);
	}

	// We now assign the pieces to the vtkPolyData.
	cover->SetPoints(points);
	cover->SetPolys(polys);

	return cover;
}





void blVTKHelperTools::SavePoints( vtkPoints* pts, const char* filename )
{
	vtkPolyData* ply = blVTKHelperTools::Points2Polydata( pts );
	blVTKHelperTools::SavePolydata( ply, filename );
	ply->Delete();
}




bool blVTKHelperTools::FileExists( const char* filename, bool no_exception )
{
	bool result = true;

	FILE *fid = fopen(filename,"r");
	if( fid == NULL )
	{
		std::cerr<<"File not found: "<<filename;

		char error[500];
		sprintf(error,"File not found: %s",filename);

		if( !no_exception ) throw std::runtime_error( error );

		result = false;
	}
	else fclose(fid);

	return result;
}



//stores in "list"
void blVTKHelperTools::ReadFilelist( const char* file, std::vector<std::string>& list, bool check_existence )
{
	list.clear();

	FileExists( file );

	std::ifstream the_file( file );
	while( !the_file.eof() )
	{
		const int maxlen = 2000;
		char line[maxlen];
		the_file.getline( line, maxlen );
		
		if( strcmp(line,"")!=0 )
		{
			if( line[0]!=' ' )
			{
				if(check_existence) FileExists( line );
				list.push_back( line );
			}
		}
	}
}


void blVTKHelperTools::Diff3Points( vnl_vector<double>& input, vnl_vector<double>& derivative )
{
	//vnl_vector<double> smoothed_input( input.size() );
	const int last_elem = input.size()-1;
	//smoothed_input[0] = input[0];
	//smoothed_input[last_elem] = input[last_elem];

	//for(int i=1; i<input.size()-1; i++)
	//	smoothed_input[i] = (input[i+1] + 2*input[i] + input[i-1])/4;

	derivative.set_size( input.size() );
	derivative[0] = 0; //input[0];
	derivative[last_elem] = 0; //input[last_elem];

	for(size_t i=1; i<input.size()-1; i++)
		derivative[i] = (input[i+1] - input[i-1])/2;
}


vtkPolyData* blVTKHelperTools::Points2Polydata( vtkPoints* points, const double* scalars)
{
	vtkPolyData *polyData = vtkPolyData::New();

	vtkCellArray *candidateCells = vtkCellArray::New();
	vtkFloatArray *candidateScalars = vtkFloatArray::New();

	bool manual_scalars = (scalars==NULL);

	for (vtkIdType j = 0; j < points->GetNumberOfPoints(); j++) {
		candidateCells -> InsertNextCell(VTK_VERTEX,&j);
		const float candidateScalar = manual_scalars ? j : scalars[j];
		candidateScalars -> InsertNextTuple(&candidateScalar);
	}


	polyData -> SetPoints(points);
	polyData -> SetVerts(candidateCells);
	candidateCells->Delete();
	polyData -> GetPointData() -> SetScalars(candidateScalars);
	candidateScalars->Delete();
	polyData -> Modified();

	return  polyData;
}



vtkPolyData* blVTKHelperTools::Points2Polydata( vtkPoints* points, double scalar )
{
	vnl_vector<double> scalars(points->GetNumberOfPoints(),scalar);

	return  Points2Polydata( points, scalars.begin() );
}



void blVTKHelperTools::ExportPolyDataPoints( vtkPolyData* shape, vnl_vector<double>& points )
{
	const int ndim = 3; //number of dimensions

	points.set_size( shape->GetNumberOfPoints()*ndim );
	int c=0;
	for( int vert = 0; vert<shape->GetNumberOfPoints(); vert++ )
	{
		vnl_vector<double> point(ndim);
		shape->GetPoint(vert, point.begin());

		for( size_t k=0; k<point.size(); k++ ) 
			points[c++] = point[k];
	}
}


void blVTKHelperTools::ImportPolyDataPoints( vtkPolyData* shape, vnl_vector<double>& points )
{
	const int ndim = 3; //number of dimensions

	int c=0;
	for( int vert = 0; vert<shape->GetNumberOfPoints(); vert++ )
	{
		vnl_vector<double> point(ndim);

		for( size_t k=0; k<point.size(); k++ ) 
			point[k] = points[c++];

		shape->GetPoints()->SetPoint(vert, point.begin());
	}

	shape->Update();
}


void blVTKHelperTools::ScaleShape(vtkPolyData *shapein, vtkPolyData *shapeout, float scale, 
														bool centerAfterScale /* = false*/)
{
	vtkSmartPointer<vtkTransform> shape_tform = vtkSmartPointer<vtkTransform>::New();
	//vtkSmartPointer<vtkTransformPolyDataFilter> shape_tformer = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	vtkSmartPointer<vtkTransformFilter> shape_tformer = vtkSmartPointer<vtkTransformFilter>::New();

	shape_tform->Scale( scale, scale, scale );

	if (centerAfterScale == true)
	{
		double* origBounds = shapein->GetBounds();	
		double origCenter[3];
		origCenter[0] = (origBounds[1]+origBounds[0])/2;
		origCenter[1] = (origBounds[3]+origBounds[2])/2;
		origCenter[2] = (origBounds[5]+origBounds[4])/2;
		
		shape_tform->Translate(origCenter[0]*(1-scale)/scale, 
								origCenter[1]*(1-scale)/scale, 
								origCenter[2]*(1-scale)/scale);
	}
	shape_tformer->SetInput(shapein);
	shape_tformer->SetTransform(shape_tform);
	//shape_tformer->SetOutput( shapeout );
	shape_tformer->Update();
	shapeout->DeepCopy(shape_tformer->GetOutput());
	vtkSmartPointer<vtkDataArray> data = shapein->GetPointData()->GetVectors();
	if( data!= NULL ) 
	{
		shapeout->GetPointData()->RemoveArray(data->GetName());
		shapeout->GetPointData()->SetVectors(data);
	}
}

void blVTKHelperTools::ShrinkImage(vtkDataSet *imagein, vtkDataSet *imageout, int factor)
{
	vtkSmartPointer<vtkImageGaussianSmooth> smoother = vtkSmartPointer<vtkImageGaussianSmooth>::New();
	vtkSmartPointer<vtkImageShrink3D> resampler = vtkSmartPointer<vtkImageShrink3D>::New();

	smoother->SetInput( imagein );
	smoother->SetStandardDeviation( 0.5*(float)factor ); //like in ITK
	smoother->Update();

	resampler->SetInput( (vtkDataObject*)smoother->GetOutput() );
	resampler->SetShrinkFactors( factor, factor, factor );
	resampler->SetOutput(imageout);
	resampler->Update();

	//shrinker doubles the spacing to produce the image of the same physical size
	//this must be fixed

	double spacing[3];
	((vtkImageData*)imageout)->GetSpacing( spacing );
	for( int kk = 0; kk<3; kk++ ) spacing[kk] /= factor;
	((vtkImageData*)imageout)->SetSpacing( spacing );
	imageout->Update();

	const int *dimensions = ((vtkImageData*)imageout)->GetDimensions();

	const double *old_origin = ((vtkImageData*)imagein)->GetOrigin();

	//this should change depending on modality. In this case it's needed for CT
	//this is just a quick solution.
	if( old_origin[0]!=0 || old_origin[1]!=0 || old_origin[2]!=0 )
	{
		std::cout<<"old_origin:"<<old_origin[0]<<" "<<old_origin[1]<<" "<<old_origin[2]<<std::endl;
		double origin[3];
		origin[0] = -(dimensions[0]*spacing[0]/2) + spacing[0]/2;
		origin[1] = -(dimensions[1]*spacing[1]/2) + spacing[1]/2;
		origin[2] = -(dimensions[2]*spacing[2]/2) + spacing[2]/2;
		((vtkImageData*)imageout)->SetOrigin( origin[0],origin[1],origin[2] );
		std::cout<<"origin:"<<origin[0]<<" "<<origin[1]<<" "<<origin[2]<<std::endl;
	}



}

void blVTKHelperTools::ScaleVolume(vtkUnstructuredGrid* volumein, vtkUnstructuredGrid* volumeout, float scale)
{
	vtkSmartPointer<vtkTransform> volume_tform = vtkSmartPointer<vtkTransform>::New();
	vtkSmartPointer<vtkTransformFilter> volume_tformer = vtkSmartPointer<vtkTransformFilter>::New();
	volume_tform->Scale( scale, scale, scale );

	volume_tformer->SetInput(volumein);
	volume_tformer->SetTransform(volume_tform);
	volume_tformer->Update();
	volumeout->DeepCopy(volume_tformer->GetOutput());
	vtkSmartPointer<vtkDataArray> data = volumein->GetPointData()->GetVectors();
	if( data!= NULL ) 
		{
			volumeout->GetPointData()->RemoveArray(data->GetName());
			volumeout->GetPointData()->SetVectors(data);
		}
}
