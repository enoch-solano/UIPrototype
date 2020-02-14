#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<QLabel*> labels = { ui->display_V1, ui->display_V2, ui->display_V3, ui->display_V4 };
    std::vector<QString> colors = { "red", "blue", "green", "grey" };

    for (int i = 0; i < labels.size(); i++) {
        labels[i]->setStyleSheet("QLabel { background-color : " + colors[i] +
                                 "; color : " + colors[i] + "; }");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
