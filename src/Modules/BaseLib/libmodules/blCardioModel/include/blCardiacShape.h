/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blCardiacShape_h
#define _blCardiacShape_h

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------


	#include "BaseLibVTKWin32Header.h"

	#include <vector>

	#include "blLightObject.h"
	#include "blSmartPointer.h"
	#include "blCardioDefines.h"
	#include "CILabExceptionMacros.h"

	#include "vtkSmartPointer.h"

	#include "vnl/vnl_vector.h"
	#include "vnl/vnl_vector_ref.h"

	class vtkPolyData;
	
	//---------------------------------------------------
	// CLASS DEFINITION
	//---------------------------------------------------
	
	/**
	 * \brief vtkPolyData with Cardio::VECTOR_NAME_SUBPART_ID array
	 * \ingroup blCardioModel
	 *
	 */
	class BASELIBVTK_EXPORT blCardiacShape: public blLightObject 
	{

	// ----------------------------------
	// TYPES
	// ----------------------------------
	public:

		typedef blCardiacShape Self;
		typedef blSmartPointer<Self> Pointer;
		blNewMacro(Self);
		blNewMacro1param(Self,vtkPolyData*);

		// Exception
		cilabDeclareExceptionMacro(Exception, std::exception)


	// ----------------------------------
	// OPERATIONS
	// ----------------------------------
	public:
		//!
		vtkPolyData* GetInputShape() const;

		//!
		void SetInputShape(vtkPolyData* val);

		//!
		void SetVectorSubpartName( const char* vectorName );

		/** 
		\brief Get subpart from cardiac model. 
		\note Throw an exception if input or output is NULL
		\note You need to call Delete( ) to the output
		*/
		vtkPolyData* GetSubpart( 
						Cardio::SubpartType subpartIDStart,
						Cardio::SubpartType subpartIDEnd = Cardio::NONE_SUBPART );

		/**
		\brief Remove coronaries from the input mesh
		\note The caller to this function must delete the memory
		*/
		vtkPolyData* RemoveCoronaries( );

		/**
		\brief Get LV from input mesh
		\note The caller to this function must delete the memory
		*/
		vtkPolyData* GetLVChamber( );

		//! Compute the centroid of LV
		void GetLVEndoCentroid( double centroid[ 3 ] );

		//! Compute normals to input shape
		vtkPolyData *ApplyNormalsFilter( );

		//!
		void GetSubpartRange( double range[2] );

		//! Get LV long axis
		void GetLVLongAxis(
			vnl_vector<double>& apex, 
			vnl_vector<double>& mithral);


	protected:
		//!
		blCardiacShape( vtkPolyData *inputShape = NULL );

		//!
		~blCardiacShape();


	private:
		vtkSmartPointer<vtkPolyData>	m_InputShape;

		//!
		std::string m_strVectorSubpartName;
};


#endif
