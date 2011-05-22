/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#ifndef CILABQTDESIGNERMACROS_H
#define CILABQTDESIGNERMACROS_H

/**
This file contains macro's that allow you to wrap your Qt widget so that it can be dragged-and-dropped in Qt designer.
Example for a widget class called TestWidget:

The header file (TestWidgetPlugin.h) will look like this:

#ifndef TESTWIDGETPLUGIN_H
#define TESTWIDGETPLUGIN_H

#include "CilabQtDesignerMacros.h"
#include "QWidgetPlugin.h"

CILAB_DECLARE_QTDESIGNERWRAPPER(TestWidget)

#endif

The cpp file (TestWidgetPlugin.cpp) will look like this

#include "TestWidgetPlugin.h"
#include "TestWidget.h"

CILAB_DEFINE_QTDESIGNERWRAPPER(TestWidget, "MyTestGroup")
Q_EXPORT_PLUGIN( TestWidgetPlugin )

\author Maarten Nieber
\date July 19 2007
*/

/**
This macro declares a class xxxPlugin, where xxx is the classname that you supply to the macro.
This xxxPlugin class is the wrapper for Qt designer (see top of this file for explanation).
Use the CILAB_DEFINE_QTDESIGNERWRAPPER macro to define the class.
See also the example at the top of this file.

\param CILAB_WIDGETNAME - The name of the widget class that should be wrapped.
*/

#define CILAB_DECLARE_QTDESIGNERWRAPPER(CILAB_WIDGETNAME) \
class CILAB_WIDGETNAME##Plugin : public QWidgetPlugin \
{ \
public: \
    CILAB_WIDGETNAME##Plugin(); \
\
    QStringList keys() const; \
    QWidget* create( const QString &classname, QWidget* parent = 0, const char* name = 0 ); \
    QString group( const QString& ) const; \
    QIconSet iconSet( const QString& ) const; \
    QString includeFile( const QString& ) const; \
    QString toolTip( const QString& ) const; \
    QString whatsThis( const QString& ) const; \
    bool isContainer( const QString& ) const; \
};\

/**
This macro defines a class xxxPlugin, where xxx is the classname that you supply to the macro.
This xxxPlugin class is the wrapper for Qt designer (see top of this file for explanation).
Inside Qt designer, the xxxPlugin will appear in the group CILAB_GROUPNAME_AS_STRING (see params).
Use the CILAB_DECLARE_QTDESIGNERWRAPPER macro to declare the class.
See also the example at the top of this file.

\param CILAB_WIDGETNAME - The name of the widget class that should be wrapped.
\param CILAB_GROUPNAME_AS_STRING - Inside Qt designer, the wrapped widget will appear in this group 
*/

#define CILAB_DEFINE_QTDESIGNERWRAPPER(CILAB_WIDGETNAME, CILAB_GROUPNAME_AS_STRING) \
static const char *CILAB_WIDGETNAME##_pixmap[] = { \
    "22 22 8 1", \
    "  c Gray100", \
    ". c Gray97", \
    "X c #4f504f", \
    "o c #00007f", \
    "O c Gray0", \
    "+ c none", \
    "@ c Gray0", \
    "# c Gray0", \
    "++++++++++++++++++++++", \
    "++++++++++++++++++++++", \
    "++++++++++++++++++++++", \
    "++++++++++++++++++++++", \
    "+OOOOOOOOOOOOOOOOOOOO+", \
    "OOXXXXXXXXXXXXXXXXXXOO", \
    "OXX.          OO OO  O", \
    "OX.      oo     O    O", \
    "OX.      oo     O   .O", \
    "OX  ooo  oooo   O    O", \
    "OX    oo oo oo  O    O", \
    "OX  oooo oo oo  O    O", \
    "OX oo oo oo oo  O    O", \
    "OX oo oo oo oo  O    O", \
    "OX  oooo oooo   O    O", \
    "OX            OO OO  O", \
    "OO..................OO", \
    "+OOOOOOOOOOOOOOOOOOOO+", \
    "++++++++++++++++++++++", \
    "++++++++++++++++++++++", \
    "++++++++++++++++++++++", \
    "++++++++++++++++++++++" \
}; \
 \
CILAB_WIDGETNAME##Plugin::CILAB_WIDGETNAME##Plugin() \
{ \
} \
 \
QStringList CILAB_WIDGETNAME##Plugin::keys() const \
{ \
    QStringList list; \
    list << #CILAB_WIDGETNAME; \
    return list; \
} \
 \
QWidget* CILAB_WIDGETNAME##Plugin::create( const QString &key, QWidget* parent, const char* name ) \
{ \
    if ( key == #CILAB_WIDGETNAME ) \
	return new CILAB_WIDGETNAME( parent, name ); \
    return 0; \
} \
 \
QString CILAB_WIDGETNAME##Plugin::group( const QString& feature ) const \
{ \
    if ( feature == #CILAB_WIDGETNAME ) \
	return CILAB_GROUPNAME_AS_STRING; \
    return QString::null; \
} \
 \
QIconSet CILAB_WIDGETNAME##Plugin::iconSet( const QString& ) const \
{ \
    return QIconSet( QPixmap( CILAB_WIDGETNAME##_pixmap ) ); \
} \
 \
QString CILAB_WIDGETNAME##Plugin::includeFile( const QString& feature ) const \
{ \
    if ( feature == #CILAB_WIDGETNAME ) \
	return #CILAB_WIDGETNAME ## ".h"; \
    return QString::null; \
} \
 \
QString CILAB_WIDGETNAME##Plugin::toolTip( const QString& feature ) const \
{ \
    if ( feature == #CILAB_WIDGETNAME ) \
	return #CILAB_WIDGETNAME; \
    return QString::null; \
} \
 \
QString CILAB_WIDGETNAME##Plugin::whatsThis( const QString& feature ) const \
{ \
    if ( feature == #CILAB_WIDGETNAME ) \
	return #CILAB_WIDGETNAME; \
    return QString::null; \
} \
 \
bool CILAB_WIDGETNAME##Plugin::isContainer( const QString& ) const \
{ \
    return FALSE; \
} \
 \

#endif 
