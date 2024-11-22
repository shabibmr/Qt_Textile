#include "BillwiseMappingDelegate.h"
#include <QDebug>
#include <QApplication>
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include <QCheckBox>

BillwiseMappingDelegate::BillwiseMappingDelegate(QSqlQueryModel *model)
{
    this->model = model;
}

void BillwiseMappingDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==8){
        bool enabled = true;
        QStyleOptionViewItem opt = option;
        QRect crect = opt.rect;
        crect.moveLeft(15);
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
        //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);  QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .
    }
    QStyledItemDelegate::paint(painter,option,index);

}

QWidget *BillwiseMappingDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 8){
        QCheckBox *CheckBox = new QCheckBox;
        CheckBox->setParent(parent);

        return CheckBox;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);

}

void BillwiseMappingDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 8){
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(editor);
        if(checkBox->isEnabled())
            model->setData(index, QVariant::fromValue(checkBox->isChecked()));

        return;
    }
    QStyledItemDelegate::setEditorData(editor, index);

}

void BillwiseMappingDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 8){
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(editor);
        if(checkBox->isEnabled())
            model->setData(index, QVariant::fromValue(checkBox->isChecked()));

        return;
    }

    QStyledItemDelegate::setModelData(editor, model, index);

}

void BillwiseMappingDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);

}


bool BillwiseMappingDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(index.column() == 8  )
        {
             model->setData(index, !model->data(index, Qt::CheckStateRole).toBool());
        }

    }
    else{
        QStyledItemDelegate::editorEvent(event,model,option,index);
    }
}
