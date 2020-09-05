#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <image_opt.h>
#include <http.h>
#include <audio.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void slotOpenFile();

    void on_SPEECHpushButton_pressed();

    void on_SPEECHpushButton_released();

private:
    Ui::MainWindow *ui;
    audio *Audio;

private:
    QString Imgfilepath;
};
#endif // MAINWINDOW_H
