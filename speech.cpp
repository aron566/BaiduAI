#include "speech.h"
#include <http.h>
#include <QMap>
#include <QByteArray>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QHostInfo>

#include <QDebug>

const QString BaiduSpeechUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2";

const QString BaiduSpeechClientID = "kB4C4e4ojyTgnsBIYBMM1DlO";

const QString BauduSpeechClientSecret = "BAo0fgL8ejScn5BFjhYPuM5Q8YN61nwl";

const  QString BaiduSpeechSatrtUrl = "https://vop.baidu.com/pro_api?dev_pid=80001&cuid=%1&token=%2";/**< 识别方言 本机唯一标识 获取的token*/
/*
 * 80001 普通话
 *
 */

/**
 * @brief speech::speech
 * @param parent
 */
speech::speech(QObject *parent) : QObject(parent)
{

}

/**
 * @brief speech::speechIdentify
 * @param audioFile
 * @return
 */
QString speech::speechIdentify(QString audioFile)
{
    if(audioFile.isEmpty())
    {
        qDebug() << "文件名不存在";
        return QString("");
    }
    bool ret = false;

    /*获取access token*/
    QString TokenUrl = QString(BaiduSpeechUrl).arg(BaiduSpeechClientID).arg(BauduSpeechClientSecret);
    QMap<QString ,QString>header;
    header.insert(QString("Content-Type") ,QString("audio/pcm;rate=16000"));

    QByteArray requestData;
    QByteArray replyData;

    /*accessToken一次获取可用30天*/
    if(accessToken.isEmpty() == true)
    {
        ret = http::http_postrequst(TokenUrl ,header ,requestData ,replyData);
        if(ret)
        {
            QString key = "access_token";
            accessToken = getJsonValue(replyData ,key);
            replyData.clear();

            qDebug() << "获取的token" << accessToken;
        }
        else
        {

        }
    }

    /*识别语音请求*/
    QString speechUrl = QString(BaiduSpeechSatrtUrl).arg(QHostInfo::localHostName()).arg(accessToken);

    /*把文件转换成QByteArray*/
    QFile file;
    file.setFileName(audioFile);
    file.open(QIODevice::ReadOnly);
    requestData = file.readAll();
    file.close();

    if(requestData.isEmpty())
    {
        return QString("语音数据为空");
    }
    /*再次发起请求*/
    ret = http::http_postrequst(speechUrl ,header ,requestData ,replyData);
    if(ret)
    {
        QString key = "result";
        QString text = getJsonValue(replyData, key);
        return text;
    }
    else
    {
        QMessageBox::warning(nullptr, "识别提示", "识别失败");
    }

    return QString("");
}

/**
 * @brief speech::getJsonValue
 * @param data
 * @param key
 * @return
 */
QString speech::getJsonValue(QByteArray &data, QString &key)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &parseError);
    QString retStr = "";
    if(parseError.error == QJsonParseError::NoError)
    {

        if(jsonDocument.isObject())
        {
            /*jsonDocument转换成json对象*/
            QJsonObject jsonObj = jsonDocument.object();
            if(jsonObj.contains(key))
            {
                QJsonValue jsonVal = jsonObj.value(key);
                if(jsonVal.isString())
                {
                    return jsonVal.toString();
                }

                /*检查是否是数组*/
                if(jsonVal.isArray())
                {
                    QJsonArray arr = jsonVal.toArray();
                    for(int index = 0;index < arr.size();index++)
                    {
                        QJsonValue subValue = arr.at(index);

                        if(subValue.isString())
                        {
                            retStr += subValue.toString() + " ";
                        }
                    }
                    return retStr;
                }
            }
            else
            {
                qDebug() << "不包含关键字:" << key;
            }//contains(key)
        }
        else
        {
            qDebug() << "不是json对象";
        }//isObject
    }
    else
    {
        qDebug() << "未成功解析JSON";
    }//NoError
    qDebug() << "未成功解析JSON："<< data.data();
    return QString("");
}
