#ifndef SERVICEREGISTERLIST_H
#define SERVICEREGISTERLIST_H

#include <QWidget>

namespace Ui {
class ServiceRegisterList;
}

class ServiceRegisterList : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceRegisterList(QWidget *parent = 0);
    ~ServiceRegisterList();

private:
    Ui::ServiceRegisterList *ui;
};

#endif // SERVICEREGISTERLIST_H
