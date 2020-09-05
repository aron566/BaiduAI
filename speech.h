#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>

class speech : public QObject
{
    Q_OBJECT
public:
    explicit speech(QObject *parent = nullptr);

signals:

public slots:

public:
    QString speechIdentify(QString audioFile);
private:
    QString getJsonValue(QByteArray &data, QString &key);
private:
    QString accessToken;
};

#endif // SPEECH_H
