/*=========================================================================

Module    : GeoAPIDemos
File      : $RCSfile: RemoveInnerContour.cpp,v $
Copyright : (C)opyright (University of Zaragoza) 2005++
See COPYRIGHT statement in top level directory.
Authors   : Hrvoje Bogunovic
Modified  : $Author: vdialinos $
Purpose   : Cutting the aneurysm
Date      : $Date: 2009/04/22 $
Version   : $Revision: 1.1.1.1 $
Changes   : $Locker:  $
$Log: RemoveInnerContour.cpp,v $
Revision 1.1.1.1  Date: 2009/04/22  vdialinos
Import the new cilab compilation environment hopefully for each
library build on the cilab


=========================================================================*/

#include "geoRemoveInnerContourFilter.h"

#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkDecimatePro.h"
#include "vtkSmartPointer.h"

void usage()
{
	cerr << "Usage: RemoveInnerContour [inputContour] [outputContour] center_x center_y center_z direction_x direction_y direction_z"<<endl;
	exit(1);
}

int main( int argc, char **argv )
{    
	char *inputContour = NULL, *outputContour = NULL;

	if ( argc < 8 )
		usage();

	inputContour = argv[1];
	outputContour = argv[2];
	double center[3];
	double direction[3];
	center[0] = atof( argv[3] );
	center[1] = atof( argv[4] );
	center[2] = atof( argv[5] );
	direction[0] = atof( argv[6] );
	direction[1] = atof( argv[7] );
	direction[2] = atof( argv[8] );
	
	vtkSmartPointer< vtkPolyDataReader > polyDataReader = vtkSmartPointer< vtkPolyDataReader >::New();
	polyDataReader->SetFileName( inputContour );

	try
	{
		polyDataReader->Update();
	}
	catch (vtkstd::exception& e)
	{
		cout << "ExceptionObject caught !" << endl;
		cout << e.what() << endl;
		return -1;
	}

	//run filter
	cout << "start RemoveInnerContour filter" << endl;
	vtkSmartPointer< geoAPI::geoRemoveInnerContourFilter > removeInnerContourFilter = vtkSmartPointer< geoAPI::geoRemoveInnerContourFilter >::New();
	removeInnerContourFilter->SetInput(polyDataReader->GetOutput());
	removeInnerContourFilter->SetCenter(center);
	removeInnerContourFilter->SetDirection(direction);
	removeInnerContourFilter->Update();
	cout << "filter RemoveInnerContour end" << endl;

	//save output
	vtkSmartPointer< vtkPolyDataWriter > writer = vtkSmartPointer< vtkPolyDataWriter >::New();
	writer->SetInput(removeInnerContourFilter->GetOutput());
	writer->SetFileName(outputContour);
	writer->Write();
	writer->Update();

}
