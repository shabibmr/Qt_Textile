#include "gmledgersearchlite.h"

GMLedgerSearchLite::GMLedgerSearchLite(QSqlQueryModel *model1,QLineEdit *parent)
    :QLineEdit(parent)
{
    qDebug()<<"GMLedgerSearchLite";
    ledHelper = new LedgerMasterDatabaseHelper();
    accHelper = new AccountGroupMasterDatabaseHelper();

    itemsModel = model1;
    for(int i=0;i<itemsModel->rowCount();i++){
        list.append(itemsModel->record(i).value(1).toString());
    }
    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText("Search Here..");
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(checkEdit()));
    this->setFocusPolicy(Qt::StrongFocus);

    listWidget =0;
    listWidget = new GMLedgerSearchListWidgetLite(itemsModel);
    qDebug()<<Q_FUNC_INFO<<"23";
    listWidget->setData(itemsModel);
    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );

}

void GMLedgerSearchLite::setItemsModel(QSqlQueryModel *value)
{
    itemsModel = value;
}

void GMLedgerSearchLite::setAccountsFlag(bool value)
{
    accountsFlag = value;
}

void GMLedgerSearchLite::checkEdit()
{
    if(!list.contains(this->text(),Qt::CaseInsensitive))
        this->setText("");

}

void GMLedgerSearchLite::editTextChanged(QString text)
{
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    }

}

void GMLedgerSearchLite::itemSelected(QString id, QString value)
{
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->setHidden(true);
    this->setFocus();
    qDebug()<<"Selected item : "<<value;
    if(!accountsFlag)
    {emit selectedLedger(ledHelper->getLedgerObjectByID(id)); qDebug()<<"EMIT DONE";}
    else
        emit SelectedAccount(accHelper->getGroupById(id));

}

void GMLedgerSearchLite::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();

}
