#ifndef SELECTITEMSWIDGET_H
#define SELECTITEMSWIDGET_H

#include <QWidget>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QToolButton>


namespace Ui {
class SelectItemsWidget;
}

class SelectItemsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectItemsWidget(    GeneralVoucherDataObject *voucher,
QWidget *parent = nullptr);
    ~SelectItemsWidget();

    GeneralVoucherDataObject *voucher;
    GeneralVoucherDataObject *filterVoucher;

    GeneralVoucherDataObject *getVoucher() const;

    void setVoucher(GeneralVoucherDataObject *value);

public slots :

    void settable();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

signals :
    void selectedRows(QList<int> rows);

private:
    Ui::SelectItemsWidget *ui;
};

#endif // SELECTITEMSWIDGET_H
