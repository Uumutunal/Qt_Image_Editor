#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class resizeDialog;
}

class resizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit resizeDialog(QWidget *parent = nullptr);
    ~resizeDialog();

    void setSize(QSize rSize);
    QSize getSize() const { return rSize; }

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_apply_clicked();

private:
    Ui::resizeDialog *ui;

    QSize rSize;
};

#endif // RESIZEDIALOG_H
