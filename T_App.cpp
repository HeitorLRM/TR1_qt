#include "T_App.hpp"
#include "ui_T_App.h"
#include "freq_window.h"
#include "resolution_window.h"
#include "preview_window.h"
#include "error_window.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QActionGroup>

#include <unistd.h>
#include <iostream>

void TransmitterAPP::make_signal_chart() {
    QLineSeries *series = new QLineSeries();

    QPen pen(QRgb(0));
    pen.setWidth(2.5);
    series->setPen(pen);

    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->legend()->hide();

    auto axisZero = new QCategoryAxis;
    axisZero->append("",0);
    QPen zeropen(QRgb(0xAAAAAA));
    pen.setWidth(1);
    axisZero->setGridLinePen(zeropen);
    chart->addAxis(axisZero, Qt::AlignRight);
    series->attachAxis(axisZero);


    chart->setTitle("Signal");

    QChartView* chartView = ui->chart_signal;
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

}

void TransmitterAPP::make_bitstream_chart() {
    QLineSeries *series = new QLineSeries();

    QPen pen(QRgb(0));
    pen.setWidth(2.5);
    series->setPen(pen);

    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->legend()->hide();

    auto axisZero = new QCategoryAxis;
    axisZero->append("",0);
    QPen zeropen(QRgb(0xAAAAAA));
    pen.setWidth(1);
    axisZero->setGridLinePen(zeropen);
    chart->addAxis(axisZero, Qt::AlignRight);

    series->attachAxis(axisZero);


    chart->setTitle("Sent Bitstream");

    QChartView* chartView = ui->chart_bitstream;
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

TransmitterAPP::TransmitterAPP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Transmitter)
{
    ui->setupUi(this);
    ui->label_pid->setText(QString("PID: ") + QString::number(getpid()));

    make_bitstream_chart();
    make_signal_chart();

    auto mod_group = new QActionGroup(this);
    mod_group->addAction(ui->actionNRZ_Polar);
    mod_group->addAction(ui->actionManchester);
    mod_group->addAction(ui->actionBipolar);
    mod_group->addAction(ui->actionASK);
    mod_group->addAction(ui->actionFSK);
    mod_group->addAction(ui->action4_QAM);
    mod_group->setExclusive(true);
    QObject::connect(mod_group, &QActionGroup::triggered, this, &TransmitterAPP::on_modulation_selected);

    QActionGroup* frame_group = new QActionGroup(this);
    frame_group->addAction(ui->actionByte_Counting);
    frame_group->addAction(ui->actionByte_Insertion);
    frame_group->setExclusive(true);
    QObject::connect(frame_group, &QActionGroup::triggered, this, &TransmitterAPP::on_framing_selected);

    QActionGroup* error_group = new QActionGroup(this);
    error_group->addAction(ui->actionHamming);
    error_group->addAction(ui->actionCRC);
    error_group->addAction(ui->actionParity_Bit);
    error_group->addAction(ui->actionNone);
    error_group->setExclusive(true);
    QObject::connect(error_group, &QActionGroup::triggered, this, &TransmitterAPP::on_error_mode_selected);

}

TransmitterAPP::~TransmitterAPP()
{
    delete ui;
}

void TransmitterAPP::on_send_bit(bool bit){
    if (bits_chart_points.size() >= 16) {
        bits_chart_points.pop_front();
        bits_chart_points.pop_front();
    }
    QPointF back;
    if (bits_chart_points.size())
        back = bits_chart_points.back();
    else
        back = QPointF(1,0);
    back.setX(back.x() + 1.0/8);
    bits_chart_points.push_back(back);
    QPointF new_bit(back.x(), float(bit));
    bits_chart_points.push_back(new_bit);

    QLineSeries* series = dynamic_cast<QLineSeries*>(ui->chart_bitstream->chart()->series().first());
    series->clear();

    for (auto& p : bits_chart_points) {
        p.setX(p.x() - 1.0/8);
        *series << p;
    }
}

void TransmitterAPP::on_send_signal(float energy) {
    if (signal_chart_points.size() >= 8 * settings.resolution) {
        signal_chart_points.pop_front();
    }
    float spacing = 1.0 / 8.0 / settings.resolution;
    QPointF new_point(1, energy);
    signal_chart_points.push_back(new_point);

    QLineSeries* series = dynamic_cast<QLineSeries*>(ui->chart_signal->chart()->series().first());
    series->clear();

    for (auto& p : signal_chart_points) {
        p.setX(p.x() - spacing);
        *series << p;
    }
    ui->chart_signal->chart()->removeSeries(series);
    ui->chart_signal->chart()->addSeries(series);

}

void TransmitterAPP::on_btn_send_clicked()
{
    std::string message = ui->input_box->toPlainText().toStdString();
    ui->input_box->setPlainText("");
    emit send_message(message);
}

void TransmitterAPP::set_settings(T_Settings s) {
    settings = s;
    emit settings_changed(settings);
}

void TransmitterAPP::on_actionFrequency_triggered()
{
    freq_window w;
    w.set_result(settings.frequency);
    w.setModal(true);
    w.exec();
    settings.frequency = w.get_result();
    emit settings_changed(settings);
}


void TransmitterAPP::on_actionSignal_Resolution_triggered()
{
    resolution_window w;
    w.set_result(settings.resolution);
    w.setModal(true);
    w.exec();
    settings.resolution = w.get_result();
    emit settings_changed(settings);
}


void TransmitterAPP::on_actionError_triggered()
{
    error_window w;
    w.set_result(settings.error_chance);
    w.setModal(true);
    w.exec();
    settings.error_chance = w.get_result();
    emit settings_changed(settings);
}

void TransmitterAPP::on_modulation_selected(QAction* action) {
    auto text = action->text().toStdString();
    if (text == "&NRZ-Polar")   settings.modulation = T_Settings::MODS::NRZ_POLAR;
    if (text == "&Manchester")  settings.modulation = T_Settings::MODS::MANCHESTER;
    if (text == "&Bipolar")     settings.modulation = T_Settings::MODS::BIPOLAR;
    if (text == "&ASK")         settings.modulation = T_Settings::MODS::_ASK;
    if (text == "&FSK")         settings.modulation = T_Settings::MODS::_FSK;
    if (text == "&8-QAM")       settings.modulation = T_Settings::MODS::_8_QAM;
    emit settings_changed(settings);
}

void TransmitterAPP::on_framing_selected(QAction* action) {
    std::string text = action->text().toStdString();
    if (text == "Byte &Insertion") settings.framing = FRAMING::BYTE_INSERION;
    if (text == "&Byte Counting")  settings.framing = FRAMING::BYTE_COUNT;
    emit settings_changed(settings);
}

void TransmitterAPP::on_error_mode_selected(QAction* action) {
    std::string text = action->text().toStdString();
    if (text == "&None")        settings.err_handling = ERROR::NONE;
    if (text == "&Hamming")     settings.err_handling = ERROR::HAMMING;
    if (text == "&CRC")         settings.err_handling = ERROR::CRC;
    if (text == "&Parity Bit")  settings.err_handling = ERROR::PARITY_BIT;
    emit settings_changed(settings);
}

void TransmitterAPP::populate_series_digital(QLineSeries* series, const std::string& data) {
    series->clear();

    int current = 0;
    for (auto it = data.begin(); it != data.end(); it++) {
        for (int i = 7; i >= 0; i--) {
            char c = *it;
            bool bit = c & (1<<i);
            *series << QPoint(current, bit);
            current++;
            *series << QPoint(current, bit);
        }
    }
}


void TransmitterAPP::populate_series_analog(QLineSeries* series, const std::string& data) {
    series->clear();

    float sub_bit_interval = 1.0/settings.resolution;
    float current = 0;
    for (auto it = data.begin(); it != data.end(); it++) {
        for (int i = 7; i >= 0; i--) {
            char c = *it;
            bool bit = c & (1<<i);
            for (uint j = 0; j < settings.resolution; j++) {
                float energy = 0;
                auto bit_progress = current - int(current);
                auto byte_progress = current/8 - int(current/8);

                switch (settings.modulation) {
                case T_Settings::MODS::NRZ_POLAR: energy = MODULATOR::NRZ_polar(bit); break;
                case T_Settings::MODS::MANCHESTER: energy = MODULATOR::manchester(bit, bit_progress); break;
                case T_Settings::MODS::BIPOLAR: energy = MODULATOR::bipolar(bit, j == 0); break;
                case T_Settings::MODS::_ASK: energy = MODULATOR::amp_shift_key(bit, bit_progress); break;
                case T_Settings::MODS::_FSK: energy = MODULATOR::freq_shift_key(bit, bit_progress); break;
                case T_Settings::MODS::_8_QAM: energy = MODULATOR::_8_QAM(bit, bit_progress, j == 0); break;
                }
                *series << QPointF(current, energy);
                current += sub_bit_interval;
            }
        }
    }
}

void TransmitterAPP::on_btn_preview_clicked() {
    Preview_Window w;
    QLineSeries
        *msg_series = new QLineSeries,
        *err_series = new QLineSeries,
        *frm_series = new QLineSeries,
        *sig_series = new QLineSeries;
    QChart
        *msg_chart = new QChart,
        *err_chart = new QChart,
        *frm_chart = new QChart,
        *sig_chart = new QChart;

    std::string message = ui->input_box->toPlainText().toStdString();
    populate_series_digital(msg_series, message);

    std::string after_error;
    switch (settings.err_handling) {
    case ERROR::NONE        : after_error = message; break;
    case ERROR::PARITY_BIT  : after_error = ENCODER::parity(message); break;
    case ERROR::CRC         : after_error = ENCODER::CRC(message); break;
    case ERROR::HAMMING     : after_error = ENCODER::hamming(message); break;
    default: break;
    }
    populate_series_digital(err_series, after_error);

    std::string framed = ENCODER::encode_msg(message, settings.framing, settings.err_handling);
    populate_series_digital(frm_series, framed);

    populate_series_analog(sig_series, framed);

    QPen pen(QRgb(0x005b96));
    pen.setWidth(2);

    msg_series->setPen(pen);
    err_series->setPen(pen);
    frm_series->setPen(pen);
    sig_series->setPen(pen);

    msg_chart->addSeries(msg_series);
    err_chart->addSeries(err_series);
    frm_chart->addSeries(frm_series);
    sig_chart->addSeries(sig_series);

    msg_chart->legend()->hide();
    err_chart->legend()->hide();
    frm_chart->legend()->hide();
    sig_chart->legend()->hide();

    msg_chart->createDefaultAxes();
    err_chart->createDefaultAxes();
    frm_chart->createDefaultAxes();
    sig_chart->createDefaultAxes();

    msg_chart->setTitle(QString("Message Bit Representation"));
    err_chart->setTitle(QString("Message After Error Detection Bits"));
    frm_chart->setTitle(QString("Framed Message"));
    sig_chart->setTitle(QString("Constructed signal"));

    w.get_msg_chart()->setChart(msg_chart);
    w.get_err_chart()->setChart(err_chart);
    w.get_frm_chart()->setChart(frm_chart);
    w.get_sig_chart()->setChart(sig_chart);

    w.setModal(true);
    w.exec();
}


