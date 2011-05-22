/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// Implementation file for 2D Delaunay triangulation.

// Port from wmlDelaunay2a.h from Magic Software, Inc.
//
// Magic Software, Inc.
// http://www.magic-software.com
// http://www.wild-magic.com
// Copyright (c) 2003.  All Rights Reserved
//
// The Wild Magic Library (WML) source code is supplied under the terms of
// the license agreement http://www.magic-software.com/License/WildMagic.pdf
// and may not be copied or disclosed except in accordance with the terms of
// that agreement.

#include "blMath.h"
#include "blDelaunay2D.h"
#include <float.h>

//----------------------------------------------------------------------------
blDelaunay2D::blDelaunay2D (const vnlVectorDouble& points,
				ShapeCellsContainer& Triangles, const BoundingBox2D* bounds)
				:m_kVertex(points.size() / 2)
{
	const int numberDimensions = 2;
	int iVQuantity = points.size() / numberDimensions; 

	//make array of points easily accessible. 
	//I.e. convert the array of coordinates into the array of points
	VectorOf2DContinuousPoints akVertex=(VectorOf2DContinuousPoints)&points[0];


    int riTQuantity = 0; //number of triangles


	double fMin, fMax;
	int i;

	//if bounds are supplied by caller - use them, if not - recalculate
	if (bounds!=NULL) 
	{
		fMin=__min(bounds->m_fXMin,bounds->m_fYMin);
		fMax=__max(bounds->m_fXMax,bounds->m_fYMax);
	}
	else 
	{
    fMin = akVertex[0].X(), fMax = fMin;
    for (i = 0; i < iVQuantity; i++)
    {
        if ( akVertex[i].X() < fMin )
            fMin = akVertex[i].X();
        else if ( akVertex[i].X() > fMax )
            fMax = akVertex[i].X();

        if ( akVertex[i].Y() < fMin )
            fMin = akVertex[i].Y();
        else if ( akVertex[i].Y() > fMax )
            fMax = akVertex[i].Y();
    }
	};

	// transform the points to [-1,1]^2 for numerical preconditioning
    double fHalfRange = ((double)0.5)*(fMax - fMin);
    double fInvHalfRange = ((double)1.0)/fHalfRange;

    // sort by x-component to prepare to remove duplicate vertices
    for (i = 0; i < iVQuantity; i++)
    {
        m_kVertex[i].m_kV.X() = -(double)1.0 + fInvHalfRange*(akVertex[i].X() -
            fMin);
        m_kVertex[i].m_kV.Y() = -(double)1.0 + fInvHalfRange*(akVertex[i].Y() -
            fMin);
        m_kVertex[i].m_iIndex = i;
    }
	std::sort(m_kVertex.begin(),m_kVertex.end());

    // remove duplicate points
    SVArray::iterator pkEnd = std::unique(m_kVertex.begin(),m_kVertex.end());
    m_kVertex.erase(pkEnd,m_kVertex.end());

    // construct supertriangle and add to triangle mesh
    iVQuantity = (int)m_kVertex.size();
    m_aiSuperV[0] = iVQuantity;
    m_aiSuperV[1] = iVQuantity+1;
    m_aiSuperV[2] = iVQuantity+2;
    m_kVertex.push_back(SortedVertex(ContinuousPoint2D(-(double)2.0,-(double)2.0),
        m_aiSuperV[0]));
    m_kVertex.push_back(SortedVertex(ContinuousPoint2D(+(double)5.0,-(double)2.0),
        m_aiSuperV[1]));
    m_kVertex.push_back(SortedVertex(ContinuousPoint2D(-(double)2.0,+(double)5.0),
        m_aiSuperV[2]));

    Triangle* pkTri = new Triangle(m_aiSuperV[0],m_aiSuperV[1],
        m_aiSuperV[2],NULL,NULL,NULL);
    m_kTriangle.insert(pkTri);
    m_apkSuperT[0] = pkTri;
    m_apkSuperT[1] = pkTri;
    m_apkSuperT[2] = pkTri;

    // incrementally update the triangulation
    for (i = 0; i < iVQuantity; i++)
    {
        // construct the insertion polygon
        TSet kPolyTri;
        GetInsertionPolygon(m_kVertex[i].m_kV,kPolyTri);

        EArray kPoly;
        GetInsertionPolygonEdges(kPolyTri,kPoly);

        // add new triangles formed by the point and insertion polygon edges
        AddTriangles(i,kPoly);

        // remove insertion polygon
        RemoveInsertionPolygon(kPolyTri);
    }

    // remove triangles sharing a vertex of the supertriangle
    RemoveTriangles();

    // assign integer values to the triangles for use by the caller
	std::map<Triangle*,int> kPermute;
    TSetIterator pkTIter = m_kTriangle.begin();
    for (i = 0; pkTIter != m_kTriangle.end(); pkTIter++)
    {
        pkTri = *pkTIter;
        kPermute[pkTri] = i++;
    }
    kPermute[NULL] = -1;

    // put Delaunay triangles into an array (vertices and adjacency info)
    riTQuantity = (int)m_kTriangle.size();
    if ( riTQuantity > 0 )
    {

		Triangles.resize(riTQuantity); //reserve space for triangles
		
		PointIdsContainer PointIds(3); //for storing triangle vertices
		//PointIds.resize(3);  //reserve space for vertices
        
		
//		raiTVertex = new int[3*riTQuantity];
//      raiTAdjacent = new int[3*riTQuantity];
        i = 0;
        pkTIter = m_kTriangle.begin();
        for (/**/; pkTIter != m_kTriangle.end(); pkTIter++)
        {
            pkTri = *pkTIter;
//            raiTVertex[i] = m_kVertex[pkTri->m_aiV[0]].m_iIndex;
//            raiTAdjacent[i++] = kPermute[pkTri->m_apkAdj[0]];
//            raiTVertex[i] = m_kVertex[pkTri->m_aiV[1]].m_iIndex;
//            raiTAdjacent[i++] = kPermute[pkTri->m_apkAdj[1]];
//            raiTVertex[i] = m_kVertex[pkTri->m_aiV[2]].m_iIndex;
//            raiTAdjacent[i++] = kPermute[pkTri->m_apkAdj[2]];

			PointIds[0]=m_kVertex[pkTri->m_aiV[0]].m_iIndex; //1st vertex
			PointIds[1]=m_kVertex[pkTri->m_aiV[1]].m_iIndex; //2nd vertex
			PointIds[2]=m_kVertex[pkTri->m_aiV[2]].m_iIndex; //3rd vertex
				
			Triangles[i++]=PointIds;

			//adjacent triangles can be stired like this
			//
			//PointIds[0]=kPermute[pkTri->m_apkAdj[0]]; 
			//PointIds[1]=kPermute[pkTri->m_apkAdj[1]];
			//PointIds[2]=kPermute[pkTri->m_apkAdj[2]];
			//
			// Careful with "i++" here, remove previous one
			// also allocate space in the beginning like for Triangles
			//raiTAdjacent[i++]=PointIds;


        }
        assert( i == riTQuantity ); //was i == 3*riTQuantity
    }
}
//----------------------------------------------------------------------------
blDelaunay2D::~blDelaunay2D ()
{
    TSetIterator pkTIter = m_kTriangle.begin();
    for (/**/; pkTIter != m_kTriangle.end(); pkTIter++)
        delete *pkTIter;
}
//----------------------------------------------------------------------------
blDelaunay2D::Triangle*
blDelaunay2D::GetContaining (const ContinuousPoint2D& rkP) const
{
    // start with supertriangle <S0,S1,V>
    Triangle* pkTri = m_apkSuperT[0];
    assert( pkTri->m_aiV[0] == m_aiSuperV[0]
        &&  pkTri->m_aiV[1] == m_aiSuperV[1] );

    const ContinuousPoint2D& rkS1 = m_kVertex[m_aiSuperV[1]].m_kV;
    ContinuousPoint2D kDiff = rkP - rkS1;
    int iSIndex = 1;
    int iVIndex = 2;  // local index following that of S1

    // The search should never iterate over more than all the triangles.  But
    // just to be safe...
    for (int i = 0; i < (int)m_kTriangle.size(); i++)
    {
        // test if P is inside the triangle
        ContinuousPoint2D kVmS1 = m_kVertex[pkTri->m_aiV[iVIndex]].m_kV - rkS1;
        double fKross = kVmS1.X()*kDiff.Y() - kVmS1.Y()*kDiff.X();
// TODO::MODIFIED BY XAVI
		if ( fabs(fKross)<DBL_EPSILON ) fKross = 0;
///////////
// TODO::COMMENTED BY XAVI
// 		if ( abs(fKross)<DBL_EPSILON ) fKross = 0;
/////////////////////////

        if ( fKross >= (double)0.0 )
            return pkTri;

        pkTri = pkTri->m_apkAdj[iSIndex];
        assert( pkTri );
        if ( pkTri->m_aiV[0] == m_aiSuperV[1] )
        {
            iSIndex = 0;
            iVIndex = 1;
        }
        else if ( pkTri->m_aiV[1] == m_aiSuperV[1] )
        {
            iSIndex = 1;
            iVIndex = 2;
        }
        else if ( pkTri->m_aiV[2] == m_aiSuperV[1] )
        {
            iSIndex = 2;
            iVIndex = 0;
        }
        else
        {
            assert( false );
        }
    }

    // by construction, point must be in some triangle in the mesh
    assert( false );
    return pkTri;
}
//----------------------------------------------------------------------------
bool blDelaunay2D::IsInsertionComponent (const ContinuousPoint2D& rkV,
    Triangle* pkTri) const
{
    // determine the number of vertices in common with the supertriangle
    int iCommon = 0, j = -1;
    for (int i = 0; i < 3; i++)
    {
        int iV = pkTri->m_aiV[i];
        if ( iV == m_aiSuperV[0] )
        {
            iCommon++;
            j = i;
        }
        if ( iV == m_aiSuperV[1] )
        {
            iCommon++;
            j = i;
        }
        if ( iV == m_aiSuperV[2] )
        {
            iCommon++;
            j = i;
        }
    }

    if ( iCommon == 0 )
        return pkTri->PointInCircle(rkV,m_kVertex);

    if ( iCommon == 1 )
        return pkTri->PointLeftOfEdge(rkV,m_kVertex,(j+1)%3,(j+2)%3);

    return pkTri->PointInTriangle(rkV,m_kVertex);
}
//----------------------------------------------------------------------------
void blDelaunay2D::GetInsertionPolygon (const ContinuousPoint2D& rkV,
    TSet& rkPolyTri) const
{
    // locate triangle containing input point, add to insertion polygon
    Triangle* pkTri = GetContaining(rkV);
    //assert( IsInsertionComponent(rkV,pkTri) );
    rkPolyTri.insert(pkTri);

    // breadth-first search for other triangles in the insertion polygon
    TSet kInterior, kBoundary;
    kInterior.insert(pkTri);
    Triangle* pkAdj;
    int i;
    for (i = 0; i < 3; i++)
    {
        pkAdj = pkTri->m_apkAdj[i];
        if ( pkAdj )
            kBoundary.insert(pkAdj);
    }

    while ( kBoundary.size() > 0 )
    {
        TSet kExterior;

        // process boundary triangles
        TSetIterator pkIter;
        for (pkIter = kBoundary.begin(); pkIter != kBoundary.end(); pkIter++)
        {
            // current boundary triangle to process
            pkTri = *pkIter;
            if ( IsInsertionComponent(rkV,pkTri) )
            {
                // triangle is an insertion component
                rkPolyTri.insert(pkTri);

                // locate adjacent, exterior triangles for later processing
                for (i = 0; i < 3; i++)
                {
                    pkAdj = pkTri->m_apkAdj[i];
                    if ( pkAdj
                    &&   kInterior.find(pkAdj) == kInterior.end()
                    &&   kBoundary.find(pkAdj) == kBoundary.end() )
                    {
                        kExterior.insert(pkAdj);
                    }
                }
            }
        }

        // boundary triangles processed, move them to interior
        for (pkIter = kBoundary.begin(); pkIter != kBoundary.end(); pkIter++)
            kInterior.insert(*pkIter);

        // exterior triangles are next in line to be processed
        kBoundary = kExterior;
    }

    // If the input point is outside the current convex hull, triangles
    // sharing a supertriangle edge have to be added to the insertion polygon
    // if the non-supertriangle vertex is shared by two edges that are visible
    // to the input point.
    for (i = 0; i < 3; i++)
    {
        pkTri = m_apkSuperT[i];
        if ( rkPolyTri.find(pkTri->m_apkAdj[1]) != rkPolyTri.end()
        &&   rkPolyTri.find(pkTri->m_apkAdj[2]) != rkPolyTri.end() )
        {
            rkPolyTri.insert(pkTri);
        }
    }
}
//----------------------------------------------------------------------------
void blDelaunay2D::GetInsertionPolygonEdges (TSet& rkPolyTri,
    EArray& rkPoly) const
{
    // locate edges of the insertion polygon
    EMap kIPolygon;
    int iV0, iV1;
    Triangle* pkTri;
    Triangle* pkAdj;
    TSetIterator pkTIter;
    for (pkTIter = rkPolyTri.begin(); pkTIter != rkPolyTri.end(); pkTIter++)
    {
        // get an insertion polygon triangle
        pkTri = *pkTIter;

        // If an adjacent triangle is not an insertion polygon triangle, then
        // the shared edge is a boundary edge of the insertion polygon.  Use
        // this edge to create a new Delaunay triangle from the input vertex
        // and the shared edge.
        for (int j = 0; j < 3; j++)
        {
            pkAdj = pkTri->m_apkAdj[j];
            if ( pkAdj )
            {
                if ( rkPolyTri.find(pkAdj) == rkPolyTri.end() )
                {
                    // adjacent triangle is not part of insertion polygon
                    iV0 = pkTri->m_aiV[j];
                    iV1 = pkTri->m_aiV[(j+1)%3];
                    kIPolygon[iV0] = Edge(iV0,iV1,pkTri,pkAdj);
                }
            }
            else
            {
                // no adjacent triangle, edge is part of insertion polygon
                iV0 = pkTri->m_aiV[j];
                iV1 = pkTri->m_aiV[(j+1)%3];
                kIPolygon[iV0] = Edge(iV0,iV1,pkTri,pkAdj);
            }
        }
    }

    // insertion polygon should be at least a triangle
    int iSize = (int)kIPolygon.size();
    assert( iSize >= 3 );

    // sort the edges
    EMap::iterator pkE = kIPolygon.begin();
    iV0 = pkE->second.m_iV0;
    for (int i = 0; i < iSize; i++)
    {
        iV1 = pkE->second.m_iV1;
        rkPoly.push_back(pkE->second);
        pkE = kIPolygon.find(iV1);
        assert( pkE != kIPolygon.end() );
    }
    assert( iV1 == iV0 );
}
//----------------------------------------------------------------------------
void blDelaunay2D::AddTriangles (int iV2, const EArray& rkPoly)
{
    // create new triangles
    int iSize = (int)rkPoly.size();
    TArray kTriangle(iSize);
    Triangle* pkTri;
    Triangle* pkAdj;
    int i, j;
    for (i = 0; i < iSize; i++)
    {
        const Edge& rkE = rkPoly[i];

        // construct new triangle
        pkTri = new Triangle(rkE.m_iV0,rkE.m_iV1,iV2,rkE.m_pkA,NULL,NULL);
        kTriangle[i] = pkTri;
        if ( rkE.m_iV0 == m_aiSuperV[0] && rkE.m_iV1 == m_aiSuperV[1] )
            m_apkSuperT[0] = pkTri;
        else if ( rkE.m_iV0 == m_aiSuperV[1] && rkE.m_iV1 == m_aiSuperV[2] )
            m_apkSuperT[1] = pkTri;
        else if ( rkE.m_iV0 == m_aiSuperV[2] && rkE.m_iV1 == m_aiSuperV[0] )
            m_apkSuperT[2] = pkTri;

        // update information of triangle adjacent to insertion polygon
        pkAdj = rkE.m_pkA;
        if ( pkAdj )
        {
            for (j = 0; j < 3; j++)
            {
                if ( pkAdj->m_apkAdj[j] == rkE.m_pkT )
                {
                    pkAdj->m_apkAdj[j] = pkTri;
                    break;
                }
            }
        }
    }

    // Insertion polygon is a star polygon with center vertex V2.  Finalize
    // the triangles by setting the adjacency information.
    for (i = 0; i < iSize; i++)
    {
        // current triangle
        pkTri = kTriangle[i];

        // connect to next new triangle
        int iNext = i+1;
        if ( iNext == iSize )
            iNext = 0;
        pkTri->m_apkAdj[1] = kTriangle[iNext];

        // connect to previous new triangle
        int iPrev = i-1;
        if ( iPrev == -1 )
            iPrev = iSize-1;
        pkTri->m_apkAdj[2] = kTriangle[iPrev];
    }

    // add the triangles to the mesh
    for (i = 0; i < iSize; i++)
        m_kTriangle.insert(kTriangle[i]);
}
//----------------------------------------------------------------------------
void blDelaunay2D::RemoveInsertionPolygon (TSet& rkPolyTri)
{
    TSetIterator pkTIter;
    for (pkTIter = rkPolyTri.begin(); pkTIter != rkPolyTri.end(); pkTIter++)
    {
        Triangle* pkTri = *pkTIter;
        m_kTriangle.erase(pkTri);
        delete pkTri;
    }
}
//----------------------------------------------------------------------------
void blDelaunay2D::RemoveTriangles ()
{
    // identify those triangles sharing a vertex of the supertriangle
    TSet kRemoveTri;
    Triangle* pkTri;
    TSetIterator pkTIter = m_kTriangle.begin();
    for (/**/; pkTIter != m_kTriangle.end(); pkTIter++)
    {
        pkTri = *pkTIter;
        for (int j = 0; j < 3; j++)
        {
            int iV = pkTri->m_aiV[j];
            if ( iV == m_aiSuperV[0]
            ||   iV == m_aiSuperV[1]
            ||   iV == m_aiSuperV[2] )
            {
                kRemoveTri.insert(pkTri);
                break;
            }
        }
    }

    // remove the triangles from the mesh
    pkTIter = kRemoveTri.begin();
    for (/**/; pkTIter != kRemoveTri.end(); pkTIter++)
    {
        pkTri = *pkTIter;
        m_kTriangle.erase(pkTri);
        delete pkTri;
    }
}
//----------------------------------------------------------------------------

