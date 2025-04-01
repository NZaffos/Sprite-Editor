// Reviewedby Nash Hawkins
/**
 *
 * @file main.cpp
 * @brief Entry point for the application.*
 * This file contains the main function, which initializes the application,
 * creates the model, and launches the main window.*
 * @author Noah Zaffos, Ethan Perkins, Caleb Standfield, Jas Sandhu, Nash Hawkins, John Chen*
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Qt application instance
    QApplication a(argc, argv);

    // setting colors
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

    // Application data model
    Model m;

    // Main window with the model passed as a parameter
    MainWindow w(&m);

    // Display the main window
    w.show();

    // Execute the application event loop
    return a.exec();
}
