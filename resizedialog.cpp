#include "resizedialog.h"
#include "ui_resizedialog.h"
#include "displayscreen.h"

resizeDialog::resizeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::resizeDialog)
{
    ui->setupUi(this);

}

resizeDialog::~resizeDialog()
{
    delete ui;
}

void resizeDialog::setSize(QSize rSize)
{
    this->rSize = rSize;
    QString info;
    QTextStream(&info) << rSize.width();

    ui->lineEdit_x->setText(info);

    info.clear();
    QTextStream(&info) << rSize.height();

    ui->lineEdit_y->setText(info);
}

void resizeDialog::on_pushButton_cancel_clicked()
{
    this->close();
}


void resizeDialog::on_pushButton_apply_clicked()
{
    QSize newSize(ui->lineEdit_x->text().toInt(), ui->lineEdit_y->text().toInt());
    setSize(newSize);
    accept();
}

