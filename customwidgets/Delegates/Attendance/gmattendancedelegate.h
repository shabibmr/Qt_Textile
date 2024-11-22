#ifndef GMATTENDANCEDELEGATE_H
#define GMATTENDANCEDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTimeEdit>
#include <QCheckBox>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchwidget.h"
#include "QVBoxLayout"
#include <QMessageBox>


class GMAttendanceDelegate : public QStyledItemDelegate
{
public:
    GMAttendanceDelegate();
    virtual ~GMAttendanceDelegate() override;
    UserProfileDatabaseHelper *itemHelper;
    explicit GMAttendanceDelegate(UserProfileDatabaseHelper *iHelper, QSqlQueryModel *model);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);

};

#endif // GMATTENDANCEDELEGATE_H
