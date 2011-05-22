/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef __blVTKHelperTools_h
#define __blVTKHelperTools_h


	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------

	#include "BaseLibVTKWin32Header.h"
	#include <vector>
	#include <vnl/vnl_vector.h>


	//---------------------------------------------------
	// FORWARD DECLARATIONS
	//---------------------------------------------------

	class vtkShortArray;
	class vtkPolyData;
	class vtkUnstructuredGrid;
	class vtkPoints;
	class vtkDataSet;
	class vtkStringArray;


	//---------------------------------------------------
	// CLASS DEFINITION
	//---------------------------------------------------


	/// \ingroup blUtilitiesVTK
	namespace blVTKHelperTools
	{

		/**
		\brief Save a vtk short array using a specific format 
		\note If there's an error, an std::exception is thrown
		*/
		extern BASELIBVTK_EXPORT void SaveVtkShortArray( const char *filename, vtkShortArray* the_array );

		/**
		\brief Load a vtk short array using a specific format 
		\note If there's an error, an std::exception is thrown
		 */
		extern BASELIBVTK_EXPORT void LoadVtkShortArray( const char *filename, vtkShortArray* the_array );

		/**
		\brief Return the short array with name arrayName in the PointData
		\note If no array is found, throw an exception
		*/
		template <class ArrayType>
		static ArrayType *GetPointDataArray( 
						vtkPolyData* polyData, 
						const char* arrayName );

		extern BASELIBVTK_EXPORT std::vector<unsigned int> LoadRegionExclusions( const char* filename );

		//! Call to GetShapeSubSurface( ) with nSubPart-0.1, nSubPart+0.1
		extern BASELIBVTK_EXPORT vtkPolyData* GetShapeSubSurface(vtkPolyData * inputShape, unsigned int nSubPart);

		/**
		\brief Apply threshold to extract the subpart, apply 
		vtkDataSetSurfaceFilter and vtkCleanPolyData
		\note The caller to this function should delete the output shape
		*/
		extern BASELIBVTK_EXPORT vtkPolyData* GetShapeSubSurface(vtkPolyData * inputShape, double tholdLower, double tholdUpper);

		extern BASELIBVTK_EXPORT vtkPolyData* GetAnnulus(vtkPolyData* shapePt);

		//! Closes only 1 hole, make sure there are no more. 
		extern BASELIBVTK_EXPORT vtkPolyData* CloseSurface(vtkPolyData* shape);
		extern BASELIBVTK_EXPORT vtkPolyData* GenerateHoleCover(vtkPolyData* edge);


		extern BASELIBVTK_EXPORT void SavePolydata( vtkPolyData* poly, const char* filename, bool binary = false );
		extern BASELIBVTK_EXPORT void SaveImage( vtkDataSet* image, const char* filename );

		extern BASELIBVTK_EXPORT void SavePoints( vtkPoints* pts, const char* filename );

		extern BASELIBVTK_EXPORT bool FileExists( const char* filename, bool no_exception = false ); //throws an exception if file does not exist

		//stores in "list"
		extern BASELIBVTK_EXPORT void ReadFilelist( const char* file, std::vector<std::string>& list, bool check_existence = false );

		//http://www.sitmo.com/eq/255
		// f(x+h) - f(x-h) / 2h
		extern BASELIBVTK_EXPORT void Diff3Points( vnl_vector<double>& input, vnl_vector<double>& derivative );


		/**
		\brief Saves points such that they can e visualized in paraview
		also saves a scalar corresponding to a position so it is easy to see the ordering of points 
		(if sampling is correct)
		if scalars != NULL, the corresponding scalar values will be assigned to the points
		\note The caller to this function should call points->Delete( )
		*/
		extern BASELIBVTK_EXPORT vtkPolyData* Points2Polydata( vtkPoints* points, double scalar );
		extern BASELIBVTK_EXPORT vtkPolyData* Points2Polydata( vtkPoints* points, const double* scalars=NULL );

		//extract points from polydata
		extern BASELIBVTK_EXPORT void ExportPolyDataPoints( vtkPolyData* shape, vnl_vector<double>& points );
		//copy points to polydata
		extern BASELIBVTK_EXPORT void ImportPolyDataPoints( vtkPolyData* shape, vnl_vector<double>& points );



		extern BASELIBVTK_EXPORT void ScaleShape(vtkPolyData* shapein, vtkPolyData* shapeout, float scale, bool centerAfterScale = false); 
		extern BASELIBVTK_EXPORT void ShrinkImage(vtkDataSet* imagein, vtkDataSet* imageout, int factor);
		extern BASELIBVTK_EXPORT void ScaleVolume(vtkUnstructuredGrid* volumein, vtkUnstructuredGrid* volumeout, float scale); 


	}

#include "blVTKHelperTools.txx"

#endif
