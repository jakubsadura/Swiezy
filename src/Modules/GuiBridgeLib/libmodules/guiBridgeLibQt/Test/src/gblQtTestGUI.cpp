#include "gblQtTestGUI.h"
#include "gblBridgeQt.h"

gbl::QtTestGUI::QtTestGUI(QWidget* parent, const char* name)
: QVBox(parent, name)
{
	txtFilename = new QTextEdit(this);
	txtFilename->setText("Hello");
	btnDoIt = new QPushButton(this);
	cmbType = new QComboBox(this);
	cmbType->insertItem("Hola");
}

void gbl::QtTestGUI::SetBridge( Bridge _bridge )
{
	if( !gbl::InitBridge(this->bridge, _bridge) )
		return;

	gbl::AddBridgeEvent(this->bridge, "SetFilename", this->btnDoIt );
	gbl::AddBridgeValue(this->bridge, "Type", this->cmbType);
	gbl::AddBridgeValue(this->bridge, "Filename", this->txtFilename);

	BridgeAsListAdapter adapter(this->bridge, "Type");
	int i = adapter.GetNrOfItems();
	adapter.SetItemText(0, "Hello");
	adapter.SetItemText(2, "Bye");
	i = adapter.GetNrOfItems();
	adapter.SetCurrentItemIndex(2);
	i = adapter.GetCurrentItemIndex(2);
	i++;
}

void gbl::QtTestGUI::EmulateClick()
{
	this->btnDoIt->animateClick();
}
