#include "meMeshStatistics.h"


me::MeshStatistics::Pointer me::MeshStatistics::New(vtkDataSet* _inputMesh)
{
	me::MeshStatistics::Pointer smartPtr; 
	me::MeshStatistics* rawPtr = new MeshStatistics(_inputMesh);

	smartPtr = rawPtr; 
	rawPtr->UnRegister(); 
	return smartPtr;
}

me::MeshStatistics::MeshStatistics(vtkDataSet* _inputMesh)
:inputMesh(_inputMesh),
meshQuality(VtkMeshQualityPtr(VtkMeshQualityPtr::New()))
{
	if(inputMesh != NULL)
		meshQuality->SetInput(inputMesh);
}

void me::MeshStatistics::setQuality(QualityType _qualityType)
{
	switch(_qualityType)
	{
	case Area:
		meshQuality->SetTriangleQualityMeasureToArea();
		break;
	case AspectRatio:
		meshQuality->SetTriangleQualityMeasureToAspectRatio();
		break;
	case EdgeRatio:
		meshQuality->SetTriangleQualityMeasureToEdgeRatio();
		break;
	case RadiusRatio:
		meshQuality->SetTriangleQualityMeasureToRadiusRatio();
		break;
	case Distortion:
		meshQuality->SetTriangleQualityMeasureToDistortion();
		break;
	case TetraEdgeRatio:
		meshQuality->SetTetQualityMeasureToEdgeRatio();
		break;
	case TetraAspectRatio:
		meshQuality->SetTetQualityMeasureToAspectRatio();
		break;
	case TetraCollapseRatio:
		meshQuality->SetTetQualityMeasureToCollapseRatio();
		break;
	case TetraRadiusRatio:
		meshQuality->SetTetQualityMeasureToRadiusRatio();
		break;
	case TetraMinAngle:
		meshQuality->SetTetQualityMeasureToMinAngle();
		break;
	case TetraAspectFrobenius:
		meshQuality->SetTetQualityMeasureToAspectFrobenius();
		break;
	// SetTetQualityMeasureToEdge doesn't exists anymore in vtk 5.4.2
	//case TetraEdge:
	//	meshQuality->SetTetQualityMeasureToEdge();
	//	break;
	}
	meshQuality->Update();
}


unsigned int me::MeshStatistics::getNumberOfCells()
{
	if(inputMesh == NULL)
		return 0;

	return inputMesh->GetNumberOfCells();
}

vtkDataArray* me::MeshStatistics::getQualityArray(QualityType _qualityType)
{
	setQuality(_qualityType);
	return meshQuality->GetOutput()->GetCellData()->GetArray( "Quality" );
}

vtkDataObject* me::MeshStatistics::computeHistogram(unsigned int _numberOfBins, QualityType _qualityType)
{
	//! get quality array
	setQuality(_qualityType);
	VtkDataArrayPtr qualityArray = VtkDataArrayPtr(meshQuality->GetOutput()->GetCellData()->GetArray( "Quality" ));
	int count = qualityArray->GetNumberOfTuples();
	vnl_matrix<double> data(count, 1);
	for(int i=0; i<count; i++)
	{
		data[i][0] = qualityArray->GetComponent(i, 0);
	}

	//! compute histogram
	//! specify the number of bins
	vnl_vector<int> nBins(2);
	nBins[0] = _numberOfBins;
	nBins[1] = 1;
	vnl_matrix<double> countMatrix; //output1
	std::vector< vnl_vector<double> > binCentersVector; // output2
	blHistogram<double>::Pointer myHist = blHistogram<double>::New();
	myHist->GetHistogram(1, data, nBins, countMatrix, binCentersVector);

	//! prepare final output data
	vtkDataArray *dataArrayX = vtkDataArray::CreateDataArray(VTK_DOUBLE);
	vtkDataArray *dataArrayY = vtkDataArray::CreateDataArray(VTK_DOUBLE);
	dataArrayX->SetNumberOfTuples(_numberOfBins);
	dataArrayY->SetNumberOfTuples(_numberOfBins);

	signal = blSignal::New();
	std::vector<blSignal::SampleType> xvector;
	std::vector<blSignal::SampleType> yvector;
	xvector.resize(_numberOfBins);
	yvector.resize(_numberOfBins); 

	//! move histogram data to final output form
	int k = 0;
	for (unsigned int iter1 = 0; iter1 < binCentersVector[0].size(); iter1++)
	{
		double d =  binCentersVector[0][iter1];
		dataArrayX->InsertTuple(k, &d);
		xvector.at(k)= (float)d;
		k++;
	}

	k = 0;
	for (unsigned int iter1 = 0; iter1 < countMatrix.rows(); iter1++)
	{
		double d = countMatrix[iter1][0];
		dataArrayY->InsertTuple(k, &d);
		yvector.at(k)= (float)d;
		k++;
	}
	
	vtkFieldData *fieldData = vtkFieldData::New();
	vtkDataObject *dataObject = vtkDataObject::New();
	fieldData->AddArray(dataArrayX);
	fieldData->AddArray(dataArrayY);
	dataObject->SetFieldData(fieldData);
	signal->SetXVector(xvector);
	signal->SetYVector(yvector);

	return dataObject;
}	

double me::MeshStatistics::getAverageValueOfQuality(QualityType _qualityType)
{
	setQuality(_qualityType);
	if(meshQuality->GetOutput()->GetFieldData()->GetArray("Mesh Triangle Quality")->GetComponent(0, 1))
    return meshQuality->GetOutput()->GetFieldData()->GetArray("Mesh Triangle Quality")->GetComponent(0, 1);
	else if (meshQuality->GetOutput()->GetFieldData()->GetArray("Mesh Tetrahedron Quality")->GetComponent(0, 1))
	return meshQuality->GetOutput()->GetFieldData()->GetArray("Mesh Tetrahedron Quality")->GetComponent(0, 1);
	else
	return 0;
}

blSignal::Pointer me::MeshStatistics::GetSignal()
{
	return signal.GetPointer();
}