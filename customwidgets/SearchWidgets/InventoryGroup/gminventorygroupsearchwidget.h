#ifndef GMINVENTORYGROUPSEARCHWIDGET_H
#define GMINVENTORYGROUPSEARCHWIDGET_H

#include <QCompleter>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QString>
#include <QLineEdit>
#include "customwidgets/SearchWidgets/Inventoryitem/itemsearchdelegate.h"
#include <QListView>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QDebug>
#include <database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h>


class GMInventoryGroupSearchWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMInventoryGroupSearchWidget(SalesInventoryGroupsDatabaseHelper *dbHelper,QSqlQueryModel *m,QLineEdit* parent = Q_NULLPTR);
    virtual ~GMInventoryGroupSearchWidget() override {}
    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    SalesInventoryGroupsDatabaseHelper *itemHelper;
    QString oldText;
    QString oldID;
    InventoryItemGroupDataModel item;

protected:


    void sendPositiveEMIT(QModelIndex index,QString itemID);

    virtual bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return && obj == this->completer()->popup())
            {

                QAbstractItemView* l = static_cast<QAbstractItemView*>(obj);
                QModelIndex i = l->model()->index(0,0);
                if(i.isValid())
                {
                    l->selectionModel()->select(i, QItemSelectionModel::Select);
                    QString itemID = completer()->completionModel()->index(i.row(), 0).data().toString();
                    activated= true;
                    sendPositiveEMIT(i,itemID);
                }
            }
        }
        else
            if (event->type() == QEvent::FocusIn)
            {
                qDebug()<<"Focus in";
                if (obj == this)
                {
                    qDebug()<<" SETTING OLD TEXT "<<this->text();
                    oldText = this->text();
                    activated=false;
                }
            }
            else if( event->type() == QEvent::FocusOut){
                qDebug()<<"Focus out";
            }
        return false;
    }


public slots:
    void completerhighlighted(QModelIndex);
    void completerActivated(QModelIndex);
    void finishEdit();

signals:

    void currentSelection(QModelIndex index);
    void itemSelected(QModelIndex index);
    void itemSelectedwithItem(InventoryItemGroupDataModel item);
};

#endif // GMINVENTORYGROUPSEARCHWIDGET_H
