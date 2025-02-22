#include "displayscreen.h"
#include <QColor>

DisplayScreen::DisplayScreen()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground);
    cropStart = QPoint(-1,-1);
    cropSize = QPoint(-1,-1);
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

    QSize size = img.size();

    //float aspectRatioW = size.width() / (this->width());
    //float aspectRatioH = size.height() / (this->height());

    float aspectRatioW = size.width() / float(this->width());
    float aspectRatioH = size.height() / float(this->height());

    QPixmap imgT = img.scaled(size.width() / aspectRatioW, size.height() / aspectRatioH, Qt::KeepAspectRatio);


    painter.setTransform(transform, true);

    if (!imgT.isNull()) {
        painter.drawPixmap(0, (height() / 2) - (imgT.height() / 2), imgT);
    }
    if (isCropping) {
        if(cropStart.x() == 0 && cropStart.y() == 0){
            this->cropStart = QPoint(100,100);
        }
        if(cropSize.x() == 0 && cropSize.y() == 0){
            this->cropSize = QPoint(300,300);
        }

        // Fill the entire widget with a solid color overlay
        QRect top(0,(height() / 2.0f) - (imgT.height() / 2.0f),imgT.width(),cropStart.y());
        QRect left(0,(height() / 2.0f) - (imgT.height() / 2.0f) + cropStart.y(),cropStart.x(),cropSize.y());
        QRect right(cropStart.x() + cropSize.x(),(height() / 2.0f) - (imgT.height() / 2.0f) + cropStart.y(),imgT.width() - cropSize.x() - cropStart.x(),cropSize.y());
        QRect bot(0,(height() / 2.0f) - (imgT.height() / 2.0f) + cropStart.y() + cropSize.y(),imgT.width(),imgT.height() - cropSize.y() - cropStart.y());



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
        //cropSsize = transform.map(mousePos) - transform.map(mouseStartPos);
        cropSize = mousePos - mouseStartPos;
        double scaleX = transform.m11();
        double scaleY = transform.m22();
        float dy = (height() / 2.0f) - (imgT.height() / 2.0f);
        dy *= scaleY;

        //QPoint start = transform.map(mouseStartPos);

        cropStart = QPoint(mouseStartPos.x() - transform.dx(), mouseStartPos.y() - (transform.dy() + dy));
        cropStart = QPoint(cropStart.x() / scaleX, cropStart.y() / scaleY);

        cropSize = QPoint(cropSize.x() / scaleX, cropSize.y() / scaleY);

        if(cropSize.x() < 0 || cropSize.y() < 0){
            //cropStart = QPoint(cropStart.y(),cropStart.x());
        }

        qDebug() << "mouseStartPos " << cropStart;
        if(mouseStartPos.y() > mousePos.y()){
            //cropStart = QPoint(cropStart.x(), cropStart.y() + cropSize.y());
            qDebug() << "mouseStartPos " << mouseStartPos;
            qDebug() << "mousePos " << mousePos;
        }

        //cropSize = QPoint(qAbs(cropSize.x()), qAbs(cropSize.y()));
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

