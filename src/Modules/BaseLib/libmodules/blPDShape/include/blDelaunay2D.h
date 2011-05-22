/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

// Port from wmlDelaunay2a.h from Magic Software, Inc.
//
// http://www.magic-software.com
// http://www.wild-magic.com
// Copyright (c) 2003.  All Rights Reserved
//
// The Wild Magic Library (WML) source code is supplied under the terms of
// the license agreement http://www.magic-software.com/License/WildMagic.pdf
// and may not be copied or disclosed except in accordance with the terms of
// that agreement.

#ifndef blDELAUNAY2D_H
#define blDELAUNAY2D_H

#include <stdlib.h>

//#include "blDefines.h"
#include "blPDShapeDefines.h"

#include "blPDShape2D.h"

#include <map>
#include <vector>
#include <set>
#include <algorithm>


//----------------------------------------------------
class blDelaunay2D
//----------------------------------------------------
/**
 * \brief blDelaunay2D class implements 2D Delaunay triangulation 
 * \ingroup blPDShape
 *	
 * This class is modified version of the class wmlDelaunay2a from  
 * Wild Magic library
 * Possibility to return information about adjacent triangles 
 * was disabled, but can be easily enabled. By adding more parameters passed to
 * the constructor and uncommenting several lines in constructor's implementation
 * 
 */
{
public:
	typedef blPDShape2D::ShapeCellsContainer ShapeCellsContainer;
	typedef blPDShape2D::PointIdsContainer PointIdsContainer;


    /// The number of triangles in the Delaunay triangulation is returned in
    /* Triangles. 
	 *
	 * The i-th triangle has vertices 2D_points[raiTVertex[i]] 
	 * akVertex[2D_points[i+1]], 
     * akVertex[2D_points[i+2]].  
	 * 
	 * VectorOf2DContinuousPoints 2D_points=(VectorOf2DContinuousPoints)&points[0]
	 *
	 * There is a possibility to store also information
     * obtained during the construction:  
	 *		triples of indices, each triple consisting of indices to adjacent
     * triangles.  The i-th triangle has edge index pairs
     *   edge0 = <2D_points[i],2D_points[i+1]>
     *   edge1 = <2D_points[i+1],2D_points[i+2]>
     *   edge2 = <2D_points[i+2],2D_points[i]>
     * The triangle adjacent to these edges have indices (raiTAdjacent array of adjacent triangles)
     *   adj0 = raiTAdjacent[i]
     *   adj1 = raiTAdjacent[i+1]
     *   adj2 = raiTAdjacent[i+2]
     * If there is no adjacent triangle, the index in raiTAdjacent is -1.
     * This thing is disabled at the moment, but it can be easily turned on 
     * 
	 *	
	 * if bounds are supplied by caller - use them, if not - recalculate 
	 *
	 */
    blDelaunay2D (const vnlVectorDouble& points, ShapeCellsContainer& Triangles,
					const BoundingBox2D* bounds=NULL);

    virtual ~blDelaunay2D ();

protected:
    // for sorting to remove duplicate input points
	//-------------------------------------------
    class SortedVertex
	//-------------------------------------------
	/** 
	*
	*  SortedVertex describes a vertex with the operators of comparison with other vertices
	*	
	*/
    {
    public:
        SortedVertex () { /**/ }

        SortedVertex (const ContinuousPoint2D& rkV, int iIndex)
        :
        m_kV(rkV)
        {
            m_iIndex = iIndex;
        }

        bool operator== (const SortedVertex& rkSV) const
        {
            return m_kV == rkSV.m_kV;
        }

        bool operator!= (const SortedVertex& rkSV) const
        {
            return !(m_kV == rkSV.m_kV);
        }

        bool operator<  (const SortedVertex& rkSV) const
        {
            if ( m_kV.X() < rkSV.m_kV.X() )
                return true;
            if ( m_kV.X() > rkSV.m_kV.X() )
                return false;
            return m_kV.Y() < rkSV.m_kV.Y();
        }

		ContinuousPoint2D m_kV;
        int m_iIndex;
    };
    typedef  std::vector<SortedVertex> SVArray;

    // triangles
	//-----------------------------------------
    class Triangle
	//-----------------------------------------
	/**
	*
	*  Class to represent triangle
	*
	*
	*/
    {
    public:
        Triangle ()
        {
            for (int i = 0; i < 3; i++)
            {
                m_aiV[i] = -1;
                m_apkAdj[i] = NULL;
            }
        }

        Triangle (int iV0, int iV1, int iV2, Triangle* pkA0, Triangle* pkA1,
            Triangle* pkA2)
        {
            m_aiV[0] = iV0;
            m_aiV[1] = iV1;
            m_aiV[2] = iV2;
            m_apkAdj[0] = pkA0;
            m_apkAdj[1] = pkA1;
            m_apkAdj[2] = pkA2;
        }

        bool PointInCircle (const ContinuousPoint2D& rkP, const SVArray& rkVertex)
            const
        {
            // assert: <V0,V1,V2> is counterclockwise ordered
            const ContinuousPoint2D& rkV0 = rkVertex[m_aiV[0]].m_kV;
            const ContinuousPoint2D& rkV1 = rkVertex[m_aiV[1]].m_kV;
            const ContinuousPoint2D& rkV2 = rkVertex[m_aiV[2]].m_kV;

            double dV0x = (double) rkV0.X();
            double dV0y = (double) rkV0.Y();
            double dV1x = (double) rkV1.X();
            double dV1y = (double) rkV1.Y();
            double dV2x = (double) rkV2.X();
            double dV2y = (double) rkV2.Y();
            double dV3x = (double) rkP.X();
            double dV3y = (double) rkP.Y();

            double dR0Sqr = dV0x*dV0x + dV0y*dV0y;
            double dR1Sqr = dV1x*dV1x + dV1y*dV1y;
            double dR2Sqr = dV2x*dV2x + dV2y*dV2y;
            double dR3Sqr = dV3x*dV3x + dV3y*dV3y;

            double dDiff1x = dV1x - dV0x;
            double dDiff1y = dV1y - dV0y;
            double dRDiff1 = dR1Sqr - dR0Sqr;
            double dDiff2x = dV2x - dV0x;
            double dDiff2y = dV2y - dV0y;
            double dRDiff2 = dR2Sqr - dR0Sqr;
            double dDiff3x = dV3x - dV0x;
            double dDiff3y = dV3y - dV0y;
            double dRDiff3 = dR3Sqr - dR0Sqr;

            double dDet =
                dDiff1x*(dDiff2y*dRDiff3 - dRDiff2*dDiff3y) -
                dDiff1y*(dDiff2x*dRDiff3 - dRDiff2*dDiff3x) +
                dRDiff1*(dDiff2x*dDiff3y - dDiff2y*dDiff3x);

            return dDet <= 0.0;
        }

        bool PointLeftOfEdge (const ContinuousPoint2D& rkP,
            const SVArray& rkVertex, int i0, int i1) const
        {
            const ContinuousPoint2D& rkV0 = rkVertex[m_aiV[i0]].m_kV;
            const ContinuousPoint2D& rkV1 = rkVertex[m_aiV[i1]].m_kV;

            double dV0x = (double) rkV0.X();
            double dV0y = (double) rkV0.Y();
            double dV1x = (double) rkV1.X();
            double dV1y = (double) rkV1.Y();
            double dV2x = (double) rkP.X();
            double dV2y = (double) rkP.Y();

            double dEdgex = dV1x - dV0x;
            double dEdgey = dV1y - dV0y;
            double dDiffx = dV2x - dV0x;
            double dDiffy = dV2y - dV0y;

            double dKross = dEdgex*dDiffy - dEdgey*dDiffx;
            return dKross >= 0.0;
        }

        bool PointInTriangle (const ContinuousPoint2D& rkP,
            const SVArray& rkVertex) const
        {
            // assert: <V0,V1,V2> is counterclockwise ordered
            const ContinuousPoint2D& rkV0 = rkVertex[m_aiV[0]].m_kV;
            const ContinuousPoint2D& rkV1 = rkVertex[m_aiV[1]].m_kV;
            const ContinuousPoint2D& rkV2 = rkVertex[m_aiV[2]].m_kV;

            double dV0x = (double) rkV0.X();
            double dV0y = (double) rkV0.Y();
            double dV1x = (double) rkV1.X();
            double dV1y = (double) rkV1.Y();
            double dV2x = (double) rkV2.X();
            double dV2y = (double) rkV2.Y();
            double dV3x = (double) rkP.X();
            double dV3y = (double) rkP.Y();

            double dEdgex = dV1x - dV0x;
            double dEdgey = dV1y - dV0y;
            double dDiffx = dV3x - dV0x;
            double dDiffy = dV3y - dV0y;

            double dKross = dEdgex*dDiffy - dEdgey*dDiffx;
            if ( dKross < 0.0 )
            {
                // P right of edge <V0,V1>, so outside the triangle
                return false;
            }

            dEdgex = dV2x - dV1x;
            dEdgey = dV2y - dV1y;
            dDiffx = dV3x - dV1x;
            dDiffy = dV3y - dV1y;
            dKross = dEdgex*dDiffy - dEdgey*dDiffx;
            if ( dKross < 0.0 )
            {
                // P right of edge <V1,V2>, so outside the triangle
                return false;
            }

            dEdgex = dV0x - dV2x;
            dEdgey = dV0y - dV2y;
            dDiffx = dV3x - dV2x;
            dDiffy = dV3y - dV2y;
            dKross = dEdgex*dDiffy - dEdgey*dDiffx;
            if ( dKross < 0.0 )
            {
                // P right of edge <V2,V0>, so outside the triangle
                return false;
            }

            // P left of all edges, so inside the triangle
            return true;
        }


        // vertices, listed in counterclockwise order
        int m_aiV[3];

        // adjacent triangles,
        //   a[0] points to triangle sharing edge (v[0],v[1])
        //   a[1] points to triangle sharing edge (v[1],v[2])
        //   a[2] points to triangle sharing edge (v[2],v[0])
        Triangle* m_apkAdj[3];
    };

    typedef  std::set<Triangle*> TSet;
    typedef  TSet::iterator TSetIterator;
    typedef  std::vector<Triangle*> TArray;

    // edges (to support constructing the insertion polygon)
	//-------------------------------------------------
    class Edge
	//-------------------------------------------------
	/**
	*
	* Class for describing triangle edges
	*
	*
	*/
    {
    public:
        Edge (int iV0 = -1, int iV1 = -1, Triangle* pkT = NULL,
            Triangle* pkA = NULL)
        {
            m_iV0 = iV0;
            m_iV1 = iV1;
            m_pkT = pkT;
            m_pkA = pkA;
        }


        int m_iV0, m_iV1;  // ordered vertices
        Triangle* m_pkT;   // insertion polygon triangle
        Triangle* m_pkA;   // triangle adjacent to insertion polygon
    };

    typedef  std::map<int,Edge> EMap;  // <V0,(V0,V1,T,A)>
    typedef  std::vector<Edge> EArray;  // (V0,V1,T,A)

    Triangle* GetContaining (const ContinuousPoint2D& rkP) const;
    bool IsInsertionComponent (const ContinuousPoint2D& rkV, Triangle* pkTri)
        const;
    void GetInsertionPolygon (const ContinuousPoint2D& rkV, TSet& rkPolyTri)
        const;
    void GetInsertionPolygonEdges (TSet& rkPolyTri, EArray& rkPoly) const;
    void AddTriangles (int iV2, const EArray& rkPoly);
    void RemoveInsertionPolygon (TSet& rkPolyTri);
    void RemoveTriangles ();

    // sorted input vertices for processing
    SVArray m_kVertex;

    // indices for the supertriangle vertices
    int m_aiSuperV[3];

    // triangles that contain a supertriangle edge
    Triangle* m_apkSuperT[3];

    // the current triangulation
    TSet m_kTriangle;
};


#endif
