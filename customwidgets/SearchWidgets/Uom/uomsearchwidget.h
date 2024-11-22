#ifndef UOMSEARCHWIDGET_H
#define UOMSEARCHWIDGET_H


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
#include <database/inventory/uom/uomdatabasehelper.h>


class GMUOMSearchWidget : public QLineEdit
{
    Q_OBJECT

public:
    explicit GMUOMSearchWidget(UomDataBaseHelper *dbHelper,QSqlQueryModel *m,QLineEdit* parent = Q_NULLPTR);
    virtual ~GMUOMSearchWidget() override {}
    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    UomDataBaseHelper *uomHelper;
    QString oldText;
    QString oldID;
    UomDataModel uom;

    void setItem(const UomDataModel &value);

protected:


    void sendPositiveEMIT(QModelIndex index,QString itemID);

    virtual bool eventFilter(QObject *obj, QEvent *event) override;



public slots:
    void completerhighlighted(QModelIndex);
    void completerActivated(QModelIndex);
    void finishEdit();

    void finishEditByRet();
signals:

    void currentSelection(QModelIndex index);
    void itemSelected(QModelIndex index);
    void itemSelectedwithItem(UomDataModel uom);
};

#endif // UOMSEARCHWIDGET_H
