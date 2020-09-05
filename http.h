#ifndef HTTP_H
#define HTTP_H

#include <QObject>

class http : public QObject
{
    Q_OBJECT
public:
    explicit http(QObject *parent = nullptr);

signals:

public slots:

public:
   static bool http_postrequst(QString Url ,QMap<QString ,QString>header ,QByteArray &requestData ,QByteArray &replyData);
};

#endif // HTTP_H
