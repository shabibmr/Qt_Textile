#include "gmledgerlinewidget.h"

GMLedgerLineWidget::GMLedgerLineWidget(LedgerMasterDatabaseHelper *iHelper,QSqlQueryModel *m,QLineEdit *parent)
    :QLineEdit (parent)
{
    srcModel = m;
    ledHelper = iHelper;
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
    gmcompleter->popup()->setAlternatingRowColors(true);

    this->installEventFilter(this);

    QObject::connect(gmcompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivated(QModelIndex )));
    QObject::connect(gmcompleter,SIGNAL(highlighted(QModelIndex)),
                     this,SLOT(completerhighlighted(QModelIndex )));

    //    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT());

//    QObject::connect(this,&QLineEdit::editingFinished,
//                     this,&GMLedgerLineWidget::finishEdit);

//    QObject::connect(this,&QLineEdit::returnPressed,
//                     this,&GMLedgerLineWidget::finishEditByRet);



    this->setCompleter(gmcompleter);
}

GMLedgerLineWidget::~GMLedgerLineWidget()
{

}

void GMLedgerLineWidget::setNewText(QString text, QString id)
{

}

void GMLedgerLineWidget::sendPositiveEMIT(QModelIndex index, QString itemID)
{
    activated = false;
//    qDebug()<<"EMIT POSITIVE with "<<itemID;
    item = ledHelper->getLedgerObjectByID(itemID);
//    qDebug()<<"item is "<<item.LedgerName;
    emit itemSelected(index);
    emit itemSelectedwithItem(item);
}

void GMLedgerLineWidget::finishEditByRet()
{
    emit itemSelectedwithItem(item);
    qDebug()<<"FINISH COMPLETE By ret at index "<< gmcompleter->popup()->currentIndex().row()<<" item is "<<item.LedgerID;
}

void GMLedgerLineWidget::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMLedgerLineWidget::completerActivated(QModelIndex index)
{
    qDebug()<<"Completer activated";
    if(index.isValid()){
        qDebug()<<"Edit Finished text is "<<completer()->completionModel()->index(index.row(), 1).data().toString();
        qDebug()<<"Index is valid ***** ROW  "<<index.row();
        QString itemID = completer()->completionModel()->index(index.row(), 0).data().toString();
        activated = true;
        sendPositiveEMIT(index,itemID);
    }
    else{
        this->setText("");
        activated = false;
//        qDebug()<<"Index i s not valid";
    }
}

void GMLedgerLineWidget::finishEdit()
{
//    qDebug()<<"finish Edit";
//    if(activated){
//        activated = false;
//        return;
//    }
//    else
//        this->setText(oldText);

//    qDebug()<<"FINISH COMPLETE";
    emit itemSelectedwithItem(item);

    qDebug()<<"FINISH COMPLETE at index "<< gmcompleter->popup()->currentIndex().row();
}
