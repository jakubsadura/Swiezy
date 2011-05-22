// Copyright 2006 P	ompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "blStrainComputationFilter.h"
#include <vnl/vnl_cross.h>
#include "blCardiacShape.h"
#include "blShapeUtils.h"


/**
*/
blStrainComputationFilter::blStrainComputationFilter()
{
	this->SetNumberOfRequiredInputs(2);
	SetNumberOfRequiredOutputs( 0 );
	m_NormalsFilter = vtkSmartPointer<vtkPolyDataNormals>::New();

	this->m_AxisCompMethod = acmOldMathieu;

	m_Apex.set_size(0);
	m_Mithral.set_size(0);

	return;
}

/**
*/
void blStrainComputationFilter::SetInput( const MeshCollectionType* inputMesh )
{
	// Process object is not const-correct so the const_cast is required here
	this->ProcessObject::SetNthInput(0,  const_cast< MeshCollectionType* >(inputMesh) );

	return;
}
/**
This method effectively produces the ouput of the filter
- set of deformed meshes using the input 3D or 4D transformation
- strain computed on each node of the deformed meshes
*/
void blStrainComputationFilter::GenerateData()
{
	// In case we want to compute strain, delegate the computation to the ComputeStrain
	// function
	ComputeStrain();

	return;
}

void blStrainComputationFilter::ComputeStrain()
{
	MeshCollectionType* inputMeshWrapper = dynamic_cast<MeshCollectionType*>(this->ProcessObject::GetInput(0));
	vtkPolyData* inputMesh = 0;
	inputMesh = inputMeshWrapper->meshes[ 0 ];

	// filter to compute normal directions
	// Computing CELL normals for input poly data
	m_NormalsFilter->SetInput(inputMesh);
	m_NormalsFilter->ComputeCellNormalsOn();
	m_NormalsFilter->Update();
	vtkCellData* normals2 = m_NormalsFilter->GetOutput()->GetCellData();

	// Specific preprocess
    m_CurrentTimeStep=0;
	if ((inputMesh->GetCellData()->GetArray("radialStrain") == NULL) || 
		(inputMesh->GetCellData()->GetArray("circStrain") == NULL) ||
		(inputMesh->GetCellData()->GetArray("longStrain") == NULL) ||
		(inputMesh->GetCellData()->GetVectors() == NULL) )
	{
	  PreProcessTimeStep( );
	}

	// From 1 to the end
	for (m_CurrentTimeStep=1; m_CurrentTimeStep < inputMeshWrapper->meshes.size(); m_CurrentTimeStep++)
	{

		if ( inputMeshWrapper->meshes[m_CurrentTimeStep]->GetNumberOfCells() != 
			 inputMesh->GetNumberOfCells() )
			break; // This should not be happening

		// Specific preprocess
		PreProcessTimeStep( );

		if ( (m_Mithral.size()==0)||(m_Apex.size()==0) )
		{
			blCardiacShape::Pointer cardiacShape = blCardiacShape::New( inputMeshWrapper->meshes.at(m_CurrentTimeStep) );
			m_Mithral.set_size(3);
			m_Apex.set_size(3);
			cardiacShape->GetLVLongAxis(m_Apex,m_Mithral);

		}

		unsigned int normalsCounter = 0;
		for ( unsigned int cellId=0; cellId < m_NormalsFilter->GetOutput()->GetNumberOfCells(); cellId++)

		{
			vtkCell* cell = inputMeshWrapper->meshes.at(m_CurrentTimeStep)->GetCell(cellId);

			// If the cell is triangular, process it. (It does not make sense to compute the normal
			// on non polygonal cells)
			if ( cell->GetCellType() == VTK_TRIANGLE ) 
			{
				// Getting normal direction of this cell
				double normalToSurface[m_ImageDimension];

				normals2->GetNormals()->GetTuple(normalsCounter, normalToSurface);

				vnl_vector<double>  radialDirection(m_ImageDimension,0.0);
				vnl_vector<double>  circumDirection(m_ImageDimension,0.0);
				vnl_vector<double>  longDirection(m_ImageDimension,0.0);

				switch (this->m_AxisCompMethod)
				{
				case acmOldMathieu:
					for (unsigned int d=0; d<radialDirection.size(); d++)
					{
						radialDirection[d] = normalToSurface[d];
					}
					radialDirection.normalize();

					// Circ. direction at this cell
					circumDirection[0] = -radialDirection[1];
					circumDirection[1] = radialDirection[0];
					circumDirection[2] = 0;
					circumDirection.normalize();

					// Long. direction at this cell
					longDirection = vnl_cross_3d(circumDirection,radialDirection);
					longDirection.normalize();
					break;

				case acmAxesLocal:
					GetLVLongRadCircDirectionsLocal(inputMeshWrapper->meshes.at(m_CurrentTimeStep), cellId, m_Apex, m_Mithral,radialDirection,
						circumDirection, longDirection);
					break;
				case acmAxesGlobal:
					GetLVLongRadCircDirectionsGlobal(inputMeshWrapper->meshes.at(m_CurrentTimeStep), cellId, m_Apex, m_Mithral,radialDirection,
						circumDirection, longDirection);
					break;
				}
        
				// Comptue strain
				StrainValues result;
				

				inputMeshWrapper->meshes[m_CurrentTimeStep]->GetCellData()->GetVectors()
					->InsertTuple3(normalsCounter,	radialDirection[0],
					radialDirection[1],
					radialDirection[2]);

				ComputeStrainOnMesh( 
					inputMesh, 
					cellId, 
					cell, 
					radialDirection, 
					circumDirection, 
					longDirection,
					result );

				vtkDataArray* currentArray = inputMeshWrapper->meshes[m_CurrentTimeStep]->GetCellData()->GetArray("radialStrain");
				if (currentArray != NULL)
				{
				currentArray->InsertTuple1(normalsCounter, result.currentStrainValRad);
				}
				currentArray = inputMeshWrapper->meshes[m_CurrentTimeStep]->GetCellData()->GetArray("circStrain");
				if (currentArray != NULL)
				{
				currentArray->InsertTuple1(normalsCounter, result.currentStrainValCirc);
				}
				currentArray = inputMeshWrapper->meshes[m_CurrentTimeStep]->GetCellData()->GetArray("longStrain");
				if (currentArray != NULL)
				{
				currentArray->InsertTuple1(normalsCounter, result.currentStrainValLong);
				}

/*
				inputMeshWrapper->meshes[m_CurrentTimeStep]->GetCellData()->GetVectors()
					->InsertTuple3(normalsCounter,	longDirection[0],
					longDirection[1],
					longDirection[2]);
*/
				

				// increment the counter of visited triangular cells
				normalsCounter++;

			} // END if ( cell->GetCellType() == VTK_TRIANGLE ) 			
		} //END for ( int cellId=0; cellId < m_NormalsFilter->GetOutput()->GetNumberOfCells(); cellId++)

	}
	// Normal exit
	return;
}

void blStrainComputationFilter::Update()
{
	MeshCollectionType* inputMeshWrapper = dynamic_cast<MeshCollectionType*>(this->ProcessObject::GetInput(0));
	vtkPolyData* inputMesh = 0;

	// Make sure input is available
	if ( inputMeshWrapper == 0 )
	{
		itkExceptionMacro(<< "No input mesh");
	}

	// Notify start event observers
	this->InvokeEvent( itk::StartEvent() );

	// Actually do something
	this->GenerateData();

	// Notify end event observers
	this->InvokeEvent( itk::EndEvent() );

}

blStrainComputationFilter::MeshCollectionType* 
blStrainComputationFilter::GetInput()
{
	MeshCollectionType* inputMeshWrapper = 
		dynamic_cast<MeshCollectionType*>(this->ProcessObject::GetInput(0));
	return inputMeshWrapper;
}

void blStrainComputationFilter::ComputeStrainOnMesh(
	vtkPolyData* inputMesh,
	unsigned int cellId,
	vtkCell* cell,
	vnl_vector<double>  &radialDirection,
	vnl_vector<double>  &circumDirection,
	vnl_vector<double>  &longDirection,
	StrainValues &result )
{
	
	double point0[m_ImageDimension], point1[m_ImageDimension], point2[m_ImageDimension];
	double point0init[m_ImageDimension], point1init[m_ImageDimension], point2init[m_ImageDimension];
	double* displacements = new double[3*m_ImageDimension]; //Number of points in a triangle * ImageDimension
	double pcoord[m_ImageDimension];
	// Going through deformed meshes
	double* deriv = new double[m_ImageDimension*m_ImageDimension];

	cell->GetParametricCenter(pcoord);
	cell->GetPoints()->GetPoint(0, point0);
	cell->GetPoints()->GetPoint(1, point1);
	cell->GetPoints()->GetPoint(2, point2);

	unsigned int numberOfCells = inputMesh->GetNumberOfCells();

	if (cellId < numberOfCells)
	{
		inputMesh->GetCell(cellId)->GetPoints()->GetPoint(0, point0init);
		inputMesh->GetCell(cellId)->GetPoints()->GetPoint(1, point1init);
		inputMesh->GetCell(cellId)->GetPoints()->GetPoint(2, point2init);
	}

	for (unsigned int d=0; d<m_ImageDimension;d++)
		displacements[d] = point0[d] - point0init[d];
	for (unsigned int d=0; d<m_ImageDimension;d++)
		displacements[d+m_ImageDimension] = point1[d] - point1init[d];
	for (unsigned int d=0; d<m_ImageDimension;d++)
		displacements[d+2*m_ImageDimension] = point2[d] - point2init[d];

	cell->Derivatives(0, pcoord, displacements, 3, deriv);

	vnl_matrix<double> strain(deriv, m_ImageDimension, m_ImageDimension);
	vnl_matrix<double> identity(m_ImageDimension, m_ImageDimension);

	identity.set_identity();
    
	strain = (strain.transpose()*strain + strain + strain.transpose())/2.;

	vnl_matrix<double> radDirectionCopy(m_ImageDimension, 1);
	vnl_matrix<double> circDirectionCopy(m_ImageDimension, 1);
	vnl_matrix<double> longDirectionCopy(m_ImageDimension, 1);
	for (unsigned int d=0; d<m_ImageDimension; d++)
	{
		radDirectionCopy(d,0) = radialDirection[d];
		circDirectionCopy(d,0) = circumDirection[d];
		longDirectionCopy(d,0) = longDirection[d];
	}

	vnl_matrix<double> currentStrainRad 
		= radDirectionCopy.transpose() * strain * radDirectionCopy;
	vnl_matrix<double> currentStrainCirc 
		= circDirectionCopy.transpose() * strain * circDirectionCopy;
	vnl_matrix<double> currentStrainLong
		= longDirectionCopy.transpose() * strain * longDirectionCopy;

	result.currentStrainValRad = currentStrainRad(0,0);
	result.currentStrainValCirc = currentStrainCirc(0,0);
	result.currentStrainValLong = currentStrainLong(0,0);

	delete [] displacements;
	delete [] deriv;
}

void blStrainComputationFilter::PreProcessTimeStep()
{
	MeshCollectionType* inputMeshWrapper = dynamic_cast<MeshCollectionType*>(this->ProcessObject::GetInput(0));


	//Add the scalar strain values to the initial mesh. All values are 0
	vtkSmartPointer<vtkFloatArray> rs_array = vtkSmartPointer<vtkFloatArray>::New();
	rs_array->SetName("radialStrain");
	vtkSmartPointer<vtkFloatArray> cs_array = vtkSmartPointer<vtkFloatArray>::New();
	cs_array->SetName("circStrain");
	vtkSmartPointer<vtkFloatArray> ls_array = vtkSmartPointer<vtkFloatArray>::New();
	ls_array->SetName("longStrain");
	vtkSmartPointer<vtkFloatArray> vectors = vtkSmartPointer<vtkFloatArray>::New();
	vectors->SetNumberOfComponents(3);
	vectors->SetName("vectors");
	if (inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetArray("radialStrain") == NULL)
	  inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->AddArray(rs_array);
    if (inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetArray("longStrain") == NULL)
	  inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->AddArray(ls_array);
	if (inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetArray("circStrain") == NULL)
	  inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->AddArray(cs_array);
	if (inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetVectors() == NULL)
	  inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->SetVectors(vectors);

	for(int i = 0; i<inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetNumberOfCells(); i++ )
	{	
		vtkDataArray* currentArray = inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetArray("radialStrain");
		if (currentArray != NULL)
		{
			currentArray->InsertTuple1(i,0.);
		}
		currentArray = inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetArray("circStrain");
		if (currentArray != NULL)
		{
			currentArray->InsertTuple1(i, 0.);
		}
		currentArray = inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetArray("longStrain");
		if (currentArray != NULL)
		{
			currentArray->InsertTuple1(i, 0.);
		}
		
		currentArray = inputMeshWrapper->meshes[ m_CurrentTimeStep ]->GetCellData()->GetVectors();
		if (currentArray != NULL)
		{
			currentArray->InsertTuple3(i, 0.,0.,0.);
		}
	}
}

//--------------------------------------------------------------
//
//   costa's stuff
//	copies from smlLongitudinal displacement
//
//--------------------------------------------------------------


void blStrainComputationFilter::GetLVLongRadCircDirectionsLocal( vtkPolyData* lv, long int cell_id, const vnl_vector<double>& apex, const vnl_vector<double>& mithral, vnl_vector<double>& radial, vnl_vector<double>& circum, vnl_vector<double>& longit )
{

	if( lv->GetCellData()->GetNormals() == 0 )
	{
		vtkSmartPointer<vtkPolyDataNormals> normalgen = vtkSmartPointer<vtkPolyDataNormals>::New();
		normalgen->SetInput(lv);
		normalgen->ConsistencyOn();
		normalgen->NonManifoldTraversalOn();
		normalgen->ComputeCellNormalsOn();
		normalgen->Update();
		lv->DeepCopy(normalgen->GetOutput());
	}

	vnl_vector<double> la = apex-mithral;

	vtkDataArray* normals = lv->GetCellData()->GetNormals();

	if (normals==0){
		std::cerr<<"Normals could not get computed"<<std::endl;
		return;
	}

	normals->GetTuple(cell_id, radial.begin());


	vnl_vector<double> vert(3);
	double pcoords[3];
	double weights[3];
	int subid = lv->GetCell(cell_id)->GetParametricCenter(pcoords);
	lv->GetCell(cell_id)->EvaluateLocation(subid, pcoords, vert.begin(), weights);

	//CorrectNormal(radial, apex, mithral, vert);

	circum = vnl_cross_3d(radial, la); //tensor comp 2
	circum.normalize();

	longit = vnl_cross_3d(circum,radial); //tensor comp 3
	longit.normalize();

}




void blStrainComputationFilter::GetLVLongRadCircDirectionsGlobal( vtkPolyData* lv, long int cell_id, const vnl_vector<double>& apex, const vnl_vector<double>& mithral, vnl_vector<double>& radial, vnl_vector<double>& circum, vnl_vector<double>& longit )
{
	if( lv->GetCellData()->GetNormals() == 0 )
	{
		vtkSmartPointer<vtkPolyDataNormals> normalgen = vtkSmartPointer<vtkPolyDataNormals>::New();
		normalgen->SetInput(lv);
		normalgen->ConsistencyOn();
		normalgen->NonManifoldTraversalOn();
		normalgen->ComputeCellNormalsOn();
		normalgen->Update();
		lv->DeepCopy(normalgen->GetOutput());
	}

	longit = apex-mithral;
	longit.normalize();

	vtkDataArray* normals = lv->GetPointData()->GetNormals();

	if (normals==0){
		std::cerr<<"Normals could not get computed"<<std::endl;
		return;
	}

	normals->GetTuple(cell_id, radial.begin());
	radial = radial - dot_product(radial,longit)*longit;
	radial.normalize();

	vnl_vector<double> vert(3);
	double pcoords[3];
	double weights[3];
	int subid = lv->GetCell(cell_id)->GetParametricCenter(pcoords);
	lv->GetCell(cell_id)->EvaluateLocation(subid, pcoords, vert.begin(), weights);

	//CorrectNormal(radial, apex, mithral, vert);

	circum = vnl_cross_3d(radial, longit); 
	circum.normalize();


}


void blStrainComputationFilter::CorrectNormal( 
	vnl_vector<double>& normal, 
	const vnl_vector<double>& apex, 
	const vnl_vector<double>& mithral, 
	const vnl_vector<double> vert )
{
	//if the normal points towards LA - reverse it
	//http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
	//find the closest point p to vert that lies on LA
	vnl_vector<double> p1 = apex;
	vnl_vector<double> p2 = mithral;
	vnl_vector<double> p3 = vert;
	double u = dot_product(p3-p1,p2-p1)/((p2-p1).squared_magnitude());
	vnl_vector<double> p = p1+u*(p2-p1);
	double d = (vert-p).two_norm();
	if(d>1) //something weird if the point is too close
	{
		double cos_angle = dot_product(vert-p,normal)/d;
		if(cos_angle < 0) normal = -normal;
	}
	else
	{
		if(d==0) normal = -normal; //endocardium normal reverse on apex
	}
	//:~ End of normal reversal
}
