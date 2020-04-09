#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    // set stylesheet
    QFile file(":/dark.qss");
    QTextStream stream(&file);
    file.open(QFile::ReadOnly | QFile::Text);
    app.setStyleSheet(stream.readAll());

    MainWindow w;
    w.show();

    return app.exec();
}
