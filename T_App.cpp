#include "T_App.h"
#include "ui_T_App.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <iostream>
#include <unistd.h>


TransmitterAPP::TransmitterAPP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Transmitter)
{
    ui->setupUi(this);
    ui->label_pid->setText(QString("PID: ") + QString::number(getpid()));
}

TransmitterAPP::~TransmitterAPP()
{
    delete ui;
}

void TransmitterAPP::on_btn_send_clicked()
{
    std::string message = ui->input_box->toPlainText().toStdString();
    ui->input_box->setPlainText("");
    emit send_message(message);
}

