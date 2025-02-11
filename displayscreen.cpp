#include "displayscreen.h"

DisplayScreen::DisplayScreen() {};

void DisplayScreen::enterEvent(QEnterEvent *ev){
    qDebug() << "enter";
}

void DisplayScreen::leaveEvent(QEvent *ev){
    qDebug() << "exit";
}
