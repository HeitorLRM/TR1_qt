#include "T_App.hpp"
#include "ui_T_App.h"
#include "freq_window.h"
#include "resolution_window.h"
#include "error_window.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <unistd.h>

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


    chart->setTitle("Signal Sent");

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

