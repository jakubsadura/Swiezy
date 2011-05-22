// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _blDataWrappers_H
#define _blDataWrappers_H

#include "BaseLibVTKWin32Header.h"
#include "itkProcessObject.h"
#include "itkDataObject.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

/**
\brief This class is an ITK wrapper for a vtkPolyData object. 
It is used as input of regStrainComputationFilter.

\ingroup blUtilitiesVTK
\author Martin Bianculli
\date 06 Nov 2008
*/

class BASELIBVTK_EXPORT blITKVtkPolyData : public itk::DataObject
{
public:
	//! Standard itk typedefs
	typedef blITKVtkPolyData Self;
	typedef itk::DataObject ParentType;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;

	/** Run-time type information (and related methods). */
	itkTypeMacro(blITKVtkPolyData,ParentType);
	itkFactorylessNewMacro(blITKVtkPolyData);

	//! Holds the vtk poly data
	vtkPolyData* mesh;
};


/**
\brief This class is an ITK container of a vtkPolyData mehses. 
It is used as output in regStrainComputationFilter.
\ingroup regLib

\author Martin Bianculli
\date 06 Nov 2008
*/
class BASELIBVTK_EXPORT blITKVtkPolyDataVector : public itk::DataObject
{
public:
	//! Standard itk typedefs
	typedef blITKVtkPolyDataVector Self;
	typedef itk::DataObject ParentType;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;

	/** Run-time type information (and related methods). */
	itkTypeMacro(blITKVtkPolyDataVector,ParentType);
	itkFactorylessNewMacro(blITKVtkPolyDataVector);

	void SetInput( std::vector< vtkSmartPointer< vtkPolyData > > polyDataVector )
	{
		meshes.resize( polyDataVector.size() );
		for ( int i  = 0 ; i < polyDataVector.size() ; i++ )
		{
			meshes[ i ] = polyDataVector[ i ];
		}
	}

	//! Holds the meshes
	std::vector<vtkPolyData* > meshes;
};





#endif //_regDataWrappers_H
