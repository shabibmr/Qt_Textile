#include "gmledgerlinematerial.h"
#include "finance/masters/ledgers/addledger.h"
GMLedgerLineMaterial::GMLedgerLineMaterial(LedgerMasterDatabaseHelper *iHelper,
                                           QSqlQueryModel *m,
                                           LedgerMasterDataModel *led,
                                           QtMaterialTextField *parent)
    :QtMaterialTextField (parent)
{
    item = led;
    srcModel = m;
    ledHelper = iHelper;
    //    filterProxyModel = new QSortFilterProxyModel();
    this->setPlaceholderText("Search... ");

//    if(led!=Q_NULLPTR)
//        setText(led->LedgerName);

    this->setMinimumWidth(300);
    setSourceData();
}

GMLedgerLineMaterial::~GMLedgerLineMaterial()
{

}

void GMLedgerLineMaterial::setSrcModel(QSqlQueryModel *value)
{
    srcModel = value;
    setSourceData();
}

void GMLedgerLineMaterial::setItem(LedgerMasterDataModel *value)
{
    item = value;
    this->setText(item->LedgerName);
}

//void GMLedgerLineMaterial::setItem(const LedgerMasterDataModel* value)
//{
//    item = value;
//    this->setText(item.LedgerName);

//}

void GMLedgerLineMaterial::setNewText(QString text, QString id)
{
    this->setText(text);

}

void GMLedgerLineMaterial::sendPositiveEMIT(QModelIndex index, QString itemID)
{
    activated = false;
    qDebug()<<"selected ledger**********"<<itemID;
    *item = ledHelper->getLedgerObjectByID(itemID);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<item->LedgerID;
    this->update();
    emit itemSelected(index);
    emit selected();
    emit itemSelectedwithItem(*item);
//    event(new QEvent(QEvent::FocusOut));
}

bool GMLedgerLineMaterial::eventFilter(QObject *obj, QEvent *event)
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
            qDebug()<<"Focus out";
            this->setText(item->LedgerName);
            return  true;
//            QtMaterialTextField::eventFilter(obj,event);
            //                qDebug()<<"Focus out";
        }
        else{
            true;
        }

        return false;

}

void GMLedgerLineMaterial::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMLedgerLineMaterial::completerActivated(QModelIndex index)
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

void GMLedgerLineMaterial::finishEdit()
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
        qDebug()<<"FINISH COMPLETE at index (led)"<< gmcompleter->popup()->currentIndex().row();
    QString itemID = completer()->completionModel()->index(gmcompleter->popup()->currentIndex().row(), 0).data().toString();
    *item = ledHelper->getLedgerObjectByID(itemID);
    //    qDebug()<<"At Material led : "<<item->LedgerName;
    if(item->LedgerID.length()>0)
    {
        emit selected();
        emit itemSelectedwithItem(*item);
        this->clearFocus();
        this->setFocusPolicy(Qt::NoFocus);
        this->setFocusPolicy(Qt::StrongFocus);

    }
    else{
        this->setText("");
        activated = false;
    }
}

void GMLedgerLineMaterial::finishEditByRet()
{
    //    qDebug()<<"finish Edit";
    //    if(activated){
    //        activated = false;
    //        return;
    //    }
    //    else
    //        this->setText(oldText);

    emit itemSelectedwithItem(*item);
    this->clearFocus();
//    this->setFocusPolicy(Qt::NoFocus);
//    this->setFocusPolicy(Qt::StrongFocus);
    qDebug()<<"FINISH COMPLETE at index (led) Ret"<< gmcompleter->popup()->currentIndex().row();

}

void GMLedgerLineMaterial::createLedger()
{
    addLedger *addLed = new addLedger();
    addLed->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
//    addLed->setAttribute(Qt::WA_DeleteOnClose);
    addLed->setLedgerName(this->text());
    addLed->show();
    QObject::connect(addLed,&addLedger::closing,[=]{
        srcModel->query().exec();
    });

}

void GMLedgerLineMaterial::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);

    if(item->LedgerID.length()>0){
    QRect rect = opt.rect;
    p.drawText(rect,Qt::AlignRight|Qt::AlignVCenter,QString::number(item->closingBalance,'f',2));
    }
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QtMaterialTextField::paintEvent(event);
}

void GMLedgerLineMaterial::setSourceData()
{
    gmcompleter = new QCompleter(this);

    gmcompleter->setModel(srcModel);
    gmcompleter->setCompletionMode(QCompleter::PopupCompletion);
    gmcompleter->setFilterMode(Qt::MatchContains);
    gmcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    gmcompleter->setCompletionColumn(1);
    gmcompleter->popup()->setItemDelegate(new GMLedgerSearchDelegate());
    gmcompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    gmcompleter->popup()->installEventFilter(this);
    gmcompleter->popup()->setAlternatingRowColors(true);

    this->installEventFilter(this);

    QObject::connect(gmcompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivated(QModelIndex )));
    QObject::connect(gmcompleter,SIGNAL(highlighted(QModelIndex)),
                     this,SLOT(completerhighlighted(QModelIndex )));

    //    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT());

//    QObject::connect(this,&QLineEdit::editingFinished,
//                     this,&GMLedgerLineMaterial::finishEdit);

//    QObject::connect(this,&QLineEdit::returnPressed,
//                     this,&GMLedgerLineMaterial::finishEditByRet);

    this->setCompleter(gmcompleter);
}
