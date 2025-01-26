#ifndef TRANSMITTER_APP_H
#define TRANSMITTER_APP_H

#include <QMainWindow>

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

private slots:
    void on_btn_send_clicked();

private:
    Ui::Transmitter *ui;
};
#endif // TRANSMITTER_APP_H
