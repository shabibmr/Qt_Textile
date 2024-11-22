#ifndef GMITEMSEARCHWIDGET_H
#define GMITEMSEARCHWIDGET_H

#include <QWidget>
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "searchlistitemwidget.h"
#include <QListWidgetItem>
#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QTableWidgetItem>
namespace Ui {
class GMItemSearchWidget;
}

class GMItemSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GMItemSearchWidget(QWidget *parent = 0);
    QList<QSharedPointer<inventoryItemDataModel>> itemsList;
    QStringList itemNames;
    ~GMItemSearchWidget();
    int lLoc =0;
public slots:
    void setData(QList<QSharedPointer<inventoryItemDataModel>> items);
    void filterData(QString text);
    void checkData();
    void setFocusExpl(QString text);

private:
    Ui::GMItemSearchWidget *ui;

protected:
    bool eventFilter(QObject* obj, QEvent *event);
    virtual void focusOutEvent(QFocusEvent* e);

signals:
    void selectedID(QString,QString);
    void nonSelected();
    void SOSelectedSig(QString);

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);
    void onTablewidgetEnterpressed(int item);
};

#endif // GMITEMSEARCHWIDGET_H
