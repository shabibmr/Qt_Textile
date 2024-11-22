#ifndef GMSEARCHLEDGERWIDGETITEM_H
#define GMSEARCHLEDGERWIDGETITEM_H

#include <QWidget>

namespace Ui {
class GMSearchLedgerWidgetItem;
}

class GMSearchLedgerWidgetItem : public QWidget
{
    Q_OBJECT
    QString ledgerID="";

public:
    explicit GMSearchLedgerWidgetItem(QWidget *parent = 0);
    ~GMSearchLedgerWidgetItem();
    QString getLedgerID();
    QString getLedgerName();
    void setData(QString LedgerID,QString LedgerName,QString Group);
private:
    Ui::GMSearchLedgerWidgetItem *ui;
};

#endif // GMSEARCHLEDGERWIDGETITEM_H
