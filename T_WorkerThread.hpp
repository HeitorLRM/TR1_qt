#ifndef T_WORKERTHREAD_H
#define T_WORKERTHREAD_H

#include <QObject>
#include <QThread>

class T_Worker : public QThread {
    Q_OBJECT
public:
    T_Worker() = default;
    void run() override;

public slots:
    void shouldRun(bool b) {should_run = b;}
    void transmit_request(std::string);

private:
    bool should_run = true;
};

#endif // T_WORKERTHREAD_H
