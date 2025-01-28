#include "preview_window.h"
#include "ui_preview_window.h"

Preview_Window::Preview_Window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Preview_Window)
{
    ui->setupUi(this);
}

Preview_Window::~Preview_Window()
{
    delete ui;
}

QChartView *Preview_Window::get_msg_chart()
{
    return ui->message;
}

QChartView *Preview_Window::get_err_chart()
{
    return ui->error_added;
}

QChartView *Preview_Window::get_frm_chart()
{
    return ui->framed;
}

QChartView *Preview_Window::get_sig_chart()
{
    return ui->signal;
}
