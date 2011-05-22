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

#ifndef __meVTKPolyLineDecimate_h
#define __meVTKPolyLineDecimate_h

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

class meVTKPolyLineDecimate: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyLineDecimate  Self;
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
		void SetDecimetePercentage(double perc)
		{ this->perc = perc;}

		/// \brief Get output VTK PolyLine.
		vtkPolyData *GetOutput()
		{ return this->output; }

		void Update();


	protected:

		meVTKPolyLineDecimate();
		~meVTKPolyLineDecimate();
		
		double Dist(double *p1, double *p2);
		//-----------------------------------------------------
		// Variables
		//-----------------------------------------------------

		vtkPolyData *input;
		vtkPolyData *output;

		vtkIdType cellId;
		double perc;

	private:

		meVTKPolyLineDecimate( const meVTKPolyLineDecimate& );  //purposely not implemented
		void operator = ( const meVTKPolyLineDecimate& );  //purposely not implemented
};

#endif //__meVTKPolyLineDecimate_h
