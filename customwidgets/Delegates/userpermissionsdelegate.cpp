#include "userpermissionsdelegate.h"
#include <QApplication>


UserPermissionsDelegate::UserPermissionsDelegate(QSqlQueryModel *model)
{
    this->model = model;

}

UserPermissionsDelegate::~UserPermissionsDelegate()
{

}
void UserPermissionsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() >=3 && index.column() <= 8)
    {
        bool enabled = true;
        QStyleOptionViewItem opt = option;
        QRect crect = opt.rect;
        crect.moveLeft(15);
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
        //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);
    }
    //    else if(index.column() ==3)
    //    {
    //        bool enabled = true;
    //                QStyleOptionViewItem opt = option;
    //                QRect crect = opt.rect;
    //                crect.moveLeft(15);
    //                 QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
    //                //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);
    //    }
    //    else if(index.column() ==4)
    //    {
    //        bool enabled = true;
    //                QStyleOptionViewItem opt = option;
    //                QRect crect = opt.rect;
    //                crect.moveLeft(15);
    //                 QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
    //                //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);
    //    }
    //    else if(index.column() ==5)
    //    {
    //        bool enabled = true;
    //                QStyleOptionViewItem opt = option;
    //                QRect crect = opt.rect;
    //                crect.moveLeft(15);
    //                 QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
    //                //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);
    //    }
    //    else if(index.column() ==6)
    //    {
    //        bool enabled = true;
    //                QStyleOptionViewItem opt = option;
    //                QRect crect = opt.rect;
    //                crect.moveLeft(15);
    //                 QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
    //                //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);
    //    }
    //    else
    QStyledItemDelegate::paint(painter,option,index);

}


QWidget *UserPermissionsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() >=3 && index.column()<= 8){
        QCheckBox *CheckBox = new QCheckBox;
        CheckBox->setParent(parent);

        return CheckBox;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void UserPermissionsDelegate::setEditorData(QWidget *editor,  const QModelIndex &index) const
{
    qDebug()<<"Call Set EDITOR Data col : "<<index.column();


    switch (index.column()) {
    case 3:
    {
        QCheckBox *allowCreateCheck = qobject_cast<QCheckBox *>(editor);
        if(allowCreateCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowCreateCheck->isChecked()));

        return;
    }
    case 4:
    {
        QCheckBox *allowReadCheck = qobject_cast<QCheckBox *>(editor);
        if(allowReadCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowReadCheck->isChecked()));

        return;
    }
    case 5:
    {
        QCheckBox *allowUpdateCheck =  qobject_cast<QCheckBox *>(editor);
        if(allowUpdateCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowUpdateCheck->isChecked()));

        return;
    }
    case 6:
    {
        QCheckBox *allowDeleteCheck = qobject_cast<QCheckBox *>(editor);
        if(allowDeleteCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowDeleteCheck->isChecked()));

        return;
    }
    case 7:
    {
        QCheckBox *allowShareCheck = qobject_cast<QCheckBox *>(editor);
        if(allowShareCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowShareCheck->isChecked()));

        return;
    }
    case 8:
    {
        QCheckBox *allowAllCheck = qobject_cast<QCheckBox *>(editor);
        if(allowAllCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowAllCheck->isChecked()));

        return;
    }

    }
    QStyledItemDelegate::setEditorData(editor, index);
}

void UserPermissionsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column()) {
    case 3:
    {
        QCheckBox *allowCreateCheck = qobject_cast<QCheckBox *>(editor);
        if(allowCreateCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowCreateCheck->isChecked()));

        return;
    }
    case 4:
    {
        QCheckBox *allowReadCheck = qobject_cast<QCheckBox *>(editor);
        if(allowReadCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowReadCheck->isChecked()));

        return;
    }
    case 5:
    {
        QCheckBox *allowUpdateCheck =  qobject_cast<QCheckBox *>(editor);
        if(allowUpdateCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowUpdateCheck->isChecked()));

        return;
    }
    case 6:
    {
        QCheckBox *allowDeleteCheck = qobject_cast<QCheckBox *>(editor);
        if(allowDeleteCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowDeleteCheck->isChecked()));

        return;
    }
    case 7:
    {
        QCheckBox *allowShareCheck = qobject_cast<QCheckBox *>(editor);
        if(allowShareCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowShareCheck->isChecked()));

        return;
    }
    case 8:
    {
        QCheckBox *allowAllCheck = qobject_cast<QCheckBox *>(editor);
        if(allowAllCheck->isEnabled())
            model->setData(index, QVariant::fromValue(allowAllCheck->isChecked()));

        return;
    }

    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void UserPermissionsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);

}

bool UserPermissionsDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}
