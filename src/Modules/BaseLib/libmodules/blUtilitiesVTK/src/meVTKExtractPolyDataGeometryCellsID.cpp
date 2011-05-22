/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include "meVTKExtractPolyDataGeometryCellsID.h"

	#include "vtkCellArray.h"
	#include "vtkCellData.h"
	#include "vtkFloatArray.h"
	#include "vtkImplicitFunction.h"
	#include "vtkInformation.h"
	#include "vtkInformationVector.h"
	#include "vtkObjectFactory.h"
	#include "vtkPointData.h"
	#include "vtkPolyData.h"

	vtkCxxRevisionMacro(meVTKExtractPolyDataGeometryCellsID, "$Revision: 1.1 $");
	vtkStandardNewMacro(meVTKExtractPolyDataGeometryCellsID);
	vtkCxxSetObjectMacro(meVTKExtractPolyDataGeometryCellsID,
						ImplicitFunction,vtkImplicitFunction);




	//---------------------------------------------------
	// OPERATIONS
	//---------------------------------------------------


/**
 * Construct object with ExtractInside turned on.
 */
meVTKExtractPolyDataGeometryCellsID::meVTKExtractPolyDataGeometryCellsID(vtkImplicitFunction *f)
{
  this->ImplicitFunction = f;
  if (this->ImplicitFunction)
    {
    this->ImplicitFunction->Register(this);
    }
    
  this->ExtractInside = 1;
  this->ExtractBoundaryCells = 0;
  this->ArrayCellsId = NULL;

  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(0);

}

meVTKExtractPolyDataGeometryCellsID::~meVTKExtractPolyDataGeometryCellsID()
{
  if ( this->ArrayCellsId != NULL )
  {
	  this->ArrayCellsId->Delete( );
	  this->ArrayCellsId = NULL;
  }
  this->SetImplicitFunction(NULL);
}

/**
 * Overload standard modified time function. If implicit function is modified,
 * then this object is modified as well.
 */
unsigned long meVTKExtractPolyDataGeometryCellsID::GetMTime()
{
  unsigned long mTime=this->MTime.GetMTime();
  unsigned long impFuncMTime;

  if ( this->ImplicitFunction != NULL )
    {
    impFuncMTime = this->ImplicitFunction->GetMTime();
    mTime = ( impFuncMTime > mTime ? impFuncMTime : mTime );
    }

  return mTime;
}

int meVTKExtractPolyDataGeometryCellsID::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{

  // -------------------------------------------  
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);

  // get the input and ouptut
  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkPointData *pd = input->GetPointData();
  vtkCellData *cd = input->GetCellData();
  vtkPoints *inPts=input->GetPoints();
  vtkIdType numPts, i, cellId = -1;
  float multiplier;
  vtkCellArray *inPolys=NULL;

  vtkDebugMacro(<< "Extracting poly data geometry");

  if ( ! this->ImplicitFunction )
    {
    vtkErrorMacro(<<"No implicit function specified");
    return 1;
    }

  numPts = input->GetNumberOfPoints();

  if ( this->ExtractInside )
    {
    multiplier = 1.0;
    }
  else 
    {
    multiplier = -1.0;
    }



  // -------------------------------------------  
  // Use a templated function to access the points. The points are
  // passed through, but scalar values are generated.
  vtkFloatArray *newScalars = vtkFloatArray::New();
  newScalars->SetNumberOfValues(numPts);

  for (int ptId=0; ptId < numPts; ptId++ )
    {
    newScalars->SetValue(ptId, this->ImplicitFunction->
                         FunctionValue(inPts->GetPoint(ptId))*multiplier);
    }



  // -------------------------------------------  
  // Now loop over all cells to see whether they are inside the implicit
  // function. Copy if they are. Note: there is an awful hack here, that
  // can result in bugs. The cellId is assumed to be arranged starting
  // with the verts, then lines, then polys, then strips.
  //
  int numIn;
  vtkIdType npts = 0;
  vtkIdType *pts = 0;
  if ( input->GetNumberOfPolys() )
  {
    inPolys = input->GetPolys();
	if ( this->ArrayCellsId != NULL )
	{
		this->ArrayCellsId->Delete( );
		this->ArrayCellsId = NULL;
	}
	this->ArrayCellsId = vtkIdList::New( );
  }

  


  // -------------------------------------------  
  // polys
  // -------------------------------------------  

  int iNumSelectedCells = 0;
  cellId = 0;
  if ( inPolys && !this->GetAbortExecute() )
    {
    for (inPolys->InitTraversal(); inPolys->GetNextCell(npts,pts); )
      {
      for (numIn=0, i=0; i<npts; i++)
        {
        if ( newScalars->GetValue(pts[i]) <= 0.0 )
          {
          numIn++;
          }
        }
      if ( (numIn == npts) || (this->ExtractBoundaryCells && numIn > 0) )
        {
			this->ArrayCellsId->InsertNextId( cellId );
        }
      cellId++;
      }
    }

  this->UpdateProgress (0.90);
  
  

  return 1;
}

void meVTKExtractPolyDataGeometryCellsID::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  if (this->ImplicitFunction)
    {
    os << indent << "Implicit Function: " 
       << (void *)this->ImplicitFunction << "\n";
    }
  else
    {
    os << indent << "Implicit Function: (null)\n";      
    }
  os << indent << "Extract Inside: " 
     << (this->ExtractInside ? "On\n" : "Off\n");
  os << indent << "Extract Boundary Cells: " 
     << (this->ExtractBoundaryCells ? "On\n" : "Off\n");
}
