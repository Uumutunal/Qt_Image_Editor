#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H
#include <QLabel>
#include <QDebug>

class DisplayScreen : public QLabel
{
    Q_OBJECT
public:
    DisplayScreen();
    DisplayScreen(QWidget* parent = nullptr) : QLabel(parent){ }

protected:
    void enterEvent(QEnterEvent *ev) override;

    void leaveEvent(QEvent *ev) override;
};


#endif // DISPLAYSCREEN_H
