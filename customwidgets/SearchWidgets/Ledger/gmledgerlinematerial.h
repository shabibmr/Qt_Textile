#ifndef GMLEDGERLINEMATERIAL_H
#define GMLEDGERLINEMATERIAL_H

#include "customwidgets/Material/components/qtmaterialtextfield.h"
#include <QCompleter>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QString>
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchdelegate.h"
#include <QListView>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QDebug>
#include <database/finance/ledgermaster/ledgermasterdatabasehelper.h>


class GMLedgerLineMaterial : public QtMaterialTextField
{
    Q_OBJECT
public:

    explicit GMLedgerLineMaterial(
            LedgerMasterDatabaseHelper *iHelper,
                                  QSqlQueryModel *m,
                                  LedgerMasterDataModel *led =Q_NULLPTR,
                                  QtMaterialTextField *parent=Q_NULLPTR);
    virtual ~GMLedgerLineMaterial() override;
    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    LedgerMasterDatabaseHelper *ledHelper;
    QString oldText;
    QString oldID;
    LedgerMasterDataModel *item;
    void setNewText(QString text, QString id);
    void paintEvent(QPaintEvent *event) override;
    void setSourceData();


//    void setItem(const LedgerMasterDataModel *value);

    void setItem(LedgerMasterDataModel *value);

    void setSrcModel(QSqlQueryModel *value);

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
    void itemSelectedwithItem(LedgerMasterDataModel item);
    void selected();
};

#endif // GMLEDGERLINEMATERIAL_H
