#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QPixmap>
#include <filesystem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->label->resize(400,400);
    //ui->label->setStyleSheet("QLabel { background-color : gray; }");

    textureScreen = new DisplayScreen(this);
    //myWidget->move(50, 50);
    textureScreen->setStyleSheet("QLabel { background-color : black; }");
    textureScreen->setMinimumSize(750, 750); // Set a minimum size
    textureScreen->setMaximumSize(750, 750); // Set a maximum size
    //myWidget->resize(500,500);
    textureScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //myWidget->show();

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

    QPixmap img(fileName);
    QImage img2(fileName);
    image = img2;

    QSize size =  img.size();

    uintmax_t size_in_bytes = std::filesystem::file_size(fileName.toStdString());
    double size_in_mb = static_cast<double>(size_in_bytes) / (1024 * 1024);

    QString info;
    QTextStream(&info) << size.width() << " X " << size.height() << " | " << size_in_mb << " MB";

    ui->label_info->setText(info);

    float aspectRatioW = size.width() / (textureScreen->width());
    float aspectRatioH = size.height() / (textureScreen->height());

    textureScreen->setPixmap(img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio));
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



