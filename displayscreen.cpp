#include "displayscreen.h"
#include <QColor>
#include <QApplication>

DisplayScreen::DisplayScreen()
{
    setMouseTracking(true);
    setAttribute(Qt::WA_TranslucentBackground);
    cropStart = QPoint(-1,-1);
    cropSize = QPoint(-1,-1);

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void DisplayScreen::setImg(QPixmap pixmap)
{
    img = pixmap;
    isCropping = false;
    cropStart = QPoint(-1,-1);
    cropSize = QPoint(-1,-1);
    QPixmap imgT = img.scaled(width(), height(), Qt::KeepAspectRatio);
    transform.translate(0,(height() / 2.0f) - (imgT.height() / 2.0f));
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

void DisplayScreen::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if(img.isNull()){
        return;
    }
    QSize size =  img.size();
    float aspectRatioW = size.width() / (this->width());
    float aspectRatioH = size.height() / (this->height());

    QPointF pos = event->position();

    QPixmap imgT = img.scaled((size.width() / aspectRatioW) * transform.m11(), (size.height() / aspectRatioH) * transform.m22(), Qt::KeepAspectRatio);

    if (numDegrees.y() != 0) {
        double scaleFactor = (numDegrees.y() > 0) ? (16.0 / numDegrees.y()) : (14.0 / -numDegrees.y());

        float offsetx = (width() / 2) - pos.x();
        float offsety = (height() / 2) - pos.y();
        QPointF offset2 = QPointF((width() / 2) - pos.x() , (height() / 2) - pos.y());

        //
        double scaleX = transform.m11();
        double scaleY = transform.m22();
        float dy = (height() / 2.0f) - (imgT.height() / 2.0f);
        dy *= scaleY;

        QPoint a = QPoint(pos.x() - transform.dx(), pos.y() - (transform.dy() + dy));
        a = QPoint(a.x() / scaleX, a.y() / scaleY);

        QPoint b(imgT.width() / 2, imgT.height() / 2);
        QPoint c = a - b;
        //

        QPointF scenePosBeforeZoom = transform.inverted().map(offset2);
        //QPointF scenePosBeforeZoom = QPointF(offset2.x() / transform.m11(), offset2.y() / transform.m22());
        // Apply scaling
        //transform.translate((imgT.width() / 2.0) - c.x(), (imgT.height() / 2.0) - c.y());
        transform.translate(imgT.width() / 2.0, imgT.height() / 2.0);
        transform.scale(scaleFactor, scaleFactor);
        c = QPoint(c.x() * transform.m11(), c.y() * transform.m22());
        transform.translate(-imgT.width() / 2.0, -imgT.height() / 2.0);
        //transform.translate((-imgT.width() / 2.0) + c.x(), (-imgT.height() / 2.0) + c.y());

        // Convert the position back and adjust translation
        QPointF scenePosAfterZoom = transform.inverted().map(offset2);
        //QPointF scenePosAfterZoom = QPointF(offset2.x() / transform.m11(), offset2.y() / transform.m22());

        QPointF offset = scenePosAfterZoom - scenePosBeforeZoom;

        qDebug() << a - b;

        if(numDegrees.y() > 0){
            //transform.translate(-offsetx / 10, -offsety / 10);
            //transform.translate(-offset.x(), -offset.y());
            //transform.translate(-c.x() / 10, -c.y() / 10);
        }
        else{
            //transform.translate(offsetx / 10, offsety / 10);
            //transform.translate(offset.x(), offset.y());
            //transform.translate(c.x() / 10, c.y() / 10);
        }

    }
    this->update();
}

void DisplayScreen::paintEvent(QPaintEvent *event)
{
    if(img.isNull()){
        return;
    }
    QPainter painter(this);

    QPixmap imgT = img.scaled(width() * transform.m11(), height() * transform.m22(), Qt::KeepAspectRatio);

    painter.setTransform(transform, true);

    if (!imgT.isNull()) {
        painter.drawPixmap(0, 0, imgT);
    }
    if (isCropping) {

        if(cropStart.x() == -1 && cropStart.y() == -1){
            this->cropStart = QPoint(100,100);
        }
        if(cropSize.x() == -1 && cropSize.y() == -1){
            this->cropSize = QPoint(300,300);
        }

        QPointF startRatio = QPointF((static_cast<double>(cropStartRatio.x())) * imgT.width() ,
                                     (static_cast<double>(cropStartRatio.y())) * imgT.height());

        QPointF sizeRatio =  QPointF((static_cast<double>(cropSizeRatio.x())) * imgT.width() ,
                                    (static_cast<double>(cropSizeRatio.y())) * imgT.height() );

        QRect top(0,0,imgT.width(),startRatio.y());
        QRect left(0,startRatio.y(),startRatio.x(),sizeRatio.y());
        QRect right(startRatio.x() + sizeRatio.x(),startRatio.y(),imgT.width() - sizeRatio.x() - startRatio.x(),sizeRatio.y());
        QRect bot(0,startRatio.y() + sizeRatio.y(),imgT.width(),imgT.height() - sizeRatio.y() - startRatio.y());

        QColor fade(50,50,50,150);
        painter.fillRect(top, fade);
        painter.fillRect(left, fade);
        painter.fillRect(right, fade);
        painter.fillRect(bot, fade);
    }
}

bool DisplayScreen::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::KeyPress) {
        qDebug() << "KeyPress";
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) ) {
            //Enter or return was pressed
            qDebug() << "enter";
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

void DisplayScreen::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key Pressed!";
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        qDebug() << "Enter key detected!";
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

    //QPixmap imgT2 = img.scaled(width() * transform.m11(), height() * transform.m22(), Qt::KeepAspectRatio);

    QPixmap imgT = img.scaled(width(), height(), Qt::KeepAspectRatio);

    if(isCropping){
        cropSize = mousePos - mouseStartPos;
        double scaleX = transform.m11();
        double scaleY = transform.m22();

        cropStart = mouseStartPos;
        cropStart = QPointF(cropStart.x() - transform.dx(), cropStart.y() - transform.dy());

        QPointF cropEnd(mousePos);
        cropEnd = QPointF(cropEnd.x() - transform.dx(), cropEnd.y() - transform.dy());


        if(cropStart.y() < 0){
            cropStart = QPointF(cropStart.x(), 0);
        }
        if(cropEnd.y() < 0){
            cropEnd = QPointF(cropEnd.x(), 0);
        }

        if(cropStart.x() < 0){
            cropStart = QPointF(0, cropStart.y());
        }
        if(cropEnd.x() < 0){
            cropEnd = QPointF(0, cropEnd.y());
        }

        if(cropEnd.x() > imgT.width() * scaleX * scaleX){
            cropEnd = QPointF(imgT.width() * scaleX * scaleX,cropEnd.y());
        }

        if(cropEnd.y() > imgT.height() * scaleX * scaleX){
            cropEnd = QPointF(cropEnd.x(),imgT.height() * scaleX * scaleX);
        }

        if(cropStart.y() > imgT.height() * scaleX * scaleX){
            cropStart = QPointF(cropStart.x(), imgT.height() * scaleX * scaleX);
        }
        if(cropStart.x() > imgT.width() * scaleX * scaleX){
            cropStart = QPointF(imgT.width() * scaleX * scaleX, cropStart.y());
        }

        cropSize = cropEnd - cropStart;
        cropSize = QPointF(cropSize.x() / scaleX, cropSize.y() / scaleY);

        if(mouseStartPos.y() > mousePos.y()){
            cropStart = QPointF(cropStart.x(), cropStart.y() + cropSize.y() * scaleX);
        }
        if(mouseStartPos.x() > mousePos.x()){
            cropStart = QPointF(cropStart.x() + cropSize.x() * scaleX, cropStart.y());
        }

        cropSize = QPoint(qAbs(cropSize.x()), qAbs(cropSize.y()));

        cropStartRatio = QPointF((static_cast<double>(cropStart.x()) / (imgT.width() * scaleX * scaleX)),
                                 (static_cast<double>(cropStart.y()) / (imgT.height() * scaleX * scaleX)));

        cropSizeRatio = QPointF((static_cast<double>(cropSize.x()) / (imgT.width() * scaleX )),
                                 (static_cast<double>(cropSize.y()) / (imgT.height() * scaleX )));

        this->update();
        return;
    }


    double scaleX = transform.m11();
    double scaleY = transform.m22();

    transform.translate(-offset.x() / scaleX, -offset.y() / scaleY);
    this->update();
}

void DisplayScreen::mouseReleaseEvent(QMouseEvent *releaseEvent)
{
    qDebug() << "released " << releaseEvent->pos();
    mouseStartPos = QPoint(-1,-1);
}

