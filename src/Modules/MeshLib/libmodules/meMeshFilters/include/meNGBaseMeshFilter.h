/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _meNGBaseMeshFilter_h
#define _meNGBaseMeshFilter_h

// BaseLib
#include "BaseLibWin32Header.h"
#include <blLightObject.h>
#include <blMacro.h>

#include "meMeshTypes.h"
#include "meNetgenMesh.h"


#include "nglib.h"


/**
\brief Base class Meshing for editing netgen surfaces

\author Chiara Riccobene 
\date 3 Nov 2009
*/

class  meNGBaseMeshFilter: public blLightObject
{
	public:
		//! Typedefs
		typedef meNGBaseMeshFilter Self;
		typedef blSmartPointer< Self > Pointer;

		//! Static constructor.
		blNewMacro( Self );

		//! Set the NG object input , before Update() has been called.
		void SetInput(meNetgenMesh::Pointer input);

		//! Get the NG object input.
		meNetgenMesh::Pointer GetInput( int id = 0);

		//!
	//	void SetSelectedCellList( vtkSmartPointer<VtkIdList> list );

	protected:
		
		//! brief Default Constructor.
		meNGBaseMeshFilter();

		//! Destructor.
		virtual ~meNGBaseMeshFilter();
		
		//!
		void GenerateSurfaceMesh( nglib::Ng_Meshing_Parameters* mp );

	private:

		//! Purposely not implemented
		meNGBaseMeshFilter( const meNGBaseMeshFilter& );	

		//! Purposely not implemented
		void operator = ( const meNGBaseMeshFilter& );	

		

	private:
	
		//!
		meNetgenMesh::Pointer m_NetgenMesh;
};


#endif // _meNGBaseMeshFilter_h
