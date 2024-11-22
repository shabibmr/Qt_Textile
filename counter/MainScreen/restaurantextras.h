#ifndef RESTAURANTEXTRAS_H
#define RESTAURANTEXTRAS_H

#include <QWidget>

#include "counter/Reports/todaysbills.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "login/loginpage.h"

namespace Ui {
class RestaurantExtras;
}

class RestaurantExtras : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantExtras(QWidget *parent = 0);
    ~RestaurantExtras();

private slots:
    void on_adminButton_clicked();

    void on_reportsButton_clicked();

    void on_openTillButton_clicked();

    void on_screenLockButton_clicked();

signals:
    void adminButtonClicked();

    void screenLockClicked();

    void reportsButtonClicked();

private:
    Ui::RestaurantExtras *ui;
};

#endif // RESTAURANTEXTRAS_H
