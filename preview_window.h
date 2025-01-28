#ifndef PREVIEW_WINDOW_H
#define PREVIEW_WINDOW_H

#include <QDialog>
#include <QChartView>

namespace Ui {
class Preview_Window;
}

class Preview_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Preview_Window(QWidget *parent = nullptr);
    ~Preview_Window();
    QChartView* get_msg_chart();
    QChartView* get_err_chart();
    QChartView* get_frm_chart();
    QChartView* get_sig_chart();

private:
    Ui::Preview_Window *ui;

private:
};

#endif // PREVIEW_WINDOW_H
