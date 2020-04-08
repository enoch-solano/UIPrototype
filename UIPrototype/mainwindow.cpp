#include "mainwindow.h"

void MainWindow::connectToServer(QString server) 
{
    socket->abort();
    socket->connectToServer(server);
    socket->waitForConnected(60 * 1000);
    socket->open(QIODevice::ReadWrite);
}

void MainWindow::disconnectFromServer() 
{
    socket->disconnectFromServer();
}

void MainWindow::readFromSocket()
{
    QByteArray data = socket->readAll();
    QString strData = QString::fromStdString(data.toStdString());

    if (strData.startsWith("initial_msg_")) {
        std::string msg = strData.toStdString();
        msg = msg.substr(std::string("initial_msg_").length());

        JSON init_packet = JSON::parse(msg);

        for (std::string preset : init_packet["presets"]) {
            QString qstr_preset = QString::fromStdString(preset);
            ui->load_preset_menu->addAction(qstr_preset);
            m_presets.push_back(qstr_preset);
        }

        return;
    }

    updateDisplay("message was: " + strData);
}

void MainWindow::writeToSocket(QString message) 
{
    // formats message properly to send message length
    message = QString::number(message.length()) + "_" + message;

    QByteArray messageData;
    QDataStream dataWriter(&messageData, QIODevice::WriteOnly);
    dataWriter << message.toStdString().c_str();
    
    socket->write(messageData);
    socket->waitForBytesWritten();
}

QElapsedTimer keyTimer;
bool first_time_pressed = true;

void MainWindow::keyPressEvent(QKeyEvent *e) 
{
    // keys found here: https://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_A) {
        key_was_pressed = 1;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) 
{
    // keys found here: https://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_A) {
        if (first_time_pressed) {
            keyTimer.start();        
        }
        key_was_pressed = 0;
    }
}

void MainWindow::slot_load_preset_handler(QAction *action) {
    for (int i = 0; i < m_presets.size(); i++) {
        if (QString::compare(action->text(), m_presets[i]) == 0) {
            m_preset_selected = i;
            return;
        }
    }
}


void MainWindow::slot_handle_key_press() {
    // retrieve the button you have clicked
    QPushButton* button_sender = qobject_cast<QPushButton*>(sender());

    // update the key state
    for (int i = 0; i < m_keys.size(); i++) {
        if (m_keys[i] == button_sender) {
            m_key_pressed[i] = 1;
            updateDisplay("pressing key");
        }
    }
}

void MainWindow::slot_handle_key_release() {
    // retrieve the button you have clicked
    QPushButton* button_sender = qobject_cast<QPushButton*>(sender());

    // update the key state
    for (int i = 0; i < m_keys.size(); i++) {
        if (m_keys[i] == button_sender) {
            m_key_released[i] = 0;
            updateDisplay("released key");
        }
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), timer(new QTimer(this)),
    m_voice_colors { "rgb(255,0,0)", "rgb(0,0,255)", "rgb(0,255,0)", "rgb(255,255,0)" },
    socket(new QLocalSocket(this)), m_preset_selected(-1)
{
    ui->setupUi(this);

    m_keys.push_back(ui->key_1);
    m_keys.push_back(ui->key_2);
    m_keys.push_back(ui->key_3);

    for (int i = 0; i < m_keys.size(); i++) {
        m_key_pressed.push_back(0);
        m_key_released.push_back(0);
    }

    for (QPushButton *key : m_keys) {
        connect(key, &QPushButton::pressed, this, &MainWindow::slot_handle_key_press);
        connect(key, &QPushButton::released, this, &MainWindow::slot_handle_key_release);
    }

    //----- socket setup -----//
    connectToServer("/tmp/socket_for_synth_eng");
    connect(socket, &QLocalSocket::readyRead, this, &MainWindow::readFromSocket);
    //------------------------//

    key_was_pressed = false;

    connect(ui->load_preset_menu, &QMenu::triggered, this, &MainWindow::slot_load_preset_handler);

    m_voice_display_LEDs = { ui->display_V1,
                             ui->display_V2,
                             ui->display_V3,
                             ui->display_V4 };

    //----- PLACES KNOBS IN CONTAINERS for EASE of MANIPULATION -----//
    m_voice_level_knobs = { ui->levelKnob_V1,
                            ui->levelKnob_V2,
                            ui->levelKnob_V3,
                            ui->levelKnob_V4 };
    m_voice_fine_knobs = { ui->fineCoarseKnob_V1,
                           ui->fineCoarseKnob_V2,
                           ui->fineCoarseKnob_V3,
                           ui->fineCoarseKnob_V4 };
    m_adsr_knobs = { ui->attackKnob, ui->decayKnob, ui->sustainKnob, ui->releaseKnob };
    m_lfo_knobs = { ui->LFOTypeKnob, ui->LFORateKnob, ui->LFOLevelKnob };
    m_filter_knobs = { ui->filterTypeKnob, ui->filterFreqKnob, ui->filerPeakKnob };
    m_fm_knobs = { ui->FMTypeKnob };
    m_vol_knobs = { ui->volumeKnob };

    m_knob_voice_level_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_voice_level_knobs), std::end(m_voice_level_knobs));

    m_knob_voice_fine_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_voice_fine_knobs), std::end(m_voice_fine_knobs));

    m_knob_adsr_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_adsr_knobs), std::end(m_adsr_knobs));

    m_knob_lfo_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_lfo_knobs), std::end(m_lfo_knobs));

    m_knob_filter_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_filter_knobs), std::end(m_filter_knobs));

    m_knob_fm_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_fm_knobs), std::end(m_fm_knobs));

    m_knob_vol_idx = m_knobs.size();
    m_knobs.insert(std::end(m_knobs), std::begin(m_vol_knobs), std::end(m_vol_knobs));

    m_knob_states = std::vector<int>(m_knobs.size());
    //---------------------------------------------------------------//

    //---- PLACES BUTTONS IN CONTAINERS for EASE of MANIPULATION ----//
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

    m_button_mute_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_mute_buttons), std::end(m_mute_buttons));

    m_button_adsr_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_adsr_buttons), std::end(m_adsr_buttons));

    m_button_voice_sel_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_voice_sel_buttons), std::end(m_voice_sel_buttons));

    m_button_voice_fine_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_voice_fine_buttons), std::end(m_voice_fine_buttons));

    m_button_lfo_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_lfo_buttons), std::end(m_lfo_buttons));

    m_button_filter_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_filter_buttons), std::end(m_filter_buttons));

    m_button_fm_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_fm_buttons), std::end(m_fm_buttons));

    m_button_vol_idx = m_buttons.size();
    m_buttons.insert(std::end(m_buttons), std::begin(m_vol_buttons), std::end(m_vol_buttons));

    m_button_states = std::vector<int>(m_buttons.size());
    //---------------------------------------------------------------//

    for (int i = 0; i < m_voice_display_LEDs.size(); i++) {
        m_voice_display_LEDs[i]->setStyleSheet("QLabel { background-color : " + m_voice_colors[i] +
                                               "; color : " + m_voice_colors[i] + "; }");
    }

    int num_knobs = 4;
    int min_level = 0;
    int max_level = 99;

    for (int i = 0; i < num_knobs; i++) {
        m_voice_level_knobs[i]->setMinimum(min_level);
        m_voice_level_knobs[i]->setMaximum(max_level);
    }

    int min_fine = 0;
    int max_fine = 99;

    for (int i = 0; i < num_knobs; i++) {
        m_voice_fine_knobs[i]->setMinimum(min_fine);
        m_voice_fine_knobs[i]->setMaximum(max_fine);
    }

    ui->infoLabel->setStyleSheet("QLabel { color : " + m_voice_colors[VOICE_1] + "; }");

    for (QPushButton* button : m_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_handle_button_press()));
    }

    for (QDial* knob : m_knobs) {
        knob->setWrapping(true);
        int size = 120;
        knob->setMinimumHeight(size);
        knob->setMinimumWidth(size);
    }


    for (int i = 0; i < 4; i++) {
        QPushButton* mute_button = m_mute_buttons[i];
        QPushButton* voice_sel_button = m_voice_sel_buttons[i];
        QPushButton* fine_button = m_voice_fine_buttons[i];

        QDial* level_knob = m_voice_level_knobs[i];
        QDial* fine_knob = m_voice_fine_knobs[i];

        if (i == 0) {
            connect(mute_button, SIGNAL(pressed()), this, SLOT(slot_v0_pressed()));
            connect(voice_sel_button, SIGNAL(pressed()), this, SLOT(slot_v0_pressed()));
            connect(fine_button, SIGNAL(pressed()), this, SLOT(slot_v0_pressed()));

            connect(level_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v0_pressed()));
            connect(fine_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v0_pressed()));
        } else if (i == 1) {
            connect(mute_button, SIGNAL(pressed()), this, SLOT(slot_v1_pressed()));
            connect(voice_sel_button, SIGNAL(pressed()), this, SLOT(slot_v1_pressed()));
            connect(fine_button, SIGNAL(pressed()), this, SLOT(slot_v1_pressed()));

            connect(level_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v1_pressed()));
            connect(fine_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v1_pressed()));
        } else if (i == 2) {
            connect(mute_button, SIGNAL(pressed()), this, SLOT(slot_v2_pressed()));
            connect(voice_sel_button, SIGNAL(pressed()), this, SLOT(slot_v2_pressed()));
            connect(fine_button, SIGNAL(pressed()), this, SLOT(slot_v2_pressed()));

            connect(level_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v2_pressed()));
            connect(fine_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v2_pressed()));
        } else if (i == 3) {
            connect(mute_button, SIGNAL(pressed()), this, SLOT(slot_v3_pressed()));
            connect(voice_sel_button, SIGNAL(pressed()), this, SLOT(slot_v3_pressed()));
            connect(fine_button, SIGNAL(pressed()), this, SLOT(slot_v3_pressed()));

            connect(level_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v3_pressed()));
            connect(fine_knob, SIGNAL(valueChanged(int)), this, SLOT(slot_v3_pressed()));
        }
    }


    for (QPushButton* button : m_adsr_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_adsr_pressed()));
    }
    for (QDial* dial : m_adsr_knobs) {
        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(slot_adsr_pressed()));
    }

    for (QPushButton* button : m_lfo_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_lfo_pressed()));
    }
    for (QDial* dial : m_lfo_knobs) {
        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(slot_lfo_pressed()));
    }

    for (QPushButton* button : m_filter_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_filter_pressed()));
    }
    for (QDial* dial : m_filter_knobs) {
        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(slot_filter_pressed()));
    }

    for (QPushButton* button : m_fm_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_fm_pressed()));
    }
    for (QDial* dial : m_fm_knobs) {
        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(slot_fm_pressed()));
    }

    for (QPushButton* button : m_vol_buttons) {
        connect(button, SIGNAL(pressed()), this, SLOT(slot_vol_pressed()));
    }
    for (QDial* dial : m_vol_knobs) {
        connect(dial, SIGNAL(valueChanged(int)), this, SLOT(slot_vol_pressed()));
    }

    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::slot_handle_timer));
    timer->start(50);
}

void MainWindow::slot_handle_timer() {
    JSON *packet = getStateOfBoard();
    writeToSocket(QString::fromStdString(packet->dump()));
    delete packet;

    m_preset_selected = -1;
}

void MainWindow::slot_handle_button_press() {
    // retrieve the button you have clicked
    QPushButton* button_sender = qobject_cast<QPushButton*>(sender());

    // update the button state
    for (int i = 0; i < m_buttons.size(); i++) {
        if (m_buttons[i] == button_sender) {
            m_button_states[i] = 1;
        }
    }
}

void MainWindow::slot_handle_knob_turn(int value) {
    // updateDisplay(QString::number(value));
}

void MainWindow::updateDisplay(QString harmonics) {
    ui->infoLabel->setText(harmonics);
}

JSON* MainWindow::getStateOfBoard() {
    JSON *packet = new JSON();

    for (int i = 0; i < 4; i++) {
        int mute_val = m_button_states[m_button_mute_idx+i] > 0;
        m_button_states[m_button_mute_idx+i] -= mute_val > 0 ? 1 : 0;

        int level_val = m_button_states[m_button_voice_sel_idx+i] > 0;
        m_button_states[m_button_voice_sel_idx+i] -= level_val > 0 ? 1 : 0;

        int fine_val = m_button_states[m_button_voice_fine_idx+i] > 0;
        m_button_states[m_button_voice_fine_idx+i] -= fine_val > 0 ? 1 : 0;

        (*packet)["voices"]["voice" + std::to_string(i)]["button"] = { { "mute",  mute_val },
                                                                       { "level", level_val },
                                                                       { "fine",  fine_val } };

        int knob_level_val = m_voice_level_knobs[i]->value();
        int knob_fine_val = m_voice_fine_knobs[i]->value();

        (*packet)["voices"]["voice" + std::to_string(i)]["knob"] = { { "level", knob_level_val },
                                                                     { "fine",  knob_fine_val } };
    }

    std::vector<int> adsr_vals;
    for (int i = 0; i < m_adsr_buttons.size(); i++) {
        int adsr_val = m_button_states[m_button_adsr_idx+i] > 0;
        m_button_states[m_button_adsr_idx+i] -= adsr_val > 0 ? 1 : 0;

        adsr_vals.push_back(adsr_val);
    }

    (*packet)["adsr"]["button"] = { { "attack",  adsr_vals[0] },
                                    { "decay",   adsr_vals[1] },
                                    { "release", adsr_vals[2] },
                                    { "sustain", adsr_vals[3] } };
    (*packet)["adsr"]["knob"] = { { "attack",  m_adsr_knobs[0]->value() },
                                  { "decay",   m_adsr_knobs[1]->value() },
                                  { "release", m_adsr_knobs[2]->value() },
                                  { "sustain", m_adsr_knobs[3]->value() } };

    std::vector<int> lfo_vals;
    for (int i = 0; i < m_lfo_buttons.size(); i++) {
        int lfo_val = m_button_states[m_button_lfo_idx+i] > 0;
        m_button_states[m_button_lfo_idx+i] -= lfo_val > 0 ? 1 : 0;

        lfo_vals.push_back(lfo_val);
    }

    (*packet)["lfo"]["button"] = { { "type",  lfo_vals[0] },
                                   { "rate",  lfo_vals[1] },
                                   { "level", lfo_vals[2] } };
    (*packet)["lfo"]["knob"] = { { "type",  m_lfo_knobs[0]->value() },
                                 { "rate",  m_lfo_knobs[1]->value() },
                                 { "level", m_lfo_knobs[2]->value() } };

    std::vector<int> filter_vals;
    for (int i = 0; i < m_lfo_buttons.size(); i++) {
        int filter_val = m_button_states[m_button_filter_idx+i] > 0;
        m_button_states[m_button_filter_idx+i] -= filter_val > 0 ? 1 : 0;

        filter_vals.push_back(filter_val);
    }

    (*packet)["filter"]["button"] = { { "type", filter_vals[0] },
                                      { "freq", filter_vals[1] },
                                      { "peak", filter_vals[2] } };
    (*packet)["filter"]["knob"] = { { "type", m_filter_knobs[0]->value() },
                                    { "freq", m_filter_knobs[1]->value() },
                                    { "peak", m_filter_knobs[2]->value() } };

    int fm_val = m_button_states[m_button_fm_idx] > 0;
    m_button_states[m_button_fm_idx] -= fm_val > 0 ? 1 : 0;

    (*packet)["fm"]["button"] = { { "type", fm_val } };
    (*packet)["fm"]["knob"] =   { { "type", m_fm_knobs[0]->value() } };

    int vol_val = m_button_states[m_button_vol_idx] > 0;
    m_button_states[m_button_vol_idx] -= vol_val > 0 ? 1 : 0;

    (*packet)["vol"]["button"] = { { "vol", vol_val } };
    (*packet)["vol"]["knob"] =   { { "vol", m_vol_knobs[0]->value() } };

    (*packet)["selected_preset"] = m_preset_selected;

    (*packet)["key_pressed"] = { { "key0", m_key_pressed[0] },
                                 { "key1", m_key_pressed[1] },
                                 { "key2", m_key_pressed[2] } };

    (*packet)["key_released"] = { { "key0", m_key_released[0] },
                                  { "key1", m_key_released[1] },
                                  { "key2", m_key_released[2] } };

    for (int i = 0; i < m_keys.size(); i++) {
        m_key_pressed[i]  = 0;
        m_key_released[i] = 0;
    }

    return packet;
}

void MainWindow::buildEmptyJSONPacket() {
    for (int i = 0; i < 4; i++) {
        m_board_info["voices"]["voice" + std::to_string(i)]["button"] = { { "mute",  0 },
                                                                          { "level", 0 },
                                                                          { "fine",  0 } };
        m_board_info["voices"]["voice" + std::to_string(i)]["knob"] = { { "level", 0 },
                                                                        { "fine",  0 } };
    }

    m_board_info["adsr"]["button"] = { { "attack",  0 },
                                       { "decay",   0 },
                                       { "release", 0 },
                                       { "sustain", 0 } };
    m_board_info["adsr"]["knob"] = { { "attack",  0 },
                                     { "decay",   0 },
                                     { "release", 0 },
                                     { "sustain", 0 } };

    m_board_info["lfo"]["button"] = { { "type",  0 },
                                      { "rate",  0 },
                                      { "level", 0 } };
    m_board_info["lfo"]["knob"] = { { "type",  0 },
                                    { "rate",  0 },
                                    { "level", 0 } };

    m_board_info["filter"]["button"] = { { "type", 0 },
                                         { "freq", 0 },
                                         { "peak", 0 } };
    m_board_info["filter"]["knob"] = { { "type", 0 },
                                       { "freq", 0 },
                                       { "peak", 0 } };

    m_board_info["fm"]["button"] = { { "type", 0 } };
    m_board_info["fm"]["knob"] =   { { "type", 0 } };


    m_board_info["vol"]["button"] = { { "vol", 0 } };
    m_board_info["vol"]["knob"] =   { { "vol", 0 } };

    std::cout << m_board_info.dump(4) << std::endl;
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}
