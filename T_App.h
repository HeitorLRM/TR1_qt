#ifndef T_APP_H
#define T_APP_H

#include <QMainWindow>
#include "T_WorkerThread.hpp"

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

signals:
    void send_message(std::string);

private slots:
    void on_btn_send_clicked();

private:
    Ui::Transmitter *ui;
};
#endif // T_APP_H
