#include "image_opt.h"
#include <QImage>
#include <QBuffer>
#include <QIODevice>
#include <QByteArray>
#include <QTextCodec>

image_opt::image_opt(QObject *parent) : QObject(parent)
{

}

/**
 * @brief image_opt::Image2Base64
 * @param filename
 * @return 对图片urlencode后的数据
 */
QByteArray image_opt::Image2Base64(QString filename)
{
    QImage img(filename);
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    /*写到内存文件*/
    img.save(&buf ,"JPG");

    /*图片转base64编码*/
    QByteArray base64 = ba.toBase64();

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    /*对图片urlencode*/
    QByteArray imgdata = codec->fromUnicode(base64).toPercentEncoding();

    return imgdata;
}
