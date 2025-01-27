#ifndef R_WORKERTHREAD_HPP
#define R_WORKERTHREAD_HPP

#include <QThread>
#include <QObject>

class R_Worker : public QThread {
    Q_OBJECT
public:
    R_Worker() = default;
    void run() override;

public slots:
    void shouldRun(bool b) {should_run = b;}

signals:
    void got_message(std::string);

private:
    bool should_run = true;
};

#endif // R_WORKERTHREAD_HPP
