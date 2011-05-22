#ifndef GBLQTTESTGUI_H
#define GBLQTTESTGUI_H

#include "gblBridge.h"
#include "QTextEdit.h"
#include "QPushButton.h"
#include "QComboBox.h"
#include "QVBox.h"

namespace gbl
{

class /*GUIBRIDGELIB_EXPORT*/ QtTestGUI : public QVBox
{
public:
	QtTestGUI(QWidget* parent = 0, const char* name= "");
	void SetBridge( Bridge _bridge );
	void EmulateClick();

	Bridge bridge;
	QPushButton* btnDoIt;
	QTextEdit* txtFilename;
	QComboBox* cmbType;
};

} // namespace gbl

#endif //GBLQTTESTGUI_H
