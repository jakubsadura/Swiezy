/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "meNetgenMesh.h"
#include "nglib.h"

using namespace nglib;

NetgenInitialization meNetgenMesh::m_NetgenInitialization;

NetgenInitialization::NetgenInitialization()
{
	nglib::Ng_Init( );
}

NetgenInitialization::~NetgenInitialization()
{
	// It crashes
	//nglib::Ng_Exit( );
}

meNetgenMesh::meNetgenMesh()
{
	m_pGeom = nglib::Ng_STL_NewGeometry( );
	m_pNGMesh = nglib::Ng_NewMesh( );
}

meNetgenMesh::~meNetgenMesh()
{
	nglib::Ng_STL_DeleteGeometry( m_pGeom );
	m_pGeom = NULL;
	nglib::Ng_DeleteMesh( m_pNGMesh );
	m_pNGMesh = 0;
}

nglib::Ng_STL_Geometry * meNetgenMesh::GetGeom() const
{
	return m_pGeom;
}

void meNetgenMesh::SetGeom( nglib::Ng_STL_Geometry * val )
{
	m_pGeom = val;
}

nglib::Ng_Mesh * meNetgenMesh::GetNGMesh() const
{
	return m_pNGMesh;
}

void meNetgenMesh::SetNGMesh( nglib::Ng_Mesh * val )
{
	m_pNGMesh = val;
}

void meNetgenMesh::CopyGeom(nglib::Ng_STL_Geometry* val)
{
	// The Geometry only contains information about triangles, we have to 
	// perform a Generate Surface to be able to use the functionalities of Netgen

	double p1[3];
	double p2[3];
	double p3[3];
	int temp[3];
	//double nv[3];

	for (	unsigned i = 1; 
	i <  nglib::Ng_STL_GetNumTriangles(  val )+1; 
	i++ )
	{
		nglib::Ng_STL_GetTriangle (  val, i , temp);
		nglib::Ng_STL_GetPoint(val,temp[0],p1);
		nglib::Ng_STL_GetPoint(val,temp[1],p2);
		nglib::Ng_STL_GetPoint(val,temp[2],p3);
		nglib::Ng_STL_AddTriangle(m_pGeom,p1,p2,p3);
	}

	nglib::Ng_STL_InitSTLGeometry(m_pGeom);

}

void meNetgenMesh::CopyMesh( nglib::Ng_Mesh* val )
{
	// The mesh contains the informations about the points the surface elements (triangles)
	// and the elements ( tetras) for the moment this last information is not included in 
	// this implementation 

	if(nglib::Ng_GetNP( val ) == 0)
		return;

	double tempPoint[3];
	// Add the points
	for (	unsigned iNumPoint = 1; 
	iNumPoint < nglib::Ng_GetNP( val )+1 ; 
	iNumPoint++ )
	{
		nglib::Ng_GetPoint( val, iNumPoint , tempPoint );
		nglib::Ng_AddPoint(m_pNGMesh,tempPoint);
	}

	int tempSurfEl[3];
	// Add the surface elements
	for (	unsigned i = 1; 
	i < nglib::Ng_GetNSE( val )+1 ; 
	i++ )
	{
		nglib::Ng_GetSurfaceElement ( val, i , tempSurfEl);
		nglib::Ng_AddSurfaceElement(m_pNGMesh,nglib::NG_TRIG,tempSurfEl);
	}

}

