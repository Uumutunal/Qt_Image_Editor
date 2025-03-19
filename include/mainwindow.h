#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QDebug>
#include "displayscreen.h"
#include "hsvdialog.h"
#include "hsv_opengl.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_crop_clicked();

    void on_pushButton_apply_clicked();

    void on_pushButton_revert_clicked();

    void on_pushButton_rescale_clicked();

    void on_pushButton_HSV_clicked();

    void updateHSV(int index, int value);

private:
    Ui::MainWindow *ui;
    QImage image;
    QImage originalImage;
    DisplayScreen *textureScreen;
    QString imageTypes = "PNG Files (*.png);;JPEG Files (*.jpg *.jpeg);;BMP Files (*.bmp);;TIFF Files (*.tiff *.tif);;All Files (*)";

    HSVDialog *hsvDialog;
    hsv_opengl *hsvProcess;

    float hsv[3] = {0,1,1};
};



#endif // MAINWINDOW_H
