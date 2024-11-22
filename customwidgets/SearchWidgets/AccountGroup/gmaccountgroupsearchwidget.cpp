#include "gmaccountgroupsearchwidget.h"

GMAccountGroupSearchWidget::GMAccountGroupSearchWidget
(AccountGroupMasterDatabaseHelper *dbHelper, QSqlQueryModel *m, QLineEdit *parent)
    :QLineEdit (parent)
{
    srcModel = m;
    itemHelper = dbHelper;
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
                     this,&GMAccountGroupSearchWidget::finishEdit);


    this->setCompleter(gmcompleter);
}

void GMAccountGroupSearchWidget::sendPositiveEMIT(QModelIndex index, QString itemID)
{
    activated = false;
    qDebug()<<"EMIT POSITIVE with UOMID : "<<itemID;
    item = itemHelper->getGroupById(itemID);
    emit itemSelected(index);
    emit itemSelectedwithItem(item);
}

void GMAccountGroupSearchWidget::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMAccountGroupSearchWidget::completerActivated(QModelIndex index)
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
        qDebug()<<"Index is not valid";
    }
}

void GMAccountGroupSearchWidget::finishEdit()
{

}
