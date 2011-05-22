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

#ifndef __meVTKPolyLineCombine_h
#define __meVTKPolyLineCombine_h

// ITK
#include <itkLightObject.h>
// VTK
#include <vtkPolyData.h>

using namespace itk;

/**
* \brief 
*
*/

class meVTKPolyLineCombine: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyLineCombine  Self;
		typedef SmartPointer<Self>             Pointer;
		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input VTK PolyLine to be converted into ITK PolyLine.
		void SetInput( vtkPolyData *input )
		{ this->input = input; }

		/// \brief Combines twwo cells, they must have a common end point.
		void CombineByCellId(vtkIdType cellId1, vtkIdType cellId2)
		{this->cellId1=cellId1;this->cellId2=cellId2;}

		/// \brief Get output VTK PolyLine.
		vtkPolyData *GetOutput()
		{ return this->output; }

		void Update();


	protected:

		meVTKPolyLineCombine();
		~meVTKPolyLineCombine();

		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *output;

		vtkIdType cellId1;
		vtkIdType cellId2;

	private:

		meVTKPolyLineCombine( const meVTKPolyLineCombine& );  //purposely not implemented
		void operator = ( const meVTKPolyLineCombine& );  //purposely not implemented
};

#endif //__meVTKPolyLineCombine_h
