#ifndef NOTIFICATIONSDELEGATE_H
#define NOTIFICATIONSDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchline.h"


class NotificationsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:

    QSqlQueryModel *model;
    bool history = false;
public:
    explicit NotificationsDelegate(QSqlQueryModel *model, bool history = false,
                                   QObject *parent = Q_NULLPTR)
        :QStyledItemDelegate(parent){
        this->model = model;
        this->history = history;
    }
    virtual ~NotificationsDelegate() override {}


    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void markRead(int row);

};

#endif // NOTIFICATIONSDELEGATE_H
