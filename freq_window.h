#ifndef FREQ_WINDOW_H
#define FREQ_WINDOW_H

#include <QDialog>

namespace Ui {
class freq_window;
}

class freq_window : public QDialog
{
    Q_OBJECT

public:
    explicit freq_window(QWidget *parent = nullptr);
    ~freq_window();
    float get_result();
    void set_result(float);

private:
    Ui::freq_window *ui;
};

#endif // FREQ_WINDOW_H
