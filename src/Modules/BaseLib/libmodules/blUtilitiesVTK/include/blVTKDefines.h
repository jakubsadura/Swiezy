/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blVTKDefines_h
#define _blVTKDefines_h

#include <vector>
class vtkPolyData;
class vtkPlane;

namespace blVTKDefines {

	/**
	* \brief vector of shapes
	* \ingroup blVTKDefines
	*/
	typedef std::vector<vtkPolyData*> ShapeVectorType;

	/**
	* \brief vector of planes
	* \ingroup blVTKDefines
	*/
	typedef std::vector<vtkPlane*> PlaneVectorType;

} // end namespace blVTKDefines

#endif // _blVTKDefines_h
