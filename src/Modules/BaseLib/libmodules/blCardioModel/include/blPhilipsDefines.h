/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _blPhilipsDefines_h
#define _blPhilipsDefines_h

#include "BaseLibVTKWin32Header.h"

/// \ingroup blCardioModel

namespace Philips
{
	/**
	 * \brief type of subpart 
	 * \ingroup blCardioModel
	 */
	enum SubpartType { 
		LVMyo = 1, //1
		LeftVentricle, //2
		LVMyo_LeftVentricle, //3
		ApexLVEndo_LVMyo_LeftVentricle, //4
		RightVentricle, //5
		LVMyo_RightVentricle, //6
		LeftAtrium, //7 
		LVMyo_LeftAtrium, //8
		LeftAtrium_LeftVentricle_MitralValve, //9
		RightAtrium, //10		
		RightAtrium_RightVentricle_TricuspidValve, //11
		Aorta_AscendingAorta, //12
		Aorta_AorticArch, //13
		Aorta_DescendingAorta, //14
		Aorta_AorticValve_LeftVentricle, //15
		SupVenaCava, //16
		RightAtrium_SupVenaCava_VirtualSupVenaCavaValve, //17
		InfVenaCava, //18
		InfVenaCava_RightAtrium_VirtualInfVenaCavaValve, //19
		CoronarySinus, //20
		PulmonaryArtery, //21
		PulmonaryArtery_PulmonaryValve_RightVentricle, //22
		SupRightPV, //23
		LeftAtrium_SupRightPV_VirtualSupRightPVValve, //24
		InfRightPV, //25
		InfRightPV_LeftAtrium_VirtualInfRightPVValve, //26
		SupLeftPV, //27
		LeftAtrium_SupLeftPV_VirtualSupLeftPVValve, //28
		InfLeftPV, //29
		InfLeftPV_LeftAtrium_VirtualInfLeftPVValve, //30
		VirtualAscendingAortaValve, //31
		VirtualDescendingAortaValve, //32
		CoronarySinus_RightAtrium_VirtualCoronarySinusValve, //33
		None_Subpart //34
	} ;
		
	SubpartType AssignLabel(char* label)
	{
		SubpartType assignedLabel = None_Subpart;

		if (!strcmp(label,"LVMyo"))
			assignedLabel = LVMyo;
		else if (!strcmp(label,"LeftVentricle"))
			assignedLabel = LeftVentricle;
		else if (!strcmp(label,"LVMyo	LeftVentricle"))
			assignedLabel = LVMyo_LeftVentricle;
		else if (!strcmp(label,"ApexLVEndo	LVMyo	LeftVentricle"))
			assignedLabel = ApexLVEndo_LVMyo_LeftVentricle;
		else if (!strcmp(label,"RightVentricle"))
			assignedLabel = RightVentricle;
		else if (!strcmp(label,"LVMyo	RightVentricle"))
			assignedLabel = LVMyo_RightVentricle;
		else if (!strcmp(label,"LeftAtrium"))
			assignedLabel = LeftAtrium;
		else if (!strcmp(label,"LVMyo	LeftAtrium"))
			assignedLabel = LVMyo_LeftAtrium;
		else if (!strcmp(label,"LeftAtrium	LeftVentricle	MitralValve"))
			assignedLabel = LeftAtrium_LeftVentricle_MitralValve;
		else if (!strcmp(label,"RightAtrium"))
			assignedLabel = RightAtrium;
		else if (!strcmp(label,"RightAtrium	RightVentricle	TricuspidValve"))
			assignedLabel = RightAtrium_RightVentricle_TricuspidValve;
		else if (!strcmp(label,"Aorta	AscendingAorta"))
			assignedLabel = Aorta_AscendingAorta;
		else if (!strcmp(label,"Aorta	AorticArch"))
			assignedLabel = Aorta_AorticArch;
		else if (!strcmp(label,"Aorta	DescendingAorta"))
			assignedLabel = Aorta_DescendingAorta;
		else if (!strcmp(label,"Aorta	AorticValve	LeftVentricle"))
			assignedLabel = Aorta_AorticValve_LeftVentricle;
		else if (!strcmp(label,"SupVenaCava"))
			assignedLabel = SupVenaCava;
		else if (!strcmp(label,"RightAtrium	SupVenaCava	VirtualSupVenaCavaValve"))
			assignedLabel = RightAtrium_SupVenaCava_VirtualSupVenaCavaValve;
		else if (!strcmp(label,"InfVenaCava"))
			assignedLabel = InfVenaCava;
		else if (!strcmp(label,"InfVenaCava	RightAtrium	VirtualInfVenaCavaValve"))
			assignedLabel = InfVenaCava_RightAtrium_VirtualInfVenaCavaValve;
		else if (!strcmp(label,"CoronarySinus"))
			assignedLabel = CoronarySinus;
		else if (!strcmp(label,"PulmonaryArtery"))
			assignedLabel = PulmonaryArtery;
		else if (!strcmp(label,"PulmonaryArtery	PulmonaryValve	RightVentricle"))
			assignedLabel = PulmonaryArtery_PulmonaryValve_RightVentricle;
		else if (!strcmp(label,"SupRightPV"))
			assignedLabel = SupRightPV;
		else if (!strcmp(label,"LeftAtrium	SupRightPV	VirtualSupRightPVValve"))
			assignedLabel = LeftAtrium_SupRightPV_VirtualSupRightPVValve;
		else if (!strcmp(label,"InfRightPV"))
			assignedLabel = InfRightPV;
		else if (!strcmp(label,"InfRightPV	LeftAtrium	VirtualInfRightPVValve"))
			assignedLabel = InfRightPV_LeftAtrium_VirtualInfRightPVValve;
		else if (!strcmp(label,"SupLeftPV"))
			assignedLabel = SupLeftPV;
		else if (!strcmp(label,"LeftAtrium	SupLeftPV	VirtualSupLeftPVValve"))
			assignedLabel = LeftAtrium_SupLeftPV_VirtualSupLeftPVValve;
		else if (!strcmp(label,"InfLeftPV"))
			assignedLabel = InfLeftPV;
		else if (!strcmp(label,"InfLeftPV	LeftAtrium	VirtualInfLeftPVValve"))
			assignedLabel = InfLeftPV_LeftAtrium_VirtualInfLeftPVValve;
		else if (!strcmp(label,"VirtualAscendingAortaValve"))
			assignedLabel = VirtualAscendingAortaValve;
		else if (!strcmp(label,"VirtualDescendingAortaValve"))
			assignedLabel = VirtualDescendingAortaValve;
		else if (!strcmp(label,"CoronarySinus	RightAtrium	VirtualCoronarySinusValve"))
			assignedLabel = CoronarySinus_RightAtrium_VirtualCoronarySinusValve;
		else
			assignedLabel = None_Subpart;
				
		return assignedLabel;
		
	}
}
#endif // _blPhilipsDefines_h
