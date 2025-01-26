#include "Transmitter_App.h"
#include "ui_Transmitter_App.h"

#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>

#include <iostream>


TransmitterAPP::TransmitterAPP(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Transmitter)
{
    ui->setupUi(this);
}

TransmitterAPP::~TransmitterAPP()
{
    delete ui;
}

void TransmitterAPP::on_btn_send_clicked()
{
    std::cout << "!" << std::endl;


}

