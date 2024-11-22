#ifndef GROUPVOUCHERS_H
#define GROUPVOUCHERS_H

#include <QWidget>

namespace Ui {
class GroupVouchers;
}

class GroupVouchers : public QWidget
{
    Q_OBJECT

public:
    explicit GroupVouchers(QWidget *parent = 0);
    ~GroupVouchers();

private:
    Ui::GroupVouchers *ui;
};

#endif // GROUPVOUCHERS_H
