#include "gmgodownsearchlite.h"

GMGodownSearchLite::GMGodownSearchLite(QSqlQueryModel *model1,QLineEdit *parent)
    :QLineEdit(parent)
{
    itemsHelper = new GodownDatabaseHelper();

    itemsModel = model1;
    for(int i=0;i<itemsModel->rowCount();i++){
        list.append(itemsModel->record(i).value(1).toString());
    }
    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText("Search Here..");
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(checkEdit()));
    this->setFocusPolicy(Qt::StrongFocus);

    listWidget =0;
    listWidget = new GMGodownSearchListWidgetLite();

    listWidget->setData(itemsModel);

    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );

}

void GMGodownSearchLite::checkEdit()
{
    if(!list.contains(this->text(),Qt::CaseInsensitive)){
        this->setText("");
//        emit noneSelected();
    }

}

void GMGodownSearchLite::editTextChanged(QString text)
{
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    }

}

void GMGodownSearchLite::itemSelected(QString id, QString value)
{
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->setHidden(true);
    this->setFocus();
    qDebug()<<"Selected item : "<<value;
    emit SelectedGodown(itemsHelper->getGodownObjById(id));

}

void GMGodownSearchLite::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();

}
