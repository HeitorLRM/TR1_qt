#ifndef T_WORKERTHREAD_H
#define T_WORKERTHREAD_H

#include <QObject>
#include <QThread>
#include "T_App.hpp"

class T_Worker : public QThread {
    Q_OBJECT
private:
    T_Worker() = default;
public:
    static T_Worker* Instance();
    void run() override;
    void emit_bit(bool);
    void emit_energy(float);

public slots:
    void shouldRun(bool b) {should_run = b;}
    void transmit_request(std::string);
    void set_settings(T_Settings);

signals:
    void sent_bit(bool);
    void sent_energy(float);

private:
    bool should_run = true;
};

#endif // T_WORKERTHREAD_H
