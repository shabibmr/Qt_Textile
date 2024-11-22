#include "gmpayslipdelegate.h"
#include <QApplication>

GMPayslipDelegate::~GMPayslipDelegate()
{

}

GMPayslipDelegate::GMPayslipDelegate(QSqlQueryModel *model)
{
    this->model = model;
//    this->itemHelper = iHelper;
}

void GMPayslipDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QRect rect = option.rect;
//    if(index.column()==1 && index.row()== index.model()->rowCount()-1){
//        //        qDebug()<<"Painting last elem";
//        QFont italic;
//        italic.setItalic(true);
//        painter->setFont(italic);
//        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

//        if (option.state & QStyle::State_Selected)
//            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
//        else
//            painter->setPen(option.palette.color(cg, QPalette::Text));

//        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-3, rect.height()),
//                          Qt::AlignLeft|Qt::AlignVCenter, "Search...");
//    }
    if(index.column() ==6)
    {
        bool enabled = true;
                QStyleOptionViewItem opt = option;
                QRect crect = opt.rect;
                crect.moveLeft(15);
                 QApplication::style()->drawControl(QStyle::CE_CheckBox, &opt, painter);
                //drawCheck(painter, opt, crect, enabled ? Qt::Checked : Qt::Unchecked);
    }
//    else
        QStyledItemDelegate::paint(painter,option,index);
}

QWidget *GMPayslipDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {

    case 14:
    {

        QtMaterialIconButton* button = new QtMaterialIconButton(QtMaterialTheme::icon("action", "payment"));
        button->setProperty("id",index.row());
        button->setEnabled(true);

//        connect(button,&QtMaterialIconButton::clicked,
//                this,&GMPayslipDelegate::deleteButtonClicked);
        return button;

        break;


    }

    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void GMPayslipDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //    qDebug()<<"Call Set EDITOR Data col : "<<index.column();
    switch (index.column()) {
        case 14:
        {
        QtMaterialIconButton* button = qobject_cast<QtMaterialIconButton*>(editor);
        button->setIcon(QtMaterialTheme::icon("action", "payment"));

            break;
        }

    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void GMPayslipDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //    qDebug()<<".........................Set MOdel Data";
    switch (index.column()) {
    case 14:
    {
        QtMaterialIconButton* button = qobject_cast<QtMaterialIconButton*>(editor);
        button->setIcon(QtMaterialTheme::icon("action", "payment"));
        model->setData(index,QVariant::fromValue(index.row()));
        return;
    }        // FR -- run with start time, add qdebug
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void GMPayslipDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void GMPayslipDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    //    qDebug()<<".............................COMMITTING DATA...........................";
//    GMLedgerLineWidget *editor = qobject_cast<GMLedgerLineWidget *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
//    emit commitData(editor);
//    emit closeEditor(editor);
}
