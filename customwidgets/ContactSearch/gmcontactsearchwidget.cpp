#include "gmcontactsearchwidget.h"
#include "ui_gmcontactsearchwidget.h"

#include <QDebug>

GMContactSearchWidget::GMContactSearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMContactSearchWidget)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText("Search...");

    QObject::connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(filterData(QString)));
    QObject::connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(checkData()));

    this->setAttribute(Qt::WA_NoSystemBackground, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->hide();
    ui->lineEdit->installEventFilter(this);
    ui->tableWidget->setHidden(true);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
}

GMContactSearchWidget::~GMContactSearchWidget()
{
    delete ui;
}

void GMContactSearchWidget::setData(QList<QSharedPointer<AddressBookSmall> > list)
{
    itemsList=list;
    int r =0;


    ui->tableWidget->setRowCount(list.size());
    for(int i =0;i<list.size()&&i<100;i++){
        ContactListWidgetItem *item = new ContactListWidgetItem();
        item->setData(list[i]->Name,list[i]->CompanyName,list[i]->phoneNumber,list[i]->_id);
        itemNames<<list[i]->Name;
        ui->tableWidget->setCellWidget(r,0,item);
        r++;
    }
    ui->tableWidget->selectRow(0);
    this->setMaximumHeight(ui->lineEdit->height()+ui->tableWidget->height());
}

void GMContactSearchWidget::filterData(QString text)
{
    ui->tableWidget->setHidden(false);

    lLoc=0;
    itemNames.clear();
    int r =0;
    ui->tableWidget->setRowCount(0);

    for(int i =0;i<itemsList.size()&&ui->tableWidget->rowCount()<31;i++){
        if(itemsList[i]->Name.contains(text,Qt::CaseInsensitive)||itemsList[i]->CompanyName.contains(text,Qt::CaseInsensitive)){
            ui->tableWidget->insertRow(r);
            ContactListWidgetItem *item = new ContactListWidgetItem();
            item->setData(itemsList[i]->Name,itemsList[i]->CompanyName,itemsList[i]->phoneNumber,itemsList[i]->_id);
            itemNames<<itemsList[i]->Name;
            ui->tableWidget->setCellWidget(r,0,item);
            r++;
        }
    }
    if(ui->tableWidget->rowCount()) ui->tableWidget->selectRow(0);
    this->setMaximumHeight(ui->lineEdit->height()+ui->tableWidget->height());
}

void GMContactSearchWidget::checkData()
{
    if(!itemNames.contains(ui->lineEdit->text(),Qt::CaseInsensitive))
    {
        ui->lineEdit->setText("");
        //filterData("");
    }
    lLoc=0;
}

void GMContactSearchWidget::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

bool GMContactSearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up)
        {
            if(lLoc>0)
                lLoc--;
            ui->tableWidget->selectRow(lLoc);

            //
            //->item(lLoc)->setSelected(true);
            //            QModelIndex i = ui->listWidget->currentIndex();
            //            QListWidgetItem *item = ui->listWidget->item(lLoc);
            //            ui->lineEdit->setText(item->data(2).toString());
            //            ui->listWidget->scrollToItem(item, QAbstractItemView::EnsureVisible);

            return true;
        }
        else if(keyEvent->key() == Qt::Key_Down)
        {

            ui->tableWidget->selectRow(lLoc);
            if(lLoc<ui->tableWidget->rowCount()-1)
                lLoc++;
            //            ui->listWidget->item(lLoc)->setSelected(true);
            //            QModelIndex i = ui->listWidget->currentIndex();
            //            QTableWidgetItem *item = ui->tableWidget->item(lLoc);
            //            ui->lineEdit->setText(item->data(2).toString());
            //            qDebug()<<"Down Pressed "<<item->data(2).toString();
            //            ui->tableWidget->scrollToItem(item, QAbstractItemView::EnsureVisible);

            return true;
        }
        else if(keyEvent->key() == Qt::Key_Enter||keyEvent->key() == Qt::Key_Return)
        {
            QModelIndex i = ui->tableWidget->currentIndex();
            if(i.row()<=ui->tableWidget->rowCount())
                if(i.row()>-1)
                    onTablewidgetEnterpressed(i.row());
            //            ui->listWidget->item(lLoc)->setSelected(true);
            //            QModelIndex i = ui->listWidget->currentIndex();
            //            QTableWidgetItem *item = ui->tableWidget->item(lLoc);
            //            ui->lineEdit->setText(item->data(2).toString());
            //            qDebug()<<"Down Pressed "<<item->data(2).toString();
            //            ui->tableWidget->scrollToItem(item, QAbstractItemView::EnsureVisible);

            return true;
        }

    }
    if(obj==ui->lineEdit){

        return false;
    }
    else{

    }

    return GMContactSearchWidget::eventFilter(obj, event);
}

void GMContactSearchWidget::on_tableWidget_clicked(const QModelIndex &index)
{
    int r = index.row();
    ContactListWidgetItem *widget = qobject_cast<ContactListWidgetItem*> (ui->tableWidget->cellWidget(r,0));
    ui->lineEdit->setText(widget->getItemName());
    emit selectedID(widget->getID(),widget->getItemName());
    this->close();
}

void GMContactSearchWidget::onTablewidgetEnterpressed(int item)
{
    ContactListWidgetItem *widget = qobject_cast<ContactListWidgetItem*> (ui->tableWidget->cellWidget(item,0));
    ui->lineEdit->setText(widget->getItemName());
    emit selectedID(widget->getID(),widget->getItemName());
    this->close();
}
