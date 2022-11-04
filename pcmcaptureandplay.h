/*
 *	auth: dujingning
 *	date: 2022.05.03
 *	licenst: MIT
*/

#ifndef PCMCAPTUREANDPLAY_H
#define PCMCAPTUREANDPLAY_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QFile>

#include <QAudio>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

class PcmCaptureAndPlay: public QThread
{
    Q_OBJECT

public:
    PcmCaptureAndPlay();
    ~PcmCaptureAndPlay();

private: /*   QT audio play  */
    QAudioFormat qAudioFormat;
    QAudioOutput *out;
    QIODevice *audioIO;
    QTimer *audioPlayTimer;
    QThread *timerTHread;
    bool initQtAudioForPlay();

private: /*   QT audio capture  */
    QAudioFormat format;
    QAudioInput* audioInput;
    QIODevice *qIODevice;
    QFile file; 			// 可以输出到文件，咱不用，直接播
    bool initQtAudioForCapture();
private: /*   QT audio capture  */
    void audioCaptureAndPlay();

public slots:
    void onReadyRead(); /*采集并填充*/

public:
    void run();
};

#endif // PCMCAPTUREANDPLAY_H
