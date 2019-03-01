#include "SoundRecordTest.h"
#include "SoundRecordImpl.h"
#include <QTimer>

ScreenRecord::ScreenRecord(QObject *parent) :
	QObject(parent)
{
	m_args["filePath"] = "test.mp3";
	m_args["bit_rate"] = 128000;

	ScreenRecordImpl *sr = new ScreenRecordImpl(this);
	sr->Init(m_args);
	QTimer::singleShot(1000, sr, SLOT(Start()));
	QTimer::singleShot(6000, sr, SLOT(Pause()));
	QTimer::singleShot(8000, sr, SLOT(Stop()));
}
