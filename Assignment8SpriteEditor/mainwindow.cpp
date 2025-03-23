#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Model* model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
    , model(model)
{
    ui->setupUi(this);

    // New Displays
    displays = new Displays(model);

    // New Controller
    controller = new Controller(model);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete displays;
    delete controller;
}
