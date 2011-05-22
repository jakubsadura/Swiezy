<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text" indent="yes"/>
<xsl:template match="/">
<xsl:text disable-output-escaping = "yes">/*=========================================================================

Program:   Medical Imaging &amp; Interaction Toolkit
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
</xsl:text>

<xsl:for-each select="//event">
<xsl:text>  </xsl:text>
<xsl:value-of select="@NAME"/>
<xsl:text> = </xsl:text>
<xsl:value-of select="@ID"/>
<xsl:if test="position()!=last()">      
<xsl:text>,</xsl:text>
</xsl:if>
<xsl:if test="boolean(./comment())">
<xsl:text>  // </xsl:text>
<xsl:value-of select="./comment()"/>
</xsl:if>
<xsl:text disable-output-escaping = "yes">
</xsl:text>
</xsl:for-each>
<xsl:text disable-output-escaping = "yes">

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
</xsl:text>


<xsl:for-each select="//action">	
<xsl:text>  </xsl:text>
<xsl:value-of select="@NAME"/>
<xsl:text> = </xsl:text>
<xsl:value-of select="@ID"/>
<xsl:if test="position()!=last()">      
<xsl:text>,</xsl:text>
</xsl:if>
<xsl:if test="boolean(./comment())">
<xsl:text>  // </xsl:text>
<xsl:value-of select="./comment()"/>
</xsl:if>
<xsl:text disable-output-escaping = "yes">
</xsl:text>
</xsl:for-each>
<xsl:text disable-output-escaping = "yes">

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

</xsl:text>
</xsl:template>
</xsl:stylesheet>