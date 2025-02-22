#include "displayscreen.h"
#include <QColor>

DisplayScreen::DisplayScreen()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground);
    cropStart = QPoint(-1,-1);
    cropSsize = QPoint(-1,-1);
}

void DisplayScreen::setImg(QPixmap pixmap)
{
    img = pixmap;
}

void DisplayScreen::resetTransform()
{
    transform.reset();
}

void DisplayScreen::toggleCropping()
{
    isCropping = !isCropping;
    this->update();
};

void DisplayScreen::enterEvent(QEnterEvent *ev){
    qDebug() << "enter";
}

void DisplayScreen::leaveEvent(QEvent *ev){
    qDebug() << "exit";
}

void DisplayScreen::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if(img.isNull()){
        return;
    }
    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);

    if (numDegrees.y() != 0) {
        double scaleFactor = (numDegrees.y() > 0) ? (16.0 / numDegrees.y()) : (14.0 / -numDegrees.y());

        transform.translate(imgT.width() / 2.0, imgT.height() / 2.0);
        transform.scale(scaleFactor, scaleFactor);
        transform.translate(-imgT.width() / 2.0, -imgT.height() / 2.0);
    }

    QPixmap transformedPixmap(imgT.size());
    transformedPixmap.fill(Qt::transparent);

    QPainter painter(&transformedPixmap);
    //painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setTransform(transform, true);

    painter.drawPixmap(0, 0, imgT);
    painter.end();

    imgT = transformedPixmap;

    this->setPixmap(imgT);

    event->accept();
}

void DisplayScreen::paintEvent(QPaintEvent *event)
{
    if(img.isNull()){
        return;
    }
    QPainter painter(this);

    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);

    painter.setTransform(transform, true);

    if (!imgT.isNull()) {
        painter.drawPixmap(0, (height() / 2) - (imgT.height() / 2), imgT);
    }
    if (isCropping) {
        if(cropStart.x() == 0 && cropStart.y() == 0){
            this->cropStart = QPoint(100,100);
        }
        if(cropSsize.x() == 0 && cropSsize.y() == 0){
            this->cropSsize = QPoint(300,300);
        }

        // Fill the entire widget with a solid color overlay
        QRect top(0,(height() / 2) - (imgT.height() / 2),imgT.width(),cropStart.y());
        QRect left(0,(height() / 2) - (imgT.height() / 2) + cropStart.y(),cropStart.x(),cropSsize.y());
        QRect right(cropStart.x() + cropSsize.x(),(height() / 2) - (imgT.height() / 2) + cropStart.y(),imgT.width() - cropSsize.x() - cropStart.x(),cropSsize.y());
        QRect bot(0,(height() / 2) - (imgT.height() / 2) + cropStart.y() + cropSsize.y(),imgT.width(),imgT.height() - cropSsize.y() - cropStart.y());

        painter.fillRect(top, QColor(0,0,255,120));
        painter.fillRect(left, QColor(0,255,0,120));
        painter.fillRect(right, QColor(255,0,0,120));
        painter.fillRect(bot, QColor(255,0,255,120));

        // Get the image position and size
        int imgX = (width() - imgT.width()) / 2;
        int imgY = (height() - imgT.height()) / 2;
        int imgW = imgT.width();
        int imgH = imgT.height();

        // Set composition mode to clear the inside area
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        //painter.fillRect(imgX, imgY, imgW - 50, imgH - 50, Qt::transparent);

        // Reset the composition mode to normal for future drawing
        //painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        // Draw the image
        //painter.drawPixmap(imgX, imgY, imgT);
    }
}

void DisplayScreen::mousePressEvent(QMouseEvent *eventPress)
{
    qDebug() << "pressed " << eventPress->pos();
    mousePos = eventPress->pos();
    mouseStartPos = eventPress->pos();

    if(isCropping){
        //cropStart = eventPress->pos();

        QSize size =  img.size();
        float aspectRatioW = size.width() / (this->width());
        float aspectRatioH = size.height() / (this->height());

        QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);
        double scaleY = transform.m22();
        float dy = (height() / 2) - (imgT.height() / 2);
        dy *= scaleY;

        //QPoint start = transform.map(cropStart);

        //qDebug() << "crop start: " << start;
        //qDebug() << "translation x: " << cropStart.x() - transform.dx();
        //qDebug() << "translation y: " << cropStart.y() - (transform.dy() + dy);
    }
}

void DisplayScreen::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = mousePos - event->pos();
    mousePos = event->pos();

    if(img.isNull()){
        return;
    }

    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);

    if(isCropping){
        cropSsize = transform.map(mousePos) - transform.map(mouseStartPos);
        //cropSsize = mousePos - mouseStartPos;
        double scaleY = transform.m22();
        float dy = (height() / 2) - (imgT.height() / 2);
        dy *= scaleY;

        QPoint start = transform.map(mouseStartPos);

        qDebug() << "crop : " << start;
        qDebug() << "translation x: " << mouseStartPos.x() - transform.dx();
        qDebug() << "translation y: " << mouseStartPos.y() - (transform.dy() + dy);

        cropStart = QPoint(mouseStartPos.x() - transform.dx(), mouseStartPos.y() - (transform.dy() + dy));
        //cropStart = mouseStartPos;
        QPoint newPoint = transform.map(cropSsize);
        qDebug() << "crop size : " << cropSsize;
        this->update();
        return;
    }

    QPixmap transformedPixmap(imgT.size());
    transformedPixmap.fill(Qt::transparent);

    QPainter painter(&transformedPixmap);

    double scaleX = transform.m11();
    double scaleY = transform.m22();

    transform.translate(-offset.x() / scaleX, -offset.y() / scaleY);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setTransform(transform, true);

    painter.drawPixmap(0, 0, imgT);
    painter.end();

    imgT = transformedPixmap;

    this->setPixmap(imgT);

    qDebug() << offset;
}

void DisplayScreen::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
    qDebug() << "released " << releaseEvent->pos();
    mouseStartPos = QPoint(-1,-1);
}

