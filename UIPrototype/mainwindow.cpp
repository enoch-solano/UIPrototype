#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::vector<QLabel*> labels = { ui->display_V1,
                                    ui->display_V2,
                                    ui->display_V3,
                                    ui->display_V4 };
    std::vector<QString> colors = { "red", "blue", "green", "grey" };

    for (int i = 0; i < labels.size(); i++) {
        labels[i]->setStyleSheet("QLabel { background-color : " + colors[i] +
                                 "; color : " + colors[i] + "; }");
    }

    int num_knobs = 4;
    int min_level = 0;
    int max_level = 99;
    std::vector<QDial*> voice_knobs = { ui->levelKnob_V1,
                                        ui->levelKnob_V2,
                                        ui->levelKnob_V3,
                                        ui->levelKnob_V4 };

    for (int i = 0; i < num_knobs; i++) {
        voice_knobs[i]->setMinimum(min_level);
        voice_knobs[i]->setMaximum(max_level);
    }

    int min_fine = 0;
    int max_fine = 99;
    std::vector<QDial*> fine_knobs = { ui->fineCoarseKnob_V1,
                                       ui->fineCoarseKnob_V2,
                                       ui->fineCoarseKnob_V3,
                                       ui->fineCoarseKnob_V4 };

    for (int i = 0; i < num_knobs; i++) {
        fine_knobs[i]->setMinimum(min_fine);
        fine_knobs[i]->setMaximum(max_fine);
    }

    ui->infoLabel->setText("data to go here!");
}

MainWindow::~MainWindow()
{
    delete ui;
}
