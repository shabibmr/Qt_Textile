#ifndef KOTREPORTDELEGATE_H
#define KOTREPORTDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchline.h"


class KOTReportDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:

    QSqlQueryModel *model;
public:
     KOTReportDelegate(QSqlQueryModel *model)
       {
        this->model = model;
    }
    virtual ~KOTReportDelegate() override {}


    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void updateStatus(int row);

};

#endif // KOTREPORTDELEGATE_H
