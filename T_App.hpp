#ifndef T_APP_H
#define T_APP_H

#include <QMainWindow>
#include "T_WorkerThread.hpp"
#include <deque>

QT_BEGIN_NAMESPACE
namespace Ui {
class Transmitter;
}
QT_END_NAMESPACE

struct T_Settings {
    float frequency;
    unsigned resolution;
    float error_chance;
}typedef T_Settings;

class TransmitterAPP : public QMainWindow
{
    Q_OBJECT

public:
    TransmitterAPP(QWidget *parent = nullptr);
    ~TransmitterAPP();

public slots:
    void on_send_bit(bool);
    void on_send_signal(float);

signals:
    void send_message(std::string);

private slots:
    void on_btn_send_clicked();

private:
    T_Settings settings = {1.0, 10, 0.0};
    std::deque<QPointF> signal_chart_points;
    std::deque<QPointF> bits_chart_points;

    void make_signal_chart();
    void make_bitstream_chart();

    Ui::Transmitter *ui;
};
#endif // T_APP_H
