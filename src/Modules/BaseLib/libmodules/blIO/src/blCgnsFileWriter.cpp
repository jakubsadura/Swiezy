/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blCgnsFileWriter.h"

#include "vtkDoubleArray.h"
#include "vtkPoints.h"
#include "vtkCellType.h"
#include "vtkIdList.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetAttributes.h"
#include "vtkIdTypeArray.h"
#include "vtkSmartPointer.h"
#include "vtkGeometryFilter.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkCellArray.h"
#include <vtkGenericCell.h>

#include <algorithm>

using namespace std;


blCgnsFileWriter::blCgnsFileWriter()
{
	m_gridWithFlowSolution = vtkSmartPointer<vtkUnstructuredGrid>::New();
	m_scalarsNamesToAddPoint = std::vector<std::string>(0);
	m_vectorsNamesToAddPoint = std::vector<std::string>(0);
	m_scalarsNamesToAddCell = std::vector<std::string>(0);
	m_vectorsNamesToAddCell = std::vector<std::string>(0);
}

blCgnsFileWriter::~blCgnsFileWriter() 
{
	m_gridWithFlowSolution = NULL;
}

void blCgnsFileWriter::SetFilename(const std::string &fileName)
{
	m_fileName = fileName;
}

void blCgnsFileWriter::SetVTKData(vtkSmartPointer<vtkUnstructuredGrid> gridWithFlowSolution)
{
	m_gridWithFlowSolution->DeepCopy(gridWithFlowSolution);
}

void blCgnsFileWriter::SetScalarsListPoint(std::vector<std::string> &scalarsNamesToAdd)
{
	m_scalarsNamesToAddPoint = scalarsNamesToAdd;
}


std::vector<std::string> blCgnsFileWriter::GetScalarsListPoint()
{
	return m_scalarsNamesToAddPoint;
}


void blCgnsFileWriter::SetVectorsListPoint(std::vector<std::string> &vectorsNamesToAdd)
{
	m_vectorsNamesToAddPoint = vectorsNamesToAdd;
}


std::vector<std::string> blCgnsFileWriter::GetVectorsListPoint()
{
	return m_vectorsNamesToAddPoint;
}

void blCgnsFileWriter::SetScalarsListCell(std::vector<std::string> &scalarsNamesToAdd)
{
	m_scalarsNamesToAddCell = scalarsNamesToAdd;
}


std::vector<std::string> blCgnsFileWriter::GetScalarsListCell()
{
	return m_scalarsNamesToAddCell;
}


void blCgnsFileWriter::SetVectorsListCell(std::vector<std::string> &vectorsNamesToAdd)
{
	m_vectorsNamesToAddCell = vectorsNamesToAdd;
}


std::vector<std::string> blCgnsFileWriter::GetVectorsListCell()
{
	return m_vectorsNamesToAddCell;
}



std::string blCgnsFileWriter::translateFieldName(std::string input, int n, int i)
{
	if ( n != 1 && n != 3 )
	{
		return input;
	}
	
	if ( n == 1 )
	{
		std::string::iterator itPos = find (input.begin(),input.end(),'_');
		std::string::iterator it;

		if ( itPos != input.end() ) //found '_'
		{
			std::string ret = "";

			for ( it = input.begin(); it != itPos; ++it ) 
			{
				ret += (*it);
			}

			return ret;
		}
		else
		{
			return input;
		}
	}
	else if ( n == 3 )
	{
		std::string::iterator itPos = find (input.begin(),input.end(),'_');
		std::string::iterator it;

		if ( itPos != input.end() ) //found '_'
		{
			std::string ret = "";

			for ( it = input.begin(); it != itPos; ++it ) 
			{
				ret += (*it);
			}
			
			if ( i == 0 )
			{
				ret += "X";
			}

			if ( i == 1 )
			{
				ret += "Y";
			}

			if ( i == 2 )
			{
				ret += "Z";
			}

			return ret;
		}
		else
		{
			return input;
		}
	}
}


void blCgnsFileWriter::Update() 
{
	int cgnsfn;
	int ibase;
	int izone;
	int ic;
	int isizes[9];
	int cell_dim = 3;
	int phys_dim = 3;
	std::string basename = "Base";
	std::string zonename = "Zone";
	std::string sectionname = "Section";
	int npoints;
	int ncells;
	int cellssize;
	int sectionIndex;
	double * x = NULL;
	double * y = NULL;
	double * z = NULL;
	int * conn = NULL;
	double *solution_array = NULL;
	double *solution_arrayX = NULL;
	double *solution_arrayY = NULL;
	double *solution_arrayZ = NULL;

	cg_open ( m_fileName.c_str(), CG_MODE_WRITE, &cgnsfn );
	cg_base_write ( cgnsfn,basename.c_str(), cell_dim, phys_dim, &ibase );

	vtkPoints * points = m_gridWithFlowSolution->GetPoints ( );
	vtkCellArray * cells = m_gridWithFlowSolution->GetCells ( );
	npoints = points->GetNumberOfPoints ( );
	ncells = cells->GetNumberOfCells ( );

	x = (double *) malloc ( npoints * sizeof(double) );
	y = (double *) malloc ( npoints * sizeof(double) );
	z = (double *) malloc ( npoints * sizeof(double) );

	isizes[0] = npoints;
	isizes[1] = ncells;
	isizes[2] = 0;

	for ( int i = 0; i < npoints; i++ ) 
	{
		double * v = (double *)points->GetPoint ( i );
		x[i] = v[0];// / 1000.0;
		y[i] = v[1];// / 1000.0;
		z[i] = v[2];// / 1000.0;
	}

	cg_zone_write(
		cgnsfn, 
		ibase, 
		zonename.c_str ( ), 
		isizes, 
		Unstructured, 
		&izone);

	//write points
	cg_coord_write(cgnsfn,ibase,izone,RealDouble,"CoordinateX",x,&ic);
	cg_coord_write(cgnsfn,ibase,izone,RealDouble,"CoordinateY",y,&ic);
	cg_coord_write(cgnsfn,ibase,izone,RealDouble,"CoordinateZ",z,&ic);

	if (x) free (x);
	if (y) free (y);
	if (z) free (z);

	//write cells
	vtkSmartPointer<vtkGenericCell> cell = vtkGenericCell::New();
	int nIds = 0;

	if (ncells>0)
	{
		m_gridWithFlowSolution->GetCell(0,cell);
		nIds = cell->GetPointIds()->GetNumberOfIds();
	}

	cellssize = ncells*nIds;

	conn = (int *) malloc (cellssize * sizeof(int));

	if (conn == (int *) NULL) 
	{
		std::cout << "Error: Out of memory. Try using 64-bit Gimias release." << std::endl;
	}

	for ( int i = 0; i < ncells; i++ ) 
	{
		m_gridWithFlowSolution->GetCell(i,cell);
		vtkIdList* cellIdList = cell->GetPointIds();
		nIds = cellIdList->GetNumberOfIds();

		for ( int j = 0; j < nIds; j++ )
		{
			conn[i*nIds+j]= cellIdList->GetId(j) + 1;
		}
	}

	//PATCH for element connectivity type:
	ElementType_t connType = (ElementType_t)NofValidElementTypes;
	if (nIds == 4)
	{
		connType = TETRA_4;
		std::cout << "Writing TETRA_4" << std::endl;
	}
	else if (nIds == 8)
	{
		connType = HEXA_8;
		std::cout << "Writing HEXA_8" << std::endl;
	}

	cg_section_write(
					cgnsfn, 
					ibase, 
					izone, 
					sectionname.c_str(),
					connType,
					1, 
					ncells, 
					0,
					conn, 
					&sectionIndex);

	if (conn) free(conn);

	//write flow solution
	int SS;
	cg_sol_write(cgnsfn,ibase,izone,"Flow Solution Point",Vertex,&SS);

	int nA = m_gridWithFlowSolution->GetPointData( )->GetNumberOfArrays( );

	for ( int i = 0; i < nA; i++ ) 
	{
		std::string str( m_gridWithFlowSolution->GetPointData( )->GetArrayName( i ) );
		vtkDataArray * dataArray;
		dataArray = m_gridWithFlowSolution->GetPointData( )->GetArray( str.c_str( ) );

		int f;

		if ( dataArray != NULL ) 
		{
			int nTuples = dataArray->GetNumberOfTuples( );
			int nComponents = dataArray->GetNumberOfComponents( ); 

			if ( nComponents == 1 ) 
			{
				if ( std::find(m_scalarsNamesToAddPoint.begin(),m_scalarsNamesToAddPoint.end(),str) == m_scalarsNamesToAddPoint.end() ) //scalar not selected
				{
					continue;
				}

				solution_array = (double *) malloc (npoints * sizeof(double));
				if (solution_array == (double *) NULL) 
				{
					std::cout << "Error: solution_array Out of memory. Try using 64-bit Gimias release." << std::endl;
				}

				for ( int j = 0; j < nTuples; j++ ) 
				{
					double * tuple = (double *)dataArray->GetTuple( j );
					double val = tuple[0];
					solution_array[j] = val;
				}
			}
			else if ( nComponents == 3 )
			{
				if ( std::find(m_vectorsNamesToAddPoint.begin(),m_vectorsNamesToAddPoint.end(),str) == m_vectorsNamesToAddPoint.end() ) //vector not selected
				{
					continue;
				}

				solution_arrayX = (double *) malloc (npoints * sizeof(double));
				if (solution_arrayX == (double *) NULL) 
				{
					std::cout << "Error: solution_arrayX Out of memory. Try using 64-bit Gimias release." << std::endl;
				}
				solution_arrayY = (double *) malloc (npoints * sizeof(double));
				if (solution_arrayY == (double *) NULL) 
				{
					std::cout << "Error: solution_arrayY Out of memory. Try using 64-bit Gimias release." << std::endl;
				}
				solution_arrayZ = (double *) malloc (npoints * sizeof(double));
				if (solution_arrayZ == (double *) NULL) 
				{
					std::cout << "Error: solution_arrayZ Out of memory. Try using 64-bit Gimias release." << std::endl;
				}

				for ( int j = 0; j < nTuples; j++ ) 
				{
					double * tuple = (double *)dataArray->GetTuple( j );
					double val0 = tuple[0];
					double val1 = tuple[1];
					double val2 = tuple[2];

					solution_arrayX[j] = val0;
					solution_arrayY[j] = val1;
					solution_arrayZ[j] = val2;
				}
			}

			if ( nComponents == 1 )
			{
				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SS,
					RealDouble,
					translateFieldName(str,nComponents,0).c_str(), 
					solution_array, 
					&f);

				if (solution_array) {
					free(solution_array);
				}
			}
			else if ( nComponents == 3 )
			{
				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SS,
					RealDouble,
					translateFieldName(str,nComponents,0).c_str(), 
					solution_arrayX, 
					&f);

				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SS,
					RealDouble,
					translateFieldName(str,nComponents,1).c_str(), 
					solution_arrayY, 
					&f);

				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SS,
					RealDouble,
					translateFieldName(str,nComponents,2).c_str(), 
					solution_arrayZ, 
					&f);

				if (solution_arrayX) 
				{
					free(solution_arrayX);
				}

				if (solution_arrayY)
				{
					free(solution_arrayY);
				}

				if (solution_arrayZ) 
				{
					free(solution_arrayZ);
				}
			}
		}
	}

	int SCell;

	cg_sol_write(cgnsfn,ibase,izone,"Flow Solution Cell",CellCenter,&SCell);

	int nB = m_gridWithFlowSolution->GetCellData( )->GetNumberOfArrays( );

	for ( int i = 0; i < nB; i++ ) 
	{
		std::string str( m_gridWithFlowSolution->GetCellData( )->GetArrayName( i ) );
		vtkDataArray * dataArray;
		dataArray = m_gridWithFlowSolution->GetCellData( )->GetArray( str.c_str( ) );

		int f;

		if ( dataArray != NULL ) 
		{
			int nTuples = dataArray->GetNumberOfTuples( );
			int nComponents = dataArray->GetNumberOfComponents( ); 

			if ( nComponents == 1 ) 
			{
				if ( std::find(m_scalarsNamesToAddCell.begin(),m_scalarsNamesToAddCell.end(),str) == m_scalarsNamesToAddCell.end() ) //scalar not selected
				{
					continue;
				}

				solution_array = (double *) malloc (npoints * sizeof(double));
				if (solution_array == (double *) NULL) 
				{
					std::cout << "Error: solution_array Out of memory. Try using 64-bit Gimias release." << std::endl;
				}

				for ( int j = 0; j < nTuples; j++ ) 
				{
					double * tuple = (double *)dataArray->GetTuple( j );
					double val = tuple[0];
					solution_array[j] = val;
				}
			}
			else if ( nComponents == 3 )
			{
				if ( std::find(m_vectorsNamesToAddCell.begin(),m_vectorsNamesToAddCell.end(),str) == m_vectorsNamesToAddCell.end() ) //vector not selected
				{
					continue;
				}

				solution_arrayX = (double *) malloc (npoints * sizeof(double));
				if (solution_arrayX == (double *) NULL) 
				{
					std::cout << "Error: solution_arrayX Out of memory. Try using 64-bit Gimias release." << std::endl;
				}
				solution_arrayY = (double *) malloc (npoints * sizeof(double));
				if (solution_arrayY == (double *) NULL) 
				{
					std::cout << "Error: solution_arrayY Out of memory. Try using 64-bit Gimias release." << std::endl;
				}
				solution_arrayZ = (double *) malloc (npoints * sizeof(double));
				if (solution_arrayZ == (double *) NULL) 
				{
					std::cout << "Error: solution_arrayZ Out of memory. Try using 64-bit Gimias release." << std::endl;
				}

				for ( int j = 0; j < nTuples; j++ ) 
				{
					double * tuple = (double *)dataArray->GetTuple( j );
					double val0 = tuple[0];
					double val1 = tuple[1];
					double val2 = tuple[2];

					solution_arrayX[j] = val0;
					solution_arrayY[j] = val1;
					solution_arrayZ[j] = val2;
				}
			}

			if ( nComponents == 1 )
			{
				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SCell,
					RealDouble,
					translateFieldName(str,nComponents,0).c_str(), 
					solution_array, 
					&f);

				if (solution_array) {
					free(solution_array);
				}
			}
			else if ( nComponents == 3 )
			{
				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SCell,
					RealDouble,
					translateFieldName(str,nComponents,0).c_str(), 
					solution_arrayX, 
					&f);

				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SCell,
					RealDouble,
					translateFieldName(str,nComponents,1).c_str(), 
					solution_arrayY, 
					&f);

				cg_field_write(
					cgnsfn, 
					ibase, 
					izone, 
					SCell,
					RealDouble,
					translateFieldName(str,nComponents,2).c_str(), 
					solution_arrayZ, 
					&f);

				if (solution_arrayX) 
				{
					free(solution_arrayX);
				}

				if (solution_arrayY)
				{
					free(solution_arrayY);
				}

				if (solution_arrayZ) 
				{
					free(solution_arrayZ);
				}
			}
		}
	}

	cg_close (cgnsfn);
}
