#ifndef RESOLUTION_WINDOW_H
#define RESOLUTION_WINDOW_H

#include <QDialog>

namespace Ui {
class resolution_window;
}

class resolution_window : public QDialog
{
    Q_OBJECT

public:
    explicit resolution_window(QWidget *parent = nullptr);    
    ~resolution_window();
    unsigned get_result();
    void set_result(unsigned);

private:
    Ui::resolution_window *ui;
};

#endif // RESOLUTION_WINDOW_H
