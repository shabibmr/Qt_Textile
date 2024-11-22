#include "gmshiftdelegate.h"
#include <QApplication>

GMShiftDelegate::~GMShiftDelegate()
{

}

GMShiftDelegate::GMShiftDelegate(GMShiftSalesModel *model)
{
    this->model = model;

}

void GMShiftDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;

    QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

    if (option.state & QStyle::State_Selected)
    {
        painter->setPen(QPen(QColor(242,139,47),4,Qt::SolidLine,Qt::RoundCap));
        painter->drawRect(rect);
        painter->setPen(option.palette.color(cg, QPalette::HighlightedText));

    }
    else
    {
        painter->setPen(option.palette.color(cg, QPalette::Text));
    }


    painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-10, rect.height()),
                      index.model()->data(index, Qt::TextAlignmentRole).toInt(),index.model()->data(index, Qt::DisplayRole).toString());

//    else
//        QStyledItemDelegate::paint(painter,option,index);
}

QWidget *GMShiftDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {

    case 2:
    {

        QLineEdit *amountEdit = new QLineEdit;
        amountEdit->setParent(parent);

//        QObject::connect(startTimeEdit, &QDateTimeEdit::dateTimeChanged,
//                                 this, &GMShiftDelegate::commitAndCloseItemEditor);
        return amountEdit;


    }

    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void GMShiftDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //    qDebug()<<"Call Set EDITOR Data col : "<<index.column();
    switch (index.column()) {

    case 2:
    {
        QLineEdit *amtEdit = qobject_cast<QLineEdit *>(editor);
        if(amtEdit->text().length() > 0)
            model->setData(index, QVariant::fromValue(amtEdit->text()));
        return;
    }

    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void GMShiftDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //    qDebug()<<".........................Set MOdel Data";
    switch (index.column()) {

    case 2:
    {
        QLineEdit *amtEdit = qobject_cast<QLineEdit *>(editor);
        if(amtEdit->text().length() > 0)
            model->setData(index, QVariant::fromValue(amtEdit->text()));
        return;
    }


    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void GMShiftDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void GMShiftDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    //    qDebug()<<".............................COMMITTING DATA...........................";
//    GMLedgerLineWidget *editor = qobject_cast<GMLedgerLineWidget *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
//    emit commitData(editor);
//    emit closeEditor(editor);
}
