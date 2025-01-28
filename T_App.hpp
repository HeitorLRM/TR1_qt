#ifndef T_APP_H
#define T_APP_H

#include "T_Settings.hpp"

#include <QMainWindow>
#include <deque>
#include <QActionGroup>
#include <QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui {
class Transmitter;
}
QT_END_NAMESPACE

class TransmitterAPP : public QMainWindow
{
    Q_OBJECT

public:
    TransmitterAPP(QWidget *parent = nullptr);
    ~TransmitterAPP();

    void set_settings(T_Settings);

public slots:
    void on_send_bit(bool);
    void on_send_signal(float);
    void on_modulation_selected(QAction*);
    void on_framing_selected(QAction*);
    void on_error_mode_selected(QAction*);

signals:
    void send_message(std::string);
    void settings_changed(T_Settings);

private slots:
    void on_btn_send_clicked();

    void on_actionFrequency_triggered();

    void on_actionSignal_Resolution_triggered();

    void on_actionError_triggered();

    void on_btn_preview_clicked();

private:
    T_Settings settings;
    std::deque<QPointF> signal_chart_points;
    std::deque<QPointF> bits_chart_points;

    void make_signal_chart();
    void make_bitstream_chart();
    void populate_series_digital(QLineSeries*, const std::string&);
    void populate_series_analog(QLineSeries*, const std::string&);
    Ui::Transmitter *ui;
};
#endif // T_APP_H
