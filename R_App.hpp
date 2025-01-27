#ifndef R_APP_H
#define R_APP_H

#include "R_Settings.hpp"

#include <QMainWindow>
#include <deque>

QT_BEGIN_NAMESPACE
namespace Ui {
class Receiver;
}
QT_END_NAMESPACE


class ReceiverAPP : public QMainWindow
{
    Q_OBJECT

public:
    ReceiverAPP(QWidget *parent = nullptr);
    ~ReceiverAPP();

    void set_settings(R_Settings);

public slots:
    void on_receive_message(std::string s);
    void on_receive_bit(bool);
    void on_modulation_selected(QAction*);

signals:
    void settings_changed(R_Settings);

private slots:
    void on_btn_clear_clicked();

    void on_actionFrequency_triggered();

    void on_actionSignal_Resolution_triggered();

private:
    R_Settings settings;
    Ui::Receiver *ui;
    std::deque<QPointF> bits_chart_points;

    void make_bitstream_chart();

};
#endif // R_APP_H
