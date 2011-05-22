/*=========================================================================

  Module    : MeshLib
  File      : $RCSfile: $
  Copyright : (C)opyright (Pompeu Fabra University) 2007++
              See COPYRIGHT statement in top level directory.
  Authors   : Nacho Larrabide
  Modified  : $Author: $
  Purpose   : 
  Date      : $Date: $
  Version   : $Revision: $
  Changes   : $Locker: $
              $Log: $

=========================================================================*/

#ifndef __meVTKPolyLineSmooth_h
#define __meVTKPolyLineSmooth_h

// ITK
#include <itkLightObject.h>
// VTK
#include <vtkPolyData.h>
#include <vtkIdList.h>

using namespace itk;

/**
* \brief 
*
*/

class meVTKPolyLineSmooth: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyLineSmooth  Self;
		typedef SmartPointer<Self>             Pointer;
		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input VTK PolyLine to be converted into ITK PolyLine.
		void SetInput( vtkPolyData *input )
		{ this->input = input; }

		/// \brief Deletes cells with Id = cellId.
		void SetCellId(vtkIdType cellId)
		{ this->cellId = cellId;}

		/// \brief Deletes cells containing the point pointId.
		void SetIterations(unsigned int iters)
		{ this->iters = iters;}

		/// \brief Get output VTK PolyLine.
		vtkPolyData *GetOutput()
		{ return this->output; }

		void Update();


	protected:

		meVTKPolyLineSmooth();
		~meVTKPolyLineSmooth();
		
		//\brief Smooth a single cell
		void SmoothCell(vtkIdType cellId);
		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *output;

		vtkIdType cellId;
		unsigned int iters;


	private:

		meVTKPolyLineSmooth( const meVTKPolyLineSmooth& );  //purposely not implemented
		void operator = ( const meVTKPolyLineSmooth& );  //purposely not implemented
};

#endif //__meVTKPolyLineSmooth_h
