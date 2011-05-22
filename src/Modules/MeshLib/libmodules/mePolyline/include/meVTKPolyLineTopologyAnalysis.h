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

#ifndef __meVTKPolyLineTopologyAnalysis_h
#define __meVTKPolyLineTopologyAnalysis_h

// ITK
#include <itkLightObject.h>
// VTK
#include <vtkPolyData.h>
#include <vtkIdList.h>

using namespace itk;

/**
* \brief This filter is intended to claen a skeleton represented a vtkPolyline. 
* It will sistematically look for cells of only two elements thta are terminalSize 
* and remove them.
*/

class meVTKPolyLineTopologyAnalysis: public LightObject
{
	public:

		//-----------------------------------------------------
		// Typedefs
		//-----------------------------------------------------

		typedef meVTKPolyLineTopologyAnalysis  Self;
		typedef SmartPointer<Self>             Pointer;
		//-----------------------------------------------------
		// Methods
		//-----------------------------------------------------

		/// \brief Static constructor
		itkFactorylessNewMacro(Self);

		/// \brief Set input VTK PolyLine to be converted into ITK PolyLine.
		void SetInput( vtkPolyData *input )
		{ this->input = input; }

		/// \brief Get output VTK PolyLine.
		vtkPolyData *GetOutput()
		{ return this->output; }

		void Update();


	protected:

		meVTKPolyLineTopologyAnalysis();
		~meVTKPolyLineTopologyAnalysis();

		void AnalyzePolyline();

		vtkPolyData *input;
		vtkPolyData *intermediateSkeleton;
		vtkPolyData *output;

	private:

		meVTKPolyLineTopologyAnalysis( const meVTKPolyLineTopologyAnalysis& );  //purposely not implemented
		void operator = ( const meVTKPolyLineTopologyAnalysis& );  //purposely not implemented
};

#endif //__meVTKPolyLineTopologyAnalysis_h
