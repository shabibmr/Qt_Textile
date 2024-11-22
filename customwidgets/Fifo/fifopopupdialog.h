#ifndef FIFOPOPUPDIALOG_H
#define FIFOPOPUPDIALOG_H

#include <QDialog>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include <QTableView>
#include "customwidgets/Models/gmfifomodel.h"

namespace Ui {
class FifoPopupDialog;
}

class FifoPopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FifoPopupDialog(inventoryItemDataModel *item, QWidget *parent = nullptr);
    ~FifoPopupDialog();



private slots:
    void on_pushButton_clicked();
    void calculate();

private:
    Ui::FifoPopupDialog *ui;
    inventoryItemDataModel *item;

};

#endif // FIFOPOPUPDIALOG_H
