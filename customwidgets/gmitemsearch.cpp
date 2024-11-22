#include "gmitemsearch.h"


GMItemSearch::GMItemSearch(QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr,QLineEdit *parent):QLineEdit(parent)
{
    //qDebug()<<"Search item constructor begins"<<QDateTime::currentDateTime().toString();

    itemsHelper = new SalesInventoryItemDatabaseHelper();
  //  items = itemsHelper->getALLInventoryObjects();
  //  qDebug()<<items.size();
    for(int i=0;i<itemsListPtr.size();i++){
        list.append(itemsListPtr[i]->ItemName);
        list.append(itemsListPtr[i]->ItemAlias);
//        qDebug()<<itemsListPtr[i]->ItemName;
    }
   // qDebug()<<"list append completed"<<QDateTime::currentDateTime().toString();

    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText("Search Here..");
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(checkEdit()));
    this->setFocusPolicy(Qt::StrongFocus);

    listWidget =0;
    listWidget = new GMItemSearchWidget();
    //qDebug()<<"set data"<<QDateTime::currentDateTime().toString();

    listWidget->setData(itemsListPtr);
    //qDebug()<<"st data end"<<QDateTime::currentDateTime().toString();

    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );
}

GMItemSearch::~GMItemSearch()
{

}

void GMItemSearch::textClicked()
{

}

void GMItemSearch::checkEdit()
{
    if(this->text().compare("+")==0){

        qDebug()<<"plus button 47";
        emit plusButton();
        this->setFocus();
        listWidget->hide();
        this->setText("");
    }
    else if(this->text().compare("-")==0){

        qDebug()<<"plus button 47";
        emit minusButton();
        this->setFocus();
        listWidget->hide();
        this->setText("");
    }
    else if(this->text().compare("*")==0){

        qDebug()<<"plus button 47";
        emit astriskpressed();
        this->setFocus();
        listWidget->hide();
        this->setText("");
    }
    if(!list.contains(this->text(),Qt::CaseInsensitive))
        this->setText("");
}

void GMItemSearch::editTextChanged(QString text)
{
    //listWidget->filterData(text);
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
        QObject::connect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));

    }
}



void GMItemSearch::itemSelected(QString id,QString value)
{
    //qDebug()<<"at 75 "<<id;
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));
    listWidget->setHidden(true);
    this->setFocus();
    emit SelectedItemID(itemsHelper->getInventoryItemByID(id));
}

void GMItemSearch::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();
}

void GMItemSearch::SOSelectedSlot(QString Vno)
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));
    listWidget->setHidden(true);
    this->setFocus();
    emit SalesOrderSelected(Vno);
}
