#ifndef GMLEDGERSEARCHWIDGET_H
#define GMLEDGERSEARCHWIDGET_H

#include <QWidget>
#include "datamodels/accounts/ledgermasterdatamodel.h"
#include "gmsearchledgerwidgetitem.h"
#include <QListWidgetItem>
#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QTableWidgetItem>

namespace Ui {
class GMLedgerSearchWidget;
}

class GMLedgerSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GMLedgerSearchWidget(QWidget *parent = 0);
    ~GMLedgerSearchWidget();
    QList<QSharedPointer<LedgerMasterDataModel>> itemsList;
    QStringList itemNames;
    int lLoc =0;
public slots:

    void setData(QList<QSharedPointer<LedgerMasterDataModel>> items);
    void filterData(QString text);
    void checkData();
    void setFocusExpl(QString text);

protected:
    bool eventFilter(QObject* obj, QEvent *event);
    virtual void focusOutEvent(QFocusEvent* e);

signals:
    void selectedItem(QString,QString);
    void nonSelected();
private slots:
    void onTablewidgetClicked(const QModelIndex &index);
    void onTablewidgetEnterpressed(int r);

    void on_tableWidget_clicked(const QModelIndex &index);

private:
    Ui::GMLedgerSearchWidget *ui;
};

#endif // GMLEDGERSEARCHWIDGET_H
