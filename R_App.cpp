#include "R_App.hpp"
#include "ui_R_App.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <iostream>
#include <unistd.h>

void ReceiverAPP::make_signal_chart() {
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


    chart->setTitle("Signal Received");

    QChartView* chartView = ui->chart_signal;
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

void ReceiverAPP::make_bitstream_chart() {
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


    chart->setTitle("Incoming Bitstream");

    QChartView* chartView = ui->chart_bitstream;
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

ReceiverAPP::ReceiverAPP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Receiver)
{
    ui->setupUi(this);
    ui->label_pid->setText(QString("PID: ") + QString::number(getpid()));

    make_bitstream_chart();
    make_signal_chart();
}

ReceiverAPP::~ReceiverAPP()
{
    delete ui;
}

void ReceiverAPP::on_receive_message(std::string message) {
    QString new_message = ui->output_box->toPlainText() + QString::fromStdString(message);
    ui->output_box->setPlainText(new_message);
}

void ReceiverAPP::on_receive_bit(bool bit) {
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

void ReceiverAPP::on_receive_signal(float energy) {
    if (signal_chart_points.size() >= 8 * settings.resolution) {
        signal_chart_points.pop_front();
    }
    float spacing = 1.0 / 8 / settings.resolution;
    QPointF new_point(1, energy);
    signal_chart_points.push_back(new_point);

    QLineSeries* series = dynamic_cast<QLineSeries*>(ui->chart_signal->chart()->series().first());
    series->clear();

    for (auto& p : signal_chart_points) {
        p.setX(p.x() - spacing);
        *series << p;
    }
}

void ReceiverAPP::on_btn_clear_clicked()
{
    ui->output_box->setPlainText("");
}

