/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meVTKTetraGenerationFilter.h"

#include "blShapeUtils.h"
#include "boost/lexical_cast.hpp"
#include "tetgen.h"


#include "blTetgenNodeFile.h"
#include "blTetgenEleFile.h"

// imports from other namespaces
using base::tetgen::readNodeFile;
using base::tetgen::readEleFile;

#include "vtkCleanPolyData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkExecutive.h"

vtkStandardNewMacro(meVTKTetraGenerationFilter);


meVTKTetraGenerationFilter::meVTKTetraGenerationFilter()
{
	m_params = meMeshTetraGenerationParamsPtr(new meMeshTetraGenerationParams);

	this->SetNumberOfOutputPorts(1);
	vtkUnstructuredGrid *output1 = vtkUnstructuredGrid::New();
	this->GetExecutive()->SetOutputData(0, output1);
	output1->Delete();
	m_grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
}


meVTKTetraGenerationFilter::~meVTKTetraGenerationFilter() 
{
}


int  meVTKTetraGenerationFilter::RequestData(
	vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{

	// -------------------------------------------  
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and ouptut
	vtkPolyData *input = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));


	std::cout << "TetraGeneration has started" << std::endl;

	blShapeUtils::ShapeUtils::SaveShapeToFile( input, "tetgeninput.stl" );

	try
	{
		tetgenio in, out;
		std::string switches;
		if (m_params->m_MaxVolume != 0 && m_params->m_AspectRatio == 0)
		{
			switches = "pa" + boost::lexical_cast<std::string>(m_params->m_MaxVolume) + "V";
			std::cout<<"Executing command : tetgen -"<<switches<<" tetgeninput.stl"<<std::endl;
		}
		if (m_params->m_MaxVolume != 0 &&m_params->m_AspectRatio != 0)
		{
			switches = "pa" + boost::lexical_cast<std::string>(m_params->m_MaxVolume) + 
				"q"+ boost::lexical_cast<std::string>(m_params->m_AspectRatio) +"V";
			std::cout<<"Executing command : tetgen -"<<switches<<" tetgeninput.stl"<<std::endl;
		}

		char* charswitch = new char[switches.size() +1];
		std::strcpy(charswitch,switches.c_str());

		in.load_stl("tetgeninput");

		tetrahedralize( charswitch , &in, &out);

		out.save_nodes("tetgenoutput");
		out.save_elements("tetgenoutput");
		out.save_faces("tetgenoutput");

		//convert .node and .ele to VTK
		//read file.node
		std::ifstream points("tetgenoutput.node", std::ios::binary);
		readNodeFile *pointsRead = new readNodeFile(points);
		//read file.ele
		std::ifstream eles("tetgenoutput.ele", std::ios::binary);
		readEleFile *elementsRead = new readEleFile(eles);
		CreateOutput(pointsRead->getPointReader(),
			elementsRead->getElementsReader());
		//conversion
	
		
		//clean
		delete []charswitch;
		delete pointsRead;
		delete elementsRead;
	

	}
	catch( vtkstd::exception& e )
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return 0;
	}

	return 1;
	std::cout << "TetraGeneration has finished" << std::endl;

}

void meVTKTetraGenerationFilter::SetParams( meMeshTetraGenerationParamsPtr params)
{
	m_params = params;
}

meMeshTetraGenerationParamsPtr meVTKTetraGenerationFilter::GetParams()
{
	return m_params;
}

vtkUnstructuredGrid* meVTKTetraGenerationFilter::GetVtkOutput()
{
/*	return vtkUnstructuredGrid::SafeDownCast(this->GetExecutive()->GetOutputData(0));*/
	return m_grid;
}

void meVTKTetraGenerationFilter::CreateOutput(iDataReaderStaticI<dVector3D> *points,
											  iDataReaderStaticI<tetrahedron> *tetrahedra)
{
	// create points
	int m_numPoints;
	m_numPoints = points->getNumSpaceElements();
	vtkPoints *vtkPoints = vtkPoints::New();
	vtkPoints->SetNumberOfPoints(m_numPoints);
	points->reset();
	while (points->hasNext()) {
		int id;
		dVector3D point = points->readNextData(&id);
		vtkPoints->SetPoint(id, point.m_x, point.m_y, point.m_z);
	}

	// add points as nodes to the mesh
	m_grid->SetPoints(vtkPoints);

	// delete points
	vtkPoints->Delete();

	// create tetrahedra
	int m_numCells = 0;
	tetrahedra->reset();
	int num = tetrahedra->getNumSpaceElements();
	vtkIdList **tetraPoints = new vtkIdList*[num];
	while (tetrahedra->hasNext()) {
		int id;
		tetrahedron tetra = tetrahedra->readNextData(&id);
		tetraPoints[id] = vtkIdList::New();
		tetraPoints[id]->SetNumberOfIds(4);
		tetraPoints[id]->SetId(0, tetra.m_cornerA);
		tetraPoints[id]->SetId(1, tetra.m_cornerB);
		tetraPoints[id]->SetId(2, tetra.m_cornerC);
		tetraPoints[id]->SetId(3, tetra.m_cornerD);
	}
	// add the tetrahedra to structured grid
	for (int i = 0; i < num; i++) {
		m_grid->InsertNextCell(VTK_TETRA, tetraPoints[i]);
		tetraPoints[i]->Delete();
	}
	delete[] tetraPoints;

	// update cell counter
	m_numCells += num;
}
