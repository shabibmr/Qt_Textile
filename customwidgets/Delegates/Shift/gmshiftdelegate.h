#ifndef GMSHIFTDELEGATE_H
#define GMSHIFTDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTimeEdit>
#include <QCheckBox>
#include "customwidgets/Models/gmshiftsalesmodel.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include <QLineEdit>
#include <QPainter>


class GMShiftDelegate : public QStyledItemDelegate
{
public:
    GMShiftDelegate();
    virtual ~GMShiftDelegate() override;
    explicit GMShiftDelegate(GMShiftSalesModel *model);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    GMShiftSalesModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);

};

#endif // GMSHIFTDELEGATE_H
