#ifndef IMAGE_OPT_H
#define IMAGE_OPT_H

#include <QObject>
#include <QString>

class image_opt : public QObject
{
    Q_OBJECT
public:
    explicit image_opt(QObject *parent = nullptr);

signals:

public slots:

public:
    static QByteArray Image2Base64(QString filename);
};

#endif // IMAGE_OPT_H
