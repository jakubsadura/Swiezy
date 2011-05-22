/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef _pacsAPITypes_H
#define _pacsAPITypes_H

#include <string>
#include <vector>
#include "dcmTypes.h"
#include "pacsAPIDcmtkInclude.h"

namespace PACS
{

	/**
	* \brief Network parameters
	* \ingroup PacsAPI
	*/
	struct NetworkParams
	{
		std::string CalledAETitle;
		std::string CallingAETitle;
		std::string serverIP;
		std::string clientIP;
		unsigned long serverPort;
		unsigned long clientPortRetrieve;
		unsigned long clientPortSend;
		int timeout;

		NetworkParams( )
		{
			CalledAETitle = "ANYSCP";
			CallingAETitle = "myFINDMOVESCU";
			serverIP = "10.80.4.143";
			clientIP = "10.80.6.5";
			serverPort = 1234;
			timeout = 30;
			clientPortRetrieve = 1235;
			clientPortSend = 1235;
		}
	};


	/**
	* \brief Association parameters
	* \ingroup PacsAPI
	*/
	struct AssociationParams
	{
		//the SOP class ID of an extraPresentation required for the specific association
		//TODO: a vector could be useful, instead
		std::string extraPresentationCtx; 
		AssociationParams( )
		{
			extraPresentationCtx="";
		}
	};

	/**
	* \brief Query parameters
	* \ingroup PacsAPI
	*/
	enum QueryRetriveLevel
	{
		PATIENT,
		STUDY,
		SERIES,
		IMAGE
	};


	/**
	* \brief Query parameters
	* \ingroup PacsAPI
	*/
	struct QueryParams
	{
		QueryRetriveLevel retrieveLevel;
		std::vector< dcmAPI::Tag > query;

		//! Specific case for birth date
		dcmAPI::Tag birthDateFrom;

		//! Specific case for birth date
		dcmAPI::Tag birthDateTo;

		QueryParams( )
		{
			retrieveLevel = PATIENT;
		}
	};

	/**
	* \brief Move parameters
	* \ingroup PacsAPI
	*/
	struct MoveParams
	{
		unsigned long retriveport;
		std::string outputStorageFolder;

		MoveParams( )
		{
			retriveport = 1235;
			outputStorageFolder = "";
		}
	};


	/**
	* \brief Store parameters
	* \ingroup PacsAPI
	*/
	struct StoreParams
	{
		//! specifies the files to be sent to the PACS
		std::vector <std::string> fullfilenames;
		StoreParams( )
		{
		}
	};



	//contains principal storeclassUID
	#define SUPPORTEDSTOREUID 63
	const char StoreClassUID[SUPPORTEDSTOREUID][256]=
	{
		UID_HardcopyGrayscaleImageStorage,
		UID_HardcopyColorImageStorage,
		UID_ComputedRadiographyImageStorage,     
		UID_DigitalXRayImageStorageForPresentation,
		UID_DigitalXRayImageStorageForProcessing,
		UID_DigitalMammographyXRayImageStorageForPresentation,
		UID_DigitalMammographyXRayImageStorageForProcessing,
		UID_DigitalIntraOralXRayImageStorageForPresentation,
		UID_DigitalIntraOralXRayImageStorageForProcessing,
		UID_StandaloneModalityLUTStorage,
		UID_StandaloneVOILUTStorage,
		UID_GrayscaleSoftcopyPresentationStateStorage,
		UID_XRayAngiographicImageStorage,
		UID_XRayFluoroscopyImageStorage,
		UID_PETImageStorage,
		UID_PETCurveStorage,
		UID_CTImageStorage,
		UID_EnhancedCTImageStorage,
		UID_NuclearMedicineImageStorage,
		UID_UltrasoundMultiframeImageStorage,
		UID_MRImageStorage,                  
		UID_EnhancedMRImageStorage,          
		UID_MRSpectroscopyStorage,           
		UID_RTImageStorage,                  
		UID_RTDoseStorage,                   
		UID_RTStructureSetStorage,          
		UID_RTBeamsTreatmentRecordStorage,   
		UID_RTPlanStorage,                   
		UID_RTBrachyTreatmentRecordStorage,  
		UID_RTTreatmentSummaryRecordStorage, 
		UID_UltrasoundImageStorage,             
		UID_RawDataStorage,                     
		UID_SpatialRegistrationStorage,         
		UID_SpatialFiducialsStorage,            
		UID_SecondaryCaptureImageStorage,       
		UID_MultiframeSingleBitSecondaryCaptureImageStorage,
		UID_MultiframeGrayscaleByteSecondaryCaptureImageStorage,
		UID_MultiframeGrayscaleWordSecondaryCaptureImageStorage,
		UID_MultiframeTrueColorSecondaryCaptureImageStorage,    
		UID_RETIRED_VLImageStorage,                             
		UID_VLEndoscopicImageStorage,                           
		UID_VideoEndoscopicImageStorage,                        
		UID_VLMicroscopicImageStorage,                          
		UID_VideoMicroscopicImageStorage,                       
		UID_VLSlideCoordinatesMicroscopicImageStorage,          
		UID_VLPhotographicImageStorage,                            
		UID_VideoPhotographicImageStorage,                         
		UID_OphthalmicPhotography8BitImageStorage,                 
		UID_OphthalmicPhotography16BitImageStorage,                
		UID_StereometricRelationshipStorage,                      
		UID_RETIRED_VLMultiFrameImageStorage,                     
		UID_StandaloneOverlayStorage,                             
		UID_TwelveLeadECGWaveformStorage,                           
		UID_GeneralECGWaveformStorage,                              
		UID_AmbulatoryECGWaveformStorage,                           
		UID_HemodynamicWaveformStorage,                             
		UID_CardiacElectrophysiologyWaveformStorage,                
		UID_EnhancedXAImageStorage,                                 
		UID_EnhancedXRFImageStorage,                                
		UID_ColorSoftcopyPresentationStateStorage,                  
		UID_PseudoColorSoftcopyPresentationStateStorage,            
		UID_BlendingSoftcopyPresentationStateStorage,               
		UID_RealWorldValueMappingStorage                          
	};

} // namespace PACS


#endif // _pacsAPITypes_H
