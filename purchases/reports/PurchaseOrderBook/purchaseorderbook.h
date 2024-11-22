#ifndef PURCHASEORDERBOOK_H
#define PURCHASEORDERBOOK_H

#include <QWidget>

namespace Ui {
class PurchaseOrderBook;
}

class PurchaseOrderBook : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseOrderBook(QWidget *parent = 0);
    ~PurchaseOrderBook();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::PurchaseOrderBook *ui;
};

#endif // PURCHASEORDERBOOK_H
