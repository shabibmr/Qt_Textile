#include "gmledgersearchwidget.h"
#include "ui_gmledgersearchwidget.h"

GMLedgerSearchWidget::GMLedgerSearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMLedgerSearchWidget)
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

GMLedgerSearchWidget::~GMLedgerSearchWidget()
{
    delete ui;
}

void GMLedgerSearchWidget::setData(QList<QSharedPointer<LedgerMasterDataModel>> list)
{
    itemsList = list;
    int r =0;
    //ui->tableWidget->setHidden(false);
    ui->tableWidget->setRowCount(list.size());
    // qDebug()<<"Setting data";
    for(int i =0; i< itemsList.size()&&i<100;i++){
        //LedgerMasterDataModel ledger:list){
        GMSearchLedgerWidgetItem *item = new GMSearchLedgerWidgetItem();
        item->setData(itemsList[i]->LedgerID,itemsList[i]->LedgerName,itemsList[i]->LedgerGroupName);
        itemNames<<itemsList[i]->LedgerID;
        ui->tableWidget->setCellWidget(r,0,item);
        r++;
    }
    this->setMaximumHeight(ui->lineEdit->height()+ui->tableWidget->height());
}

void GMLedgerSearchWidget::filterData(QString text)
{
    ui->tableWidget->setHidden(false);
    lLoc=0;
    int r =0;
    ui->tableWidget->setRowCount(0);
    for(int i =0; i< itemsList.size()&&ui->tableWidget->rowCount()<31;i++){
        if(itemsList[i]->LedgerName.contains(text,Qt::CaseInsensitive)){
            ui->tableWidget->insertRow(r);
            GMSearchLedgerWidgetItem *item = new GMSearchLedgerWidgetItem();
            item->setData(itemsList[i]->LedgerID,itemsList[i]->LedgerName,itemsList[i]->LedgerGroupName);
            itemNames<<itemsList[i]->LedgerID;
            ui->tableWidget->setCellWidget(r,0,item);
            r++;
        }
    }
    if(ui->tableWidget->rowCount()) ui->tableWidget->selectRow(0);
}

void GMLedgerSearchWidget::checkData()
{
    if(!itemNames.contains(ui->lineEdit->text(),Qt::CaseInsensitive))
    {
        ui->lineEdit->setText("");
        //filterData("");
    }
    lLoc=0;
}

void GMLedgerSearchWidget::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

bool GMLedgerSearchWidget::eventFilter(QObject *obj, QEvent *event)
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
        else if(keyEvent->key() == Qt::Key_Enter||keyEvent->key() == Qt::Key_Return||
                keyEvent->key() == Qt::Key_Tab)
        {
            QModelIndex i = ui->tableWidget->currentIndex();

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
    return GMLedgerSearchWidget::eventFilter(obj, event);
}

void GMLedgerSearchWidget::focusOutEvent(QFocusEvent *e)
{

}

void GMLedgerSearchWidget::onTablewidgetClicked(const QModelIndex &index)
{
    int r = index.row();
    qDebug()<<"Ledger Search Clicked";
    GMSearchLedgerWidgetItem *widget = qobject_cast<GMSearchLedgerWidgetItem*> (ui->tableWidget->cellWidget(r,0));
    ui->lineEdit->setText(widget->getLedgerName());
    emit selectedItem(widget->getLedgerID(),widget->getLedgerName());
    this->close();
}

void GMLedgerSearchWidget::onTablewidgetEnterpressed(int r)
{
    GMSearchLedgerWidgetItem *widget = qobject_cast<GMSearchLedgerWidgetItem*> (ui->tableWidget->cellWidget(r,0));
    ui->lineEdit->setText(widget->getLedgerName());
    //qDebug()<<widget->getLedgerName();
    emit selectedItem(widget->getLedgerID(),widget->getLedgerName());
    this->close();
}

void GMLedgerSearchWidget::on_tableWidget_clicked(const QModelIndex &index)
{
    int r = index.row();
    qDebug()<<"Ledger Search Clicked";
    GMSearchLedgerWidgetItem *widget = qobject_cast<GMSearchLedgerWidgetItem*> (ui->tableWidget->cellWidget(r,0));
    ui->lineEdit->setText(widget->getLedgerName());
    emit selectedItem(widget->getLedgerID(),widget->getLedgerName());
    this->close();
}
