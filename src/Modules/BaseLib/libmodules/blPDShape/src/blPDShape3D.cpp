/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
// This class defines a Point Distribution Shape.
 
#pragma warning (disable : 4786)

#include <float.h>
#include <time.h>
#include <vnl/vnl_cross_product_matrix.h>

#include "blPDShape3D.h"
#include "blMath.h"



/**
 *	Constructor
 */
//--------------------------------------------------
blPDShape3D::blPDShape3D()
//--------------------------------------------------
{}



/**
 *	Destructor
 */
//--------------------------------------------------
blPDShape3D::~blPDShape3D()
//--------------------------------------------------
{}



/**
 *	This method computes normal vectors at polygon points.
 *	
 *	This method assumes that all polygons are convex.
 *	Polygons defined in counterclockwise direction, normal vector: outwards
 *	Polygons defined clockwise direction, normal vector: inwards		
 */
//--------------------------------------------------
 void blPDShape3D::ComputeNormals()
//--------------------------------------------------
{
	
	// if cell links are not built, it is necessary to build them.
	if (! this->linksBuilt) 
	{
		this->ptMesh->BuildCellLinks();
		
		this->linksBuilt = true;
	}
	
	// compute first normals at cells
	this->Compute3DCellsNormalVectors();

	// then, compute normals at points
	this->Compute3DPointsNormalVectors();
}



/** 
 *	This method computes the coordinates of a normal vector, given 3 points
 *
 *	u = (p2-p1), v = (p0-p1), n = u x v
 *
 *	If points defined in counterclockwise direction, 
 *	normal vector --> outwards.
 *
 *	If points defined in clockwise direction, 
 *	normal vector --> inwards.
 */
//--------------------------------------------------
void blPDShape3D::Compute3DNormalVector(MeshType::PointType &point0, 
										MeshType::PointType &point1, 
										MeshType::PointType &point2,
                                        TNormalVector &output)
//--------------------------------------------------	
{
  // u x v = i(uy*vz - bl*vy) + j (bl*vx - ux*vz) + k (ux*vy + uy*vx)

	typedef MeshType::PointType::CoordRepType CoordRepType;	
	typedef vnl_vector<CoordRepType> vnl_vectorType;

	// referencing the same memory block
	vnl_vectorType vector0 = point0.Get_vnl_vector();	 
	vnl_vectorType vector1 = point1.Get_vnl_vector();
	vnl_vectorType vector2 = point2.Get_vnl_vector();

	// u = (p2-p1), v = (p0-p1)
	vnl_vectorType vectorU, vectorV;

	vectorU = vector2;
	vectorV = vector0;

	vectorU -= vector1;
	vectorV -= vector1;
	
	// n = u x v

	// creates a matrix M to do cross product: u x v

	//	 0  -bl   uy
	//	bl    0	 -ux
	// -uy	 ux	   0

	vnl_cross_product_matrix crossMatrix(vectorU);

	// n = M * v
	vnl_vectorType normal = vectorV.pre_multiply(crossMatrix);

	// normalize
	normal = normal.normalize();

	output.Set_vnl_vector(normal);
}



/** 
 *	This method computes normal vector at polygon cells in the mesh
 */
//--------------------------------------------------
void blPDShape3D::Compute3DCellsNormalVectors()
//--------------------------------------------------	
{

#ifdef DEBUG_MESSAGES_PDSHAPE3D
	std::cout << "blPDShape3D::Compute3DCellsNormalVectors()" << std::endl;
#endif

	// typedefs
// ----------------------------------------------------------------------------
	
	// point type
	typedef MeshType::PointType PointType;	///	

	// cell type
	typedef MeshType::CellType CellType; ///
	
	// cell identifier
	typedef MeshType::CellIdentifier CellIdentifier; ///
	typedef CellType::PointIdIterator PointIdIterator; ///

	// cell container
	typedef MeshType::CellsContainer CellsContainer; ///
	typedef MeshType::CellsContainerPointer CellsContainerPointer; ///


	// variables
// ----------------------------------------------------------------------------	
	
	// Get the cells container
	CellsContainerPointer cellContPt = this->ptMesh->GetCells();
	
	// Define cell iterators
	CellsContainer::Iterator cellIter = cellContPt->Begin();
	CellsContainer::Iterator cellIterEnd = cellContPt->End();
	
	// Cell pointer and identifier for current cell
	CellType * cell;
	CellIdentifier cellId;

	// Point Identifier Iterator
	PointIdIterator pointIdIter;		
	
	// define 3 points
	PointType *point0, *point1, *point2;
	point0 = new PointType();
	point1 = new PointType();
	point2 = new PointType();

	// new vector that represents a normal
	TNormalVector normal;


	// calculate normals at polygon cells
// ----------------------------------------------------------------------------	

	// For each cell C in Mesh
	while (cellIter != cellIterEnd)
	{
		cell = cellIter.Value();
		cellId = cellIter.Index();

		// If cell is Polygon (TRIANGLE, QUADRILATERAL, POLYGON)
		// Suppose all points are convex
		
		if (cell->GetType() == CellType::TRIANGLE_CELL ||
			cell->GetType() == CellType::QUADRILATERAL_CELL ||
			cell->GetType() == CellType::POLYGON_CELL)

		{

#ifdef DEBUG_MESSAGES_PDSHAPE3D
	std::cout << "CellId " << cellId ;
#endif

			// get point idenfier iterator
			pointIdIter = cell->PointIdsBegin();		

			// get the first 3 points of polygon
			this->ptMesh->GetPoint(*pointIdIter, point0);
			this->ptMesh->GetPoint(*(pointIdIter + 1), point1);
			this->ptMesh->GetPoint(*(pointIdIter + 2), point2);

			// compute normal vector
			this->Compute3DNormalVector(*point0, *point1, *point2, normal);

			// set cell data
			this->ptMesh->SetCellData(cellId, normal);

#ifdef DEBUG_MESSAGES_PDSHAPE3D
			std::cout << " .Normal :" << normal[0] << " " << normal[1] 
											<< " " << normal[2] << std::endl;
#endif

		}

		cellIter++;
	}	
	
	// delete the points created above
	delete point0;
	delete point1;
	delete point2;
}



/** 
 *	This method computes normal vector at polygon points in the mesh
 */
//--------------------------------------------------
void blPDShape3D::Compute3DPointsNormalVectors()
//--------------------------------------------------	
{

#ifdef DEBUG_MESSAGES_PDSHAPE3D
		std::cout << "blPDShape3D::Compute3DPointsNormalVectors " << std::endl;
#endif

	// typedefs
// ----------------------------------------------------------------------------
	
	// point type
	typedef MeshType::PointType PointType;		///

	// point identifier
	typedef MeshType::PointIdentifier PointIdentifier; ///

	// point container
	typedef MeshType::PointsContainer PointsContainer; ///
	typedef MeshType::PointsContainerIterator PointsContainerIterator; ///
	
	typedef MeshType::PointDataContainerPointer PointDataContainerPointer;

	// cell type
	typedef MeshType::CellType CellType;
	
	// cell identifier
	typedef MeshType::CellIdentifier CellIdentifier; ///

	// cell autopointer
	typedef MeshType::CellAutoPointer CellAutoPointer;

	// cell links container pointer
	typedef MeshType::CellLinksContainerPointer CellLinksContainerPointer; ///
	
	// point-cell links container
	typedef MeshType::PointCellLinksContainer PointCellLinksContainer; ///
	typedef PointCellLinksContainer::iterator PointCellLinksIter; ///


	// variables
// ----------------------------------------------------------------------------	
	
	// Get the points container
	// -----------------------------------
	PointsContainer * pointsContPt = this->ptMesh->GetPoints();
	
	// Define point iterators
	PointsContainerIterator pointsIter = pointsContPt->Begin();
	PointsContainerIterator pointsIterEnd = pointsContPt->End();
	
	
	// Point pointer and identifier for current point
	PointType point;
	PointIdentifier pointId;


/*	
	// PointDataContainer
	// -------------------------------------
	PointDataContainerPointer pointDataContainerPt = this->ptMesh->GetPointData();

	// if container is null, create a new point data container
	if (pointDataContainerPt.IsNull())
	{
		pointDataContainerPt = PointDataContainer::New();
		this->ptMesh->SetPointData(pointDataContainerPt.GetPointer());
	}

	// reserve memory for all landmarks
	pointDataContainerPt->Reserve(this->numberLandmarks);
*/


	// Get the cellLinks container 
	// -----------------------------------
	CellLinksContainerPointer cellLinksContPt = 
						this->ptMesh->GetCellLinks();

	
	// cell identifier and cell normal
	CellIdentifier cellId;
	TNormalVector cellNormal; // or TCellPixelType

	// point normal vector to calculate
	TNormalVector pointNormal;	


	// Calculate normal vector at points
// ----------------------------------------------------------------------------

			
	// For each point P in Mesh
	while (pointsIter != pointsIterEnd)
	{
		point = pointsIter.Value();
		pointId = pointsIter.Index();

#ifdef DEBUG_MESSAGES_PDSHAPE3D
		std::cout << "Point Id: " << pointId ;
#endif

		PointCellLinksContainer pointCellLinksCont; 

		// get the cells containing the current point
		bool inAnyCell = 
			cellLinksContPt->GetElementIfIndexExists(pointId, 
														&pointCellLinksCont);

		// if point belongs to any cell
		if (inAnyCell)
		{

#ifdef DEBUG_MESSAGES_PDSHAPE3D
		std::cout << "--> Cells: " ;
#endif

			// iterator through PointCellLinksContainer
			PointCellLinksIter pointCellLinksIter = pointCellLinksCont.begin();
			PointCellLinksIter pointCellLinksIterEnd = pointCellLinksCont.end();

			// set to zero
			pointNormal.Fill(0.0);	
			
			bool nonPolygonCellFound = false;
				
			// go through the cells to get cell normals and calculate point normal
			// if the point belongs to a cell whose type is not a polygon,
			// its normal will be 0.0

			while (!nonPolygonCellFound && 
					pointCellLinksIter != pointCellLinksIterEnd)
			{
								
				// get cell id
				cellId = *pointCellLinksIter;

#ifdef DEBUG_MESSAGES_PDSHAPE3D
		std::cout << cellId << " ";
#endif

				CellAutoPointer cellPt;
				this->ptMesh->GetCell(cellId, cellPt);

				// ONLY if cell is a polygon, calculate normal 3D
				if (cellPt->GetType() == CellType::TRIANGLE_CELL ||
					cellPt->GetType() == CellType::QUADRILATERAL_CELL ||
					cellPt->GetType() == CellType::POLYGON_CELL)

				{
					// get the normal at cell and add it to pointNormal
					if (this->ptMesh->GetCellData(cellId, &cellNormal))
					{					
						// add cell normal to point normal vector
						pointNormal += cellNormal;	
					}	
				}

				// if cell is not a polygon, set normal to 0.0
				else
				{
					pointNormal.Fill(0.0);
					nonPolygonCellFound = true;
				}

				// update iterator
				pointCellLinksIter++;
			}
			
			// normalize the resulting normal vector
			TNormalVector::ValueType norm = pointNormal.GetNorm();

			if (norm != 0)
			{
				pointNormal/=norm;
			}
			
			// set the normal vector as point data 
			this->ptMesh->SetPointData(pointId, pointNormal);

#ifdef DEBUG_MESSAGES_PDSHAPE3D
		std::cout << ". Normal " << pointNormal[0] << " " 
						<< pointNormal[1] << " " << pointNormal[2] << std::endl;
#endif
		}

		// update point iterator
		pointsIter++;
	}		
}



/* 
 * \brief creates a copy of this PDShape
 *
 */
//--------------------------------------------------
blPDShapeInterface::Pointer blPDShape3D::GetCopy()
//--------------------------------------------------
{
	// get points
	vnl_vector<double> points(this->numberDimensions * this->numberLandmarks);
	this->GetPoints(points);

	// get polygons, polylines
	ShapeCellsContainer polylines, polygons;
	this->GetPolylines(polylines);
	this->GetPolygons(polygons);

	// new shape
	blPDShape3D::Pointer newShape = blPDShape3D::New();
	newShape->InitializeFromPoints(points.begin(), this->numberLandmarks);
	newShape->InitializeCells(polygons, polylines);

	blPDShapeInterface::Pointer shapeInterface = newShape.GetPointer();

	return shapeInterface;
}




//added by Costa 20/11/03

/* 
 * This method performs 3D Delaunay triangulation of the mesh
 * points are retrieved by GetPoints, result is stroed in TesselationCells
 * Function was ported with slight changes from WildMagic Library
 *
 */
//--------------------------------------------------
void blPDShape3D::Tesselate(bool forceTesselation)
//--------------------------------------------------
{
	//force tesselation
	if(forceTesselation) TesselationCells.clear(); 

	//Tesselation was already performed
	if( !TesselationCells.empty() ) return;


	int iVertexQuantity = numberLandmarks;  //number of landmarks
	int m_iVertexQuantity = iVertexQuantity;
	int i;
    int m_iTetrahedronQuantity = 0;

    assert( numberDimensions == 3 );
    assert( iVertexQuantity >= 4 );

	PointIdsContainer PointIds; //for storing tetrahedron vertices
	

    // The extra four slots are required for temporary storage -- WildMagic stuff
	vnlVectorDouble akVertex((iVertexQuantity+4)*numberDimensions);
	GetPoints(akVertex); //get all the landmarks (not IDs, but real points)

	//for easy access to points, 'cause each element of akPoint is a point
	//while each element of akVertex is a coordinate
	VectorOf3DContinuousPoints akPoint=(VectorOf3DContinuousPoints)&akVertex[0]; 

	//tweaking stuff
	const double ms_iQuantityFactor = 16;
	const double ms_fEpsilon = 0.00001;
	const double ms_fRange = 10.0;
	const int ms_iTSize = 75;
	//:~

    // compute the axis-aligned bounding box of the vertices
    /*m_fXMin = akPoint[0].X();
    m_fXMax = m_fXMin;
    m_fYMin = akPoint[0].Y();
    m_fYMax = m_fYMin;
    m_fZMin = akPoint[0].Z();
    m_fZMax = m_fZMin;

    int i;
    for (i = 1; i < m_iVertexQuantity; i++)
    {
        Real fValue = akPoint[i].X();
        if ( m_fXMax < fValue )
            m_fXMax = fValue;
        if ( m_fXMin > fValue )
            m_fXMin = fValue;

        fValue = akPoint[i].Y();
        if ( m_fYMax < fValue )
            m_fYMax = fValue;
        if ( m_fYMin > fValue )
            m_fYMin = fValue;

        fValue = akPoint[i].Z();
        if ( m_fZMax < fValue )
            m_fZMax = fValue;
        if ( m_fZMin > fValue )
            m_fZMin = fValue;
    } */


	const blPDShape3D::MeshType::BoundingBoxType* BoundingBox = ptMesh->GetBoundingBox();
	itk::FixedArray<double,6> Bounds = BoundingBox->GetBounds();
	
    double m_fXMin = Bounds[0];
    double m_fXMax = Bounds[1];
    double m_fYMin = Bounds[2];
    double m_fYMax = Bounds[3];
    double m_fZMin = Bounds[4];
    double m_fZMax = Bounds[5];

    double m_fXRange = m_fXMax-m_fXMin;
    double m_fYRange = m_fYMax-m_fYMin;
    double m_fZRange = m_fZMax-m_fZMin;

	//Seed the random-number generator 
	blMath::SymmetricRandom( (unsigned)time( NULL ) );


    // need to scale the data later to do a correct tetrahedron count
    double fMaxRange = m_fXRange;
    if ( fMaxRange < m_fYRange )
        fMaxRange = m_fYRange;
    if ( fMaxRange < m_fZRange )
        fMaxRange = m_fZRange;
    double fMaxRangeCube = fMaxRange*fMaxRange*fMaxRange;

    // Tweak the points by very small random numbers to help avoid
    // cosphericities in the vertices.
    double fAmplitude = (0.5)*ms_fEpsilon*fMaxRange;
    for (i = 0; i < m_iVertexQuantity; i++) 
    {
        akPoint[i].X() += fAmplitude * blMath::SymmetricRandom();
        akPoint[i].Y() += fAmplitude * blMath::SymmetricRandom();
        akPoint[i].Z() += fAmplitude * blMath::SymmetricRandom();
    }

    double aafWork[4][3] =
    {
        { (8.0)*ms_fRange, -ms_fRange, -ms_fRange },
        { -ms_fRange, (8.0)*ms_fRange, -ms_fRange },
        { -ms_fRange, -ms_fRange, (8.0)*ms_fRange },
        { -ms_fRange, -ms_fRange, -ms_fRange }
    };

    for (i = 0; i < 4; i++)
    {
        akPoint[m_iVertexQuantity+i].X() = m_fXMin+m_fXRange*aafWork[i][0];
        akPoint[m_iVertexQuantity+i].Y() = m_fYMin+m_fYRange*aafWork[i][1];
        akPoint[m_iVertexQuantity+i].Z() = m_fZMin+m_fZRange*aafWork[i][2];
    }

    int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i11, aiII[3];
    double fTmp;

    int iSixTSize = 6*ms_iTSize;
    int** aaiTmp = new int*[iSixTSize+1];
    aaiTmp[0] = new int[3*(iSixTSize+1)];
    for (i0 = 1; i0 < iSixTSize+1; i0++)
        aaiTmp[i0] = aaiTmp[0] + 3*i0;

    // Estimate of how many tetrahedrons there can be.  Since theoretically
    // the number is O(N^2), this could be quite large.  You may need to
    // increase the quantity factor if a call to this function fails.
    i1 = (int)(ms_iQuantityFactor*m_iVertexQuantity);

    int* aaID = new int[i1];
    for (i0 = 0; i0 < i1; i0++)
        aaID[i0] = i0;

    int** aaiA3S = new int*[i1];
    aaiA3S[0] = new int[4*i1];
    for (i0 = 1; i0 < i1; i0++)
        aaiA3S[i0] = aaiA3S[0] + 4*i0;
    aaiA3S[0][0] = m_iVertexQuantity;
    aaiA3S[0][1] = m_iVertexQuantity+1;
    aaiA3S[0][2] = m_iVertexQuantity+2;
    aaiA3S[0][3] = m_iVertexQuantity+3;

    // circumscribed centers and radii
    double** aafCCR = new double*[i1];
    aafCCR[0] = new double[4*i1];
    for (i0 = 1; i0 < i1; i0++)
        aafCCR[i0] = aafCCR[0] + 4*i0;
    aafCCR[0][0] = 0.0;
    aafCCR[0][1] = 0.0;
    aafCCR[0][2] = 0.0;
    aafCCR[0][3] = DBL_MAX; //!! depends on float type

    int iTetraQuantity = 1;  // number of tetrahedra
    i4 = 1;

    // compute tetrahedralization
    for (i0 = 0; i0 < m_iVertexQuantity; i0++)
    {  
        i1 = i7 = -1;
        i9 = 0;
        for (i11 = 0; i11 < iTetraQuantity; i11++)
        {  
            i1++;
            while ( aaiA3S[i1][0] < 0 ) 
                i1++;
            fTmp = aafCCR[i1][3];
            for (i2 = 0; i2 < 3; i2++)
            {
                double fZ = akPoint[i0][i2]-aafCCR[i1][i2];
                fTmp -= fZ*fZ;
                if ( fTmp < 0.0 ) 
                    goto Corner3;
            }
            i9--;
            i4--;
            aaID[i4] = i1;
            for (i2 = 0; i2 < 4; i2++)
            {
                aiII[0] = 0;
                if ( aiII[0] == i2 ) 
                    aiII[0]++;
                for (i3 = 1; i3 < 3; i3++)
                {
                    aiII[i3] = aiII[i3-1] + 1;
                    if ( aiII[i3] == i2 ) 
                        aiII[i3]++;
                }
                if ( i7 > 2 )
                {
                    i8 = i7;
                    for (i3 = 0; i3 <= i8; i3++)
                    {
                        for (i5 = 0; i5 < 3; i5++)
                        {
                            if ( aaiA3S[i1][aiII[i5]] != aaiTmp[i3][i5] ) 
                                goto Corner1;
                        }
                        for (i6 = 0; i6 < 3; i6++)
                            aaiTmp[i3][i6] = aaiTmp[i8][i6];
                        i7--;
                        goto Corner2;
Corner1:;
                    }
                }
                if ( ++i7 > iSixTSize )
                {
                    // Temporary storage exceeded.  Increase ms_iTSize and
                    // call the constructor again.
                    assert( false );
                    goto ExitDelaunay;
                }
                for (i3 = 0; i3 < 3; i3++) 
                    aaiTmp[i7][i3] = aaiA3S[i1][aiII[i3]];
Corner2:;
            }
            aaiA3S[i1][0] = -1;
Corner3:;
        }

        for (i1 = 0; i1 <= i7; i1++)
        {
            for (i2 = 0; i2 < 3; i2++)
                for (aafWork[3][i2] = 0, i3 = 0; i3 < 3; i3++)
                {
                    aafWork[i3][i2] = akPoint[aaiTmp[i1][i2]][i3] -
                        akPoint[i0][i3];
                    aafWork[3][i2] += (0.5)*aafWork[i3][i2]*(
                        akPoint[aaiTmp[i1][i2]][i3] + akPoint[i0][i3]);
                }

            fTmp = (aafWork[0][0]*(aafWork[1][1]*aafWork[2][2] -
                aafWork[1][2]*aafWork[2][1])) - (aafWork[1][0]*(
                aafWork[0][1]*aafWork[2][2] - aafWork[0][2]*aafWork[2][1])) +
                (aafWork[2][0]*(aafWork[0][1]*aafWork[1][2] - aafWork[0][2]*
                aafWork[1][1]));
            assert( fTmp != 0.0 );
            fTmp = (1.0)/fTmp;
            aafCCR[aaID[i4]][0] = ((aafWork[3][0]*(aafWork[1][1]*
                aafWork[2][2] - aafWork[1][2]*aafWork[2][1])) -
                (aafWork[1][0]*(aafWork[3][1]*aafWork[2][2] - aafWork[3][2]*
                aafWork[2][1])) + (aafWork[2][0]*(aafWork[3][1]*
                aafWork[1][2] - aafWork[3][2]*aafWork[1][1])))*fTmp;
            aafCCR[aaID[i4]][1] = ((aafWork[0][0]*(aafWork[3][1]*
                aafWork[2][2] - aafWork[3][2]*aafWork[2][1])) -
                (aafWork[3][0]*(aafWork[0][1]*aafWork[2][2] - aafWork[0][2]*
                aafWork[2][1])) + (aafWork[2][0]*(aafWork[0][1]*
                aafWork[3][2] - aafWork[0][2]*aafWork[3][1])))*fTmp;
            aafCCR[aaID[i4]][2] = ((aafWork[0][0]*(aafWork[1][1]*
                aafWork[3][2] - aafWork[1][2]*aafWork[3][1])) -
                (aafWork[1][0]*(aafWork[0][1]*aafWork[3][2] - aafWork[0][2]*
                aafWork[3][1])) + (aafWork[3][0]*(aafWork[0][1]*
                aafWork[1][2] - aafWork[0][2]*aafWork[1][1])))*fTmp;

            for (aafCCR[aaID[i4]][3] = 0, i2 = 0; i2 < 3; i2++) 
            {
                double fZ = akPoint[i0][i2] - aafCCR[aaID[i4]][i2];
                aafCCR[aaID[i4]][3] += fZ*fZ;
                aaiA3S[aaID[i4]][i2] = aaiTmp[i1][i2];
            }

            aaiA3S[aaID[i4]][3] = i0;
            i4++;
            i9++;
        }
        iTetraQuantity += i9;
    }

    // count the number of tetrahedra
    m_iTetrahedronQuantity = 0;
    i0 = -1;
    for (i11 = 0; i11 < iTetraQuantity; i11++)
    {
        i0++;
        while ( aaiA3S[i0][0] < 0 )
            i0++;
        if ( aaiA3S[i0][0] < m_iVertexQuantity )
        {
            for (i1 = 0; i1 < 3; i1++)
            {
                for (i2 = 0; i2 < 3; i2++) 
                {
                    aafWork[i2][i1] = akPoint[aaiA3S[i0][i1]][i2] -
                        akPoint[aaiA3S[i0][3]][i2];
                }
            }

            fTmp = ((aafWork[0][0]*(aafWork[1][1]*aafWork[2][2] -
                aafWork[1][2]*aafWork[2][1])) - (aafWork[1][0]*(
                aafWork[0][1]*aafWork[2][2] - aafWork[0][2]*aafWork[2][1])) +
                (aafWork[2][0]*(aafWork[0][1]*aafWork[1][2] - aafWork[0][2]*
                aafWork[1][1])));

            if ( fabs(fTmp) > ms_fEpsilon*fMaxRangeCube )
                m_iTetrahedronQuantity++;
        }
    }


    // create the tetrahedrons
	// tetrahedrons are added one by one, so we can use itkMesh stuff right away
	// reserve space for triangles
	TesselationCells.resize(m_iTetrahedronQuantity);
	
	//reserve space for vertices
	//number of vertices per cell=number of dimensions + 1
	PointIds.resize(numberDimensions+1); 


    // create the tetrahedra
    //m_akTetrahedron = new Tetrahedron[m_iTetrahedronQuantity];
    m_iTetrahedronQuantity = 0;
    i0 = -1;
    for (i11 = 0; i11 < iTetraQuantity; i11++)
    {
        i0++;
        while ( aaiA3S[i0][0] < 0 )
            i0++;
        if ( aaiA3S[i0][0] < m_iVertexQuantity )
        {
            for (i1 = 0; i1 < 3; i1++)
            {
                for (i2 = 0; i2 < 3; i2++) 
                {
                    aafWork[i2][i1] = akPoint[aaiA3S[i0][i1]][i2] -
                        akPoint[aaiA3S[i0][3]][i2];
                }
            }

            fTmp = ((aafWork[0][0]*(aafWork[1][1]*aafWork[2][2] -
                aafWork[1][2]*aafWork[2][1])) - (aafWork[1][0]*(
                aafWork[0][1]*aafWork[2][2] - aafWork[0][2]*aafWork[2][1])) +
                (aafWork[2][0]*(aafWork[0][1]*aafWork[1][2]-aafWork[0][2]*
                aafWork[1][1])));

            if ( fabs(fTmp) > ms_fEpsilon*fMaxRangeCube )
            {
				/////////////////////////////////////////////
                int iDelta = (fTmp < 0.0 ? 1 : 0);

                /*Tetrahedron& rkTetra =
                    m_akTetrahedron[m_iTetrahedronQuantity];
                rkTetra.m_aiVertex[0] = aaiA3S[i0][0];
                rkTetra.m_aiVertex[1] = aaiA3S[i0][1];
                rkTetra.m_aiVertex[2] = aaiA3S[i0][2+iDelta];
                rkTetra.m_aiVertex[3] = aaiA3S[i0][3-iDelta];*/
				
				PointIds[0]=aaiA3S[i0][0]; //1st vertex
				PointIds[1]=aaiA3S[i0][1]; //2nd vertex
				PointIds[2]=aaiA3S[i0][2+iDelta]; //3rd vertex
				PointIds[3]=aaiA3S[i0][3-iDelta]; //3rd vertex
				
				TesselationCells[m_iTetrahedronQuantity]=PointIds;
				
				///////////////////////////////////////////////

                m_iTetrahedronQuantity++;
            }
        }
    }

	//just in case (if the size is coorect - nothing will happen)
	TesselationCells.resize(m_iTetrahedronQuantity);


ExitDelaunay:;
    delete[] aaiTmp[0];
    delete[] aaiTmp;
    delete[] aaID;
    delete[] aaiA3S[0];
    delete[] aaiA3S;
    delete[] aafCCR[0];
    delete[] aafCCR;
//    delete[] akPoint;
}
//:~
