#ifndef R_WORKERTHREAD_HPP
#define R_WORKERTHREAD_HPP

#include <QThread>
#include <QObject>

class R_Worker : public QThread {
    Q_OBJECT
private:
    R_Worker() = default;
public:
    static R_Worker* Instance();
    void run() override;
    void emit_bit(bool);
    void emit_energy(float);

public slots:
    void shouldRun(bool b) {should_run = b;}

signals:
    void got_message(std::string);
    void got_bit(bool);
    void got_energy(float);

private:
    bool should_run = true;
};

#endif // R_WORKERTHREAD_HPP
