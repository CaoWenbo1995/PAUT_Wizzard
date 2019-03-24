#include <QApplication>

#include "PAConfigSolutionDlg.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	PAConfigSolutionDlg dlg;
	dlg.show();

	return app.exec();
}