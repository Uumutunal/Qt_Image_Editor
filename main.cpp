#include "mainwindow.h"

#include <QApplication>
#include <QFile>

QString loadStyleSheet(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return QString();

    QTextStream stream(&file);
    return stream.readAll();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString styleSheet = loadStyleSheet(":/styles/style.qss");
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return a.exec();
}
