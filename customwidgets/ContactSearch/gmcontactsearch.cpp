#include "gmcontactsearch.h"


GMContactSearch::GMContactSearch(QList<QSharedPointer<AddressBookSmall> > itemsListPtr, QLineEdit *parent)
{
    itemsHelper = new AddressBookDatabaseHelper();
  //  items = itemsHelper->getALLInventoryObjects();
  //  qDebug()<<items.size();
    for(int i=0;i<itemsListPtr.size();i++){
        list.append(itemsListPtr[i]->CompanyName);
    }
   // qDebug()<<"list append completed"<<QDateTime::currentDateTime().toString();

    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText("Search Here..");
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(returnPressed()),this,SLOT(checkEdit()));
    this->setFocusPolicy(Qt::StrongFocus);

    listWidget =0;
    listWidget = new GMContactSearchWidget();
    //qDebug()<<"set data"<<QDateTime::currentDateTime().toString();

    listWidget->setData(itemsListPtr);
    //qDebug()<<"st data end"<<QDateTime::currentDateTime().toString();

    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );
    this->setMinimumWidth(300);
}

GMContactSearch::~GMContactSearch()
{

}


void GMContactSearch::checkEdit()
{
    if(!list.contains(this->text(),Qt::CaseInsensitive))
        this->setText("");
}

void GMContactSearch::editTextChanged(QString text)
{
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedID(int,QString)),this,SLOT(itemSelected(int,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    }
}

void GMContactSearch::itemSelected(int id, QString value)
{
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(int,QString)),this,SLOT(itemSelected(int,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->setHidden(true);
    this->setFocus();
    emit SelectedItemID(itemsHelper->getContact(id));
}

void GMContactSearch::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(int,QString)),this,SLOT(itemSelected(int,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();
}
