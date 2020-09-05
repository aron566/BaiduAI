#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

const QString url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2";
const QString client_id = "lb6NRnyge1I23EgRSfLwa5mW";     /**< 百度云API Key*/
const QString secret = "E9jYERAieNhsF8lFPjGB3co4ZzKFRytn";/**< 百度云Secret Key*/

const QString BaiduDetectUrl = "https://aip.baidubce.com/rest/2.0/image-classify/v1/animal?access_token=%1";


/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*语音识别*/
    Audio = new audio(this);
    /*初始化提示信息*/
    ui->SPEECHpushButton->setText("按住说话");

    /*连接菜单打开文件信号*/
    connect(ui->actionopen ,&QAction::triggered ,this ,&MainWindow::slotOpenFile);
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_pushButton_clicked
 */
void MainWindow::on_pushButton_clicked()
{
    if(Imgfilepath.isEmpty() == true)
    {
        return;
    }

    /*参考文档：https://cloud.baidu.com/doc/IMAGERECOGNITION/s/Xk3bcxdum*/
    QByteArray img = image_opt::Image2Base64(Imgfilepath);
    QByteArray imageData = "image=" + img;/**< body*/

    /*获取access_token*/
    QByteArray replyData;

    QString Url = QString(url).arg(client_id).arg(secret);
    QMap<QString ,QString>header;
    header.insert(QString("Content-Type") ,QString("application/x-www-form-urlencoded"));
    bool ret = http::http_postrequst(Url ,header ,imageData ,replyData);
    if(ret == false)
    {
        return;
    }
    /*获得token解析参考：http://ai.baidu.com/ai-doc/REFERENCE/Ck3dwjhhu*/
    QJsonObject Obj = QJsonDocument::fromJson(replyData).object();
    QString AccsessToken = Obj.value("access_token").toString();

    /*请求识别*/
    replyData.clear();
    QString imgUrl = BaiduDetectUrl.arg(AccsessToken);
    ret = http::http_postrequst(imgUrl ,header ,imageData ,replyData);
    if(ret == false)
    {
        return;
    }
    Obj = QJsonDocument::fromJson(replyData).object();
    QJsonValue val = Obj.value("result");
    if(val.isArray())
    {
        QJsonValue first = val.toArray().at(0);
        if(first.isObject())
        {
            QString resultStr = first.toObject().value("name").toString();
            /*识别结果显示输入*/
            ui->lineEdit->setText(resultStr);
            return;
        }
    }
    ui->lineEdit->setText("unknow");
}

/**
 * @brief MainWindow::slotOpenFile
 */
void MainWindow::slotOpenFile()
{
    Imgfilepath = QFileDialog::getOpenFileName(this ,"选择图片" ,QCoreApplication::applicationFilePath());
    if(Imgfilepath.isEmpty() == true)
    {
        QMessageBox::warning(this ,"提示" ,"选择一张图片");
    }
    else
    {
        QPixmap pix(Imgfilepath);
        ui->label->setPixmap(pix);
    }
}

/**
 * @brief MainWindow::on_SPEECHpushButton_pressed
 */
void MainWindow::on_SPEECHpushButton_pressed()
{
    ui->SPEECHpushButton->setText("松开识别");

    /*开始录音*/
    Audio->startAudio("audiofile");
}

/**
 * @brief MainWindow::on_SPEECHpushButton_released
 */
void MainWindow::on_SPEECHpushButton_released()
{
    ui->SPEECHpushButton->setText("按住说话");
    /*停止录音*/
    Audio->stopAudio();

    /*启动识别*/
    QString retStr = Audio->startSpeech();
    ui->lineEdit->setText(retStr);
}
