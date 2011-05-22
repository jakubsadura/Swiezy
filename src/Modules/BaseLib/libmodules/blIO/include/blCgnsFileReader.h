/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef blCgnsFileReader_h
#define blCgnsFileReader_h

// basic includes
#include <vector>
#include <string>
#include <map>
using std::string;

// CGNSLIB
#include "cgnslib.h"

#include "BaseLibVTKWin32Header.h"
#include <blLightObject.h>

// VTK
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetAttributes.h"
#include "vtkSmartPointer.h"
#include "vtkCellType.h"
#include "vtkUnstructuredGrid.h"


/** 
Reader for CGNS files. For each time step, it reads the cgns grid into a vector of unstructured grids.
The flow solution is saved as vector and scalar point data. If you use compression, then only 
one time is generated, containing the grid of the first file (first time step), and multiple point 
data corresponding to each time step.

\author Albert Sanchez
\date 04 May 2010
*/


class BASELIBVTK_EXPORT blCgnsFileReader : public blLightObject
{
public:
	struct FourStringVectors
	{
		std::vector<std::string> ScalarPoint;
		std::vector<std::string> VectorPoint;
		std::vector<std::string> ScalarCell;
		std::vector<std::string> VectorCell;
	};
public:
	//! Typedefs
	typedef blCgnsFileReader Self;
	typedef blSmartPointer< Self > Pointer;

	//! Static constructor.
	blNewMacro( Self );

	//! Set the filenames of each time step
	void SetFilenames(const std::vector<std::string> &fileNames);

	//! Returns the scalars (first) and vectors (second) present in the already specified files
	FourStringVectors ReadVariables();

	//! Reads all the scalars (first) and vectors (second) present in the already specified files and it sets them as the ones to be added
	void ReadAll();

	//! Sets the selected scalars (first) and vectors (second)
	void WriteVariables(std::pair<std::vector<std::string>,std::vector<std::string> > &variables);

	//! Read the data from the already specified files
	void Update();

	//! Get an unstructured grid for every time step. The flow solution is saved as vector and scalar point data.
	std::vector<vtkSmartPointer<vtkUnstructuredGrid> > GetOutput();

	//! Set the scalar point data you want to read
	void SetScalarsListPoint(std::vector<std::string> &scalarsNamesToAdd);
	//! Get the scalar point data names
	std::vector<std::string> GetScalarsListPoint();

	//! Set the vector point data you want to read
	void SetVectorsListPoint(std::vector<std::string> &vectorsNamesToAdd);
	//! Get the vector point data names
	std::vector<std::string> GetVectorsListPoint();

	//! Set the scalar cell data you want to read
	void SetScalarsListCell(std::vector<std::string> &scalarsNamesToAdd);
	//! Get the scalar cell data names
	std::vector<std::string> GetScalarsListCell();

	//! Set the vector cell data you want to read
	void SetVectorsListCell(std::vector<std::string> &vectorsNamesToAdd);
	//! Get the vector cell data names
	std::vector<std::string> GetVectorsListCell();


	//! Compress means that only one time step is generated with multiple point data (corresponding to each time step)
	void setCompress(bool onOff);
	//! 
	bool GetCompress();

	//!
	int GetTimeSteps();


public:
	//!
	typedef float Node[3];

	//! 
	typedef struct {
		int cnt;
		char name[33];
	} Variable;

protected:
	//!
	blCgnsFileReader();
	//!
	virtual ~blCgnsFileReader();

private:
	//! Purposely not implemented
	blCgnsFileReader( const blCgnsFileReader& );	

	//! Purposely not implemented
	void operator = ( const blCgnsFileReader& );	

	void write_solution (int nz, int *mask, int cgnsfn, int cgnsbase, vtkSmartPointer<vtkUnstructuredGrid> volume);
	int get_variables (int nz, ZoneType_t zonetype, int *sizes, int cgnsfn, int cgnsbase, int CellDim);
	void write_volume_cells(int nz, int cgnsfn, int cgnsbase, vtkSmartPointer<vtkUnstructuredGrid> volume);


private:
	Node *nodes;
	GridLocation_t varloc;

	Variable *vars;
	int nvars;
	int cgnssol;
	std::map<std::string,int> m_StringToVarLocType;
	std::map<std::string,int> m_VarNameToNumSol;

	int varrng[2][3];
	int ndata;

	int m_NCells;
	int m_NPoints;

	std::vector<std::string> m_filenames;
	std::vector<std::string> m_scalarsNamesPoint;
	std::vector<std::string> m_vectorsNamesPoint;
	std::vector<std::string> m_scalarsNamesCell;
	std::vector<std::string> m_vectorsNamesCell;
	bool m_compress;
	int m_currentStep;

	int m_TimeSteps;

	std::vector<vtkSmartPointer<vtkUnstructuredGrid> > m_VTKOutput;

};

#endif // blCgnsFileReader_h
