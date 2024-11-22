#ifndef DENOMINATIONS_H
#define DENOMINATIONS_H

#include <QWidget>
#include <QMessageBox>

#include "datamodels/Counter/denominationsdatamodel.h"
#include "datamodels/Counter/shiftdatamodel.h"

namespace Ui {
class Denominations;
}

class Denominations : public QWidget
{
    Q_OBJECT

public:
    explicit Denominations(ShiftDataModel *shift = new ShiftDataModel(), QWidget *parent = 0);
    ~Denominations();

    DenominationsDataModel *obj;

    ShiftDataModel *shift;

    void setObj(DenominationsDataModel *value);

private slots:
    void on_save_clicked();

    void on_cancel_clicked();

    void on_thousandLineEdit_textChanged(const QString &arg1);

    void on_coinsLineEdit_textChanged(const QString &arg1);

    void on_fiveHunderdLineEdit_textChanged(const QString &arg1);

    void on_twoHundredLineEdit_textChanged(const QString &arg1);

    void on_hundredLineEdit_textChanged(const QString &arg1);

    void on_fityLineEdit_textChanged(const QString &arg1);

    void on_twentyLineEdit_textChanged(const QString &arg1);

    void on_tenLineEdit_textChanged(const QString &arg1);

    void on_fiveLineEdit_textChanged(const QString &arg1);

    void calcTotal();

signals:

    void totalSales(DenominationsDataModel *obj);

private:
    Ui::Denominations *ui;
};

#endif // DENOMINATIONS_H
