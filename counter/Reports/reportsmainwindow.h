#ifndef REPORTSMAINWINDOW_H
#define REPORTSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class ReportsMainWindow;
}

class ReportsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReportsMainWindow(QWidget *parent = 0);
    ~ReportsMainWindow();

private:
    Ui::ReportsMainWindow *ui;
};

#endif // REPORTSMAINWINDOW_H
