#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>

#define VOICE_1 0
#define VOICE_2 1
#define VOICE_3 2
#define VOICE_4 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::vector<QString> m_voice_colors;
    std::vector<QLabel*> m_voice_labels;
    std::vector<QDial*> m_voice_knobs;
    std::vector<QDial*> m_fine_knobs;

    std::vector<QPushButton*> m_mute_buttons;
    int m_mute_idx;
    std::vector<QPushButton*> m_adsr_buttons;
    int m_adsr_idx;
    std::vector<QPushButton*> m_voice_sel_buttons;
    int m_voice_sel_idx;
    std::vector<QPushButton*> m_voice_fine_buttons;
    int m_voice_fine_idx;
    std::vector<QPushButton*> m_lfo_buttons;
    int m_lfo_idx;
    std::vector<QPushButton*> m_filter_buttons;
    int m_filter_idx;
    std::vector<QPushButton*> m_fm_buttons;
    int m_fm_idx;
    std::vector<QPushButton*> m_vol_buttons;
    int m_vol_idx;

    std::vector<QPushButton*> m_buttons;
    std::vector<int> m_button_states; // >0 --> pressed, 0 --> not pressed

    void updateHarmonicsDisplay(QString harmonics);

public slots:
    void slot_handle_button_press();

};

#endif // MAINWINDOW_H
