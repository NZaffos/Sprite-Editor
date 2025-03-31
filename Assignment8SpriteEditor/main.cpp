#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(60, 60, 60));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::AlternateBase, QColor(55, 55, 55));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(70, 70, 70));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);

    a.setPalette(darkPalette);

    Model m;
    MainWindow w(&m);
    w.show();
    return a.exec();
}
