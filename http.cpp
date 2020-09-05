#include "http.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

http::http(QObject *parent) : QObject(parent)
{

}

bool http::http_postrequst(QString Url ,QMap<QString ,QString>header ,QByteArray &requestData ,QByteArray &replyData)
{
    QNetworkAccessManager manager;/**< 发送请求的动作*/
    QNetworkRequest request;/**< 请求内容（包括url和头）*/
    request.setUrl(Url);
    QMapIterator<QString ,QString> it(header);
    while(it.hasNext())
    {
        it.next();
        request.setRawHeader(it.key().toLatin1() ,it.value().toLatin1());
    }

    /*发送请求等待响应*/
    QNetworkReply *Reply = manager.post(request ,requestData);
    QEventLoop l;
    connect( Reply ,&QNetworkReply::finished ,&l ,&QEventLoop::quit);
    l.exec();

    if(Reply != nullptr && Reply->error() == QNetworkReply::NoError)
    {
        replyData = Reply->readAll();
        return true;
    }
    return false;
}
