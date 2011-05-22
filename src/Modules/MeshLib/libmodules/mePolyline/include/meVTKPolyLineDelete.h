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

#ifndef __meVTKPolyLineDelete_h
#define __meVTKPolyLineDelete_h

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

class meVTKPolyLineDelete: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyLineDelete  Self;
		typedef SmartPointer<Self>             Pointer;
		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input VTK PolyLine to be converted into ITK PolyLine.
		void SetInput( vtkPolyData *input )
		{ this->input = input; }

		// \brief Delete the cells specified in the list.
		void DeleteCells(vtkIdList *cellIds);

		/// \brief Deletes cells with Id = celLId.
		void DeleteCellWithId(vtkIdType cellId);

		/// \brief Deletes cells containing the point pointId.
		void DeleteCellsWithPoint(vtkIdType pointId);

		/// \brief Get output VTK PolyLine.
		vtkPolyData *GetOutput()
		{ return this->output; }

		void Update();


	protected:

		meVTKPolyLineDelete();
		~meVTKPolyLineDelete();
		
		// \brief Selects for deletion all the cells that contaitn a given point
		void SelectCellsFromPoint();

		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *output;

		vtkIdList *cellIds;
		vtkIdType pointId;

	private:

		meVTKPolyLineDelete( const meVTKPolyLineDelete& );  //purposely not implemented
		void operator = ( const meVTKPolyLineDelete& );  //purposely not implemented
};

#endif //__meVTKPolyLineDelete_h
