#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H
#include <QLabel>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QWheelEvent>

class DisplayScreen : public QLabel
{
    Q_OBJECT
public:
    DisplayScreen();
    DisplayScreen(QWidget* parent = nullptr) : QLabel(parent){ }

    void setImg(QPixmap pixmap);

    QPixmap img;

protected:
    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;

    void wheelEvent (QWheelEvent *event) override;
};


#endif // DISPLAYSCREEN_H
