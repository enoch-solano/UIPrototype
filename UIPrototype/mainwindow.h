#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "single_include/nlohmann/json.hpp"
#include <QMainWindow>
#include <iostream>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>

//-- client-side includes --//
#include <QDataStream>
#include <QLocalSocket>
//--------------------------//

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

    std::vector<QString> m_presets;
    int m_preset_selected;

    std::vector<QString> m_voice_colors;
    std::vector<int> m_voice_colors_vals;
    std::vector<QLabel*> m_voice_display_LEDs;

    std::vector<QPushButton*> m_keys;
    std::vector<int> m_key_pressed;
    std::vector<int> m_key_released;

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

//---- client side socket stuff ----//
    QLocalSocket *socket;

    void connectToServer(QString server);
    void disconnectFromServer();
    void writeToSocket(QString message);

public slots:
    void readFromSocket();

//----------------------------------//

public slots:
    void slot_handle_button_press();
    void slot_handle_knob_turn(int value);
    void slot_handle_timer();

    void slot_v0_pressed() { section_clicked = 0; }
    void slot_v1_pressed() { section_clicked = 1; }
    void slot_v2_pressed() { section_clicked = 2; }
    void slot_v3_pressed() { section_clicked = 3; }

    void slot_adsr_pressed()   { section_clicked = 4; }
    void slot_lfo_pressed()    { section_clicked = 5; }
    void slot_filter_pressed() { section_clicked = 6; }
    void slot_fm_pressed()     { section_clicked = 7; }
    void slot_vol_pressed()    { section_clicked = 8; }

    void slot_load_preset_handler(QAction *action);

    void slot_handle_key_press();
    void slot_handle_key_release();

    void slot_voice_select();

private:
    int key_was_pressed;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
