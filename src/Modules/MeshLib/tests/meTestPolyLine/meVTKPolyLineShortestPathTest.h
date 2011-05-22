#ifndef MEVTKPOLYLINESHORTESTPATHTEST_H
#define MEVTKPOLYLINESHORTESTPATHTEST_H

#include <cxxtest/TestSuite.h>

#include <meVTKPolyLineShortestPath.h>

#include "CILabAssertMacros.h"

using namespace itk;

// VTK
#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkGenericCell.h"


class meVTKPolyLineShortestPathTest : public CxxTest::TestSuite 
{
public:
	void testIt( void )
	{	
		
		//regression test
		
		meVTKPolyLineShortestPath::Pointer centerline = meVTKPolyLineShortestPath::New(); 
		
		//the input file
		vtkPolyDataReader *polydataReader = vtkPolyDataReader::New();
		char *input_name = "visc_cl_TEST.vtk" ;
		polydataReader->SetFileName( input_name ); //input name
		polydataReader->Update();

		//the file to compare with at time 2
		vtkPolyDataReader *polydataReader0 = vtkPolyDataReader::New();
		char *input_name0 = "visc_cl_TESToutT0.vtk" ;
		polydataReader0->SetFileName( input_name0 ); //input name
		polydataReader0->Update();
		

		//cilabAssertMacro(this->polydataReader0 != NULL);
		//cilabAssertMacro(this->polydataReader != NULL);



		vtkPolyDataWriter *pdWriter = vtkPolyDataWriter::New();
		pdWriter->SetFileName( "visc_cl_TESTout.vtk" ); //output name

		vtkPolyData *input0; //the output test file at T=0
		vtkPolyData *input;	//the input file to read
		vtkPolyData *output;	//the output of the input file at T=today

		input0 = polydataReader0->GetOutput();
		input = polydataReader->GetOutput();

		//copy the points from the input file at time 0 (visc_cl_TESToutT0.vtk)
		vtkPoints *InputPoints0 = vtkPoints::New();	//represent and manipulate 3D points
		InputPoints0->DeepCopy(input0->GetPoints());
		
		//the cells to test. If this values are changes, the file at T=0 will not be te same!
		int i = 0;	
		int j = 25;

		//update the output
		centerline->SetInput(polydataReader->GetOutput());	//set the input file name
		//centerline->ComputeCenterline(i,j);
		centerline->Update();

		pdWriter->SetInput( centerline->GetOutput() );	//get the output
		pdWriter->Update();

		//the output points
		vtkPolyDataReader *polydataReaderOut = vtkPolyDataReader::New();
		char *output_name = "visc_cl_TESTout.vtk" ; //the output file at present
		polydataReaderOut->SetFileName( output_name ); //input name
		polydataReaderOut->Update();

		output = polydataReaderOut->GetOutput();
		//copy the points from the output
		vtkPoints *OutputPoints = vtkPoints::New();	
		OutputPoints->DeepCopy(output->GetPoints());
		vtkIdType NOutPts = OutputPoints->GetNumberOfPoints();

		//The points from the input at time 0 (visc_cl_TESToutT0.vtk)
		vtkIdType NOutPts0 = InputPoints0->GetNumberOfPoints();

		vtkGenericCell *cell1 = vtkGenericCell::New();
		
		input->GetCell(1, cell1);	//the cells to be combined

		TS_ASSERT_EQUALS(NOutPts,NOutPts0);
	
	}



};

 

#endif // MEVTKPOLYLINESHORTESTPATHTEST_H
