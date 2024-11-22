#include "gmbankreconciliationdelegate.h"
#include <QtGui>
#include <QApplication>
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

#include <QDateEdit>
#include <QCheckBox>
#include "customwidgets/Material/components/qtmaterialautocomplete.h"

GMBankReconciliationDelegate::GMBankReconciliationDelegate( QSqlQueryModel *model)
{
    this->model = model;

}

GMBankReconciliationDelegate::~GMBankReconciliationDelegate()
{

}

QWidget *GMBankReconciliationDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    switch (index.column())
    {

    case  9:
    {
        QDateEdit *bankDateEdit = new QDateEdit;
        bankDateEdit->setDate(QDate::currentDate());
        bankDateEdit->setDisplayFormat("yyyy-MM-dd");
        bankDateEdit->setParent(parent);
        bankDateEdit->setFrame(false);

        return bankDateEdit;
    }
    case 8 :
    {
        QtMaterialAutoComplete *status = new QtMaterialAutoComplete;
        QStringList statusList; statusList<<"Presented"<<"Cleared"<<"Rejected";
        status->setDataSource(statusList);
        return status;
    }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void GMBankReconciliationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    switch (index.column()) {
    case 9:
    {
        QDate date = index.model()->data(index, Qt::UserRole).toDate();

        QDateEdit *bankDateEdit = new QDateEdit;
        bankDateEdit->setDate(date);
        bankDateEdit->setFrame(false);
        break;
    }
    case 8 :
    {
        QString statusVal = index.model()->data(index, Qt::UserRole).toString();
        QtMaterialAutoComplete *status = new QtMaterialAutoComplete;
        QStringList statusList; statusList<<"Presented"<<"Cleared"<<"Rejected";
        status->setDataSource(statusList);
        status->setText(statusVal);
        break;
    }
    }

    QStyledItemDelegate::setEditorData(editor, index);

}

void GMBankReconciliationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    switch (index.column()) {
    case 8:
    {
        QDateEdit *bankDateEdit = qobject_cast<QDateEdit *>(editor);

        if(bankDateEdit->date().isValid())
            model->setData(index, QVariant::fromValue(bankDateEdit->date()));
        return;
    }

    case 9:
    {
        QtMaterialAutoComplete *status = qobject_cast<QtMaterialAutoComplete *>(editor);

        if(status->text().length() > 0)
            model->setData(index, QVariant::fromValue(status->text()));
        return;
    }


    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void GMBankReconciliationDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void GMBankReconciliationDelegate::commitAndCloseItemEditor(QModelIndex index)
{

    GMInventoryItemSearch *editor = qobject_cast<GMInventoryItemSearch *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}

void GMBankReconciliationDelegate::commitAndCloseUOMEditor(QModelIndex index)
{

    GMUOMSearchWidget *editor = qobject_cast<GMUOMSearchWidget *>(sender());
//    qDebug()<<"uom Set as from Commitor"<<editor->uom.UomName;
    emit commitData(editor);
    emit closeEditor(editor);
}
