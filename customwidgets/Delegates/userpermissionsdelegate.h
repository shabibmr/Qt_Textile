#ifndef USERPERMISSIONSDELEGATE_H
#define USERPERMISSIONSDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QSqlQueryModel>
#include <QCheckBox>

#include "customwidgets/Models/gmaccesscontrolmodel.h"

class UserPermissionsDelegate: public QStyledItemDelegate
{

public:

    explicit UserPermissionsDelegate (QSqlQueryModel *model);
    virtual ~UserPermissionsDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSqlQueryModel *model;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // QAbstractItemDelegate interface
public:
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // USERPERMISSIONSDELEGATE_H
