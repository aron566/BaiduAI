#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioFormat>
#include <QFile>
#include <speech.h>

class audio : public QObject
{
    Q_OBJECT
public:
    explicit audio(QObject *parent = nullptr);

signals:

public slots:

public:
    void startAudio(QString fileName);

    void stopAudio();

    QString startSpeech();
private:
    QFile *Audiofile;
    QAudioInput *AudioDevice;
    speech *Speech;
    QString AudiofileName;
};

#endif // AUDIO_H
