#ifndef GMACCOUNTGROUPSEARCHWIDGET_H
#define GMACCOUNTGROUPSEARCHWIDGET_H

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
#include <database/finance/accountmaster/accountgroupmasterdatabasehelper.h>


class GMAccountGroupSearchWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMAccountGroupSearchWidget(AccountGroupMasterDatabaseHelper *dbHelper,QSqlQueryModel *m,QLineEdit* parent = Q_NULLPTR);
    virtual ~GMAccountGroupSearchWidget() override {}
    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    AccountGroupMasterDatabaseHelper *itemHelper;
    QString oldText;
    QString oldID;
    AccountGroupDataModel item;

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
    void itemSelectedwithItem(AccountGroupDataModel item);
};

#endif // GMACCOUNTGROUPSEARCHWIDGET_H
