#ifndef PENDINGORDERSDELEGATE_H
#define PENDINGORDERSDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchline.h"


class PendingOrdersDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:

    LedgerMasterDatabaseHelper *ledHelper;
    QSqlQueryModel *model;
    UserProfileDatabaseHelper *empHelper;
public:
    explicit PendingOrdersDelegate(LedgerMasterDatabaseHelper *ledHelper,
                                   QSqlQueryModel *model,
                                   UserProfileDatabaseHelper *empHelper,
                                   QObject *parent = Q_NULLPTR)
        :QStyledItemDelegate(parent){
        this->ledHelper = ledHelper;
        this->model = model;
        this->empHelper = empHelper;
    }
    virtual ~PendingOrdersDelegate() override {}


    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void printKot(QString voucherNo,QString voucherPrefix);
    void saveSalesVoucher(QString voucherNo,QString voucherPrefix);

public slots:
      void commitAndCloseItemEditor(QModelIndex index);
      void commitAndCloseEmplEditor(QModelIndex index);

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // PENDINGORDERSDELEGATE_H
