/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blcardioDefines_h
#define _blcardioDefines_h

#include "BaseLibVTKWin32Header.h"

/// \ingroup blCardioModel

namespace Cardio
{
	//! SubpartID type
	typedef short SubpartIDType;

	//! regionID type
	typedef short RegionIDType;

	/// \ingroup blCardioModel
	enum ModelType { 
		//! Full heart with 4 chambers and aorta (endo and epi)
		FULL_HEART_MODEL, 
		FOUR_CHAMBER_MODEL, 
		//! Left and right ventricle
		TWO_CHAMBER_MODEL, 
		LV_LA_MODEL, 
		LA_RA_MODEL, 
		//! Left ventricle
		ONE_CHAMBER_MODEL, 
		LA_MODEL, 
		RA_MODEL, 
		VEINS_MODEL, 
		ARTERIES_MODEL, 
		PURKINJE_MODEL, 
		PECTINATE_MODEL, 
		MYOFIBER_MODEL, 
		CARTO_MODEL, 
		//! Aorta only endo part
		AORTA_ENDO_MODEL,
		//! Left atrium only endo part
		LEFT_ATRIUM_ENDO_MODEL,
		//! New Left ventricle endo and epi
		LEFT_VENTRICLE_MODEL,
		//! Right atrium only endo part
		RIGHT_ATRIUM_ENDO_MODEL,
		//! Right ventricle only endo part
		RIGHT_VENTRICLE_ENDO_MODEL,
		//! Left ventricle and atrium
		LEFT_VENTRICLE_AND_ATRIUM,
		UNKNOWN_MODEL } ;

	/**
	 * \brief type of subpart 
	 * \ingroup blCardioModel
	 */
	enum SubpartType { 
		LV_ENDO_SUBPART = 1, //1
		LV_EPI_SUBPART, //2
		LA_SUBPART, //3
		RV_SUBPART, //4
		RA_SUBPART, //5
		AO_SUBPART, //6
		FS_SUBPART, //7 
		PURKINJE_SUBPART, //8
		ARTERIES_SUBPART, //9
		VEINS_SUBPART, //10		
		PECTINATE_SUBPART, //11
		NONE_SUBPART } ;


	const int NUM_17SEGMENTS = 17;


	/**
	\brief Name of the scalars vector defining subparts in the shape of
	the model
	\ingroup blCardioModel
	*/
	extern BASELIBVTK_EXPORT const char* VECTOR_NAME_SUBPART_ID;

	/**
	\brief Name of the scalars vector defining regions in the shape of the model
	\ingroup blCardioModel
	*/
	extern BASELIBVTK_EXPORT const char* VECTOR_NAME_REGION_ID;

	//! 
	enum REGIONIDTYPE{

		// LV_EPI

		LV_EPI_INF_ID = 1,
		first_LV_ID = LV_EPI_INF_ID,
		FIRST_LV_EPI_ID = LV_EPI_INF_ID,
		LV_EPI_MI_ID = 2,
		LV_EPI_AO_ID = 3,
		LV_EPI_SEPT_ID = 4,
		LV_EPI_LAT_ID = 5,
		LV_EPI_APEX_ID = 6,
		LV_EPI_SUP_ID = 7,
		LAST_LV_EPI_ID = LV_EPI_SUP_ID,

		// LV_ENDO
		LV_ENDO_SUP_LAT_ID = 8,
		FIRST_LV_ENDO_ID = LV_ENDO_SUP_LAT_ID,
		LV_ENDO_INF_LAT_ID = 9,
		LV_ENDO_SUP_SEPT_ID = 10,
		LV_ENDO_INF_SEPT_ID = 11,
		LAST_LV_ENDO_ID = LV_ENDO_INF_SEPT_ID,

		// RV_EPI
		RV_EPI_ID = 12,
		RV_ENDO_ID = 13,
		first_RV_ID = RV_ENDO_ID,
		RV_PV_ID = 14,
		RV_AN_ID = 15,
		last_RV_ID = RV_AN_ID,

		// LA
		LA_EPI_ID = 16,
		LA_ENDO_ID = 17,
		first_LA_ID = LA_ENDO_ID,
		LA_AN_ID = 18,
		last_LA_ID = LA_AN_ID,
		LA_RIPV_ID = 19,
		LA_RSPV_ID = 20,
		LA_LSPV_ID = 21,
		LA_LIPV_ID = 22,
		LA_AP_ID = 23,

		// RA
		RA_ENDO_ID = 24,
		first_RA_ID = RA_ENDO_ID,
		RA_EPI_ID = 25,
		RA_SVC_ID = 26,
		RA_IVC_ID = 27,
		RA_CS_ID = 28,
		RA_RAA_ID = 29,
		RA_AN_ID = 30,
		last_RA_ID = RA_AN_ID,

		// AO
		AO_ENDO_ID = 31,
		first_AO_ID = AO_ENDO_ID,
		AO_EPI_ID = 31,
		last_AO_ID = AO_EPI_ID,

		// FS
		aorta_FS_ID = 32,
		first_FS_ID = aorta_FS_ID,
		mitral_FS_ID = 33,
		tricuspid_FS_ID = 34,
		last_FS_ID = tricuspid_FS_ID,

		//	his_ID = 120,
		left_PK_ID = 35,
		first_PK_ID = left_PK_ID,
		right_PK_ID = 36,
		last_PK_ID = right_PK_ID,

		first_LDA_ID = 37,
		first_ARTERIES_ID = first_LDA_ID,
		last_LDA_ID = 37,

		first_RCA_ID = 38,
		last_RCA_ID = 38,
		last_ARTERIES_ID = last_RCA_ID,

		first_LCX_ID = 39,
		last_LCX_ID = 39,

		first_VEIN_LEFT_ID = 40,
		first_VEINS_ID = first_VEIN_LEFT_ID,
		last_VEIN_LEFT_ID = 40,

		first_VEIN_RIGHT_ID = 41,
		last_VEIN_RIGHT_ID = 41,
		last_VEINS_ID = last_VEIN_RIGHT_ID,

		pect_ID = 42,
		highest_ID = 42,

	}; 


}
#endif // _cardioDefines_h
