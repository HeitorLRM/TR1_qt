#include "error_window.h"
#include "ui_error_window.h"

error_window::error_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::error_window)
{
    ui->setupUi(this);
}

error_window::~error_window()
{
    delete ui;
}

void error_window::set_result(float f) {
    ui->plainTextEdit->setPlainText(QString::number(f));
}

float error_window::get_result() {
    return ui->plainTextEdit->toPlainText().toFloat();
}
