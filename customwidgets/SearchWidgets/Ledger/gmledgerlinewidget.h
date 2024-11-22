#ifndef GMLEDGERLINEWIDGET_H
#define GMLEDGERLINEWIDGET_H

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
#include <database/finance/ledgermaster/ledgermasterdatabasehelper.h>

class GMLedgerLineWidget : public QLineEdit
{
    Q_OBJECT
public:

    explicit GMLedgerLineWidget(LedgerMasterDatabaseHelper *iHelper,QSqlQueryModel *m,QLineEdit *parent=Q_NULLPTR);
    virtual ~GMLedgerLineWidget() override;
    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    LedgerMasterDatabaseHelper *ledHelper;
    QString oldText;
    QString oldID;
    LedgerMasterDataModel item;
    void setNewText(QString text, QString id);

protected:


    void sendPositiveEMIT(QModelIndex index,QString itemID);

    virtual bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
//            if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return && obj == this->completer()->popup())
//            {

//                QAbstractItemView* l = static_cast<QAbstractItemView*>(obj);
//                QModelIndex i = l->model()->index(0,0);
//                if(i.isValid())
//                {
//                    l->selectionModel()->select(i, QItemSelectionModel::Select);
//                    QString itemID = completer()->completionModel()->index(i.row(), 0).data().toString();
//                    activated= true;
//                    qDebug()<<"Item Pressed at "<<i.row()<<"item id "<<itemID;
//                    sendPositiveEMIT(i,itemID);
//                }
//            }
        }
        else
            if (event->type() == QEvent::FocusIn)
            {
//                qDebug()<<"Focus in";
                if (obj == this)
                {
//                  qDebug()<<" SETTING OLD TEXT "<<this->text();
                    oldText = this->text();
                    this->completer()->complete();
                    activated=false;
                }
            }
            else if( event->type() == QEvent::FocusOut){
                this->setText(item.LedgerName);
                qDebug()<<"Focus out text "<<item.LedgerName;
            }
        return false;
    }


public slots:
    void completerhighlighted(QModelIndex);
    void completerActivated(QModelIndex);
    void finishEdit();

    void finishEditByRet();
signals:

    void currentSelection(QModelIndex index);
    void itemSelected(QModelIndex index);
    void itemSelectedwithItem(LedgerMasterDataModel item);
};

#endif // GMLEDGERLINEWIDGET_H
