#ifndef HSVDIALOG_H
#define HSVDIALOG_H

#include <QDialog>

namespace Ui {
class HSVDialog;
}

class HSVDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HSVDialog(QWidget *parent = nullptr);
    ~HSVDialog();

private slots:
    void on_pushButton_accept_clicked();

    void on_pushButton_cancel_clicked();

    void on_horizontalSlider_hue_sliderMoved(int position);

    void on_horizontalSlider_saturation_sliderMoved(int position);

    void on_horizontalSlider_value_sliderMoved(int position);

    void on_lineEdit_hue_textEdited(const QString &arg1);

    void on_lineEdit_saturation_textEdited(const QString &arg1);

    void on_lineEdit_value_textEdited(const QString &arg1);

private:
    Ui::HSVDialog *ui;

    float hue;
    float saturation;
    float value;
};

#endif // HSVDIALOG_H
