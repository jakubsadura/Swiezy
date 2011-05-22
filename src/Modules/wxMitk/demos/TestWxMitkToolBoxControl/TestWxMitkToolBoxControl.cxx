#include "TestWxMitkToolBoxControl.h"
#include "wxMitkToolBoxControl.h"
#include "wxMitkToolBoxItem.h"
#include "wxMitkMaterialEditorWidget.h"
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <iostream>


IMPLEMENT_APP_CONSOLE(MyApp)

bool MyApp::OnInit()
{
	wxFrame* mainWindow = new wxFrame(NULL, wxID_ANY, "Test Material editor");
	mitk::wxMitkToolBoxControl* toolbox = new mitk::wxMitkToolBoxControl(mainWindow, wxID_ANY, true, wxDefaultPosition, wxSize(300, 600));

	wxTextCtrl* aTextBox = new wxTextCtrl(toolbox, wxID_ANY, "First item");
	mitk::wxMitkMaterialEditorWidget* bLayoutedWidget = new mitk::wxMitkMaterialEditorWidget(toolbox, wxID_ANY);
	mitk::wxMitkToolBoxItem* aItem = new mitk::wxMitkToolBoxItem(toolbox, wxID_ANY, aTextBox, "First tool");
	mitk::wxMitkToolBoxItem* bItem = new mitk::wxMitkToolBoxItem(toolbox, wxID_ANY, bLayoutedWidget, "Second tool");
	toolbox->Add(aItem);
	toolbox->Add(bItem);
	toolbox->Remove(aItem);
	toolbox->Add(aItem);
	mainWindow->Show(true);
	return true;
}

