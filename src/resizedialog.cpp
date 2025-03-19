#include "resizedialog.h"
#include "ui/ui_resizedialog.h"
#include <QIntValidator>

resizeDialog::resizeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::resizeDialog)
{
    ui->setupUi(this);

    setMaximumSize(800,600);

    ui->lineEdit_x->setValidator(new QIntValidator(this));
    ui->lineEdit_y->setValidator(new QIntValidator(this));
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

    aspectRatio = rSize.width() / static_cast<float>(rSize.height());
}

void resizeDialog::on_pushButton_cancel_clicked()
{
    this->close();
}


void resizeDialog::on_pushButton_apply_clicked()
{
    QSize newSize(ui->lineEdit_x->text().toInt(), ui->lineEdit_y->text().toInt());
    setSize(newSize);
    qDebug() << newSize;
    accept();
}


void resizeDialog::on_checkBox_keepAspectRatio_checkStateChanged(const Qt::CheckState &arg1)
{

}


void resizeDialog::on_lineEdit_x_textChanged(const QString &arg1)
{
    if(ui->checkBox_keepAspectRatio->isChecked() && !isEditing){
        isEditing = true;
        int newHeight = arg1.toInt() / aspectRatio;
        QString info;
        QTextStream(&info) << newHeight;
        ui->lineEdit_y->setText(info);
        isEditing = false;
    }
}


void resizeDialog::on_lineEdit_y_textChanged(const QString &arg1)
{
    if(ui->checkBox_keepAspectRatio->isChecked() && !isEditing){
        isEditing = true;
        int newWidth = arg1.toInt() * aspectRatio;
        QString info;
        QTextStream(&info) << newWidth;
        ui->lineEdit_x->setText(info);
        isEditing = false;
    }
}

