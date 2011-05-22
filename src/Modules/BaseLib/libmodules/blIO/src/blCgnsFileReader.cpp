/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "blCgnsFileReader.h"

#include "blTextUtils.h"

#include "vtkDoubleArray.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkCellData.h"
#include "vtkPointData.h"

using namespace std;


int sort_variables (const void *v1, const void *v2)
{
	blCgnsFileReader::Variable *var1 = (blCgnsFileReader::Variable *)v1;
	blCgnsFileReader::Variable *var2 = (blCgnsFileReader::Variable *)v2;

	return (strcmp (var1->name, var2->name));
}


blCgnsFileReader::blCgnsFileReader()
{
	cgnssol = 1;
}

blCgnsFileReader::~blCgnsFileReader() 
{
}

void blCgnsFileReader::SetFilenames(const std::vector<std::string> &fileNames) 
{
	m_filenames = fileNames;
}


void blCgnsFileReader::SetScalarsListPoint(std::vector<std::string> &scalarsNamesToAdd) 
{
	m_scalarsNamesPoint = scalarsNamesToAdd;
}

std::vector<std::string> blCgnsFileReader::GetScalarsListPoint() 
{
	return m_scalarsNamesPoint;
}

void blCgnsFileReader::SetVectorsListPoint(std::vector<std::string> &vectorsNamesToAdd)
{
	m_vectorsNamesPoint = vectorsNamesToAdd;
}

std::vector<std::string> blCgnsFileReader::GetVectorsListPoint() 
{
	return m_vectorsNamesPoint;
}



void blCgnsFileReader::SetScalarsListCell(std::vector<std::string> &scalarsNamesToAdd) 
{
	m_scalarsNamesCell = scalarsNamesToAdd;
}

std::vector<std::string> blCgnsFileReader::GetScalarsListCell() 
{
	return m_scalarsNamesCell;
}

void blCgnsFileReader::SetVectorsListCell(std::vector<std::string> &vectorsNamesToAdd)
{
	m_vectorsNamesCell = vectorsNamesToAdd;
}

std::vector<std::string> blCgnsFileReader::GetVectorsListCell() 
{
	return m_vectorsNamesCell;
}


void blCgnsFileReader::setCompress(bool onOff) 
{
	m_compress = onOff;
}

bool blCgnsFileReader::GetCompress() 
{
	return m_compress;
}

std::vector<vtkSmartPointer<vtkUnstructuredGrid> > blCgnsFileReader::GetOutput() 
{
	return m_VTKOutput;
}

blCgnsFileReader::FourStringVectors blCgnsFileReader::ReadVariables() 
{
	std::string stdStr = m_filenames[0];
	int cgnszone = 0;
	int cgnsbase = 1;
	char name[33];
	int CellDim, PhyDim;
	int nz = 1;
	int cgnsfn;
	int len, nv, nsols;
	DataType_t datatype;

	std::vector<std::string>  retScalarsPoint(0);
	std::vector<std::string>  retVectorsPoint(0);	
	std::vector<std::string>  retScalarsCell(0);
	std::vector<std::string>  retVectorsCell(0);

	// open file
	cg_open(stdStr.c_str(), CG_MODE_READ, &cgnsfn);

	// get info
	cg_base_read (cgnsfn, cgnsbase, name, &CellDim, &PhyDim);

	cg_nsols (cgnsfn, cgnsbase, nz, &nsols);

	nvars = 0;

	std::map<int,int> SolTOVarLocType;

	for (int i=1; i<=nsols; i++)
	{
		cg_sol_info (cgnsfn, cgnsbase, nz, i, name, &varloc);
		
		SolTOVarLocType[i] = varloc;

		cg_nfields (cgnsfn, cgnsbase, nz, i, &nv);
		
		nvars += nv;
	}

	// get variable names

	vars = (Variable *) malloc (nvars * sizeof(Variable));

	int startIndex = 0;
	m_StringToVarLocType.clear();

	m_VarNameToNumSol.clear();
	
	for (int i=1; i<=nsols; i++)
	{
		int nVV;
		cg_nfields (cgnsfn, cgnsbase, nz, i, &nVV);

		for (int j=1; j<=nVV; j++) {
			cg_field_info (cgnsfn, cgnsbase, nz, i, j,&datatype, name);
			vars[startIndex+j-1].cnt = 1;
			strcpy (vars[startIndex+j-1].name, name);

			m_VarNameToNumSol[name] = i;

			m_StringToVarLocType[std::string(name)] = SolTOVarLocType[i];
		}

		startIndex += nVV;
	}


	qsort (vars, nvars, sizeof(Variable), sort_variables);

	/* get number of scalars and vectors */

	for (nv = 2; nv < nvars; nv++) {
		len = strlen(vars[nv].name) - 1;
		if (vars[nv].name[len] == 'Z') {
			strcpy (name, vars[nv].name);
			name[len] = 'Y';
			if (0 == strcmp (name, vars[nv-1].name)) {
				name[len] = 'X';
				if (0 == strcmp (name, vars[nv-2].name)) {
					vars[nv-2].cnt = 3;
					vars[nv-1].cnt = 0;
					vars[nv].cnt = 0;
				}
			}
		}
	}

	cg_close (cgnsfn);
	
	
	for (nv = 0; nv < nvars; nv++) 
	{
		if ( m_StringToVarLocType[std::string(vars[nv].name)] == Vertex )
		{
			if ( vars[nv].cnt == 1 )
			{
				retScalarsPoint.push_back( std::string(vars[nv].name) );
			}
			else if ( vars[nv].cnt == 3 )
			{
				retVectorsPoint.push_back( std::string(vars[nv].name) );
			}
		}
		else if ( m_StringToVarLocType[std::string(vars[nv].name)] == CellCenter )
		{
			if ( vars[nv].cnt == 1 )
			{
				retScalarsCell.push_back( std::string(vars[nv].name) );
			}
			else if ( vars[nv].cnt == 3 )
			{
				retVectorsCell.push_back( std::string(vars[nv].name) );
			}
		}
	}

	FourStringVectors ret;
	ret.ScalarPoint = retScalarsPoint;
	ret.VectorPoint = retVectorsPoint;
	ret.ScalarCell = retScalarsCell;
	ret.VectorCell = retVectorsCell;

	return ret;
	
}

void blCgnsFileReader::WriteVariables(std::pair<std::vector<std::string>,std::vector<std::string> > &variables) 
{
	SetScalarsListPoint(variables.first);
	SetVectorsListPoint(variables.second);
}



void blCgnsFileReader::Update()
{
	
	for (int i=0; i<m_scalarsNamesPoint.size(); i++)
	{
		m_StringToVarLocType[m_scalarsNamesPoint[i]] = Vertex;
	}
	for (int i=0; i<m_scalarsNamesCell.size(); i++)
	{
		m_StringToVarLocType[m_scalarsNamesCell[i]] = CellCenter;
	}
	for (int i=0; i<m_vectorsNamesPoint.size(); i++)
	{
		m_StringToVarLocType[m_vectorsNamesPoint[i]] = Vertex;
	}
	for (int i=0; i<m_vectorsNamesCell.size(); i++)
	{
		m_StringToVarLocType[m_vectorsNamesCell[i]] = CellCenter;
	}

	m_VTKOutput = std::vector<vtkSmartPointer<vtkUnstructuredGrid> >(0);

	vtkSmartPointer<vtkUnstructuredGrid> volume;

	m_currentStep = 0;

	for (std::vector<std::string>::const_iterator it = m_filenames.begin(); it != m_filenames.end(); it++) 
	{
		m_currentStep++;

		if ( m_currentStep <= 1 ) 
		{
			volume = vtkSmartPointer<vtkUnstructuredGrid>::New();
		} 
		else if ( !m_compress ) 
		{
			volume = NULL;
			volume = vtkSmartPointer<vtkUnstructuredGrid>::New();
		} 
		else // m_currentStep > 1 and m_compress
		{
			//DO NOTHING: we must use the same volume
		}

		std::string stdStr = (*it);
		int cgnszone = 0;
		int cgnsbase = 1;

		int cgnsfn;

		if (cg_open (stdStr.c_str(), CG_MODE_READ, &cgnsfn)) {
			std::cout << "CGNS ERROR: open. " << stdStr << std::endl;
			return;
		}

		char name[33];
		int CellDim, PhyDim;


		if (cg_base_read (cgnsfn, cgnsbase, name, &CellDim, &PhyDim)) {
			std::cout << "CGNS ERROR: base read." << std::endl;
			return;
		}

		if (PhyDim != 3) {
			std::cout << "CGNS ERROR: invalid physical dimension: " << PhyDim << std::endl;
			return;
		}

		int nzones;

		if (cg_nzones (cgnsfn, cgnsbase, &nzones)) {
			std::cout << "CGNS ERROR: nzones." << std::endl;
			return;
		}

		if (nzones == 0) {
			std::cout << "CGNS ERROR: no zones in the CGNS file."<< std::endl;
			return;
		}

		if (cgnszone && cgnszone > nzones) {
			std::cout << "CGNS ERROR: zone number invalid."<< std::endl;
			return;
		}

		int nz = 1;

		ZoneType_t zonetype;
		int sizes[9];

		if (cg_zone_type (cgnsfn, cgnsbase, nz, &zonetype) || cg_zone_read (cgnsfn, cgnsbase, nz, name, sizes)) {
			std::cout << "CGNS ERROR: zone type or zone read."<< std::endl;
			return;
		}

		if (zonetype != Structured && zonetype != Unstructured) {
			std::cout << "CGNS ERROR: invalid zone type: !=Structured && !=Unstructured"<< std::endl;
			return;
		}

		int ncoords;

		if (cg_ncoords (cgnsfn, cgnsbase, nz, &ncoords)) {
			std::cout << "CGNS ERROR: ncoords."<< std::endl;
			return;
		}

		if (ncoords < PhyDim) {
			std::cout << "CGNS ERROR: less than PhyDim coordinates."<< std::endl;
			return;
		}

		if ( !m_compress || m_currentStep <=1 ) 
		{

			/* check for rind */

			if (cg_goto (cgnsfn, cgnsbase, "Zone_t", nz,"GridCoordinates_t", 1, "end")) {
				std::cout << "CGNS ERROR: goto."<< std::endl;
				return;
			}

			int rind[6];
			int i, j, n;

			if ((i = cg_rind_read (rind)) != CG_OK) {
				if (i != CG_NODE_NOT_FOUND) {
					std::cout << "CGNS ERROR: rind_read: " << i << std::endl;
					return;
				}

				for (n = 0; n < 6; n++)
					rind[n] = 0;
			}

			/* get grid coordinate range */

			int rng[2][3];
			int nnodes;

			if (zonetype == Structured) {
				for (n = 0; n < 3; n++) {
					rng[0][n] = 1;
					rng[1][n] = 1;
				}
				nnodes = 1;
				for (n = 0; n < CellDim; n++) {
					rng[0][n] = rind[2*n] + 1;
					rng[1][n] = rind[2*n] + sizes[n];
					nnodes *= sizes[n];
				}
			}
			else {
				nnodes = sizes[0] + rind[0] + rind[1];
				rng[0][0] = 1;
				rng[1][0] = nnodes;
			}

			/* read the nodes */
			float *xyz;

			xyz = (float *) malloc (nnodes * sizeof(float));
			nodes = (Node *) malloc (nnodes * sizeof(Node));

			if (xyz == NULL || nodes == NULL) {
				std::cout << "CGNS ERROR: malloc failed for nodes."<< std::endl;
				return;
			}

			char coordtype[4];

			for (i = 1; i <= ncoords; i++) {
				DataType_t datatype;
				if (cg_coord_info (cgnsfn, cgnsbase, nz, i, &datatype, name) ||
					cg_coord_read (cgnsfn, cgnsbase, nz, name,
					RealSingle, rng[0], rng[1], xyz)) {
						std::cout << "CGNS ERROR: coord_info or coord_read."<< std::endl;
						return;
				}
				if (0 == strcmp (name, "CoordinateX") ||
					0 == strcmp (name, "CoordinateR")) {
						j = 0;
				}
				else if (0 == strcmp (name, "CoordinateY") ||
					0 == strcmp (name, "CoordinateTheta")) {
						j = 1;
				}
				else if (0 == strcmp (name, "CoordinateZ") ||
					0 == strcmp (name, "CoordinatePhi")) {
						j = 2;
				}
				else {
					continue;
				}
				if (coordtype[j] == ' ' || strchr ("XYZ", name[10]) != NULL) {
					coordtype[j] = name[10];
				}

				for (n = 0; n < nnodes; n++) {
					nodes[n][j] = xyz[n];
				}
			}

			free (xyz);

			/* change coordinate system to cartesian */

			double rad, theta, phi;

			if (0 == strncmp (coordtype, "RTZ", PhyDim)) {
				for (n = 0; n < nnodes; n++) {
					rad   = nodes[n][0];
					theta = nodes[n][1];
					nodes[n][0] = (float)(rad * cos (theta));
					nodes[n][1] = (float)(rad * sin (theta));
				}
			}
			else if (0 == strcmp (coordtype, "RTP")) {
				for (n = 0; n < nnodes; n++) {
					rad   = nodes[n][0];
					theta = nodes[n][1];
					phi   = nodes[n][2];
					nodes[n][0] = (float)(rad * sin (theta) * cos (phi));
					nodes[n][1] = (float)(rad * sin (theta) * sin (phi));
					nodes[n][2] = (float)(rad * cos (theta));
				}
			}
			else if (strncmp (coordtype, "XYZ", PhyDim)) {
				std::cout << "CGNS ERROR: unknown coordinate system."<< std::endl;
				return;
			}


			//if ( !m_compress || m_currentStep <=1 ) 
			//{
			vtkPoints* vertexPoints = vtkPoints::New();
			vertexPoints->SetDataTypeToFloat();
			volume->SetPoints(vertexPoints);
			vertexPoints->SetNumberOfPoints(nnodes);
			vertexPoints->Delete();

			for (n = 0; n < nnodes; n++) 
			{
				volume->GetPoints()->InsertPoint(n,nodes[n][0],nodes[n][1],nodes[n][2]);
			}

			//std::string strArr[4] = {"ZoneTypeNull", "ZoneTypeUserDefined","Structured", "Unstructured"};
			//std::cout << "Zone Type: " << strArr[zonetype] << std::endl;

			if (zonetype == Unstructured) 
			{
				write_volume_cells (nz, cgnsfn, cgnsbase,volume);
			}
		}

		// always 
		get_variables(nz, zonetype, sizes, cgnsfn, cgnsbase, CellDim);
		m_NCells = sizes[1];
		m_NPoints = sizes[0];
		ReadVariables();
		write_solution(nz, NULL, cgnsfn, cgnsbase,volume);

		if ( !m_compress || m_currentStep <=1 ) 
		{
			free (nodes);
		}

		volume->Update();

		if ( m_currentStep == 1 || !m_compress ) m_VTKOutput.push_back(volume);

		cg_close (cgnsfn);
	}

	m_TimeSteps = m_currentStep;
}

int blCgnsFileReader::get_variables (int nz, ZoneType_t zonetype, int *sizes, int cgnsfn, int cgnsbase, int CellDim)
{
	char name[33];
	int n, len, nv, nsols;
	int rind[6];
	DataType_t datatype;

	cg_nsols (cgnsfn, cgnsbase, nz, &nsols);
	nvars = 0;

	for (int i=1; i<=nsols; i++)
	{
		cg_sol_info (cgnsfn, cgnsbase, nz, i, name, &varloc);
		if (varloc != Vertex && varloc != CellCenter) continue;

		cg_nfields (cgnsfn, cgnsbase, nz, i, &nv);
		nvars += nv;
	}

	if (nvars < 1) return 0;
	
	/* check for rind */
	if (cg_goto (cgnsfn, cgnsbase, "Zone_t", nz,
		"FlowSolution_t", cgnssol, "end")) 
	{
		std::cout << "CGNS ERROR: cg_goto flow solution" << std::endl;
		return 0;
	}

	if ((n = cg_rind_read (rind)) != CG_OK) {

		if (n != CG_NODE_NOT_FOUND) 
		{
			std::cout << "CGNS ERROR: n != CG_NODE_NOT_FOUND" << std::endl;
			return 0;
		}

		for (n = 0; n < 6; n++)
		{
			rind[n] = 0;
		}
	}

	/* get solution data range */

	if (zonetype == Structured) {
		nv = varloc == Vertex ? 0 : CellDim;
		for (n = 0; n < 3; n++) {
			varrng[0][n] = 1;
			varrng[1][n] = 1;
		}
		ndata = 1;
		for (n = 0; n < CellDim; n++) {
			varrng[0][n] = rind[2*n] + 1;
			varrng[1][n] = rind[2*n] + sizes[n+nv];
			ndata *= sizes[n+nv];
		}
	}
	else {
		nv = varloc == Vertex ? 0 : 1;
		ndata = sizes[nv];
		varrng[0][0] = rind[0] + 1;
		varrng[1][0] = rind[0] + ndata;
	}

	/* get variable names */

	vars = (Variable *) malloc (nvars * sizeof(Variable));
	if (vars == NULL)
	{
		std::cout << "CGNS ERROR: malloc failed for variable names" << std::endl;
		return 0;
	}

	for (int i=1; i<=nsols; i++)
	{
		GridLocation_t _VarLoc;
		int _NV;

		cg_sol_info (cgnsfn, cgnsbase, nz, i, name, &_VarLoc);
		if (_VarLoc != Vertex && _VarLoc != CellCenter) continue;

		cg_nfields (cgnsfn, cgnsbase, nz, i, &_NV);


		for (int j = 1; j < _NV; j++) {
			if (cg_field_info (cgnsfn, cgnsbase, nz, i, j,
				&datatype, name))
			{
				std::cout << "CGNS ERROR: cg_field_info" << std::endl;
				return 0;
			}

			vars[nv].cnt = 1;
			strcpy (vars[nv].name, name);
		}
	}

	qsort (vars, nvars, sizeof(Variable), sort_variables);

	/* get number of scalars and vectors */

	for (nv = 2; nv < nvars; nv++) {
		len = strlen(vars[nv].name) - 1;
		if (vars[nv].name[len] == 'Z') {
			strcpy (name, vars[nv].name);
			name[len] = 'Y';
			if (0 == strcmp (name, vars[nv-1].name)) {
				name[len] = 'X';
				if (0 == strcmp (name, vars[nv-2].name)) {
					vars[nv-2].cnt = 3;
					vars[nv-1].cnt = 0;
					vars[nv].cnt = 0;
				}
			}
		}
	}
	return nvars;
}

void blCgnsFileReader::write_volume_cells(int nz, int cgnsfn, int cgnsbase, vtkSmartPointer<vtkUnstructuredGrid> volume) {
	int i, n, ns, nsect, is, ie, nn, ip, size;
	int maxsize, maxelems, nelems, elemcnt, elemsize;
	int *conn, *par, *types, cell[9];
	ElementType_t elemtype, et;
	char name[33];

	if (cg_nsections (cgnsfn, cgnsbase, nz, &nsect)) {
		std::cout << "CGNS ERROR: cg_nsections" << std::endl;
		return;
	}

	if (nsect < 1) {
		std::cout << "CGNS ERROR: no sections defined" << std::endl;
		return;
	}

	maxsize = maxelems = 0;
	for (ns = 1; ns <= nsect; ns++) {
		if (cg_section_read (cgnsfn, cgnsbase, nz, ns,
			name, &elemtype, &is, &ie, &nn, &ip) ||
			cg_ElementDataSize (cgnsfn, cgnsbase, nz, ns, &size)) 
		{
			std::cout << "CGNS ERROR: cg_section_read" << std::endl;
			return;
		}

		nelems = ie - is + 1;
		if (maxelems < nelems) maxelems = nelems;
		if (maxsize < size) maxsize = size;
	}

	conn = (int *) malloc (maxsize * sizeof(int));
	if (conn == NULL) {
		std::cout << "CGNS ERROR: malloc failed for element connectivity" << std::endl;
		return;
	}

	par = NULL;

	/* count volume cells */

	elemcnt = elemsize = 0;
	for (ns = 1; ns <= nsect; ns++) {
		if (cg_section_read (cgnsfn, cgnsbase, nz, ns,
			name, &elemtype, &is, &ie, &nn, &ip))
		{
			std::cout << "CGNS ERROR: cg_section_read" << std::endl;
			return;
		}

		if (elemtype < TETRA_4 || elemtype > MIXED) continue;
		nelems = ie - is + 1;
		if (elemtype == MIXED) {
			if (cg_elements_read (cgnsfn, cgnsbase, nz, ns, conn, par))
			{
				std::cout << "CGNS ERROR: cg_elements_read" << std::endl;
				return;
			}

			for (i = 0, n = 0; n < nelems; n++) {
				elemtype = (ElementType_t)conn[i++];
				switch (elemtype) {
					case TETRA_4:
					case TETRA_10:
						elemcnt++;
						elemsize += 5;
						break;
					case PYRA_5:
					case PYRA_14:
						elemcnt++;
						elemsize += 6;
						break;
					case PENTA_6:
					case PENTA_15:
					case PENTA_18:
						elemcnt++;
						elemsize += 7;
						break;
					case HEXA_8:
					case HEXA_20:
					case HEXA_27:
						elemcnt++;
						elemsize += 9;
						break;
				}

				if (cg_npe (elemtype, &size) || size == 0) 
				{
					std::cout << "CGNS ERROR: invalid element type in MIXED" << std::endl;
					return;
				}

				i += size;
			}
		}
		else {
			switch (elemtype) {
				case TETRA_4:
				case TETRA_10:
					size = 5;
					break;
				case PYRA_5:
				case PYRA_14:
					size = 6;
					break;
				case PENTA_6:
				case PENTA_15:
				case PENTA_18:
					size = 7;
					break;
				case HEXA_8:
				case HEXA_20:
				case HEXA_27:
					size = 9;
					break;
				default:
					size = 0;
					break;
			}
			if (size) {
				elemcnt += nelems;
				elemsize += (size * nelems);
			}
		}
	}

	if (elemcnt == 0) {
		free (conn);
		if (par != NULL) free (par);
		return;
	}

	types = (int *) malloc (elemcnt * sizeof(int));
	if (types == NULL)
	{
		std::cout << "CGNS ERROR: malloc failed for cell types" << std::endl;
		return;
	}

	elemcnt = 0;
	for (ns = 1; ns <= nsect; ns++) {
		if (cg_section_read (cgnsfn, cgnsbase, nz, ns,
			name, &elemtype, &is, &ie, &nn, &ip))
		{
			std::cout << "CGNS ERROR: cg_section_read" << std::endl;
			return;
		}

		if (elemtype < TETRA_4 || elemtype > MIXED) continue;
		nelems = ie - is + 1;
		if (cg_elements_read (cgnsfn, cgnsbase, nz, ns, conn, par))
		{
			std::cout << "CGNS ERROR: cg_elements_read" << std::endl;
			return;
		}

		et = elemtype;
		for (i = 0, n = 0; n < nelems; n++) {
			VTKCellType cellType;
			if (elemtype == MIXED) et =  (ElementType_t)conn[i++];
			switch (et) {
				case TETRA_4:
				case TETRA_10:
					nn = 4;
					types[elemcnt++] = 10;
					cellType = VTK_TETRA;
					break;
				case PYRA_5:
				case PYRA_14:
				case PYRA_13:
					nn = 5;
					types[elemcnt++] = 14;
					cellType = VTK_PYRAMID;
					break;
				case PENTA_6:
				case PENTA_15:
				case PENTA_18:
					nn = 6;
					types[elemcnt++] = 13;
					cellType = VTK_WEDGE;
					break;
				case HEXA_8:
				case HEXA_20:
				case HEXA_27:
					nn = 8;
					types[elemcnt++] = 12;
					cellType = VTK_HEXAHEDRON;
					break;
				default:
					nn = 0;
					break;
			}
			if (nn) {
				cell[0] = nn;
				for (ip = 0; ip < nn; ip++)
					cell[ip+1] = conn[i+ip] - 1;


				vtkIdList* list = vtkIdList::New();
				for (int i=0; i<nn;i++)
					list->InsertNextId(cell[i+1]);

				volume->InsertNextCell(cellType,list);
				list->Delete();

			}
			if (cg_npe (et, &size) || size == 0) 
			{
				std::cout << "CGNS ERROR: invalid element type" << std::endl;
				return;	
			}

			i += size;
		}
	}
	free (conn);
	if (par != NULL) free (par);

	free (types);
}


void blCgnsFileReader::write_solution (int nz, int *mask, int cgnsfn, int cgnsbase, vtkSmartPointer<vtkUnstructuredGrid> volume)
{
	int n, nv, nd;
	int nscal, nvect;
	float *data;

	nscal = nvect = 0;
	for (nv = 0; nv < nvars; nv++) 
	{
		if (vars[nv].cnt == 1)
			nscal++;
		else if (vars[nv].cnt == 3)
			nvect++;
	}

	if (nvect) 
	{
		data = (float *) malloc (3 * std::max(m_NPoints,m_NCells) * sizeof(float));
	}
	else 
	{
		data = (float *) malloc (std::max(m_NPoints,m_NCells) * sizeof(float));
	}
	if (data == NULL)
	{
		std::cout << "CGNS ERROR: malloc failed for solution data" << std::endl;
		return;
	}

	ndata = std::max(m_NPoints,m_NCells);

	if (mask == NULL)
	{
		nd = ndata;
	}
	else 
	{
		for (nd = 0, n = 0; n < ndata; n++) {
			if (mask[n]) nd++;
		}
	}


	if (nscal) {
		for (nv = 0; nv < nvars; nv++) {
			if (vars[nv].cnt != 1) continue;
			std::string varName(vars[nv].name);

			if (cg_field_read (cgnsfn, cgnsbase, nz, m_VarNameToNumSol[varName],
				vars[nv].name, RealSingle,
				varrng[0], varrng[1], data))
			{
				std::cout << "CGNS ERROR: cg_field_read" << std::endl;
			}

			if ( std::find(m_scalarsNamesPoint.begin(),m_scalarsNamesPoint.end(),varName) == m_scalarsNamesPoint.end()
				&& std::find(m_scalarsNamesCell.begin(),m_scalarsNamesCell.end(),varName) == m_scalarsNamesCell.end()
				) { //not found
				continue;
			}

			std::string varNameWithoutSuffix = varName;

			if (m_compress) {
				char num [200];
				std::sprintf(num,"%d",m_currentStep);
				std::string numStr(num);

				while (numStr.size()<3) numStr = "0" + numStr;

				varName += "_";
				varName += numStr;
			}

			vtkFloatArray *dataArray = vtkFloatArray::New();

			int n;

			if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
			{
				n = m_NPoints;
			}
			else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
			{
				n = m_NCells;
			}


			for (int i = 0; i < n; i++)
			{
				dataArray->InsertNextValue(data[i]);
			}

			dataArray->SetName(varName.c_str());
			
			if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
			{
				volume->GetPointData()->AddArray(dataArray);
			}
			else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
			{
				volume->GetCellData()->AddArray(dataArray);
			}

			dataArray->Delete();
		}
	}


	if (nvect) {
		for (nv = 0; nv < nvars; nv++) {
			if (vars[nv].cnt != 3) continue;
			
			std::string varName(vars[nv].name);

			if (cg_field_read (cgnsfn, cgnsbase, nz, m_VarNameToNumSol[varName],
				vars[nv].name, RealSingle,
				varrng[0], varrng[1], data) ||
				cg_field_read (cgnsfn, cgnsbase, nz, m_VarNameToNumSol[varName],
				vars[nv+1].name, RealSingle,
				varrng[0], varrng[1], &data[ndata]) ||
				cg_field_read (cgnsfn, cgnsbase, nz, m_VarNameToNumSol[varName],
				vars[nv+2].name, RealSingle,
				varrng[0], varrng[1], &data[2*ndata]))
			{
				std::cout << "CGNS ERROR: cg_field_read" << std::endl;
			}

			if ( std::find(m_vectorsNamesPoint.begin(),m_vectorsNamesPoint.end(),varName) == m_vectorsNamesPoint.end()
				&& std::find(m_vectorsNamesCell.begin(),m_vectorsNamesCell.end(),varName) == m_vectorsNamesCell.end()
				) { //not found
				continue;
			}

			std::string varNameWithoutSuffix = varName;

			if (varName[varName.size()-1] == 'X') {
				varName = varName.substr(0,varName.size()-1);
			}

			if (m_compress) {
				char num [200];
				std::sprintf(num,"%d",m_currentStep);
				std::string numStr(num);

				while (numStr.size()<3) numStr = "0" + numStr;

				varName += "_";
				varName += numStr;
			}

			vtkFloatArray *dataArray = vtkFloatArray::New();

			int n;

			if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
			{
				n = m_NPoints;
			}
			else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
			{
				n = m_NCells;
			}


			dataArray->SetNumberOfComponents(3);
			dataArray->SetNumberOfTuples(n);

			for (int i = 0; i < n; i++)
			{
				dataArray->SetTuple3(i,data[i],data[ndata+i],data[2*ndata+i]);
			}

			dataArray->SetName(varName.c_str());

			if (m_compress) 
			{
				if ( varName.substr(0,8) == "Velocity" ) 
				{
					if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
					{
						volume->GetPointData()->AddArray(dataArray);
					}
					else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
					{
						volume->GetCellData()->AddArray(dataArray);
					}
				}

				else 
				{
					if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
					{
						volume->GetPointData()->AddArray(dataArray);
					}
					else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
					{
						volume->GetCellData()->AddArray(dataArray);
					}
				}

			}
			else 
			{
				if ( varName.substr(0,8) == "Velocity" )
				{
					if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
					{
						volume->GetPointData()->AddArray(dataArray);
					}
					else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
					{
						volume->GetCellData()->AddArray(dataArray);
					}
				}

				else
				{
					if (m_StringToVarLocType[varNameWithoutSuffix] == Vertex)
					{
						volume->GetPointData()->AddArray(dataArray);
					}
					else if (m_StringToVarLocType[varNameWithoutSuffix] == CellCenter)
					{
						volume->GetCellData()->AddArray(dataArray);
					}
				}
			}
			dataArray->Delete();
		}
	}
	free (data);
}


int blCgnsFileReader::GetTimeSteps() 
{	
	return m_TimeSteps;
}

void blCgnsFileReader::ReadAll()
{
	FourStringVectors fsv = ReadVariables();
	SetScalarsListPoint(fsv.ScalarPoint);
	SetScalarsListCell(fsv.ScalarCell);
	SetVectorsListPoint(fsv.VectorPoint);
	SetVectorsListCell(fsv.VectorCell);
}