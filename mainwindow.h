#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QDebug>
#include "displayscreen.h"
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

private:
    Ui::MainWindow *ui;
    QImage image;
    DisplayScreen *textureScreen;
    QString imageTypes = "PNG Files (*.png);;JPEG Files (*.jpg *.jpeg);;BMP Files (*.bmp);;TIFF Files (*.tiff *.tif);;All Files (*)";
};



#endif // MAINWINDOW_H
