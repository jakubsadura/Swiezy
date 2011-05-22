/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __meVTKExtractPolyDataGeometryCellsID_h
#define __meVTKExtractPolyDataGeometryCellsID_h

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include "BaseLibVTKWin32Header.h"
	#include "vtkPolyDataAlgorithm.h"
	#include "vtkIdList.h"
	
	class vtkImplicitFunction;


	//---------------------------------------------------
	// CLASS DEFINITION
	//---------------------------------------------------


	/**
	 * \brief Extract vtkPolyData cells ID that lies either entirely inside or 
	 * outside of a specified implicit function
	 * \ingroup meTools
	 *
	 * This filter is a copy of vtkExtractPolyDataGeometry but, the output
	 * is a vtkIDList of the selected cells id (polygons).
	 *
	 * See Also:
	 *  vtkExtractPolyDataGeometry vtkExtractGeometry vtkClipPolyData
	 */
	class BASELIBVTK_EXPORT meVTKExtractPolyDataGeometryCellsID : public vtkPolyDataAlgorithm
	{


	// ----------------------------------
	// OPERATIONS
	// ----------------------------------
	public:

		vtkTypeRevisionMacro(meVTKExtractPolyDataGeometryCellsID,vtkPolyDataAlgorithm);
		void PrintSelf(ostream& os, vtkIndent indent);

		/// Construct object with ExtractInside turned on.
		static meVTKExtractPolyDataGeometryCellsID *New();

		/// Return the MTime taking into account changes to the implicit function
		unsigned long GetMTime();

		/// Specify the implicit function for inside/outside checks.
		virtual void SetImplicitFunction(vtkImplicitFunction*);
		vtkGetObjectMacro(ImplicitFunction,vtkImplicitFunction);

		/**
		 * \brief Boolean controls whether to extract cells that are inside of implicit 
		 * function (ExtractInside == 1) or outside of implicit function 
		 * (ExtractInside == 0).
		 */
		vtkSetMacro(ExtractInside,int);
		vtkGetMacro(ExtractInside,int);
		vtkBooleanMacro(ExtractInside,int);

		/**
		 * \brief Boolean controls whether to extract cells that are partially inside.
		 * By default, ExtractBoundaryCells is off.
		 */
		vtkSetMacro(ExtractBoundaryCells,int);
		vtkGetMacro(ExtractBoundaryCells,int);
		vtkBooleanMacro(ExtractBoundaryCells,int);

		/**
		 * \brief m_ArrayCellsId
		 */
		vtkSetObjectMacro( ArrayCellsId, vtkIdList );
		vtkGetObjectMacro( ArrayCellsId, vtkIdList );


	// ----------------------------------
	// PROTECTED OPERATIONS
	// ----------------------------------
	protected:
		meVTKExtractPolyDataGeometryCellsID(vtkImplicitFunction *f=NULL);
		~meVTKExtractPolyDataGeometryCellsID();

		/// Usual data generation method
		int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);


	// ----------------------------------
	// PRIVATE OPERATIONS
	// ----------------------------------
	private:
		meVTKExtractPolyDataGeometryCellsID(const meVTKExtractPolyDataGeometryCellsID&);  // Not implemented.
		void operator=(const meVTKExtractPolyDataGeometryCellsID&);  // Not implemented.


	// ----------------------------------
	// ATTRIBUTES
	// ----------------------------------
	protected:
		vtkImplicitFunction *ImplicitFunction;
		int ExtractInside;
		int ExtractBoundaryCells;
		vtkIdList* ArrayCellsId;
	};

#endif
