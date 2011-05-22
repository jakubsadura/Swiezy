/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/
#ifndef coreMeshCreationProcessor_H
#define coreMeshCreationProcessor_H

#include "corePluginMacros.h"
#include "coreBaseFilter.h"

#include "coreVTKPolyDataHolder.h"
#include "coreVTKImageDataHolder.h"

namespace Core
{
/**
\brief Processor class that creates a mesh using Otsu segmentation 
starting from a ROI image.

\sa Core::MeshCreation
\ingroup MeshEditorPlugin
\author Chiara Riccobene
\date 23 June 2009
*/
class PLUGIN_EXPORT MeshCreationProcessor : public Core::BaseProcessor
{
public:
	coreDeclareSmartPointerClassMacro(MeshCreationProcessor, Core::BaseFilter);

	//!
	void Update( );

	//! 
	void SetDelaunayType( bool typeValue);

	//!
	void SetMarchingCubesType( bool typeValue);

protected:
	MeshCreationProcessor(void);
	virtual ~MeshCreationProcessor(void);

	//!
	void GetSurfaceFromSegmentation( 
		vtkImageDataPtr inputImage, 
		vtkPolyDataPtr surface );

private:
	coreDeclareNoCopyConstructors(MeshCreationProcessor);

	//! enables the marching cubes algo
	bool m_bMarchingCubes;

	//! enables the delaunay algo
	bool m_bDelaunay;
};

} // namespace Core

#endif

