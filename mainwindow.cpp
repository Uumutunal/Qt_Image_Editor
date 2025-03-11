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
{
    ui->setupUi(this);
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

