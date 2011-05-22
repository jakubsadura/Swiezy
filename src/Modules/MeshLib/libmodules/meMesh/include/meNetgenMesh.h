/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNetgenMesh_H
#define _meNetgenMesh_H

#include "nglib.h"

/**
Initialize Netgen and de initialize it
\author Xavi Planes
\date 03 nov 2009
\ingroup MeshLib
*/
class NetgenInitialization
{
public:
	//!
	NetgenInitialization( );

	//!
	~NetgenInitialization( );
};


/**
The input geometry Ng_STL_Geometry is used as a geometry to perform all 
operations.

The output mesh Ng_Mesh is the result of the filtering.

\author Xavi Planes
\date 03 nov 2009
\ingroup MeshLib
*/
class meNetgenMesh : public blLightObject
{
public:
	typedef meNetgenMesh Self;
	typedef blSmartPointer<Self> Pointer;
	blNewMacro(Self);

	//!
	nglib::Ng_STL_Geometry * GetGeom() const;
	void SetGeom(nglib::Ng_STL_Geometry * val);
	//! copy the geometry instead of taking the reference(used in setInput of meNGBaseFilter)
	void CopyGeom(nglib::Ng_STL_Geometry* val);
	//!
	nglib::Ng_Mesh * GetNGMesh() const;
	void SetNGMesh(nglib::Ng_Mesh * val);
	//! copy the mesh instead of taking the reference(used in setInput of meNGBaseFilter)
	void CopyMesh( nglib::Ng_Mesh* val );
protected:
	//!
	meNetgenMesh( );

	//!
	~meNetgenMesh( );

protected:
	//! Input geometry (from vtkPolyData for example)
	nglib::Ng_STL_Geometry *m_pGeom;
	//! Output mesh (result of filtering)
	nglib::Ng_Mesh *m_pNGMesh;
	//!
	static NetgenInitialization m_NetgenInitialization;
};



#endif // _meNetgenMesh_H

