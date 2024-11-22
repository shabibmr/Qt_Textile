#ifndef PURCHASEBILLSPENDING_H
#define PURCHASEBILLSPENDING_H

#include <QWidget>

namespace Ui {
class PurchaseBillsPending;
}

class PurchaseBillsPending : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseBillsPending(QWidget *parent = 0);
    ~PurchaseBillsPending();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::PurchaseBillsPending *ui;
};

#endif // PURCHASEBILLSPENDING_H
