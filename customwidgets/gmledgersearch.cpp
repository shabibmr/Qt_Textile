#include "gmledgersearch.h"

GMLedgerSearch::GMLedgerSearch(QList<QSharedPointer<LedgerMasterDataModel>> ls,QLineEdit *parent)
    :QLineEdit(parent)
{
    ledgersHelper = new LedgerMasterDatabaseHelper();
    ledgers = ls;

    for(int i=0;i<ledgers.size();i++){
        list.append(ledgers[i]->LedgerName);
    }
    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText("Search Here..");
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(checkEdit()));
    this->setFocusPolicy(Qt::StrongFocus);
    listWidget =0;
    listWidget = new GMLedgerSearchWidget();
    listWidget->setData(ledgers);
    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );
    this->setMinimumWidth(450);

}

void GMLedgerSearch::textClicked()
{

}

void GMLedgerSearch::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
}

void GMLedgerSearch::checkEdit()
{
    if(!list.contains(this->text(),Qt::CaseInsensitive))
        this->setText("");
}

void GMLedgerSearch::editTextChanged(QString text)
{
    //listWidget->filterData(text);
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedItem(QString,QString)),this,SLOT(ledgerSelected(QString,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    }
}

void GMLedgerSearch::setMainParent(QWidget *Parent)
{

}

void GMLedgerSearch::ledgerSelected(QString id, QString value)
{
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedItem(QString,QString)),this,SLOT(ledgerSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->setHidden(true);
    this->setFocus();
    emit SelectedLedger(ledgersHelper->getLedgerObjectByID(id));
}

void GMLedgerSearch::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedItem(QString,QString)),this,SLOT(ledgerSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->hide();
    listWidget=0;

    this->setFocus();
    emit clearItem();
}

GMLedgerSearch::~GMLedgerSearch()
{

}
