/**
 * @file main.cpp
 * @brief Entry point for the application.
 *
 * This file contains the main function, which initializes the application,
 * creates the model, and launches the main window.
 *
 * @author Noah Zaffos
 * @author Ethan Perkins
 * @author Caleb Standfield
 * @author Jas Sandhu
 * @author Nash Hawkins
 * @author John Chen
 *
 * @reviewedby Nash Hawkins
 */
#include "mainwindow.h"
#include <QApplication>

/**
 * @brief The main function that starts the application.
 *
 * It initializes the QApplication, creates the Model,
 * and opens the MainWindow to begin the user interface.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument values.
 * @return Exit status of the application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //Qt application instance
    Model m; // Application data model
    MainWindow w(&m);// Main window with the model passed as a parameter
    w.show(); // Display the main window
    return a.exec(); // Execute the application event loop
}
