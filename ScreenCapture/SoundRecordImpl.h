#pragma once
#include <Windows.h>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <QObject>
#include <QString>
#include <QMutex>

#ifdef	__cplusplus
extern "C"
{
#endif
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFifoBuffer;
struct AVAudioFifo;
struct AVFrame;
struct SwsContext;
struct SwrContext;
#ifdef __cplusplus
};
#endif

//���̣߳�Qt GUI�߳�
//���̣߳�RecordAudioThreadProc
//���̣߳�AcquireSoundThreadProc
//���Է�����Ƶ�����ٶȱȲɼ��ٶ�Ҫ�죬����Ƶ�෴
//���ָ��̣߳����룩���ܻ������߳�֮ǰ�˳����ɼ���
//����������Ļ������Խ����߳�.join()���ڸ��߳��ͷ���Դ֮ǰ

class ScreenRecordImpl : public QObject
{
	Q_OBJECT
public:
	ScreenRecordImpl(QObject * parent = Q_NULLPTR);

	private slots :
	void Start();
	void Pause();
	void Stop();

private:
	//��fifobuf��ȡ��Ƶ֡�����룬д��������������ļ�
	void RecordAudioThreadProc();
	//����Ƶ��������ȡ֡��д��fifobuf
	void AcquireSoundThreadProc();
	int OpenAudio();
	int OpenOutput();
	QString GetSpeakerDeviceName();
	QString GetMicrophoneDeviceName();
	AVFrame* AllocAudioFrame(AVCodecContext* c, int nbSamples);
	//ȡ�����������֡��д�������
	void FlushEncoder();

private:
	QString				m_filePath;

	int m_aIndex;		//������Ƶ������
	int m_aOutIndex;	//�����Ƶ������
	AVFormatContext		*m_aFmtCtx;
	AVFormatContext		*m_oFmtCtx;
	AVCodecContext		*m_aDecodeCtx;
	AVCodecContext		*m_aEncodeCtx;
	SwrContext			*m_swrCtx;
	AVAudioFifo			*m_aFifoBuf;

	//AVFrame				*m_aOutFrame;
	//uint8_t				*m_aOutFrameBuf;
	int					m_aOutFrameSize;	//һ����Ƶ֡������������
	std::atomic_bool	m_stop;

	//Frame�ﵥ��ͨ����������
	//��ʱ�����������е�nb_samples����һ����
	int					m_nbSamples;	
	std::mutex			m_mtx;
	std::condition_variable	m_cvNotEmpty;
	std::condition_variable m_cvNotFull;
};