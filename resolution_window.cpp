#include "resolution_window.h"
#include "ui_resolution_window.h"

resolution_window::resolution_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::resolution_window)
{
    ui->setupUi(this);
}

resolution_window::~resolution_window()
{
    delete ui;
}

void resolution_window::set_result(unsigned f) {
    ui->plainTextEdit->setPlainText(QString::number(f));
}

unsigned resolution_window::get_result() {
    return ui->plainTextEdit->toPlainText().toUInt();
}

