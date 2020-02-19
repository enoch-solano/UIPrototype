#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_voice_colors { "red", "blue", "green", "grey" }
{
    ui->setupUi(this);
    m_voice_labels = { ui->display_V1,
                       ui->display_V2,
                       ui->display_V3,
                       ui->display_V4 };
    m_voice_knobs = { ui->levelKnob_V1,
                      ui->levelKnob_V2,
                      ui->levelKnob_V3,
                      ui->levelKnob_V4 };
    m_fine_knobs = { ui->fineCoarseKnob_V1,
                     ui->fineCoarseKnob_V2,
                     ui->fineCoarseKnob_V3,
                     ui->fineCoarseKnob_V4 };

    m_mute_buttons = { ui->mute_V1, ui->mute_V2, ui->mute_V3, ui->mute_V4 };
    m_adsr_buttons = { ui->attackButton, ui->decayButton, ui->sustainButton, ui->releaseButton };
    m_voice_sel_buttons = { ui->levelButton_V1, ui->levelButton_V2, ui->levelButton_V3, ui->levelButton_V4 };
    m_voice_fine_buttons = { ui->fineCoarseButton_V1,
                             ui->fineCoarseButton_V2,
                             ui->fineCoarseButton_V3,
                             ui->fineCoarseButton_V4 };
    m_lfo_buttons = { ui->LFOTypeButton, ui->LFORateButton, ui->LFOLevelButton };
    m_filter_buttons = { ui->filterTypeButton, ui->filterFreqButton, ui->filterPeakButton };
    m_fm_buttons = { ui->FMTypeButton };
    m_vol_buttons = { ui->volumeButton };

    m_mute_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_mute_buttons), std::end(m_mute_buttons));

    m_adsr_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_adsr_buttons), std::end(m_adsr_buttons));

    m_voice_sel_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_voice_sel_buttons), std::end(m_voice_sel_buttons));

    m_voice_fine_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_voice_fine_buttons), std::end(m_voice_fine_buttons));

    m_lfo_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_lfo_buttons), std::end(m_lfo_buttons));

    m_filter_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_filter_buttons), std::end(m_filter_buttons));

    m_fm_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_fm_buttons), std::end(m_fm_buttons));

    m_vol_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_vol_buttons), std::end(m_vol_buttons));

    m_button_states = std::vector<int>(m_buttons.size());

    for (int i = 0; i < m_voice_labels.size(); i++) {
        m_voice_labels[i]->setStyleSheet("QLabel { background-color : " + m_voice_colors[i] +
                                         "; color : " + m_voice_colors[i] + "; }");
    }

    int num_knobs = 4;
    int min_level = 0;
    int max_level = 99;

    for (int i = 0; i < num_knobs; i++) {
        m_voice_knobs[i]->setMinimum(min_level);
        m_voice_knobs[i]->setMaximum(max_level);
    }

    int min_fine = 0;
    int max_fine = 99;

    for (int i = 0; i < num_knobs; i++) {
        m_fine_knobs[i]->setMinimum(min_fine);
        m_fine_knobs[i]->setMaximum(max_fine);
    }

    ui->infoLabel->setStyleSheet("QLabel { color : " + m_voice_colors[VOICE_1] + "; }");

    for (QPushButton* button : m_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_handle_button_press()));
    }

    updateHarmonicsDisplay("this data is cool :)");
}


void MainWindow::slot_handle_button_press() {
    // retrieve the button you have clicked
    QPushButton* button_sender = qobject_cast<QPushButton*>(sender());

    for (int i = 0; i < m_buttons.size(); i++) {
        if (m_buttons[i] == button_sender) {
            m_button_states[i] = 50;
        }
    }
}

void MainWindow::updateHarmonicsDisplay(QString harmonics) {
    // 45 characters per line
    ui->infoLabel->setText(harmonics);
}

MainWindow::~MainWindow()
{
    delete ui;
}
