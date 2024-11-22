#include "gmcontactlinematerial.h"

GMContactLineMaterial::GMContactLineMaterial(AddressBookDatabaseHelper *iHelper,QSqlQueryModel *m,QtMaterialTextField *parent)
    :QtMaterialTextField (parent)
{
    srcModel = m;
    ledHelper = iHelper;
    filterProxyModel = new QSortFilterProxyModel();
    this->setPlaceholderText("Search... ");

    qDebug()<<"Model Size : "<<m->rowCount();

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
                     this,&GMContactLineMaterial::finishEdit);

    QObject::connect(this,&QLineEdit::returnPressed,
                     this,&GMContactLineMaterial::finishEditByRet);



    this->setCompleter(gmcompleter);
}

GMContactLineMaterial::~GMContactLineMaterial()
{

}

void GMContactLineMaterial::setItem(const AddressBookDataModel &value)
{
    item = value;
    this->setText(item.ContactName);

}

void GMContactLineMaterial::setNewText(QString text, QString id)
{
    this->setText(text);

}

void GMContactLineMaterial::sendPositiveEMIT(QModelIndex index, QString itemID)
{
    activated = false;
    item = ledHelper->getContactByPhone(itemID);
    emit itemSelected(index);
    emit itemSelectedwithItem(item);
}

void GMContactLineMaterial::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMContactLineMaterial::completerActivated(QModelIndex index)
{
    if(index.isValid()){
//        qDebug()<<"Edit Finished text is "<<completer()->completionModel()->index(index.row(), 1).data().toString();
        qDebug()<<"Index is valid ***** ROW  "<<index.row();
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

void GMContactLineMaterial::finishEdit()
{
//    qDebug()<<"finish Edit";
//    if(activated){
//        activated = false;
//        return;
//    }
//    else
//        this->setText(oldText);

    emit itemSelectedwithItem(item);

    qDebug()<<"FINISH COMPLETE at index "<< gmcompleter->popup()->currentIndex().row();

}

void GMContactLineMaterial::finishEditByRet()
{
//    qDebug()<<"finish Edit";
//    if(activated){
//        activated = false;
//        return;
//    }
//    else
//        this->setText(oldText);

    emit itemSelectedwithItem(item);

    qDebug()<<"FINISH COMPLETE at index "<< gmcompleter->popup()->currentIndex().row();

}

void GMContactLineMaterial::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QtMaterialTextField::paintEvent(event);
}
