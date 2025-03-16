#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <filesystem>
#include <QPainter>
#include "resizedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hsvDialog(new HSVDialog(this))
{
    ui->setupUi(this);

    connect(hsvDialog, &HSVDialog::valueChanged, this, &MainWindow::updateHSV);

    hsvProcess = new hsv_opengl();

    //ui->label->resize(400,400);
    //ui->label->setStyleSheet("QLabel { background-color : gray; }");

    textureScreen = new DisplayScreen(this);
    //myWidget->move(50, 50);
    textureScreen->setStyleSheet("QLabel { background-color : gray; }");
    textureScreen->setMinimumSize(750, 750);
    textureScreen->setMaximumSize(750, 750);
    //myWidget->resize(500,500);
    textureScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //myWidget->show();

    ui->pushButton_apply->setEnabled(false);


    ui->verticalLayout->addWidget(textureScreen);
    //this->layout()->addWidget(myWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
        "",
        imageTypes);

    if(fileName.isEmpty()){
        return;
    }

    QPixmap img(fileName);
    QImage img2(fileName);
    image = img2;
    originalImage = img2;
    //TODO: not accessible
    img = QPixmap::fromImage(img2);

    QSize size =  img.size();

    //Image Size
    uintmax_t size_in_bytes = std::filesystem::file_size(fileName.toStdString());
    double size_in_mb = static_cast<double>(size_in_bytes) / (1024 * 1024);

    //Image Info
    QString info;
    QTextStream(&info) << size.width() << " X " << size.height() << " | " << size_in_mb << " MB";

    ui->label_info->setText(info);

    textureScreen->resetTransform();
    textureScreen->setImg(img);
}

void MainWindow::on_pushButton_save_clicked()
{
    if(image.isNull()){
        qDebug() << "image is null";
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save Image",
        "",
        imageTypes
        );


    if (!filePath.isEmpty() && !image.isNull()) {
        if (image.save(filePath)) {
            qDebug() << "Image saved successfully to:" << filePath;
        } else {
            qDebug() << "Failed to save image!";
        }
    }
}

void MainWindow::on_pushButton_crop_clicked()
{
    if(image.isNull()){
        return;
    }
    textureScreen->toggleCropping();
    ui->pushButton_apply->setEnabled(!ui->pushButton_apply->isEnabled());
}


void MainWindow::on_pushButton_apply_clicked()
{
    QRect cropRect = textureScreen->getCropRegion();
    QImage croppedImage = image.copy(cropRect);
    QPixmap img = QPixmap::fromImage(croppedImage);
    image = croppedImage;
    textureScreen->setImg(img);
    ui->pushButton_apply->setEnabled(false);

}


void MainWindow::on_pushButton_revert_clicked()
{
    image = originalImage;
    textureScreen->setImg(QPixmap::fromImage(image));
}


void MainWindow::on_pushButton_rescale_clicked()
{
    resizeDialog resize;
    resize.setSize(image.size());

    if (resize.exec() == QDialog::Accepted) {
        QSize newSize = resize.getSize();
        textureScreen->resizedSize = newSize;
        textureScreen->reSize(originalImage);
        image = image.scaled(newSize);
        qDebug() << "Selected size:" << newSize;
    }

}

void MainWindow::on_pushButton_HSV_clicked()
{
    hsvDialog->show();
}

void MainWindow::updateHSV(int index, int value)
{
    if(index == 0){
        hsv[index] = (value / 200.0f) * 360.0;
    }
    else{
        hsv[index] = value / 100.0f;
    }

    qDebug() << hsv[0] << " " << hsv[1] << " " << hsv[2];

    if(!image.isNull()){
        //image = adjustHSV();
        //textureScreen->setImg(QPixmap::fromImage(adjustHSV()));

        if (image.format() != QImage::Format_RGBA8888) {
            image = image.convertToFormat(QImage::Format_RGBA8888);
        }

        hsvProcess->processImage(image, hsv[0], hsv[1],hsv[2]);
        QImage result = hsvProcess->getResult();
        textureScreen->updateHSV(QPixmap::fromImage(result));
    }

}

QImage MainWindow::adjustHSV()
{
    QImage newImage = image.convertToFormat(QImage::Format_RGB32);
/*
    for (int y = 0; y < newImage.height(); ++y)
    {
        for (int x = 0; x < newImage.width(); ++x)
        {
            QColor color = newImage.pixelColor(x, y);

            int h, s, v;
            color.getHsv(&h, &s, &v); // Get HSV values

            h = (h + hsv[0]) % 360;
            if (h < 0) h += 360;  // Ensure hue stays in [0, 359]

            s = qBound(0, s + hsv[1], 255); // Clamp Saturation
            v = qBound(0, v + hsv[2], 255);      // Clamp Value

            color.setHsv(h, s, v);
            newImage.setPixelColor(x, y, color);
        }
    }
*/
    return newImage;
}

