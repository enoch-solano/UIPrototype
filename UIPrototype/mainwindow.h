#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "single_include/nlohmann/json.hpp"
#include <QMainWindow>
#include <iostream>
#include <QTimer>

#define VOICE_1 0
#define VOICE_2 1
#define VOICE_3 2
#define VOICE_4 3

using JSON = nlohmann::json;

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

    QTimer *timer;

    std::vector<QString> m_voice_colors;
    std::vector<QLabel*> m_voice_display_LEDs;


    std::vector<QDial*> m_voice_level_knobs;
    std::vector<QDial*> m_voice_fine_knobs;
    std::vector<QDial*> m_adsr_knobs;
    std::vector<QDial*> m_lfo_knobs;
    std::vector<QDial*> m_filter_knobs;
    std::vector<QDial*> m_fm_knobs;
    std::vector<QDial*> m_vol_knobs;

    // index of buttons in m_buttons
    int m_knob_voice_level_idx;
    int m_knob_voice_fine_idx;
    int m_knob_adsr_idx;
    int m_knob_lfo_idx;
    int m_knob_filter_idx;
    int m_knob_fm_idx;
    int m_knob_vol_idx;

    std::vector<QDial*> m_knobs;
    std::vector<int> m_knob_states; // value of knobs

    std::vector<QPushButton*> m_mute_buttons;
    std::vector<QPushButton*> m_adsr_buttons;
    std::vector<QPushButton*> m_voice_sel_buttons;
    std::vector<QPushButton*> m_voice_fine_buttons;
    std::vector<QPushButton*> m_lfo_buttons;
    std::vector<QPushButton*> m_filter_buttons;
    std::vector<QPushButton*> m_fm_buttons;
    std::vector<QPushButton*> m_vol_buttons;

    JSON m_board_info;
    void buildEmptyJSONPacket();

    // index of buttons in m_buttons
    int m_button_mute_idx;
    int m_button_adsr_idx;
    int m_button_voice_sel_idx;
    int m_button_voice_fine_idx;
    int m_button_lfo_idx;
    int m_button_filter_idx;
    int m_button_fm_idx;
    int m_button_vol_idx;

    std::vector<QPushButton*> m_buttons;
    std::vector<int> m_button_states; // >0 --> pressed, 0 --> not pressed

    void updateDisplay(QString harmonics);

    JSON *getStateOfBoard();

    int section_clicked;


public slots:
    void slot_handle_button_press();
    void slot_handle_knob_turn(int value);
    void slot_handle_timer();

    void slot_v0_pressed();
    void slot_v1_pressed();
    void slot_v2_pressed();
    void slot_v3_pressed();

    void slot_adsr_pressed();
    void slot_filter_pressed();
    void slot_lfo_pressed();
    void slot_fm_pressed();
    void slot_vol_pressed();
};

#endif // MAINWINDOW_H
