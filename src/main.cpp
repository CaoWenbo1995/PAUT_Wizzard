#include <QApplication>

#include "PAConfigSolutionDlg.h"

#pragma warning (disable: 4996)

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	PAConfigSolutionDlg dlg;
	dlg.show();

	return app.exec();
}