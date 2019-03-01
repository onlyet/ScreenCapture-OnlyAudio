#include <QApplication>
#include "SoundRecordTest.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	ScreenRecord sr;

	return a.exec();
}
