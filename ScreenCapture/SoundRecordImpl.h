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
private:
	enum RecordState {
		NotStarted,
		Started,
		Paused,
		Stopped,
		Unknown,
	};
public:
	ScreenRecordImpl(QObject * parent = Q_NULLPTR);
	void Init(const QVariantMap& map);

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
	void Release();

private:
	QString						m_filePath;
	int							m_bitrate;
	int							m_aIndex;	//������Ƶ������
	int							m_aOutIndex;//�����Ƶ������
	AVFormatContext				*m_aFmtCtx;
	AVFormatContext				*m_oFmtCtx;
	AVCodecContext				*m_aDecodeCtx;
	AVCodecContext				*m_aEncodeCtx;
	SwrContext					*m_swrCtx;
	AVAudioFifo					*m_aFifoBuf;

	std::atomic_bool			m_stop;

	//Frame�ﵥ��ͨ����������
	//��ʱ�����������е�nb_samples����һ����
	int							m_nbSamples;	
	std::mutex					m_mtx;
	std::condition_variable		m_cvNotEmpty;	//��fifoBuf���ˣ������̹߳���
	std::condition_variable		m_cvNotFull;	//��fifoBuf���ˣ��ɼ��̹߳���
	std::mutex					m_mtxPause;
	std::condition_variable		m_cvNotPause;	//�������ͣ��ʱ�򣬲ɼ��̹߳���
	RecordState					m_state;
};