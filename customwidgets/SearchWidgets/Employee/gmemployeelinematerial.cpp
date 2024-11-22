#include "gmemployeelinematerial.h"


GMEmployeeLineMaterial::GMEmployeeLineMaterial(UserProfileDatabaseHelper *iHelper,QSqlQueryModel *m,QtMaterialTextField *parent)
    :QtMaterialTextField (parent)
{
    srcModel = m;
    userHelper = iHelper;
    filterProxyModel = new QSortFilterProxyModel();
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
//    gmcompleter->setWidget(this);

//    QRect cr = cursorRect();
//        cr.setWidth(300);
//        gmcompleter->complete(cr);

    this->installEventFilter(this);
    QObject::connect(gmcompleter,SIGNAL(activated(QModelIndex)),
                     this,SLOT(completerActivated(QModelIndex )));
    QObject::connect(gmcompleter,SIGNAL(highlighted(QModelIndex)),
                     this,SLOT(completerhighlighted(QModelIndex )));
    //    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT());
//    QObject::connect(this,&QLineEdit::editingFinished,
//                     this,&GMEmployeeLineMaterial::finishEdit);
//    QObject::connect(this,&QLineEdit::textChanged,
//                     this,&GMEmployeeLineMaterial::finishEdit);

    QObject::connect(this,&QLineEdit::returnPressed,
                     this,&GMEmployeeLineMaterial::finishEditByRet);
    this->setCompleter(gmcompleter);
}

GMEmployeeLineMaterial::~GMEmployeeLineMaterial()
{

}

void GMEmployeeLineMaterial::setItem(const UserProfileDataModel &value)
{
    item = value;
    this->setText(item.Name);
}

void GMEmployeeLineMaterial::setNewText(QString text, QString id)
{

}
bool GMEmployeeLineMaterial::eventFilter(QObject *obj, QEvent *event)
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
                    int itemID = completer()->completionModel()->index(i.row(), 0).data().toInt();
                    activated= true;
                    qDebug()<<"Item Pressed at "<<i.row()<<"item id "<<itemID;
                    sendPositiveEMIT(i,itemID);
                }
            }
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
//                    return true;
                }
            }
            else if( event->type() == QEvent::FocusOut){
                qDebug()<<"Setting Name to : "<<item.Name;
                this->setText(item.Name);
                return  false;
                //                qDebug()<<"Focus out";
            }
        return false;

}

void GMEmployeeLineMaterial::sendPositiveEMIT(QModelIndex index, int itemID)
{
    activated = false;

    item = userHelper->getEmployeeByID(itemID);

    emit itemSelected(index);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit itemSelectedwithItem(item);
    event(new QEvent(QEvent::FocusOut));
}

void GMEmployeeLineMaterial::completerhighlighted(QModelIndex index)
{
    emit currentSelection(index);
}

void GMEmployeeLineMaterial::completerActivated(QModelIndex index)
{
    if(index.isValid()){
//        qDebug()<<"Edit Finished text is "<<completer()->completionModel()->index(index.row(), 1).data().toString();
//        qDebug()<<"Index is valid ***** ROW  "<<index.row();
        int itemID = completer()->completionModel()->index(index.row(), 0).data().toInt();
        activated = true;
        sendPositiveEMIT(index,itemID);
    }
    else{
        this->setText("");
        activated = false;
        qDebug()<<"Index is not valid";
    }
}


void GMEmployeeLineMaterial::finishEditByRet()
{
//    qDebug()<<"finish Edit";
//    if(activated){
//        activated = false;
//        return;
//    }
//    else
//        this->setText(oldText);

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit itemSelectedwithItem(item);

//    qDebug()<<"FINISH COMPLETE at index "<< gmcompleter->popup()->currentIndex().row();

}


void GMEmployeeLineMaterial::finishEdit()
{
//    qDebug()<<"finish Edit";
//    if(activated){
//        activated = false;
//        return;
//    }
//    else
//        this->setText(oldText);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit itemSelectedwithItem(item);
//    qDebug()<<"FINISH COMPLETE";
}


void GMEmployeeLineMaterial::paintEvent(QPaintEvent *event)
{

        QStyleOption opt;
        opt.init(this);
        QPainter p(this);

        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        QtMaterialTextField::paintEvent(event);


}
