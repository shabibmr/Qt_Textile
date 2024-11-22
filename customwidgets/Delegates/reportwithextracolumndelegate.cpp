#include "reportwithextracolumndelegate.h"

ReportWithExtraColumnCommonDelegate::ReportWithExtraColumnCommonDelegate(QSqlQueryModel *model)
{
    this->model = model;
}

QWidget *ReportWithExtraColumnCommonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ReportWithExtraColumnCommonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void ReportWithExtraColumnCommonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void ReportWithExtraColumnCommonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
