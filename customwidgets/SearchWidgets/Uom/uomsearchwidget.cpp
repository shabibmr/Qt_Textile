#include "uomsearchwidget.h"


GMUOMSearchWidget::GMUOMSearchWidget(UomDataBaseHelper *dbHelper, QSqlQueryModel *m, QLineEdit *parent)
    :QLineEdit (parent)
{
    srcModel = m;
    uomHelper = dbHelper;
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

    QObject::connect(this,&QLineEdit::editingFinished,
                     this,&GMUOMSearchWidget::finishEdit);
    QObject::connect(this,&QLineEdit::returnPressed,
                     this,&GMUOMSearchWidget::finishEditByRet);


    this->setCompleter(gmcompleter);
}

void GMUOMSearchWidget::setItem(const UomDataModel &value)
{
    uom = value;
    this->setText(uom.UomSymbol);
}

void GMUOMSearchWidget::sendPositiveEMIT(QModelIndex index, QString uomID)
{
    activated = false;
    qDebug()<<"EMIT POSITIVE with UOMID : "<<uomID;
    uom = uomHelper->getUomObjectByID(uomID);
    uom.convRate = completer()->completionModel()->index(index.row(), 2).data().toFloat();
    emit itemSelected(index);
    emit itemSelectedwithItem(uom);
}

void GMUOMSearchWidget::finishEditByRet()
{
    //    qDebug()<<"finish Edit";
    //    if(activated){
    //        activated = false;
    //        return;
    //    }
    //    else
    //        this->setText(oldText);

    emit itemSelectedwithItem(uom);

    qDebug()<<"FINISH COMPLETE at index "<< gmcompleter->popup()->currentIndex().row();

}

// bool GMUOMSearchWidget:: eventFilter(QObject *obj, QEvent *event)
//{
//    if (event->type() == QEvent::KeyPress)
//    {
//        if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return && obj == this->completer()->popup())
//        {

//            QAbstractItemView* l = static_cast<QAbstractItemView*>(obj);
//            QModelIndex i = l->model()->index(0,0);
//            if(i.isValid())
//            {
//                l->selectionModel()->select(i, QItemSelectionModel::Select);
//                QString itemID = completer()->completionModel()->index(i.row(), 0).data().toString();
//                activated= true;
//                sendPositiveEMIT(i,itemID);
//            }
//        }
//    }
//    else
//        if (event->type() == QEvent::FocusIn)
//        {
////                qDebug()<<"Focus in";
//            if (obj == this)
//            {
////                    qDebug()<<" SETTING OLD TEXT "<<this->text();
//                oldText = this->text();
//                activated=false;
//            }
//        }
//        else if( event->type() == QEvent::FocusOut){
//            this->setText(uom.UomSymbol);
////                qDebug()<<"Focus out";
//        }
//    return false;
//}


bool GMUOMSearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        if(static_cast<QKeyEvent *>(event)->key() == Qt::Key_Return)
            if( obj == this->completer()->popup()){
                qDebug()<<"Entered : "<<this->text();
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
            this->setText(uom.UomName);

            //                qDebug()<<"Focus out";
        }
    return false;
}

void GMUOMSearchWidget::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMUOMSearchWidget::completerActivated(QModelIndex index)
{
    if(index.isValid()){
                qDebug()<<"Edit Finished text is "<<completer()->completionModel()->index(index.row(), 1).data().toString();
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
}

void GMUOMSearchWidget::finishEdit()
{
//    qDebug()<<"Enter Clicked";
}
