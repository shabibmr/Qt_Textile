#ifndef GMGODOWNLINEMATERIAL_H
#define GMGODOWNLINEMATERIAL_H

#include "customwidgets/Material/components/qtmaterialtextfield.h"
#include <QCompleter>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QString>
#include "customwidgets/SearchWidgets/Inventoryitem/itemsearchdelegate.h"
#include <QListView>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QDebug>
#include "database/inventory/godown/godowndatabasehelper.h"

class GMGodownLineMaterial : public QtMaterialTextField
{
    Q_OBJECT
public:

    explicit GMGodownLineMaterial(
            GodownDatabaseHelper *iHelper,
                                  QSqlQueryModel *m,
                                  QString *godownID= nullptr,
                                  QtMaterialTextField *parent=Q_NULLPTR);
    virtual ~GMGodownLineMaterial() override;

    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    GodownDatabaseHelper *dbHelper;
    QString oldText;
    QString oldID;
    QString *godown;
    void setNewText(QString text, QString id);
    void paintEvent(QPaintEvent *event) override;


//    void setItem(const LedgerMasterDataModel *value);

    void setGodown(QString *value);

protected:


    void sendPositiveEMIT(QModelIndex index,QString itemID);

    virtual bool eventFilter(QObject *obj, QEvent *event) override;


public slots:
    void completerhighlighted(QModelIndex);
    void completerActivated(QModelIndex);
    void finishEdit();

    void finishEditByRet();

    void createLedger();
signals:

    void currentSelection(QModelIndex index);
    void itemSelected(QModelIndex index);
    void itemSelectedwithItem(QString godown);
    void selected();
};

#endif // GMGODOWNLINEMATERIAL_H
