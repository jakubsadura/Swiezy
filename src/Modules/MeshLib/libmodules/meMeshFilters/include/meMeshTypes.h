/*=========================================================================

  Module    : meMesh
  File      : $RCSfile: meMeshTypes.h,v $
  Copyright : (C)opyright (University of Pompeu Fabra) 2006++
              See COPYRIGHT statement in top level directory.
  Authors   : Xavi Planes
  Modified  : $Author: xplanes $
  Purpose   : Mesh interface
  Date      : $Date: 2008-05-30 09:54:30 $
  Version   : $Revision: 1.12 $
  Changes   : $Locker:  $


=========================================================================*/

	#ifndef _meMeshTypes_h
	#define _meMeshTypes_h

	//---------------------------------------------------
	// HEADERS
	//---------------------------------------------------


	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	typedef enum 
	{
		MESH_ERROR_CODE_TYPE_OK,
		MESH_ERROR_CODE_TYPE_ERROR
	} MESH_ERROR_CODE_TYPE;



	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	typedef enum 
	{
		MESH_SMOOTHING_TYPE_NETGEN,
		MESH_SMOOTHING_TYPE_TAUBIN,
		MESH_SMOOTHING_TYPE_UMBRELLA
	} MESH_SMOOTHING_TYPE;



	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshSmoothingParams
	{
	public:

		MESH_SMOOTHING_TYPE	m_Type;

		/**
		 * \brief Umbrella operator order. Default[2].
		 *
		 * Order can be 0, 1 or 2. Order 0 is NO fairing. Order 1 
		 * homogenize triangle coordinates.
		 * It is good for adding flat caps and optimize triangle size. 
		 * Order 2 homogenize triangle curvatures,
		 * so boundary curvature is propagated towards the inside of the patch.
		 */
		unsigned int m_UmbrellaOrder;

		/**
		 * \brief Percentage of initial average movement to stop iterating.
		 *
		 * Note default settings make this criterion default one. 
		 * Default [0.0001] (0.01%).
		 */
		double m_Percentage;

		/**
		 * \brief Number of fairing iterations. If it is set different from 0,
		 * it will used instead of average percentage movement. Default [0].
		 */
		unsigned int m_NumIterations;

		/**
		 * \brief This flag determines if m_Percentage or m_NumIterations 
		 * is used in the stop criterion. Default is true.
		 */
		bool m_UsePercentage;

		meMeshSmoothingParams( )
		{
			m_Type = MESH_SMOOTHING_TYPE_TAUBIN;
			m_UmbrellaOrder = 2;
			m_Percentage = 0.0001;
			m_NumIterations = 0;
			m_UsePercentage = true;
		}
	};

	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshEdgeSwappingParams
	{
	public:

		/**
		 * \brief Maximum angle in RADIANS between to triangles to allow 
		 * edge swapping. Default [0.0] (coplanars).
		 */
		double m_Angle;

		/**
		 * \brief Minimum number of edges needed on a interior edges 
		 * relaxation to continue relaxing in next iteration. Default [10].
		 */
		unsigned int m_MinimumEdgesSwapped;

		meMeshEdgeSwappingParams( )
		{
			m_Angle = 0.0;
			m_MinimumEdgesSwapped = 10;
		}
	};


	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshSkeletonizationParams
	{
	public:

		/**
		 * \brief Distance in mm between pixels of the intermediate image 
		 * that is used to calculate the skeleton of the mesh.
		 * Default 0.025.
		 */
		double m_SkeletonizationPrecisionInMm;

		/**
		 * \brief Get output as VTK PolyData. Default [true].
		 */
		bool m_OutputAsPolyData;

		meMeshSkeletonizationParams( )
		{
			m_SkeletonizationPrecisionInMm = 0.45;
			m_OutputAsPolyData = true;
		}
	};

	/**
	* \brief 
	* \ingroup meMeshModule
	*/
	class meMeshTetraGenerationParams
	{
	public:

		/**
		* \brief 
		*/
		double m_MaxVolume;
		/**
		* \brief 
		*/
		double m_AspectRatio;

		/**
		* \brief
		*/
		bool m_ExportTetras;

		meMeshTetraGenerationParams( )
		{
			m_MaxVolume = 0.5;
			m_AspectRatio = 0.0;
			m_ExportTetras = true;
		}
	};

	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshLocalRefinerParams
	{
	public:

		/** 
		 * \brief Maximum angle in RADIANS between to triangles to allow 
		 * edge swapping. Default [0.0] (coplanars).
		 */
		double m_Angle;

		/**
		 * \brief Minimum number of edges needed on a interior edges 
		 * relaxation to continue relaxing in next iteration. Default [10].
		 */
		unsigned int m_MinimumEdgesSwapped;

		/// Set the density factor for subdivision. Default [sqrt(2.0)].
		double m_DensityFactor;

		meMeshLocalRefinerParams( )
		{
			m_DensityFactor = 1.41421;
			m_Angle = 0.0;
			m_MinimumEdgesSwapped = 10;
		}
	};


	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshLoopSubdivisionParams
	{
	public:

		/**
		 * \brief Sets the number of subdivisions on each triangle. 
		 * Default is 1.
		 */
		unsigned int m_NumberOfSubdivisions;

		meMeshLoopSubdivisionParams( )
		{
			m_NumberOfSubdivisions = 1;
		}
	};


	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshParamsOptimize
	{
	public:

		/// maximal mesh size
		double m_maxh;

		/// safety factor for curvatures (elements per radius)
		double m_curvaturesafety;

		/// minimal number of segments per edge
		double m_segmentsperedge;

		meMeshParamsOptimize( )
		{
			m_maxh = 1000;	
			m_curvaturesafety = 0.2;
			m_segmentsperedge = 0.2;
		}
	};


	/**
	 * \brief 
	 * \ingroup meMeshModule
	 */
	class meMeshVolumeClosingParams
	{
	public:

		/**
		 * \brief Maximum angle in RADIANS between to triangles to allow 
		 * edge swapping. Default [0.0] (coplanars).
		 */
		double m_Angle;

		/**
		 * \brief Minimum number of edges needed on a interior edges 
		 * relaxation to continue relaxing in next iteration. Default [10].
		 */
		unsigned int m_MinimumEdgesSwapped;

		/// Set the density factor for subdivision. Default [sqrt(2.0)].
		double m_DensityFactor;

		/**
		 * \brief Umbrella operator order. Default[0].
		 * 
		 * Order can be 0, 1 or 2. Order 0 is NO fairing. Order 1 
		 * homogenize triangle coordinates.
		 * It is good for adding flat caps and optimize triangle size. 
		 * Order 2 homogenize triangle curvatures,
		 * so boundary curvature is propagated towards the inside of the patch.
		 */
		unsigned int m_Order;

		meMeshVolumeClosingParams( )
		{
			m_DensityFactor = 1.41421;
			m_Angle = 0.0;
			m_MinimumEdgesSwapped = 10;
			m_Order = 1;
		}

	};

	typedef boost::shared_ptr<meMeshVolumeClosingParams> meMeshVolumeClosingParamsPtr;
	typedef boost::shared_ptr<meMeshSkeletonizationParams> meMeshSkeletonizationParamsPtr;
	typedef boost::shared_ptr<meMeshParamsOptimize> meMeshParamsOptimizePtr;
	typedef boost::shared_ptr<meMeshLoopSubdivisionParams> meMeshLoopSubdivisionParamsPtr;
	typedef boost::shared_ptr<meMeshSmoothingParams> meMeshSmoothingParamsPtr;
	typedef boost::shared_ptr<meMeshEdgeSwappingParams> meMeshEdgeSwappingParamsPtr;
	typedef boost::shared_ptr<meMeshLoopSubdivisionParams> meMeshLoopSubdivisionParamsPtr;
	typedef boost::shared_ptr<meMeshLocalRefinerParams> meMeshLocalRefinerParamsPtr;
	typedef boost::shared_ptr<meMeshTetraGenerationParams> meMeshTetraGenerationParamsPtr;

#endif

