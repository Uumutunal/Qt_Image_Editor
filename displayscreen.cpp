#include "displayscreen.h"


DisplayScreen::DisplayScreen() {}

void DisplayScreen::setImg(QPixmap pixmap)
{
    img = pixmap;
};

void DisplayScreen::enterEvent(QEnterEvent *ev){
    qDebug() << "enter";
    if(img.isNull()){
        return;
    }
    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);

    QTransform transform;
    //transform.translate(250, 250);
    transform.scale(.5, .5);
    //transform.rotate(45);

    QPixmap transformedPixmap(imgT.size());
    transformedPixmap.fill(Qt::transparent);

    QPainter painter(&transformedPixmap);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, imgT);
    painter.end();

    imgT = transformedPixmap;

    //this->setPixmap(imgT);
}

void DisplayScreen::leaveEvent(QEvent *ev){
    qDebug() << "exit";
    if(img.isNull()){
        return;
    }
    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);

    QTransform transform;
    //transform.translate(250, 250);
    transform.scale(2, 2);
    //transform.rotate(45);

    QPixmap transformedPixmap(imgT.size() * 2);
    transformedPixmap.fill(Qt::transparent);

    QPainter painter(&transformedPixmap);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, imgT);
    painter.end();

    imgT = transformedPixmap;

    //this->setPixmap(imgT);
}

void DisplayScreen::wheelEvent(QWheelEvent *event)
{
    qDebug() << "wheel";
    QPoint numDegrees = event->angleDelta() / 8;

    if(img.isNull()){
        return;
    }
    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);

    QTransform transform;
    //transform.translate(250, 250);
    transform.scale(7.0f / numDegrees.y(), 1);
    //transform.rotate(45);

    QPixmap transformedPixmap(imgT.size());
    transformedPixmap.fill(Qt::transparent);

    QPainter painter(&transformedPixmap);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, imgT);
    painter.end();

    imgT = transformedPixmap;

    this->setPixmap(imgT);

    event->accept();
}
