#include "gmitemsearchwidget.h"
#include "ui_gmitemsearchwidget.h"


GMItemSearchWidget::GMItemSearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMItemSearchWidget)
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

GMItemSearchWidget::~GMItemSearchWidget()
{
    delete ui;
}

void GMItemSearchWidget::setData(QList<QSharedPointer<inventoryItemDataModel>> list)
{
    itemsList=list;
    int r =0;


    ui->tableWidget->setRowCount(list.size());


    for(int i =0;i<list.size()&&i<100;i++){
        SearchListItemWidget *item = new SearchListItemWidget();
        item->setData(list[i]->ItemName,list[i]->ItemAlias,list[i]->ItemCode,list[i]->ItemID);
        itemNames<<list[i]->ItemName;
        ui->tableWidget->setCellWidget(r,0,item);
        r++;
    }
    this->setMaximumHeight(ui->lineEdit->height()+ui->tableWidget->height());
}

void GMItemSearchWidget::filterData(QString text)
{
    ui->tableWidget->setHidden(false);

    lLoc=0;
    itemNames.clear();
    int r =0;
    ui->tableWidget->setRowCount(0);
    qDebug()<<text;

    for(int i =0;i<itemsList.size()&&ui->tableWidget->rowCount()<45;i++){
        if(itemsList[i]->ItemName.contains(text,Qt::CaseInsensitive)||
                itemsList[i]->ItemAlias.contains(text,Qt::CaseInsensitive)){
            ui->tableWidget->insertRow(r);
            SearchListItemWidget *item = new SearchListItemWidget();
            item->setData(itemsList[i]->ItemName,itemsList[i]->ItemAlias,itemsList[i]->ItemCode,itemsList[i]->ItemID);
            itemNames<<itemsList[i]->ItemName;
            ui->tableWidget->setCellWidget(r,0,item);
            r++;
        }
    }
   if(ui->tableWidget->rowCount()) ui->tableWidget->selectRow(0);
    this->setMaximumHeight(ui->lineEdit->height()+ui->tableWidget->height());
}


void GMItemSearchWidget::checkData()
{
    if(!itemNames.contains(ui->lineEdit->text(),Qt::CaseInsensitive))
    {
        ui->lineEdit->setText("");
        //filterData("");
    }
    lLoc=0;
}

void GMItemSearchWidget::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

bool GMItemSearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    //    if(obj==ui->lineEdit){
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
        else if(keyEvent->key() == Qt::Key_Enter||keyEvent->key() == Qt::Key_Return||
                keyEvent->key() == Qt::Key_Tab)
        {

            QString  text= ui->lineEdit->text();
            if(text.startsWith("AB",Qt::CaseInsensitive)){      qDebug()<<text;
                QString vno = text.mid(2,text.length()-2);
                emit SOSelectedSig(QString::number(vno.toInt()));
                this->close();
            }
            else{

                QModelIndex i = ui->tableWidget->currentIndex();
                if(i.row()>-1)
                    onTablewidgetEnterpressed(i.row());
            }

            return true;

        }

    }
    if(obj==ui->lineEdit){

        return false;
    }
    else{

    }
    //    }
    //    else
    //    {/*
    //        if(obj==ui->listWidget){
    //            if(event->type() == QEvent::KeyPress){
    //                ui->lineEdit->eventFilter(obj,event);
    //            }
    //        }*/
    //    }
    return GMItemSearchWidget::eventFilter(obj, event);
}

void GMItemSearchWidget::focusOutEvent(QFocusEvent *e)
{
    // emit nonSelected();
}



void GMItemSearchWidget::on_tableWidget_clicked(const QModelIndex &index)
{
    int r = index.row();
    SearchListItemWidget *widget = qobject_cast<SearchListItemWidget*> (ui->tableWidget->cellWidget(r,0));
    ui->lineEdit->setText(widget->getItemName());
    emit selectedID(widget->getID(),widget->getItemName());
    this->close();

}

void GMItemSearchWidget::onTablewidgetEnterpressed(int r)
{
    SearchListItemWidget *widget = qobject_cast<SearchListItemWidget*> (ui->tableWidget->cellWidget(r,0));
    ui->lineEdit->setText(widget->getItemName());
    emit selectedID(widget->getID(),widget->getItemName());
    this->close();

}
