/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKSkeleton.h"
#include "meVTKPolyLineClean.h"

// BaseLib
#include "blPolyDataDistanceTransformFilter.h"
#include "blBinaryImageToVTKPolyLine.h"

//ITK
#include <itkRecursiveGaussianImageFilter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkGradientImageFilter.h>
// ITK Our's
#include <itkAverageOutwardFluxImageFilter.h>
#include <itkMedialCurveImageFilter.h>
// VTK
#include <vtkLine.h>
#include <vtkGenericCell.h>
#include <vtkPolyLine.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkMergePoints.h>
#include <vtkSplineFilter.h>
#include <vtkStripper.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkDecimatePro.h>
#include <vtkPolyDataWriter.h>
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "blShapeUtils.h"

vtkStandardNewMacro(meVTKSkeleton);


meVTKSkeleton::meVTKSkeleton()
{
	inputData		= NULL;
	outputData		= NULL;
	offset[0] = 0.0;
	offset[1] = 0.0;
	offset[2] = 0.0;
	spacing[0] = 0.0;
	spacing[1] = 0.0;
	spacing[2] = 0.0;
	dimensions[ 0 ] = 0;
	dimensions[ 1 ] = 0;
	dimensions[ 2 ] = 0;
	m_uiMaxDimension = 0;
	sign	= true;
	splineSmoothOutputOn =false;
	sigma = 1.0;
	threshold = 0.0;
	decimateReduction  = 0.0;
}


meVTKSkeleton::~meVTKSkeleton() 
{
}


int meVTKSkeleton::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{

	// -------------------------------------------  
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and ouptut
	vtkPolyData *inputDataSurf = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkPolyData *outputPolyData = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	if ( inputData.IsNull() && inputDataSurf == NULL)
		return 0;
		
	SetInputAsSurface(inputDataSurf);

	try
	{
		if (inputData.IsNull() )
		{
			// Compute the distance map
			///////////////////////////////////////////////
			ComputeDistanceMap( inputDataSurf );
			
			///////////////////////////////////////////////
			// Compute spacing and offset
			ComputeSpacingAndOffset( inputDataSurf);

			////////////////////////////////////////////////////////////////////////////////
			// Converting ImageData -> ITK
			////////////////////////////////////////////////////////////////////////////////
			//Creation of the distance image
			CreateDistanceImage( inputDataSurf );
		}
		else
		{
			// If the output image 
			////Associated region
			this->spacing = this->inputData->GetSpacing();
			this->offset = this->inputData->GetOrigin();
			this->region = this->inputData->GetRequestedRegion();
			this->sizeR = this->region.GetSize();
			this->startR = this->region.GetIndex();
		}
			//////////////////////////////////////////////////////////////////////////////////
			// 	Compute the associated average outward flux and skeleton
			//////////////////////////////////////////////////////////////////////////////////
		ComputeAssociatedAverageOutwardFlux();

		outputPolyData->DeepCopy(GetOutputAsVTKPolyData());
	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		throw;
	}

	return 1;
	
}

void meVTKSkeleton::ComputeDistanceMap(vtkPolyData* input )
{
	if ((decimateReduction<1.0) && (decimateReduction>0.0))
	{
		vtkSmartPointer<vtkDecimatePro> decimateFilter = vtkSmartPointer<vtkDecimatePro>::New();
		decimateFilter->SetInput( input );
		decimateFilter->SetTargetReduction( decimateReduction );
		decimateFilter->PreserveTopologyOn();
		decimateFilter->BoundaryVertexDeletionOff();
		decimateFilter->SplittingOff ();
		decimateFilter->Update();

		input->DeepCopy( decimateFilter->GetOutput() );
	}

	vtkSmartPointer<vtkCleanPolyData> cleaner;
	vtkSmartPointer<vtkPolyDataNormals> normals;

	if ( sign )
	{
		normals = vtkPolyDataNormals::New();
		normals->SetInput( input );  
		normals->ComputePointNormalsOn();
		normals->ComputeCellNormalsOn();
		normals->ConsistencyOn();
		normals->AutoOrientNormalsOn();
		normals->Update();

		double clean = 1.0e-10;
		double abs   = 1.0e-10;
		cleaner = vtkCleanPolyData::New();
		cleaner->SetInput( normals->GetOutput() );
		cleaner->SetTolerance( clean );
		cleaner->SetAbsoluteTolerance( abs );
		cleaner->Update();
		
		input->DeepCopy( cleaner->GetOutput() );
	}
}

void meVTKSkeleton::ComputeSpacingAndOffset( vtkPolyData* input)
{

	// Initial values
	offs[0] = this->offset[0];
	offs[1] = this->offset[1];
	offs[2] = this->offset[2];
	spac[0] = this->spacing[0];
	spac[1] = this->spacing[1];
	spac[2] = this->spacing[2]; 

	if ( spac[0] == 0.0 )
	{
		double bounds[6];
		///////////////////////////////////////////////
		//Compute the dimensions of the bounding box.
		input->GetBounds( bounds );

		//Apply offset to the bounding box. 
		bounds[0] = bounds[0] - offs[0];
		bounds[1] = bounds[1] + offs[0];
		bounds[2] = bounds[2] - offs[1];
		bounds[3] = bounds[3] + offs[1];	
		bounds[4] = bounds[4] - offs[2];
		bounds[5] = bounds[5] + offs[2];

		if ( this->m_uiMaxDimension != 0 )
		{
			for( unsigned int i = 0; i < 3; i++ )
			{
				spac[i] = ( bounds[2*i+1] - bounds[2*i] ) / this->m_uiMaxDimension;
			}
			spac[ 0 ] = max( max( spac[ 0 ], spac[ 1 ]), spac[ 2 ] );
			spac[ 1 ] = max( max( spac[ 0 ], spac[ 1 ]), spac[ 2 ] );
			spac[ 2 ] = max( max( spac[ 0 ], spac[ 1 ]), spac[ 2 ] );
		}
		else
		{
			for( unsigned int i = 0; i < 3; i++ )
			{
				spac[i] = ( bounds[2*i+1] - bounds[2*i] ) / this->dimensions[i];
			}
		}
	}
}

void meVTKSkeleton::CreateDistanceImage( vtkPolyData* input)
{
	blPolyDataDistanceTransformFilter::Pointer dtFilter = blPolyDataDistanceTransformFilter::New();
	dtFilter->SetInput( input );
	dtFilter->SetSpacing( spac );
	dtFilter->SetOffset( offs );
	dtFilter->SetSigned( sign );
	dtFilter->Update();
	inputData = SkeletonAPITypes::InputImageType::New();
	////Associated region
	this->startR[0]=0;
	this->startR[1]=0;
	this->startR[2]=0;
	unsigned int idim[3];
	dtFilter->GetDimensions( idim );
	this->sizeR[0]=idim[0];
	this->sizeR[1]=idim[1];
	this->sizeR[2]=idim[2];

	this->region.SetSize( this->sizeR );
	this->region.SetIndex( this->startR );
	//Allocating distance image
	this->inputData->SetRegions( this->region );
	this->inputData->SetSpacing( dtFilter->GetSpacing() );
	this->inputData->SetOrigin( dtFilter->GetOutput()->GetOrigin() );
	this->inputData->Allocate();
				
	//Creation of image iterator
	SkeletonAPITypes::InputRegionIteratorType it( this->inputData, this->region );
	double *ptr = (double *) dtFilter->GetOutput()->GetScalarPointer();

	for( it.GoToBegin(); !it.IsAtEnd(); ++it  )
	{
		it.Set( *ptr );
		ptr++;
	}
}

void meVTKSkeleton::ComputeAssociatedAverageOutwardFlux()
{
	//To have a good quality gradient of the distance map, perform a light smooth over it. Define  
	//convolution kernels in each direction and use them recursively. 
	typedef itk::RecursiveGaussianImageFilter< SkeletonAPITypes::InputImageType , SkeletonAPITypes::InputImageType > RecursiveGaussianFilterType;
	RecursiveGaussianFilterType::Pointer gaussianFilterX = RecursiveGaussianFilterType::New();
	RecursiveGaussianFilterType::Pointer gaussianFilterY = RecursiveGaussianFilterType::New();
	RecursiveGaussianFilterType::Pointer gaussianFilterZ = RecursiveGaussianFilterType::New();

	gaussianFilterX->SetDirection( 0 );
	gaussianFilterY->SetDirection( 1 );
	gaussianFilterZ->SetDirection( 2 );

	gaussianFilterX->SetOrder( RecursiveGaussianFilterType::ZeroOrder );
	gaussianFilterY->SetOrder( RecursiveGaussianFilterType::ZeroOrder );
	gaussianFilterZ->SetOrder( RecursiveGaussianFilterType::ZeroOrder );

	gaussianFilterX->SetNormalizeAcrossScale( false );
	gaussianFilterY->SetNormalizeAcrossScale( false );
	gaussianFilterZ->SetNormalizeAcrossScale( false );

	gaussianFilterX->SetInput( this->inputData );
	gaussianFilterY->SetInput( gaussianFilterX->GetOutput() );
	gaussianFilterZ->SetInput( gaussianFilterY->GetOutput() );

	gaussianFilterX->SetSigma( this->sigma );
	gaussianFilterY->SetSigma( this->sigma );
	gaussianFilterZ->SetSigma( this->sigma );

	typedef itk::GradientImageFilter< SkeletonAPITypes::InputImageType , SkeletonAPITypes::InputPixelType, SkeletonAPITypes::InputPixelType > GradientFilterType;
	typedef GradientFilterType::OutputImageType::PixelType GradientPixelType;
	// Compute the gradient.
	GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
	gradientFilter->SetInput( gaussianFilterZ->GetOutput() );

	// Compute the average outward flux.
	typedef itk::AverageOutwardFluxImageFilter< SkeletonAPITypes::InputImageType, SkeletonAPITypes::InputPixelType, GradientPixelType > AOFFilterType;
	AOFFilterType::Pointer aofFilter = AOFFilterType::New();
	aofFilter->SetInput( this->inputData );
	aofFilter->SetGradientImage( gradientFilter->GetOutput() );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. Compute the skeleton
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	typedef itk::MedialCurveImageFilter< SkeletonAPITypes::InputImageType > MedialCurveFilter;
	MedialCurveFilter::Pointer medialFilter = MedialCurveFilter::New();
	medialFilter->SetInput( this->inputData );
	medialFilter->SetAverageOutwardFluxImage( aofFilter->GetOutput() );
	medialFilter->SetThreshold( this->threshold );
	medialFilter->Update();
	this->outputData = medialFilter->GetOutput();
}
void meVTKSkeleton::ConvertVtkToPolyline(vtkUnstructuredGrid *vtkUG, vtkPolyData *vtkPD) {
	vtkPD->SetPoints(vtkUG->GetPoints());
	vtkPD->Allocate(this->totalPoints);
	vtkGenericCell *ugLine = vtkGenericCell::New();

	for (vtkIdType i=0;i<vtkUG->GetNumberOfCells();i++)
	{
		vtkUG->GetCell(i,ugLine);
		vtkPD->InsertNextCell(ugLine->GetCellType(),ugLine->GetPointIds());
	}
}

void meVTKSkeleton::SetInputAsSurface(vtkPolyData *input)
{
	// Extract edges, but only boundary edges
	vtkSmartPointer<vtkFeatureEdges> extractEdges = 
			vtkSmartPointer<vtkFeatureEdges>::New();
	extractEdges->SetInput( input );
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

	unsigned int numHoles = connectivity->GetNumberOfExtractedRegions();
	// Append polydata, input is added and then addd hole patch mesh one by one
	vtkSmartPointer<vtkAppendPolyData> append = vtkSmartPointer<vtkAppendPolyData>::New();
	append->AddInput( input );

	for ( unsigned int i = 0; i < numHoles; i++ )
	{
		// Select boundary (hole) to close
		connectivity->InitializeSpecifiedRegionList();
		connectivity->AddSpecifiedRegion( i );
		connectivity->Update();

		// Create an ordered polygon
		vtkSmartPointer<vtkStripper> stripper = vtkSmartPointer<vtkStripper>::New();
		stripper->SetInput( connectivity->GetOutput() );
		stripper->Update();

		// Convert to polydata (3D polyline)
		vtkSmartPointer<vtkPolyData> polygon = 
					vtkSmartPointer<vtkPolyData>::New();
		polygon->SetPoints( stripper->GetOutput()->GetPoints() );
		polygon->SetPolys( stripper->GetOutput()->GetLines() );

		// Triangulate it
		vtkSmartPointer<vtkTriangleFilter> triangulator = 
				vtkSmartPointer<vtkTriangleFilter>::New();
		triangulator->SetInput( polygon );
		triangulator->Update();

		append->AddInput( triangulator->GetOutput() );
	}
	append->Update();

	input->DeepCopy( append->GetOutput() );
}

vtkPolyData* meVTKSkeleton::GetOutputAsVTKPolyData(bool removeSmallBranches) 
{
	vtkPolyData*  output = vtkPolyData::New();
	blBinaryImageToVTKPolyLine::Pointer polyConverter = blBinaryImageToVTKPolyLine::New();
	polyConverter->SetInput(this->outputData);
	polyConverter->Update();
	
	if( removeSmallBranches )
	{
		meVTKPolyLineClean::Pointer clean = meVTKPolyLineClean::New();
		clean->SetInput(polyConverter->GetOutput());
		clean->SetTerminalSize(4);
		clean->Update();
		output->DeepCopy(clean->GetOutput());
	}
	else
	{
		output->DeepCopy(polyConverter->GetOutput());
	}
	return output;
}

void meVTKSkeleton::SetInputAsITKImage(SkeletonAPITypes::InputImagePointer inputData)
{
	this->inputData = inputData;
}

SkeletonAPITypes::InputImagePointer meVTKSkeleton::GetInputAsITKImage( )
{
	return this->inputData;
}

void meVTKSkeleton::SetSplineSmoothOutput(bool splineSmoothOutputOn)
{
	this->splineSmoothOutputOn = splineSmoothOutputOn;
}

SkeletonAPITypes::OutputImagePointer meVTKSkeleton::GetOutputAsITKImage(void)
{
	return this->outputData;
}

void meVTKSkeleton::SetInputSurfaceDecimationReduction(double decimateReduction)
{
	this->decimateReduction = decimateReduction;
}

double meVTKSkeleton::GetInputSurfaceDecimationReduction()
{
	return decimateReduction;
}

void meVTKSkeleton::SetDistanceTransformFilterDimensions(unsigned int *dimensions)
{
	this->dimensions[0] = dimensions[0];
	this->dimensions[1] = dimensions[1];
	this->dimensions[2] = dimensions[2];
}

void meVTKSkeleton::GetDistanceTransformFilterDimensions(unsigned int *dimensions)
{
	dimensions[0] = this->dimensions[0];
	dimensions[1] = this->dimensions[1];
	dimensions[2] = this->dimensions[2];
}

void meVTKSkeleton::SetDistanceTransformFilterMaxDimension(
						unsigned int uiMaxDimension)
{
	this->m_uiMaxDimension = uiMaxDimension;
}

void meVTKSkeleton::GetDistanceTransformFilterMaxDimension(
						unsigned int &uiMaxDimension)
{
	uiMaxDimension = this->m_uiMaxDimension;
}

void meVTKSkeleton::SetDistanceTransformFilterSpacing(double *spacing){
	this->spacing[0] = spacing[0];
	this->spacing[1] = spacing[1];
	this->spacing[2] = spacing[2];
	Modified();
}

void meVTKSkeleton::GetDistanceTransformFilterSpacing(double *spacing){
	spacing[0] = this->spacing[0];
	spacing[1] = this->spacing[1];
	spacing[2] = this->spacing[2];
}

void meVTKSkeleton::SetDistanceTransformFilterOffset(double  *offset){
	this->offset[0] = offset[0];
	this->offset[1] = offset[1];
	this->offset[2] = offset[2];
}

void meVTKSkeleton::GetDistanceTransformFilterOffset(double  *offset){
	offset[0] = this->offset[0];
	offset[1] = this->offset[1];
	offset[2] = this->offset[2];
}

void meVTKSkeleton::SetDistanceTransformSmoothingSigma(double sigma){
	this->sigma = sigma;
}
double meVTKSkeleton::GetDistanceTransformSmoothingSigma(){
	return this->sigma;
}

void meVTKSkeleton::SetMedialCurveThreshold(double threshold){
	this->threshold = threshold;
}

double meVTKSkeleton::GetMedialCurveThreshold(){
	return threshold;
}

void meVTKSkeleton::SetDistanceTransformFilterSing(bool sign){
	this->sign = sign;
}

void meVTKSkeleton::GetDistanceTransformFilterSing(bool sign){
	sign = this->sign;
}
