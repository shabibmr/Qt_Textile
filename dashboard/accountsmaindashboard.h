#ifndef ACCOUNTSMAINDASHBOARD_H
#define ACCOUNTSMAINDASHBOARD_H

#include <QWidget>

namespace Ui {
class AccountsMainDashboard;
}

class AccountsMainDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit AccountsMainDashboard(QWidget *parent = nullptr);
    ~AccountsMainDashboard();

private:
    Ui::AccountsMainDashboard *ui;
};

#endif // ACCOUNTSMAINDASHBOARD_H
