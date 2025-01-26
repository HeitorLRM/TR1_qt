#ifndef RECEIVER_APP_H
#define RECEIVER_APP_H

#include <QMainWindow>

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

private slots:
    void on_btn_clear_clicked();

private:
    Ui::Receiver *ui;
};
#endif // RECEIVER_APP_H
