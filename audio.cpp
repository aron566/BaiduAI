#include "audio.h"
#include <QMessageBox>
#include <QDebug>

/**
 * @brief audio::audio
 * @param parent
 */
audio::audio(QObject *parent) : QObject(parent)
{
    Speech = new speech(this);
}

/**
 * @brief audio::startAudio
 * @param fileName
 */
void audio::startAudio(QString fileName)
{
    if(fileName.isEmpty())
    {
        QMessageBox::warning(nullptr, "QAudioDeviceInfo", "录音文件必须填写");
        return;
    }
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultInputDevice();
    if(device.isNull())
    {
       QMessageBox::warning(nullptr, "QAudioDeviceInfo", "录音设备不存在");
    }
    else
    {
        /*记录录音文件*/
        AudiofileName = fileName;
        /*音频编码要求*/
        QAudioFormat m_format;
        /*设置采样频率*/
        m_format.setSampleRate(16000);
        /*设置通道数*/
        m_format.setChannelCount(1);
        /*设置位深*/
        m_format.setSampleSize(16);
        /*设置编码格式*/
        m_format.setCodec("audio/pcm");

        /*判断设备是否支持该格式*/
        if(!device.isFormatSupported(m_format))
        {
           /*找最接近的格式*/
           m_format = device.nearestFormat(m_format);
        }

        //打开文件
        Audiofile = new QFile;
        Audiofile->setFileName(fileName);
        Audiofile->open(QIODevice::WriteOnly);

        //创建录音对象
        AudioDevice = new QAudioInput(m_format, this);
        AudioDevice->start(Audiofile);
    }
}

/**
 * @brief audio::startSpeech
 * @return QString识别到的语音字符串
 */
QString audio::startSpeech()
{
    if(AudiofileName.isEmpty())
    {
        qDebug() << "文件名不存在";
        return QString("");
    }
    return Speech->speechIdentify(AudiofileName);
}

/**
 * @brief audio::stopAudio
 */
void audio::stopAudio()
{
    /*停止录音*/
    AudioDevice->stop();

    /*关闭文件*/
    Audiofile->close();

    /*删除对象*/
    delete Audiofile;
    Audiofile = nullptr;
}
