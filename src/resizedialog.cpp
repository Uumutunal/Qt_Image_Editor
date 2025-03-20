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
    accept();
}


void resizeDialog::on_lineEdit_x_textEdited(const QString &arg1)
{
    int newHeight = arg1.toInt() / aspectRatio;
    QString info;
    QTextStream(&info) << newHeight;
    ui->lineEdit_y->setText(info);
}


void resizeDialog::on_lineEdit_y_textEdited(const QString &arg1)
{
    int newWidth = arg1.toInt() * aspectRatio;
    QString info;
    QTextStream(&info) << newWidth;
    ui->lineEdit_x->setText(info);
}

