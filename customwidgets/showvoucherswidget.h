#ifndef SHOWVOUCHERSWIDGET_H
#define SHOWVOUCHERSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/finance/TransactionHelper/generalvoucherdatabasehelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>



namespace Ui {
class ShowVouchersWidget;
}

class ShowVouchersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowVouchersWidget(QWidget *parent = nullptr);

    QList<GeneralVoucherDataObject *> vouchersList;
    QMap<QString, QString> sectionsList;
    QString section;
    QString LedgerID;
    QMap<QString,QVariant> filter;
    GeneralVoucherDatabaseHelper* dbHelper;
    int targetVoucherType;
    int sourceVoucherType;
    QDateTime fromDate ;
    QDateTime toDate;

    bool showWidget = true;


    ~ShowVouchersWidget();

//    QComboBox* sectionCombo;
    QLabel *sectionLabel;


    void setVoucherTypes(int srcVType, int tgtVType)        ;
    void setVouchersList();
    void setSectionsList();


    void setLedger(QString ledgerId);
    void setSection(QString section1);
    void setDate(QDateTime fromDate, QDateTime toDate);
     void selectAllVouchers();
     void import();
     void setShowWidget(bool value);

private slots:
     void selectVoucher(int state);
    void on_ImportPushButton_clicked();

    void on_CancelPushButton_clicked();
    void sectionSelected(QString text);

private:
    Ui::ShowVouchersWidget *ui;
signals:
    void importVoucher(GeneralVoucherDataObject *);
    void exportVoucher(GeneralVoucherDataObject *);
    void noVoucherToExport();

};

#endif // SHOWVOUCHERSWIDGET_H
