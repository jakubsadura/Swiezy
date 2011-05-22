#include "TestWxMitkMaterialEditorWidget.h"
#include "wxMitkMaterialEditorWidget.h"
#include "wxMitkSphereViewCanvas.h"
#include "wxMitkSurfaceRepresentationControl.h"
#include "wxMitkColorSelectorControl.h"
#include "wx/button.h"
#include "wx/sizer.h"
#include "wx/frame.h"
#include <iostream>


IMPLEMENT_APP_CONSOLE(MyApp)

bool MyApp::OnInit()
{
	wxFrame* mainWindow = new wxFrame(NULL, wxID_ANY, "Test Material editor");
	mitk::wxMitkMaterialEditorWidget* materialWidget = new mitk::wxMitkMaterialEditorWidget(mainWindow, wxID_ANY, wxDefaultPosition, wxSize(300, 600));
	mainWindow->Show(true);
	return true;
}

