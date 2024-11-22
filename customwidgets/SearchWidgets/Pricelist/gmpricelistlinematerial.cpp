#include "gmpricelistlinematerial.h"



GMPriceListLineMaterial::GMPriceListLineMaterial(PricelistDatabaseHelper *iHelper,
                                           QSqlQueryModel *m,
                                           QString *led,
                                           QtMaterialTextField *parent)
    :QtMaterialTextField (parent)
{
    priceListId = led;
    srcModel = m;
    dbHelper = iHelper;
    //    filterProxyModel = new QSortFilterProxyModel();
    if(priceListId!=nullptr){
        setText(dbHelper->getPriceListNameByID(*priceListId));
    }
    this->setPlaceholderText("Search... ");


    this->setMinimumWidth(300);
    gmcompleter = new QCompleter(this);

    gmcompleter->setModel(srcModel);
    gmcompleter->setCompletionMode(QCompleter::PopupCompletion);
    gmcompleter->setFilterMode(Qt::MatchContains);
    gmcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    gmcompleter->setCompletionColumn(1);
    gmcompleter->popup()->setItemDelegate(new ItemSearchDelegate());
    gmcompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    gmcompleter->popup()->installEventFilter(this);
    gmcompleter->popup()->setAlternatingRowColors(true);

    this->installEventFilter(this);

    QObject::connect(gmcompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivated(QModelIndex )));
    QObject::connect(gmcompleter,SIGNAL(highlighted(QModelIndex)),
                     this,SLOT(completerhighlighted(QModelIndex )));

    //    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT());

    QObject::connect(this,&QLineEdit::editingFinished,
                     this,&GMPriceListLineMaterial::finishEdit);

    QObject::connect(this,&QLineEdit::returnPressed,
                     this,&GMPriceListLineMaterial::finishEditByRet);

    this->setCompleter(gmcompleter);
}

GMPriceListLineMaterial::~GMPriceListLineMaterial()
{

}

void GMPriceListLineMaterial::setPriceListId(QString *value)
{
    priceListId = value;
    this->setText(dbHelper->getPriceListNameByID(*value));
}

//void GMPriceListLineMaterial::setItem(const LedgerMasterDataModel* value)
//{
//    item = value;
//    this->setText(item.LedgerName);

//}

void GMPriceListLineMaterial::setNewText(QString text, QString id)
{
    this->setText(text);

}

void GMPriceListLineMaterial::sendPositiveEMIT(QModelIndex index, QString itemID)
{
    qDebug()<<Q_FUNC_INFO<<itemID<<index;
    activated = false;
    qDebug()<<"selected price list**********"<<itemID;
//    *(item) = itemID;
    priceListId = &itemID;
    qDebug()<<"1";
    emit itemSelected(index);
    qDebug()<<"2";
    emit selected();
    qDebug()<<"emiting item"<<itemID;
    emit itemSelectedwithItem(*priceListId);
    this->clearFocus();
}

bool GMPriceListLineMaterial::eventFilter(QObject *obj, QEvent *event)
{

        if (event->type() == QEvent::KeyPress)
        {
            if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return && obj == this->completer()->popup())
            {

                QAbstractItemView* l = static_cast<QAbstractItemView*>(obj);
                QModelIndex i = l->model()->index(0,0);
                if(i.isValid())
                {
                    l->selectionModel()->select(i, QItemSelectionModel::Select);
                    QString itemID = completer()->completionModel()->index(i.row(), 0).data().toString();
                    activated= true;
                    qDebug()<<"Item Pressed at "<<i.row()<<"item id "<<itemID;
                    sendPositiveEMIT(i,itemID);
                }
            }
            else if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_F1){
                createLedger();
            }
        }
        else if (event->type() == QEvent::FocusIn)
        {
            //                qDebug()<<"Focus in";
            if (obj == this)
            {
                //                  qDebug()<<" SETTING OLD TEXT "<<this->text();
                oldText = this->text();
                //                    this->completer()->complete();
                activated=false;
            }
        }
        else if( event->type() == QEvent::FocusOut){
            this->setText(dbHelper->getPriceListNameByID(*priceListId));
           return true;
            //                qDebug()<<"Focus out";
        }
        else{
           QtMaterialTextField::eventFilter(obj,event);
        }

        return false;

}

void GMPriceListLineMaterial::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMPriceListLineMaterial::completerActivated(QModelIndex index)
{
    if(index.isValid()){
        qDebug()<<"Edit Finished text is "<<completer()->completionModel()->index(index.row(), 1).data().toString();
        qDebug()<<"Index is valid ***** ROW  "<<index.row();
        QString itemID = completer()->completionModel()->index(index.row(), 0).data().toString();
        activated = true;
        qDebug()<<"87";
        sendPositiveEMIT(index,itemID);
    }
    else{
        this->setText("");
        activated = false;
        qDebug()<<"Index is not valid";
    }
}

void GMPriceListLineMaterial::finishEdit()
{
    //    qDebug()<<"finish Edit";
    //    if(activated){
    //        activated = false;
    //        return;
    //    }
    //    else
    //        this->setText(oldText);
    //    emit itemSelectedwithItem(item);
    //    sendPositiveEMIT(gmcompleter->popup()->currentIndex(),
    //                     completer()->completionModel()->index(gmcompleter->popup()->currentIndex().row(), 0).data().toString());
    //    qDebug()<<"FINISH COMPLETE at index (led)"<< gmcompleter->popup()->currentIndex().row();
    QString itemID = completer()->completionModel()->index(gmcompleter->popup()->currentIndex().row(), 0).data().toString();
    *priceListId = itemID;
    //    qDebug()<<"At Material led : "<<item->LedgerName;

}

void GMPriceListLineMaterial::finishEditByRet()
{
    //    qDebug()<<"finish Edit";
    //    if(activated){
    //        activated = false;
    //        return;
    //    }
    //    else
    //        this->setText(oldText);

    emit itemSelectedwithItem(*priceListId);
    this->clearFocus();
    this->setFocusPolicy(Qt::NoFocus);
    this->setFocusPolicy(Qt::StrongFocus);
    //    qDebug()<<"FINISH COMPLETE at index (led) Ret"<< gmcompleter->popup()->currentIndex().row();

}

void GMPriceListLineMaterial::createLedger()
{
    //    addLedger *addLed = new addLedger();
    //    addLed->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
    //    addLed->setAttribute(Qt::WA_DeleteOnClose);
    //    addLed->setLedgerName(this->text());
    //    addLed->show();

    //    QObject::connect(addLed,&addLedger::closing,[=]{
    //        srcModel->query().exec();
    //    });


}

void GMPriceListLineMaterial::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QtMaterialTextField::paintEvent(event);
}
