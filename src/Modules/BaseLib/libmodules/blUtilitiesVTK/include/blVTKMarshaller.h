/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef BLVTKTOSTRINGCONVERTER_H
#define BLVTKTOSTRINGCONVERTER_H

#include "BaseLibVTKWin32Header.h"
#include "CILabNamespaceMacros.h"
#include <vtkPolyData.h>
#include <vtkStructuredPoints.h>
#include <set>

CILAB_BEGIN_NAMESPACE(blVTKHelperTools)

/**
This struct stores the raw C string representation of a VTK object that can be sent across a dll boundary.
Different VTK versions may be used on either side of the dll boundary.
A VTKPackage is created on one side of the dll and is consumed on the other side.

\author Maarten Nieber
\date 24 feb 2009
*/

struct VTKPackage
{
	typedef char* RawStringType;
	VTKPackage(RawStringType argObjectAsString, int argLength) : objectAsString(argObjectAsString), length(argLength)
	{
	}

	RawStringType objectAsString;
	int length;
};

/**
This class creates a VTKPackage from a VTK object.

\author Maarten Nieber
\date 24 feb 2009
*/

class BASELIBVTK_EXPORT VTKMarshaller
{
public:
	//!
	VTKMarshaller();
	//! Deletes all the raw C string representations for packages that were created with this marshaller and not 
	//! yet Discarded with the Discard function.
	~VTKMarshaller();
	//! Creates a VTKPackage from \a pd, deletes \a pd and sets it to NULL.
	VTKPackage ConvertIntoPackageAndDelete(vtkPolyData*& pd);
	//! Creates a VTKPackage from \a pd.
	VTKPackage ConvertIntoPackage(vtkPolyData* pd);
	//! Creates a VTKPackage from \a sp, deletes \a sp and sets it to NULL.
	VTKPackage ConvertIntoPackageAndDelete(vtkStructuredPoints*& sp);
	//! Creates a VTKPackage from \a sp.
	VTKPackage ConvertIntoPackage(vtkStructuredPoints* pd);
	//! Deletes all the raw C string representation for \a package.
	void Discard(const VTKPackage& package);

private:
	//! Helper function
	VTKPackage ConvertIntoPackageAndDeleteImpl( vtkPolyData*& pd, bool deletePd );
	//! Helper function
	VTKPackage ConvertIntoPackageAndDeleteImpl( vtkStructuredPoints*& pd, bool deletePd );

private:
	//!
	typedef std::set<VTKPackage::RawStringType> RawStringSet;
	//! All raw C string representations created by this class are stored, and this class is responsible for 
	//! deleting them.
	RawStringSet m_RawStrings;
};

/**
This class does the inverse operation of VTKMarshaller. The client becomes the owner of the created vtk objects.

\author Maarten Nieber
\date 24 feb 2009
*/

class BASELIBVTK_EXPORT VTKUnMarshaller
{
public:
	//!
	VTKUnMarshaller();
	//! The client becomes the owner of the created vtk object.
	vtkPolyData* ConvertToPolyData(const VTKPackage& input);
	//! The client becomes the owner of the created vtk object.
	vtkStructuredPoints* ConvertToStructuredPoints(const VTKPackage& input);
};

CILAB_END_NAMESPACE(blVTKHelperTools)

#endif //BLVTKTOSTRINGCONVERTER_H
