#ifndef SALESBILLSPENDING_H
#define SALESBILLSPENDING_H

#include <QWidget>

namespace Ui {
class SalesBillsPending;
}

class SalesBillsPending : public QWidget
{
    Q_OBJECT

public:
    explicit SalesBillsPending(QWidget *parent = 0);
    ~SalesBillsPending();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::SalesBillsPending *ui;
};

#endif // SALESBILLSPENDING_H
