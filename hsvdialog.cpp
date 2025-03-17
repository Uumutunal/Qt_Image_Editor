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

void HSVDialog::resetValues()
{
    ui->lineEdit_hue->setText("0");
    ui->lineEdit_saturation->setText("100");
    ui->lineEdit_value->setText("100");

    ui->horizontalSlider_hue->setSliderPosition(0);
    ui->horizontalSlider_saturation->setSliderPosition(100);
    ui->horizontalSlider_value->setSliderPosition(100);
}

void HSVDialog::on_pushButton_accept_clicked()
{
    accept();
}


void HSVDialog::on_pushButton_cancel_clicked()
{
    resetValues();
    emit valueChanged(0, 0);
    emit valueChanged(1, 100);
    emit valueChanged(2, 100);
    this->close();
}


void HSVDialog::on_horizontalSlider_hue_sliderMoved(int position)
{
    QString info;
    QTextStream(&info) << position;
    ui->lineEdit_hue->setText(info);

    emit valueChanged(0, position);
}

void HSVDialog::on_horizontalSlider_saturation_sliderMoved(int position)
{
    QString info;
    QTextStream(&info) << position;
    ui->lineEdit_saturation->setText(info);

    emit valueChanged(1, position);
}


void HSVDialog::on_horizontalSlider_value_sliderMoved(int position)
{
    QString info;
    QTextStream(&info) << position;
    ui->lineEdit_value->setText(info);

    emit valueChanged(2, position);
}


void HSVDialog::on_lineEdit_hue_textEdited(const QString &arg1)
{
    ui->horizontalSlider_hue->setSliderPosition(arg1.toInt());

    emit valueChanged(0, arg1.toInt());
}


void HSVDialog::on_lineEdit_saturation_textEdited(const QString &arg1)
{
    ui->horizontalSlider_saturation->setSliderPosition(arg1.toInt());

    emit valueChanged(1, arg1.toInt());
}


void HSVDialog::on_lineEdit_value_textEdited(const QString &arg1)
{
    ui->horizontalSlider_value->setSliderPosition(arg1.toInt());

    emit valueChanged(2, arg1.toInt());
}

