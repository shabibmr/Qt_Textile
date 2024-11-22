#include "gmitemsearchlite.h"

GMItemSearchLite::GMItemSearchLite(QSqlQueryModel *model1,QLineEdit *parent)
    :QLineEdit(parent)

{
    itemsHelper = new SalesInventoryItemDatabaseHelper();
    //  items = itemsHelper->getALLInventoryObjects();
    //  qDebug()<<items.size();
    //    for(int i=0;i<itemsListPtr.size();i++){
    //        list.append(itemsListPtr[i]->ItemName);
    //    }
    // qDebug()<<"list append completed"<<QDateTime::currentDateTime().toString();

    itemsModel = model1;
    for(int i=0;i<itemsModel->rowCount();i++){
        list.append(itemsModel->record(i).value(1).toString());
    }

    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText("Search Here..");
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(returnPressed()),this,SLOT(checkEdit()));
    this->setFocusPolicy(Qt::StrongFocus);
    listWidget =0;
    listWidget = new GMItemSearchListWidgetLite(itemsModel);
    //qDebug()<<"set data"<<QDateTime::currentDateTime().toString();

    listWidget->setData(itemsModel);
    //qDebug()<<"st data end"<<QDateTime::currentDateTime().toString();

    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );
}

void GMItemSearchLite::checkEdit()
{
    qDebug()<<">>>>>Running :"<<this->text()<<Q_FUNC_INFO;

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

        if(this->text().startsWith("99")){
            qDebug()<<"text continues"<<this->text();
            return;
        }
    if(!list.contains(this->text(),Qt::CaseInsensitive) && false)
        this->setText("");
}

void GMItemSearchLite::setGroupFlag(bool value)
{
    accflag = value;
}

void GMItemSearchLite::editTextChanged(QString text)
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
        QObject::connect(listWidget,SIGNAL(selectedItembyBarcode(inventoryItemDataModel)),this,SLOT(itemCustomBarcodeSelected(inventoryItemDataModel )));
        QObject::connect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));

    }
}

void GMItemSearchLite::itemCustomBarcodeSelected(inventoryItemDataModel item)
{
    qDebug()<<"()()()()(()()()()()()()()()()(()() Emit via Single"<<Q_FUNC_INFO;
    this->setText(item.ItemName);
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(selectedItembyBarcode(inventoryItemDataModel)),this,SLOT(itemCustomBarcodeSelected(inventoryItemDataModel )));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));
    listWidget->setHidden(true);
    this->setFocus();
    qDebug()<<"Selected item : "<<item.ItemName<<item.ItemCode;
    emit SelectedItemID(item);
    QObject::connect(listWidget,SIGNAL(selectedItembyBarcode(inventoryItemDataModel)),this,SLOT(itemCustomBarcodeSelected(inventoryItemDataModel )));


}


void GMItemSearchLite::itemSelected(QString id, QString value)
{
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(selectedItembyBarcode(inventoryItemDataModel)),this,SLOT(itemCustomBarcodeSelected(inventoryItemDataModel )));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));
    listWidget->setHidden(true);
    this->setFocus();
    if(accflag == false){
        item = itemsHelper->getInventoryItemByID(id);
        emit SelectedItemID(item);
        qDebug()<<"item is Set Calling emit";
        emit ItemSet();
    }
    else
    {
        emit SelectedGroup(id);
    }
}

void GMItemSearchLite::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(selectedItembyBarcode(inventoryItemDataModel)),this,SLOT(itemCustomBarcodeSelected(inventoryItemDataModel )));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();
}

void GMItemSearchLite::SOSelectedSlot(QString Vno)
{

    qDebug()<<" V no : "<<Vno;
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    QObject::disconnect(listWidget,SIGNAL(selectedItembyBarcode(inventoryItemDataModel)),this,SLOT(itemCustomBarcodeSelected(inventoryItemDataModel )));
    QObject::disconnect(listWidget,SIGNAL(SOSelectedSig(QString)),this,SLOT(SOSelectedSlot(QString)));

    listWidget->setHidden(true);
    this->setFocus();
    emit SalesOrderSelectedSignal(Vno);
}
