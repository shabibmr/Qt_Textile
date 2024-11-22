#ifndef PURCHASEREGISTER_H
#define PURCHASEREGISTER_H

#include <QWidget>
#include "database/Reports/ledgerhelper.h"
#include "sharetools/sharetools.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Delegates/reportcommondelegate.h"


namespace Ui {
class PurchaseRegister;
}

class PurchaseRegister : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseRegister(QWidget *parent = 0);
    ~PurchaseRegister();

    LedgerHelper *ledHelper;
    QtMaterialIconButton *shareButton;
    QSqlQueryModel *model;
    ShareTools *shareOptionsWidget;

    ReportsTableView *tableView;

private slots:
    void setTableView();

    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void showShareOptions();

private:
    Ui::PurchaseRegister *ui;
};

#endif // PURCHASEREGISTER_H
