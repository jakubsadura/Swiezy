#ifndef wxMitkVisor_H
#define wxMitkVisor_H

#include <wxMitkApp.h>

class wxComboBox;
namespace mitk { class wxMitkMultiRenderWindow; }

/**
The VisorApp class
*/
class wxMitkVisor : public mitk::wxMitkApp
{
public:
	virtual int OnRun(void);

private:
	void OnLayoutChanges(wxCommandEvent& event);
	mitk::wxMitkMultiRenderWindow* renderWindow;
	wxComboBox* cmbbox;
	DECLARE_EVENT_TABLE();
};

DECLARE_APP(wxMitkVisor)

#endif // wxMitkVisor_H
