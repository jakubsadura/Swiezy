/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _BLSHAPEUTILS_h
#define _BLSHAPEUTILS_h

#include "BaseLibVTKWin32Header.h"

#include <vnl/vnl_cross_product_matrix.h>
//#include <vnl/vnl_dot_product_matrix.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkShortArray.h>
#include <vtkFloatArray.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkLookupTable.h>
#include <vtkUnstructuredGrid.h>

#include <vector>
#include <map>

#include "blPDShape.h"

class vtkIdList;

/// \ingroup blUtilitiesVTK
namespace blShapeUtils
{
	typedef vtkSmartPointer<vtkPolyData> VtkPolyDataPtr;
	/**
	 * \brief Static class with some utilities.
	 * \ingroup blUtilitiesVTK
	 */
	class BASELIBVTK_EXPORT ShapeUtils
	{
		public:

		/** Enumeration of the valid shape formats that blShapeUtils can handle */
		enum VTKSurfaceMeshFormats { UnknownType, VTKPolyDataType, VTKXMLPolyDataType, STLType, PLYType };
		
		/** Enumeration of the valid volume formats that blShapeUtils can handle */
		enum VTKVolumeMeshFormats { UnknownVolumeType, VTKUnstructuredGridType };

		/**Enumeration of the valid types that can be set in the GetTubesFromDataPoint */
		enum Glyph2DShapes { UnknownGlyphType, ArrowType, DashType };

		//! Type of scalar array
		enum SCALAR_ARRAY_TYPE{
			SCALAR_ARRAY_POINT_DATA,
			SCALAR_ARRAY_CELL_DATA,
			SCALAR_ARRAY_NONE
		};

		//! Class for storing the array name and mode
		class VTKScalarType
		{
		public:
			std::string name;
			// mode 0-pointdata, 1-celldata
			SCALAR_ARRAY_TYPE mode;
		};

		typedef std::vector<VTKScalarType> ScalarArrayType;

		/**\brief typedef for a map to count the number of points per region
		* \todo template it!
		*/
		typedef std::map <short, unsigned int> RegionMapType;

		/**\brief typedef for a map to count the number of points per subpart
		* \todo template it!
		*/
		typedef std::map <short, unsigned int> SubpartMapType;

		/**\brief gets a vtkImageData from an itkImage*/
		//static vtkImageData * itk2vtk(itkImage * itkImagePt);

		/**\brief orders a data array (vtkFloatArray in this case)*/
		void BubbleSort(vtkFloatArray *numbers, int *indexes, unsigned int array_size);

		// Please use the one in blImageUtils
		//static void SaveVTKImageData(vtkImageData* paramImage, const char * paramFileName, bool paramSaveAsBinary);

		/**\brief gets the volume of shape already closed
		* \param shapePt input shape (vtkPolyData)
		* \param return_projected Return projected volume
		* \param convertToMl if the volume is given in cubic milliliter or in milliliter
		*/
		static double GetClosedSurfaceVolume(
						vtkPolyData* paramShape, 
						bool return_projected,
						bool convertToMl = true);

		/**\brief gets the volume of shape already closed, Corne implementation (independent of VTK)
		*  \param shapePt input shape (vtkPolyData)
		*/
		static int GetClosedSurfaceVolumeUsingProjection(vtkPolyData* paramShape, double &paramVolume);


		/**\brief gets the volume of shape with only one hole
		*  \param shapePt input shape (vtkPolyData)
		*/
		// Deprecated: Use SMoLib3D::smlCardiacUtilities functions
		//static int GetVolume(vtkPolyData* paramShape, double &paramVolume);

		/**\brief converts a vtkPoints array to a vtkPolyData by adding cell data
		*  \param paramShape input shape
		*  \param thickness amount of distance in millimeters (if negatice, shrinks)
		*/
		//static void GrowOrShrink(vtkPolyData * paramShape, float paramThickness);

		/**\brief applies a given scale transformation to an input shape
		*  \param paramInputShape input shape
		*  \param paramOutputShape output shape
		*  \param paramScale scale factor in x, y, z
		*/
		static void ScaleShape(vtkPolyData* paramInputShape, 
							   vtkPolyData* paramOutputShape, 
							   const double * paramScale);

		/**\brief applies a given scale transformation to an input shape
		*  \param paramInputShape input shape
		*  \param paramOutputShape output shape
		*  \param paramTimes subdivision times (typ. 1-3)
		*/
		static int SubdivideMesh(vtkPolyData * paramInputShape, 
								vtkPolyData * paramOutputShape, 
								unsigned int paramTimes);

		/**\brief finds a transformation that transforms 'paramSourcePoints'
		* into 'paramTargetPoints' and applies this transformation to a mesh
		*  \param paramInputShape input shape (mesh that will be transformed)
		*  \param paramOutputShape output shape
		*  \param paramSourcePoints
		*  \param paramTargetPoints
		*  \param TrType type of transformation -> 0: RigidBody, 1: Similarity, 2: Affine
		*    default: Similarity
		*/
		static void AdjustShapeToLandmarks(vtkPolyData* paramInputShape, vtkPolyData* paramOutputShape, 
											vtkPoints *paramSourcePoints, vtkPoints *paramTargetPoints, 
											int TrType=1);

		/**\brief fills a subpart map
		*  \param shapePt input shape
		*  \param manualPt manual shape
		*  \param subpartMap <short,unsigned int>
		*  \return number of different scalars (warning: this is not the same as: max - min + 1)
		*/
//		static void FillSubpartMap(vtkPolyData * shapePt, vtkPolyData * manualPt, subpartMapType &subpartMap)

		/**\brief gets the number of different scalars in a given property (caution: should be tempeted) 
		*  \param shapePt input shape
		*  \param scalar_map <short,unsigned int>
		*  \param property_name
		*  \return number of different scalars (this is not the same as max - min + 1)
		*/
		static unsigned int GetNumberOfDifferentRegions(vtkPolyData* shapePt, 
														RegionMapType &scalar_map, 
														const char* property_name);

		static double GetMaxPropertyValue(vtkPolyData * shapePt, const char* property_name);

		static double GetMinPropertyValue(vtkPolyData * shapePt, const char* property_name);

		/// Checks that the extension is correct
		static bool CheckSaveFileExtension(const char *shapeFileName);

		/**
		* \brief writes a shape (vtkPolyData) to file with format according 
		* to file extension (.vtk, .stl, .vtp, .iv)
		* \param shapePt input shape
		* \param shapeFileName file name
		* \param header Header of the file
		*/
		static void SaveShapeToFile(
						vtkPolyData *shapePt, 
						const char *shapeFileName,
						const char *header = NULL );

		static void SaveVolumeToFile(
			vtkUnstructuredGrid *volumePt, 
			const char *volumeFileName,
			const char *header = NULL);
		/// Checks that the extension is correct
		//static bool CheckLoadFileExtension(const char *shapeFileName);

		/**\brief compares the filename extension with vtk valid types and attempt loading them to verify the type of mesh that it represents
		*  \param shapeFileName file name
		*  \return an enumeration field describing what format the file contains (VTKSurfaceMeshFormats)
		*/
		static VTKSurfaceMeshFormats GetTypeOfVTKData(const char *shapeFileName);

		/**\brief compares the filename extension with vtk valid types and attempt loading them to verify the type of mesh that it represents
		*  \param volumeFileName file name
		*  \return an enumeration field describing what format the file contains (VTKVolumeMeshFormats)
		*/
		static VTKVolumeMeshFormats GetTypeOfVTKVolumeData(const char *volumeFileName);

		/**\brief reads a shape from file (automatically adapts to .vtk, .stl) and retrieves a vtkPolyData
		*  \param shapeFileName file name
		*  \return input shape (vtkPolyData*)
		* You need to call Delete( ) to the output of this function to 
		* destroy the object
		*/
		static vtkPolyData* LoadShapeFromFile(const char *shapeFileName);

		/**\brief reads a volume from file (automatically adapts to .vtk) and retrieves a vtkUnstructuredGrid
		*  \param volumeFileName file name
		*  \return input volume (vtkUnstructuredGrid*)
		* You need to call Delete( ) to the output of this function to 
		* destroy the object
		*/
		static vtkUnstructuredGrid* LoadVolumeFromFile(const char *volumeFileName);

		/**\brief returns an array with the normals corresponding to the input shape
		*  \param shapePt input shape
		*  \return array with normals
		*/
		//static vtkDataArray* GetNormals(vtkPolyData* shapePt, bool b_flip = false);

		/**\brief returns the input shape plus an array with corresponding normals
		*  \param paramShapePt input shape
		*  \return shape with normals
		*/
		static vtkPolyData* GetShapeWithNormals(vtkPolyData* paramShapePt);

		/**\brief gets a region delimited between "start" and "end" for "name" 
		* property (only scalar data)
		*  \param shapePt input shape
		*  \param start lower delimitation
		*  \param end upper delimitation
		*  \param name requested property
		* \return If output has no points -> return NULL
		*/
		static vtkPolyData* GetShapeRegion(
						vtkPolyData* shapePt, 
						unsigned int start, 
						unsigned int end, 
						const char *name);

		/**\brief gets a region delimited between "start" and "end" for "name" 
		* property (only scalar data)
		*  \param VolumePt input volume
		*  \param start lower delimitation
		*  \param end upper delimitation
		*  \param name requested property
		* \return If output has no points -> return NULL
		*/
		static vtkPolyData* GetVolumeRegion(
			vtkUnstructuredGrid* volumePt, 
			unsigned int start, 
			unsigned int end, 
			const char *name);

		/**\brief gets a thresholded region of a shape
		*  \param shapePt input shape
		*  \param subpart requested subpart
		*/
		//static vtkPolyData* GetShapeSubpart(vtkPolyData * shapePt, unsigned int subpart);


    /**\brief gets the point to surface (p2s) distance between two shapes
		*  \param manualPt target shape (vtkPolyData)
		*  \param segmentedPt reference (vtkPolyData)
		*  \param max error by reference
		*  \param mean error by reference
		*  \param std_dev error by reference
		*/
		static void GetP2S(vtkPolyData * manualPt, 
							vtkPolyData *segmentedPt, 
							double& mean, 
							double& std_dev, 
							double& max, 
							double& last, 
							bool b_array = false);

		//First function computes the error from shape1 to shape2, returns shape1 with scalars, array of distances per region per vertex. 
		//Active scalars are used to separate regions
		//distances saved in vnl_vectors like this:
		//distances[2i] = one element vector with rgion ID
		//distances[2i+1] = vector of distances
		static vtkPolyData* GetP2S(vtkPolyData *shapePt1, vtkPolyData *shapePt2, std::vector< vnl_vector<double> >& distances);
		//First function computes the error from shape1 to shape2, returns shape1 with scalars, array of distances per vertex, regions are ignored
		static vtkPolyData* GetP2S(vtkPolyData *shapePt1, vtkPolyData *shapePt2, vnl_vector<double>& distances);
		//surface to surface error
		static void GetS2S(vtkPolyData *shapePt1, vtkPolyData *shapePt2, std::vector< vnl_vector<double> >& distances);


		/**\brief gets the point to point (p2p) distance between two shapes
		*  \param manualPt target shape (vtkPolyData)
		*  \param segmentedPt reference (vtkPolyData)
		*  \param max error by reference
		*  \param mean error by reference
		*  \param std_dev error by reference
		*/
		static void GetP2P(vtkPolyData * manualPt, 
							vtkPolyData *segmentedPt, 
							double& mean, 
							double& std_dev, 
							double& max, 
							double& last, 
							bool b_array = false);

		/**\brief gets a tube from an input polydata
		*  \return vtkPolydata pointer 
		*/
		static VtkPolyDataPtr GetTubesFromPointData(VtkPolyDataPtr shapePt, 
												const char* name, 
												int max_points=5000,
												float scale = 2.0, 
												float radius = 0.2,
												Glyph2DShapes Glyphtype = UnknownGlyphType,
												int sides = 10,
												const char* scalar = NULL);

		/**\brief gets a tube from an input spline (polydata)
		*  \param shapePt input shape
		*  \param start_rad start radii
		*  \param end_rad end radii
		*  \param nsides number of sides in the tube
		*  \return vtkPolydata pointer 
		*/
		//static vtkPolyData* GetTubesFromSplines(vtkPolyData *shapePt, float start_rad = 2.5, float end_rad = 0.1, int nsides = 6);

		/**\brief gets a plane (vtkPlane) fitted to an input set of points (polydata)*/
		//static  vtkPlane *FitPlaneToPoints(vtkPolyData* shapePt);


		/**
		 * \brief Compare two arrays
		 *
		 * Compare the number of cells and the id of each cell
		 */
		static bool CompareVtkCellArray( 
						vtkCellArray*		ptrArray1,
						vtkCellArray*		ptrArray2 );

		/**
		 * \brief Compare two vtkPolydatas and return if they are equal
		 * \note vtkPolyData with precision float are written with 3 
		 * decimals, so default value for fEpsilon could be 0.001. However,
		 * the vtkPolyData generated with 32 bit machine are different than
		 * 64 bit machine. I put 1.5 default value.
		 *
		 * Compare:
		 * - Number of points, lines and polylines
		 * - For each point, the position
		 * - For each line, the id of the points
		 * - For each polyline, the id of the points
		 *
		 * \param [in] ptrShape1 input shape 1
		 * \param [in] ptrShape2 input shape 2
		 * \param [out] fMaxDifference max difference in points coordinates 
		 *				position
		 * \param [in] fEpsilon Error range to compare the points 
		 *				coordinates position
		 * 
		 */
		static bool CompareShapes( 
						vtkPolyData*		ptrShape1,
						vtkPolyData*		ptrShape2,
						float &fMaxDifference,
						float fEpsilon = 1.5 );


		/// Convert from vtkCellArray to blPDShapeInterface
		static void Convert( 
						vtkCellArray*		vtkCells,
						blPDShapeInterface::ShapeCellsContainer &pdCells );

		/// Convert from blPDShapeInterface to vtkCellArray
		static void Convert( 
						blPDShapeInterface::ShapeCellsContainer &pdCells,
						vtkCellArray*		vtkCells );


		/**
		 * \brief Convert from vtkPolyData to blPDShape
		 *
		 * New instance of blPDShape will be created (2D or 3D)
		 */
		template <unsigned int Dim>
		static void Convert( 
							vtkPolyData* vtkShape, 
							typename blPDShape<Dim>::Pointer &pdShape );

		/**
		 * \brief Convert from blPDShape to vtkPolyData
		 *
		 * \note vtkShape must be created by the caller of this function
		 */
		template <unsigned int Dim>
		static void Convert( 
							typename blPDShape<Dim>::Pointer &pdShape,
							vtkPolyData* vtkShape );


		/**
		 * \brief Compare two shapes loading the files from disk
		 */
		static bool CompareShapes( 
							const char *shapeFileName1, 
							const char *shapeFileName2 );


		/**
		 * \brief Get neighbor points of the iSelectedPoint in polyData. Only searches in the lines of \a polydata.
		 *
		 * \note neighborPoints must be allocated by the caller of this
		 * function
		 *
		 * \param [in] polyData Input mesh
		 * \param [in] iSelectedPoint Selected point of the mesh
		 */
		static void GetNeighborPoints( 
						vtkPolyData* polyData, 
						int iSelectedPoint,
						vtkIdList	*neighborPoints );


		/**
		 * \brief Get tangent vector between two points
		 *
		 * \note If the iNumPoint is greater than the number of neighbor points,
		 * get the iNumPoint % ( number of neighbors )
		 *
		 * \param [in] polyData Input mesh
		 * \param [in] iPoint1 First point 
		 * \param [in] iPoint2 Second point
		 * \param [out] tangent array of size 3
		 * \return false if neighbor not found
		 */
		static void GetTangentVector( 
						vtkPolyData* polyData, 
						int iPoint1,
						int iPoint2,
						double tangent[ 3 ] );


		/**
		 * \brief Get vector of scalar array names
		 */
		static void GetScalarsVector( 
			vtkDataSet* data,
			std::vector<VTKScalarType> &scalarnames );

		//! Check if it Is a skeleton
		static bool IsSkeleton( vtkPolyData* processingData );


		/** Returns float array with the values of data array for cellID
		for each time step
		*/
		static std::vector<float> GetDataArrayValues(
			const std::vector<vtkPolyData* > &meshes,
			int cellID,
			const std::string &arrayName );

		/** Computes the number of holes in the given shape
		*/
		static unsigned int ComputeNumberOfHoles(
			vtkPolyData* inputShape);

	};
}

#include "blShapeUtils.txx"

#endif // _SHAPEUTILS_h
