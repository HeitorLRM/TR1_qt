#include "R_App.h"
#include "ui_R_App.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <iostream>
#include <unistd.h>


ReceiverAPP::ReceiverAPP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Receiver)
{
    ui->setupUi(this);
    ui->label_pid->setText(QString("PID: ") + QString::number(getpid()));

    QLineSeries *series = new QLineSeries();

    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    QChartView* chartView = ui->chart_bitstream;
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

}

ReceiverAPP::~ReceiverAPP()
{
    delete ui;
}

void ReceiverAPP::on_btn_clear_clicked()
{
    ui->output_box->setPlainText("");
}

