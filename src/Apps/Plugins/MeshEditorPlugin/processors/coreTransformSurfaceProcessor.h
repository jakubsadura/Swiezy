// Copyright 2007 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _coreTransformSurfaceProcessor_H
#define _coreTransformSurfaceProcessor_H

#include "coreBaseProcessor.h"
#include "coreReportExceptionMacros.h"

namespace Core{

/**
\brief Apply Transform input data (vtkPolyData) using 
vtkTransformPolyDataFilter and two input points

\ingroup MeshEditorPlugin
\author Xavi Planes
\date 28 07 2008
*/
class TransformSurfaceProcessor : public Core::BaseProcessor
{
public:
	typedef enum
	{
		INPUT_SURFACE_MESH,
		INPUT_POINT,
		INPUT_NUM
	}INPUT_TYPE;

public:
	//!
	coreDeclareSmartPointerClassMacro(TransformSurfaceProcessor, Core::BaseProcessor);

	//! Call the Vtk filter
	void Update();


private:
	//!
	TransformSurfaceProcessor( );

	//!
	~TransformSurfaceProcessor();

	//! Purposely not implemented
	TransformSurfaceProcessor( const Self& );

	//! Purposely not implemented
	void operator = ( const Self& );


private:

	//!
	double m_StartingPoint[ 3 ];
};

} // Core

#endif //_coreTransformSurfaceProcessor_H
