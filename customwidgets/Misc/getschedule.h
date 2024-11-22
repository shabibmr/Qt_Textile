#ifndef GETSCHEDULE_H
#define GETSCHEDULE_H

#include <QWidget>

namespace Ui {
class GetSchedule;
}

class GetSchedule : public QWidget
{
    Q_OBJECT

public:
    explicit GetSchedule(QWidget *parent = nullptr);
    ~GetSchedule();

private:
    Ui::GetSchedule *ui;
};

#endif // GETSCHEDULE_H
