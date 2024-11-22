#ifndef PHYSICALSTOCKREGISTER_H
#define PHYSICALSTOCKREGISTER_H

#include <QWidget>

namespace Ui {
class PhysicalStockRegister;
}

class PhysicalStockRegister : public QWidget
{
    Q_OBJECT

public:
    explicit PhysicalStockRegister(QWidget *parent = 0);
    ~PhysicalStockRegister();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::PhysicalStockRegister *ui;
};

#endif // PHYSICALSTOCKREGISTER_H
