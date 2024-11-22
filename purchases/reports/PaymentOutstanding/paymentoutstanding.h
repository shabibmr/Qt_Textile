#ifndef PAYMENTOUTSTANDING_H
#define PAYMENTOUTSTANDING_H

#include <QWidget>

namespace Ui {
class PaymentOutstanding;
}

class PaymentOutstanding : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentOutstanding(QWidget *parent = 0);
    ~PaymentOutstanding();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::PaymentOutstanding *ui;
};

#endif // PAYMENTOUTSTANDING_H
