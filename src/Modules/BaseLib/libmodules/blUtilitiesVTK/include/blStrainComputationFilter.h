// Copyright 2006 Pompeu Fabra University (Computational Imaging Laboratory), Barcelona, Spain. Web: www.cilab.upf.edu.
// This software is distributed WITHOUT ANY WARRANTY; 
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _blStrainComputationFilter_H
#define _blStrainComputationFilter_H

#include "BaseLibVTKWin32Header.h"

#include "itkProcessObject.h"
#include "itkDataObject.h"

#include "blDataWrappers.h"

#include "vtkPolyData.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyDataNormals.h"
#include "vtkCell.h"
#include "vtkCellData.h"
#include "vtkCellType.h"

#include <vnl/vnl_vector.h>	


/**
\brief Compte the strain of a vector of input meshes
The position 0 of the input vector should be the input mesh.
The strain of the input mesh will be 0.

\author Xavi Planes
\ingroup RegLib
\date 07 August 2009
*/
class BASELIBVTK_EXPORT blStrainComputationFilter : public itk::ProcessObject
{
protected:
	//! Image dimension is currently set to 3 by default. 
	const static unsigned int m_ImageDimension = 3;

	//! Struct with all strain values
	struct StrainValues{

		double currentStrainValRad;
		double currentStrainValCirc;
		double currentStrainValLong;

		StrainValues( )
		{
			currentStrainValRad = 0.;
			currentStrainValCirc = 0.;
			currentStrainValLong = 0.;
		}
	};
public:
	//! Type of strains
	enum DirectionStrainProjection {
		Radial,
		Longitudinal,
		Circumferential,
	};

	enum AxisComputationMethod {
		acmOldMathieu,
		acmAxesLocal,  //crashes somewhere
		acmAxesGlobal  //crashes somewhere	
	};

	typedef blStrainComputationFilter Self;
	typedef itk::ProcessObject ParentType;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self>  ConstPointer;
	typedef blITKVtkPolyData MeshType;
	typedef blITKVtkPolyDataVector MeshCollectionType; 
	
	///** Run-time type information (and related methods). */
	itkTypeMacro(blStrainComputationFilter,ParentType);
	itkFactorylessNewMacro(blStrainComputationFilter);

public:
	//! Constructor
	blStrainComputationFilter();

	//! This filter takes two inputs: a mesh and a transformation
	virtual void SetInput(
		const MeshCollectionType* inputMesh );

	//! Access the input of the filter (wrapper object around vtkPolyData)
	MeshCollectionType* GetInput();

	//! 
	virtual void Update();

	AxisComputationMethod GetAxisComputationMethod() const { return m_AxisCompMethod; }
	void SetAxisComputationMethod( AxisComputationMethod val ) { m_AxisCompMethod = val; }

	void SetApex(vnl_vector<double> apex)
		{m_Apex=apex;}

	void SetMithral(vnl_vector<double> mithral)
		{m_Mithral=mithral;}


protected:
	//!
	virtual void GenerateData();
	
	//! This method is called by Generate Data 
	virtual void ComputeStrain();

	//! Initial preprocess for each time step
	void PreProcessTimeStep( );

	/**
	\param [in] inputMesh input mesh (time step 0)
	\param [in] cellId Id of the current input mesh
	\param [in] cell cell of the current mesh (time step > 0)
	*/
	
	virtual void ComputeStrainOnMesh( 
		vtkPolyData* inputMesh,
		unsigned int cellId,
		vtkCell* cell,
		vnl_vector<double>  &radialDirection,
		vnl_vector<double>  &circumDirection,
		vnl_vector<double>  &longDirection,
		StrainValues &result );



	void GetLVLongRadCircDirectionsLocal(vtkPolyData* lv, long int cell_id, const vnl_vector<double>& apex, const vnl_vector<double>& mithral, vnl_vector<double>& radial, vnl_vector<double>& circum, vnl_vector<double>& longit);

	void GetLVLongRadCircDirectionsGlobal(vtkPolyData* lv, long int cell_id, const vnl_vector<double>& apex, const vnl_vector<double>& mithral, vnl_vector<double>& radial, vnl_vector<double>& circum, vnl_vector<double>& longit);

	void CorrectNormal( vnl_vector<double>& normal, const vnl_vector<double>& apex, 
		const vnl_vector<double>& mithral, const vnl_vector<double> vert );


protected:
	//! Filter for computing normal directions
	vtkSmartPointer<vtkPolyDataNormals> m_NormalsFilter;

	//!
	unsigned int m_CurrentTimeStep;

	AxisComputationMethod m_AxisCompMethod;

	vnl_vector<double> m_Apex;
	vnl_vector<double> m_Mithral;
};



#endif //_blStrainComputationFilter_H
