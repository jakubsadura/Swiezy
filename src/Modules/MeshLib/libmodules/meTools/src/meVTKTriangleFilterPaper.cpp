#include "meVTKTriangleFilterPaper.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkTriangleStrip.h"
#include "vtkDoubleArray.h"
#include "mevtkPolygon.h"

//-------------------------------------------------------------------------------------------------
 meVTKTriangleFilterPaper::meVTKTriangleFilterPaper()
//-------------------------------------------------------------------------------------------------
{
	// Create input to allow DeepCopy on SetInput function
	this->surrounding = vtkPolyData::New();
	this->input = vtkPolyData::New();
	this->output = vtkPolyData::New();
}
//------------------------------------------------------------------------------------------------
 meVTKTriangleFilterPaper::~meVTKTriangleFilterPaper()
//------------------------------------------------------------------------------------------------
{
	this->surrounding->Delete();
	this->input->Delete();
	if ( this->output != NULL )
		this->output->Delete();
}
//------------------------------------------------------------------
//int meVTKTriangleFilterPaper::RequestData( vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)
//------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
void meVTKTriangleFilterPaper::Update()
//------------------------------------------------------------------------------------------------
{
  /*
  // get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
  */
  
	vtkIdType numCells=this->input->GetNumberOfCells();
	vtkIdType cellNum=0;
	vtkIdType numPts, newId;
	vtkIdType npts = 0;
	vtkIdType *pts = 0;
	int i, j;
	vtkCellData *inCD=this->input->GetCellData();
	vtkCellData *outCD=this->output->GetCellData();
	vtkIdType updateInterval;
	vtkCellArray *cells;
	vtkPoints *inPts=this->input->GetPoints();

	int abort=0;
	updateInterval = numCells/100 + 1;
	outCD->CopyAllocate(inCD,numCells);

	// Do each of the verts, lines, polys, and strips separately
	// verts
	if ( !abort && this->input->GetVerts()->GetNumberOfCells() > 0 )
	{
		cells = this->input->GetVerts();
	}

	// lines
	if ( !abort && this->input->GetLines()->GetNumberOfCells() > 0 )
	{
		cells = this->input->GetLines();
	}
	
	// polys
	vtkCellArray *newPolys=NULL;
	if ( !abort && this->input->GetPolys()->GetNumberOfCells() > 0 )
	{
		cells = this->input->GetPolys();
		newId = this->output->GetNumberOfCells();
		newPolys = vtkCellArray::New();
		newPolys->EstimateSize(cells->GetNumberOfCells(),3);
		this->output->SetPolys(newPolys);
		vtkIdList *ptIds = vtkIdList::New();
		ptIds->Allocate(VTK_CELL_SIZE);
		int numSimplices;
		//vtkPolygon *poly=vtkPolygon::New();
		meVTKPolygon *poly=meVTKPolygon::New();
		vtkIdType triPts[3];
		std::cout<<"Triangulation Paper"<<std::endl;

		for (cells->InitTraversal(); cells->GetNextCell(npts,pts) && !abort; cellNum++)
		{
			
			if ( npts == 3 )
			{
				newPolys->InsertNextCell(3,pts);
				outCD->CopyData(inCD, cellNum, newId++);
			}
			else //triangulate polygon
			{  
				//initialize polygon
			       	poly->PointIds->SetNumberOfIds(npts);
				poly->Points->SetNumberOfPoints(npts);
				for (i=0; i<npts; i++)
				{
				
					poly->PointIds->SetId(i,pts[i]);
					poly->Points->SetPoint(i,inPts->GetPoint(pts[i]));
				
				}
				//poly->TriangulatePaper(ptIds);	
				poly->TriangulatePaper(ptIds,this->surrounding);
				numPts = ptIds->GetNumberOfIds();
				numSimplices = numPts / 3;
				for ( i=0; i < numSimplices; i++ )
				{
					for (j=0; j<3; j++)
					{
						triPts[j] = poly->PointIds->GetId(ptIds->GetId(3*i+j));
					}
					newPolys->InsertNextCell(3, triPts);
					outCD->CopyData(inCD, cellNum, newId++);
				}//for each simplex
			}//triangulate polygon
		}
		ptIds->Delete();
		poly->Delete();
		
	}

	if ( newPolys != NULL )
	{
		newPolys->Delete();
	}

	// Update output
	this->output->SetPoints(this->input->GetPoints());
	this->output->GetPointData()->PassData(this->input->GetPointData());
	this->output->Squeeze();

	
	//	return 1;
}


