#ifndef R_APP_H
#define R_APP_H

#include <QMainWindow>
#include <deque>

QT_BEGIN_NAMESPACE
namespace Ui {
class Receiver;
}
QT_END_NAMESPACE

struct R_Settings {
    float frequency;
    unsigned resolution;
    float error_chance;
}typedef R_Settings;

class ReceiverAPP : public QMainWindow
{
    Q_OBJECT

public:
    ReceiverAPP(QWidget *parent = nullptr);
    ~ReceiverAPP();

public slots:
    void on_receive_message(std::string s);
    void on_receive_bit(bool);
    void on_receive_signal(float);

private slots:
    void on_btn_clear_clicked();

private:
    R_Settings settings = {1.0, 10, 0.0};
    Ui::Receiver *ui;
    std::deque<QPointF> signal_chart_points;
    std::deque<QPointF> bits_chart_points;

    void make_signal_chart();
    void make_bitstream_chart();

};
#endif // R_APP_H
