#include "gmattendancedelegate.h"
#include <QApplication>

GMAttendanceDelegate::~GMAttendanceDelegate()
{

}

GMAttendanceDelegate::GMAttendanceDelegate(UserProfileDatabaseHelper *iHelper,
                                                         QSqlQueryModel *model)
{
    this->model = model;
    this->itemHelper = iHelper;
}

void GMAttendanceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

QWidget *GMAttendanceDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
//    case 1 :
//    {
//        GMEmployeeSearchWidget *itemLine = new GMEmployeeSearchWidget(itemHelper,model);
//        itemLine->setFrame(false);
//        itemLine->setParent(parent);
//        QObject::connect(itemLine, &GMEmployeeSearchWidget::itemSelected,
//                         this, &GMAttendanceDelegate::commitAndCloseItemEditor);
//        return itemLine;
//    }
    case 2:
    {

        QDateTimeEdit *startTimeEdit = new QDateTimeEdit;
        startTimeEdit->setDateTime(QDateTime::currentDateTime());
        startTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
        startTimeEdit->setParent(parent);
        startTimeEdit->setFrame(false);

//        QObject::connect(startTimeEdit, &QDateTimeEdit::dateTimeChanged,
//                                 this, &GMAttendanceDelegate::commitAndCloseItemEditor);
        return startTimeEdit;


    }

    case 6:
    {

        QCheckBox *halfDayEdit = new QCheckBox;
        halfDayEdit->setParent(parent);

//        QObject::connect(startTimeEdit, &QDateTimeEdit::dateTimeChanged,
//                                 this, &GMAttendanceDelegate::commitAndCloseItemEditor);
        return halfDayEdit;


    }
    case 3:
    {

        QDateTimeEdit *endTimeEdit = new QDateTimeEdit;
        endTimeEdit->setDateTime(QDateTime::currentDateTime()) ;
        endTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
        endTimeEdit->setParent(parent);
        endTimeEdit->setFrame(false);

//        QObject::connect(startTimeEdit, &QDateTimeEdit::dateTimeChanged,
//                                 this, &GMAttendanceDelegate::commitAndCloseItemEditor);
        return endTimeEdit;


    }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void GMAttendanceDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //    qDebug()<<"Call Set EDITOR Data col : "<<index.column();
    switch (index.column()) {

    case 2:
    {
        QDateTimeEdit *startTimeEdit = qobject_cast<QDateTimeEdit *>(editor);

        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        if(startTimeEdit->dateTime().isValid())
            model->setData(index, QVariant::fromValue(startTimeEdit->dateTime()));
        return;
    }

    case 3:
    {
        QDateTimeEdit *endTimeEdit = qobject_cast<QDateTimeEdit *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        qDebug()<<"end time"<<endTimeEdit->dateTime();
        if(endTimeEdit->dateTime().isValid())
            model->setData(index, QVariant::fromValue(endTimeEdit->dateTime()));
        return;
    }

    case 6:
    {
        QCheckBox *halfDayEdit = qobject_cast<QCheckBox *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        if(halfDayEdit->isEnabled())
            model->setData(index, QVariant::fromValue(halfDayEdit->isChecked()));
        return;
    }
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void GMAttendanceDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //    qDebug()<<".........................Set MOdel Data";
    switch (index.column()) {
//    case 1:
//    {
//        GMEmployeeSearchWidget *itemLine = qobject_cast<GMEmployeeSearchWidget *>(editor);
//        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
//        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
//        if(itemLine->item.Employee_ID.length())
//            model->setData(index, QVariant::fromValue(itemLine->item));
//        return;
//    }

    case 2:
    {
        QDateTimeEdit *startTimeEdit = qobject_cast<QDateTimeEdit *>(editor);

        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        if(startTimeEdit->dateTime().isValid())
            model->setData(index, QVariant::fromValue(startTimeEdit->dateTime()));
        return;
    }

    case 3:
    {
        QDateTimeEdit *endTimeEdit = qobject_cast<QDateTimeEdit *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        qDebug()<<"end time"<<endTimeEdit->dateTime();
        if(endTimeEdit->dateTime().isValid())
            model->setData(index, QVariant::fromValue(endTimeEdit->dateTime()));
        return;
    }

    case 6:
    {
        QCheckBox *halfDayEdit = qobject_cast<QCheckBox *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        if(halfDayEdit->isEnabled())
            model->setData(index, QVariant::fromValue(halfDayEdit->isChecked()));
        return;
    }
        // FR -- run with start time, add qdebug
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void GMAttendanceDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void GMAttendanceDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    //    qDebug()<<".............................COMMITTING DATA...........................";
//    GMLedgerLineWidget *editor = qobject_cast<GMLedgerLineWidget *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
//    emit commitData(editor);
//    emit closeEditor(editor);
}
