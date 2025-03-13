#include "hsvdialog.h"
#include "ui_hsvdialog.h"
#include <QIntValidator>

HSVDialog::HSVDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HSVDialog)
{
    ui->setupUi(this);

    ui->lineEdit_hue->setValidator(new QIntValidator(this));
    ui->lineEdit_saturation->setValidator(new QIntValidator(this));
    ui->lineEdit_value->setValidator(new QIntValidator(this));
}

HSVDialog::~HSVDialog()
{
    delete ui;
}

void HSVDialog::on_pushButton_accept_clicked()
{
    accept();
}


void HSVDialog::on_pushButton_cancel_clicked()
{
    this->close();
}


void HSVDialog::on_horizontalSlider_hue_sliderMoved(int position)
{
    QString info;
    QTextStream(&info) << position;
    ui->lineEdit_hue->setText(info);
}

void HSVDialog::on_horizontalSlider_saturation_sliderMoved(int position)
{
    QString info;
    QTextStream(&info) << position;
    ui->lineEdit_saturation->setText(info);
}


void HSVDialog::on_horizontalSlider_value_sliderMoved(int position)
{
    QString info;
    QTextStream(&info) << position;
    ui->lineEdit_value->setText(info);
}


void HSVDialog::on_lineEdit_hue_textEdited(const QString &arg1)
{
    ui->horizontalSlider_hue->setSliderPosition(arg1.toInt());
}


void HSVDialog::on_lineEdit_saturation_textEdited(const QString &arg1)
{
    ui->horizontalSlider_saturation->setSliderPosition(arg1.toInt());
}


void HSVDialog::on_lineEdit_value_textEdited(const QString &arg1)
{
    ui->horizontalSlider_value->setSliderPosition(arg1.toInt());
}

