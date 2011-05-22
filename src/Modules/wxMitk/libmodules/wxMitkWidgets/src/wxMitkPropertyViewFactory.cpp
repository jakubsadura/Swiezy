/*
* Copyright (c) 2009,
* Computational Image and Simulation Technologies in Biomedicine (CISTIB),
* Universitat Pompeu Fabra (UPF), Barcelona, Spain. All rights reserved.
* See license.txt file for details.
*/

#include "wxMitkPropertyViewFactory.h"
#include "wx/stattext.h"

// the different view and editor classes
//#include <wxMitkBasePropertyView.h>
#include <wxMitkBoolPropertyView.h>
#include <wxMitkBoolPropertyEditor.h>
#include <wxMitkStringPropertyView.h>
#include <wxMitkStringPropertyEditor.h>
#include <wxMitkStringPropertyOnDemandEdit.h>
//#include <wxMitkColorPropertyView.h>
//#include <wxMitkColorPropertyEditor.h>
//#include <wxMitkNumberPropertyView.h>
//#include <wxMitkNumberPropertyEditor.h>

wxMitkPropertyViewFactory* wxMitkPropertyViewFactory::GetInstance()
{
	static wxMitkPropertyViewFactory instance;
	return &instance;
}

wxMitkPropertyViewFactory::wxMitkPropertyViewFactory()
{
}

wxMitkPropertyViewFactory::~wxMitkPropertyViewFactory()
{
}
wxWindow* wxMitkPropertyViewFactory::CreateView(const mitk::BaseProperty* property, unsigned int /*type*/, wxWindow* parent, const char* name)
{
	if( false )
	{
	}
	else if ( const mitk::StringProperty* prop = dynamic_cast<const mitk::StringProperty*>(property) )
	{
	  // a string property
	  return new wxMitkStringPropertyView(prop, parent, name);
	}
	//else if ( const mitk::ColorProperty* prop = dynamic_cast<const mitk::ColorProperty*>(property) )
	//{
	//  // a color property
	//  return new wxMitkColorPropertyView(prop, parent, name);
	//}
	else if ( const mitk::BoolProperty* prop = dynamic_cast<const mitk::BoolProperty*>(property) )
	{
		// a bool property
		return new wxMitkBoolPropertyView(prop, parent, name);
	}
	//else if ( const mitk::IntProperty* prop = dynamic_cast<const mitk::IntProperty*>(property) )
	//{
	//  // a number property
	//  return new wxMitkNumberPropertyView(prop, parent, name);
	//}
	//else if ( const mitk::FloatProperty* prop = dynamic_cast<const mitk::FloatProperty*>(property) )
	//{
	//  // a number property
	//  return new wxMitkNumberPropertyView(prop, parent, name);
	//}
	//else if ( const mitk::DoubleProperty* prop = dynamic_cast<const mitk::DoubleProperty*>(property) )
	//{
	//  // a number property
	//  return new wxMitkNumberPropertyView(prop, parent, name);
	//}
	//else if ( property != NULL )
	//{
	//  // some unknown property --> use the GetValueAsString() method to 
	//  return new wxMitkBasePropertyView(prop, parent, name);
	//}
	else
	{
		return new wxStaticText(parent, -1, wxT("Hallo"));
	}

	return NULL;
}

wxWindow* wxMitkPropertyViewFactory::CreateEditor(mitk::BaseProperty* property, unsigned int type, wxWindow* parent, const char* name)
{
	if (!property) return NULL;

	if( false )
	{

	}
	else if ( mitk::StringProperty* prop = dynamic_cast<mitk::StringProperty*>(property) )
	{
	  switch (type)
	  {
	    case etON_DEMAND_EDIT:
	      // a string property
	      return new wxMitkStringPropertyOnDemandEdit(prop, parent, name);
	    default:
	      // a string property
	      return new wxMitkStringPropertyEditor(prop, parent, name);
	  }
	}
	//else if ( mitk::ColorProperty* prop = dynamic_cast<mitk::ColorProperty*>(property) )
	//{
	//  // a color property
	//  return new wxMitkColorPropertyEditor(prop, parent, name);
	//}
	else if ( mitk::BoolProperty* prop = dynamic_cast<mitk::BoolProperty*>(property) )
	{
		// a bool property
		return new wxMitkBoolPropertyEditor(prop, parent, name);
	}
	//else if ( mitk::IntProperty* prop = dynamic_cast<mitk::IntProperty*>(property) )
	//{
	//  // a number property
	//  return new wxMitkNumberPropertyEditor(prop, parent, name);
	//}
	//else if ( mitk::FloatProperty* prop = dynamic_cast<mitk::FloatProperty*>(property) )
	//{
	//  // a number property
	//  wxMitkNumberPropertyEditor* pe = new wxMitkNumberPropertyEditor(prop, parent, name);
	//  pe->setDecimalPlaces(2);
	//  return pe;
	//}
	//else if ( mitk::DoubleProperty* prop = dynamic_cast<mitk::DoubleProperty*>(property) )
	//{
	//  // a number property
	//  wxMitkNumberPropertyEditor* pe = new wxMitkNumberPropertyEditor(prop, parent, name);
	//  pe->setDecimalPlaces(2);
	//  return pe;
	//}
	//else
	//{
	//  // some unknown property --> no editor possible
	//  return NULL;
	//}
	else
	{
		return new wxStaticText(parent, -1, wxT("Hallo"));
	}
}

