#include "gminventoryitemsearch.h"
#include <QMessageBox>
#include <sharetools/sharetools.h>
#include <QVBoxLayout>

GMInventoryItemSearch::GMInventoryItemSearch(SalesInventoryItemDatabaseHelper *iHelper,
                                             QSqlQueryModel *m, QString priceLisId,
                                             QLineEdit *parent)
    :QLineEdit (parent)
{
    srcModel = m;
    itemHelper = iHelper;
    this->priceListId = priceLisId;
    filterProxyModel = new QSortFilterProxyModel();
    this->setPlaceholderText("Search... ");
    gmcompleter = new QCompleter(this);
    gmcompleter->setModel(srcModel);
    gmcompleter->setCompletionMode(QCompleter::PopupCompletion);
    gmcompleter->setFilterMode(Qt::MatchContains);
    gmcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    gmcompleter->setCompletionColumn(1);
    gmcompleter->popup()->setItemDelegate(new ItemSearchDelegate());
    gmcompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    gmcompleter->popup()->installEventFilter(this);

    this->installEventFilter(this);

    QObject::connect(gmcompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivated(QModelIndex )));
    QObject::connect(gmcompleter,SIGNAL(highlighted(QModelIndex)),
                     this,SLOT(completerhighlighted(QModelIndex )));

    //    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT());

    QObject::connect(this,&QLineEdit::editingFinished,
                     this,&GMInventoryItemSearch::finishEdit);
    QObject::connect(this,&QLineEdit::returnPressed,
                     this,&GMInventoryItemSearch::finishEditByRet);


    this->setCompleter(gmcompleter);
}

GMInventoryItemSearch::~GMInventoryItemSearch()
{
    //Destroy;
}

void GMInventoryItemSearch::setItem(const inventoryItemDataModel &value)
{
    item = value;
    this->setText(item.ItemName);
}

void GMInventoryItemSearch::createItem()
{
    QString itemName = this->text();
    AddItems *additemWidget= new AddItems(this);
    additemWidget->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
    additemWidget->setAttribute(Qt::WA_DeleteOnClose);
    additemWidget->setItemCode(itemName);
    additemWidget->show();
    QObject::connect(additemWidget,&AddItems::savedItem,this,[=](CompoundItemDataObject item){
        srcModel->query().exec();
    });
}


void GMInventoryItemSearch::sendPositiveEMIT(QModelIndex index,QString itemID)
{
    activated = false;
    qDebug()<<"item search EMIT POSITIVE with "<<itemID<<priceListId;
    item = itemHelper->getInventoryItemByID(itemID, true, QDate::currentDate(), priceListId);
    //    qDebug()<<"item is "<<item.ItemName;

    if(item.isSerailNumbered){
//        auto dialog = new QDialog(this,Qt::Window);
//        ShareTools *st = new ShareTools();
//        QLayout *lay = new QVBoxLayout(st);
//        lay->addWidget(st);
//        dialog->setLayout(lay);
//        QObject::connect(st,&ShareTools::pdfClicked,this,[=]{
//            item.quantity = 99;
//            emitSelectedVals(index,&item);
//        });
//        dialog->show();

        emitSelectedVals(index,&item);
    }
    else{
        emitSelectedVals(index,&item);
    }

}

void GMInventoryItemSearch::emitSelectedVals(QModelIndex index, inventoryItemDataModel *item)
{
    emit itemSelected(index);
    emit itemSelectedwithItem(*item);
}



bool GMInventoryItemSearch::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return || static_cast<QKeyEvent *>(event)->key() == Qt::Key_Enter)
            if(obj == this){
                //                qDebug()<<"Entered : "<<this->text();
                if(this->completer()->popup()->model()->rowCount()==0 ){
                    qDebug()<<"None appears check barcode";
                    QString itemID = itemHelper->getInventoryItemByBarcode(this->text()).BaseItem.ItemID;
                    activated= true;
                    sendPositiveEMIT(QModelIndex(),itemID);
                }
            }
            else if( obj == this->completer()->popup()){
                if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_F1){
                               qDebug()<<("F1 Pressed");
                               createItem();
                           }
                //                qDebug()<<"Entered : "<<this->text();
                if(this->completer()->popup()->model()->rowCount()==1 ){
                    qDebug()<<"1 appears ";
                    QAbstractItemView* l = static_cast<QAbstractItemView*>(obj);
                    QModelIndex i = l->model()->index(0,0);
                    if(i.isValid())
                    {
                        l->selectionModel()->select(i, QItemSelectionModel::Select);
                        QString itemID = completer()->completionModel()->index(i.row(), 0).data().toString();
                        activated= true;
                        sendPositiveEMIT(i,itemID);
                    }
                }
            }
            else if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_F1){
                qDebug()<<("F1 Pressed");
                createItem();
            }

        //            if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return && obj == this->completer()->popup())
        //            {
        //                QAbstractItemView* l = static_cast<QAbstractItemView*>(obj);
        //                QModelIndex i = l->model()->index(0,0);
        //                if(i.isValid())
        //                {
        //                    l->selectionModel()->select(i, QItemSelectionModel::Select);
        //                    QString itemID = completer()->completionModel()->index(i.row(), 0).data().toString();
        //                    activated= true;
        //                    sendPositiveEMIT(i,itemID);
        //                }
        //            }

    }
    else
        if (event->type() == QEvent::FocusIn)
        {
            //                qDebug()<<"Focus in";
            if (obj == this)
            {
                //                  qDebug()<<" SETTING OLD TEXT "<<this->text();
                oldText = this->text();
                this->completer()->complete();
                activated=false;
            }
        }
        else if( event->type() == QEvent::FocusOut){
            this->setText(item.ItemName);

            //                qDebug()<<"Focus out";
        }
    return false;
}

void GMInventoryItemSearch::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMInventoryItemSearch::completerActivated(QModelIndex index)
{
    if(index.isValid()){
        //        qDebug()<<"Edit Finished text is "<<completer()->completionModel()->index(index.row(), 1).data().toString();
        //        qDebug()<<"Index is valid ***** ROW  "<<index.row();
        QString itemID = completer()->completionModel()->index(index.row(), 0).data().toString();
        activated = true;
        sendPositiveEMIT(index,itemID);
    }
    else{
        this->setText("");
        activated = false;
        //        qDebug()<<"Index is not valid";
    }
    //    qDebug()<<"============================Completer Activated COMPLETE";

}

void GMInventoryItemSearch::finishEditByRet()
{

    qDebug()<<item.price;
    emit itemSelectedwithItem(item);
//    qDebug()<<"FINISH COMPLETE at index (item search)"<< gmcompleter->popup()->currentIndex().row();

}


void GMInventoryItemSearch::finishEdit()
{

    QModelIndex index ;//= completer()->completionModel()->
    QString itemID = completer()->completionModel()->index(index.row(), 0).data().toString();

}

void GMInventoryItemSearch::openCompleter()
{
    completer()->complete();
}



void GMInventoryItemSearch::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QLineEdit::paintEvent(event);
}
