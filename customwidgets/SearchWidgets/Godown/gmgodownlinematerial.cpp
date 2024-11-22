#include "gmgodownlinematerial.h"



GMGodownLineMaterial::GMGodownLineMaterial(GodownDatabaseHelper *iHelper,
                                           QSqlQueryModel *m,
                                           QString *led,
                                           QtMaterialTextField *parent)
    :QtMaterialTextField (parent)
{
    godown = led;
    srcModel = m;
    dbHelper = iHelper;
    //    filterProxyModel = new QSortFilterProxyModel();
    if(godown!=nullptr){
        setText(dbHelper->getGodownNameById(*godown));
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
                     this,&GMGodownLineMaterial::finishEdit);

    QObject::connect(this,&QLineEdit::returnPressed,
                     this,&GMGodownLineMaterial::finishEditByRet);

    this->setCompleter(gmcompleter);
}

GMGodownLineMaterial::~GMGodownLineMaterial()
{

}

void GMGodownLineMaterial::setGodown(QString *value)
{
    godown = value;

    QString godownId = *godown;
    if(godownId.length() == 0)
        *godown = (dbHelper->getDefaultGodown());
    this->setText(dbHelper->getGodownNameById(*godown));
}

//void GMGodownLineMaterial::setItem(const LedgerMasterDataModel* value)
//{
//    item = value;
//    this->setText(item.LedgerName);

//}

void GMGodownLineMaterial::setNewText(QString text, QString id)
{
    this->setText(text);

}

void GMGodownLineMaterial::sendPositiveEMIT(QModelIndex index, QString itemID)
{
    activated = false;
    qDebug()<<"selected ledger**********"<<itemID;
    *godown = itemID;
    emit itemSelected(index);
    emit selected();
    emit itemSelectedwithItem(*godown);
    this->clearFocus();
}

bool GMGodownLineMaterial::eventFilter(QObject *obj, QEvent *event)
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
            this->setText(dbHelper->getGodownNameById(*godown));
           return true;
            //                qDebug()<<"Focus out";
        }
        else{
           QtMaterialTextField::eventFilter(obj,event);
        }

        return false;

}

void GMGodownLineMaterial::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMGodownLineMaterial::completerActivated(QModelIndex index)
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

void GMGodownLineMaterial::finishEdit()
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
    *godown = itemID;
    //    qDebug()<<"At Material led : "<<item->LedgerName;

}

void GMGodownLineMaterial::finishEditByRet()
{
    //    qDebug()<<"finish Edit";
    //    if(activated){
    //        activated = false;
    //        return;
    //    }
    //    else
    //        this->setText(oldText);

    emit itemSelectedwithItem(*godown);
    this->clearFocus();
    this->setFocusPolicy(Qt::NoFocus);
    this->setFocusPolicy(Qt::StrongFocus);
    //    qDebug()<<"FINISH COMPLETE at index (led) Ret"<< gmcompleter->popup()->currentIndex().row();

}

void GMGodownLineMaterial::createLedger()
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

void GMGodownLineMaterial::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QtMaterialTextField::paintEvent(event);
}

