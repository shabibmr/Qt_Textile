#ifndef GMPRICELISTLINEMATERIAL_H
#define GMPRICELISTLINEMATERIAL_H

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
#include "database/inventory/pricelist/pricelistdatabasehelper.h"

class GMPriceListLineMaterial : public QtMaterialTextField
{
    Q_OBJECT
public:

    explicit GMPriceListLineMaterial(
            PricelistDatabaseHelper *iHelper,
                                  QSqlQueryModel *m,
                                  QString *PriceListID= nullptr,
                                  QtMaterialTextField *parent=Q_NULLPTR);
    virtual ~GMPriceListLineMaterial() override;

    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    PricelistDatabaseHelper *dbHelper;
    QString oldText;
    QString oldID;
    QString *priceListId;
    void setNewText(QString text, QString id);
    void paintEvent(QPaintEvent *event) override;


//    void setItem(const LedgerMasterDataModel *value);

    void setPriceListId(QString *value);

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
    void itemSelectedwithItem(QString priceListId);
    void selected();
};

#endif // GMPRICELISTLINEMATERIAL_H
