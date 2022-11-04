#include "pcmcaptureandplay.h"

PcmCaptureAndPlay::PcmCaptureAndPlay()
{

}

PcmCaptureAndPlay::~PcmCaptureAndPlay()
{
    if (audioIO) {
        if (audioIO->isOpen()) audioIO->close();
        delete audioIO;    //qIODevice need to manual release, or will memory leak
        audioIO = nullptr;
    }

    if (out) {
        out->stop();
        delete out;
        out = nullptr;
    }
}

bool PcmCaptureAndPlay::initQtAudioForPlay()
{
    qAudioFormat.setSampleRate(16000);
    qAudioFormat.setChannelCount(1);
    qAudioFormat.setSampleSize(16);
    qAudioFormat.setCodec("audio/pcm");
    qAudioFormat.setByteOrder(QAudioFormat::LittleEndian);
    qAudioFormat.setSampleType(QAudioFormat::SignedInt);

    out = new QAudioOutput(qAudioFormat);
    if (!out) {
        return false;
    }

    audioIO = out->start();
    return true;
}

bool PcmCaptureAndPlay::initQtAudioForCapture()
{
    qIODevice = nullptr;
    format.setSampleRate(16000);
    format.setChannelCount(1);      // 设定声道数目，mono(平声道)的声道数目是1；stero(立体声)的声道数目是2
    format.setSampleSize(16);       // 采样位深
    format.setCodec("audio/pcm");   // 设置唯一支持的codec
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format))
    {
        format = info.nearestFormat(format);
    }

    file.setFileName("test.raw");
    file.open( QIODevice::WriteOnly | QIODevice::Truncate );

    audioInput = new QAudioInput(format, nullptr);
    qIODevice = audioInput->start(); // 这里可以直接写入到文件，咱不用直接就是播放 audioInput->start(file);
    //qIODevice need to manual release, or will memory leak

    if (qIODevice) {
        qDebug() << "device available";
        return true;
    }

    return false;
}

void PcmCaptureAndPlay::onReadyRead()
{
    qint64 len = 0, size = 4096;
    char buffer[4096]{0};

    len = qIODevice->read(buffer, size); //读取音频

    if (len <= 0) {
        return;
    }

    qDebug() << "read pcm size" << len;

    qint64 bufferPlay = out->periodSize();

    if (out->bytesFree() < bufferPlay) { //
        qDebug() << "play buffer not enough";
        return;
    }

    audioIO->write((const char*)(buffer),len);
}

void PcmCaptureAndPlay::audioCaptureAndPlay()
{
    initQtAudioForPlay(); // for play
    initQtAudioForCapture(); // for capture
    connect(qIODevice, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void PcmCaptureAndPlay::run()
{
    audioCaptureAndPlay();
    exec();
}
