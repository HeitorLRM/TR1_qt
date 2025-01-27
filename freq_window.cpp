#include "freq_window.h"
#include "ui_freq_window.h"

freq_window::freq_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::freq_window)
{
    ui->setupUi(this);
}

freq_window::~freq_window()
{
    delete ui;
}

void freq_window::set_result(float f) {
    ui->plainTextEdit->setPlainText(QString::number(f));
}

float freq_window::get_result() {
    return ui->plainTextEdit->toPlainText().toFloat();
}
