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
    void resetTransform();

    void toggleCropping();

    QPixmap img;

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
    void wheelEvent (QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *releaseEvent) override;
    void mousePressEvent(QMouseEvent *eventPress) override;
    void paintEvent(QPaintEvent *event) override;

    QTransform transform;
    QPoint mousePos;
    QPoint mouseStartPos;

    QPoint cropStart;
    QPoint cropSsize;

    bool isCropping = false;
};


#endif // DISPLAYSCREEN_H
