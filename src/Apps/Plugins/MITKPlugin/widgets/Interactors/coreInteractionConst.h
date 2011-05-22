/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2006-11-24 15:13:54 +0100 (vie, 24 nov 2006) $
Version:   $Revision: 8590 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef COREMITKINTERACTCONST_H
#define COREMITKINTERACTCONST_H

//##Documentation
//## @file CoreMitkInteractionConst.h
//## @brief Constants for most interaction classes, due to the generic StateMachines.
//##
//## Changes in Type, ButtonState or Key has to be don in mitkEventMapper.cpp, too.
//## @ingroup Interaction

/*Prefixes for Constants:
E = Enumeration
EID = EventId's
Op = Operations
Ac = Action
Type_ = Type of Event
BS_ = ButtonStates and Buttons
Key_ = Keys like in QT
*/
namespace Core{


//Constants for EventIds; use the according constant to through an event in the code
typedef enum EEventIds
{
  EIDNULLEVENT = 0,
  EIDENABLEPANELWIDGET = 1,  // Enable panel widget
  EIDINPUTDATAENTITYMODIFIED = 2,  // Input DataEntity of the panel widget has been modified
  EIDDISABLEPANELWIDGET = 3,  // Panel widget has been disabled
  EIDRESET = 5,  // Reset the panel widget to the initial state
  EIDTIMECHANGED = 9,  // Current time step has been changed
  EIDUNDO = 12,  // Undo last action
  EIDPOINTSMODIFIED = 4,  // Interactor points has been modified
  EIDLOADLANDMARKS = 7,  // Load landmarks from file
  EIDSAVELANDMARS = 8,  // Save landmarks to file
  EIDNEWLANDMARKS = 15,  // Landmarks data entity has been changed
  EIDCARDIACMODELCHANGED = 6,  // Cardiac model has been changed
  EIDYES = 10,  // After checking something in some state, the result is yes
  EIDNO = 11,  // After checking something in some state, the result is NO
  EIDACTIONFINISHED = 14,  // Action has finished
  EIDAPPLYMODELCONSTRAINTS = 13  // Apply model constraints


};

//##Constants for Operations
//## xomments are always examples of the usage
typedef enum EOperations
{
  OpNOTHING = 0,
};

//##Constants for EventMapping...
//##connects the statemachine.xml-File with the implemented conditions.
//##within one statemachine the choice of the actionconstants is freely
//##
//## ActionId
typedef enum EActions
{
  AcDONOTHING = 0,
  AcENABLEINTERACTORS = 1,  // Enable interactors
  AcDISABLEINTERACTORS = 2,  // Disable mitk::Interactors
  AcRESETINTERACTORS = 3,  // Reset interactors to initial state, without points
  AcCHECKINTERACTORCONDITIONS = 4,  // Check that the conditions for using interactions are ok
  AcCHECKINTERACTORFULL = 5,  // Check that the interactor for adding landmarks is full
  AcADDINTERACTORPOINTEXTRAINFO = 6,  // Add extra info to the interactor point
  AcUPDATEPROCESSOR = 7,  // Call processor Update
  AcUPDATETIMESTEP = 10,  // Update time step to the processor and interactors
  AcUNDO = 11,  // Undo last action
  AcUPDATENUMBEROFTIMESTEPS = 16,  // Update time step of the landmark points
  AcACTIONSFINISHED = 17,  // The actions of one transition has been finished and we can go to the next state
  AcAPPLYMODELCONSTRAINTS = 12,  // Apply model constraints
  AcLOADLANDMARKS = 8,  // Load the landmarks from file
  AcSAVELANDMARKS = 9,  // Save landmarks to file
  AcCHECKMOVINGPOINT = 13,  // User is moving a point of the interactor
  AcSTARTMOVINGPOINT = 15,  // The user started moving a point
  AcFINISHEDMOVINGPOINT = 14,  // The user finished moving a point
  AcCHECKLANDMARKSNOTNULL = 18  // 


};


/*
//!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!
//EventMechanism:
//!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!
*/

//Type of an Event;
typedef enum EEventType
{
  Type_None = 0,				// invalid event
  Type_MaxUser = 65535
};


//##ButtonState
// mouse/keyboard state values
//QT combinations if MOUSEBUTTONRelease: left MouseButton + ControlButton: 0x201
typedef enum EButtonStates
{
  BS_NoButton        = 0x0000,
};
//##Key
typedef enum EKeys
{
  Key_none = 0xffff//= Key_unknown
};

}//namespace mitk
#endif //ifndef COREMITKINTERACTCONST_H

