#include "gmitemsearchlistwidgetlite.h"
#include "ui_gmitemsearchlistwidgetlite.h"

#include <QMessageBox>

GMItemSearchListWidgetLite::GMItemSearchListWidgetLite(QSqlQueryModel *model1,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMItemSearchListWidgetLite)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(filterData(QString)));
    QObject::connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(checkData()));

    model = model1;
    sort = new GMItemSearchProxyModel();
    sort->setDynamicSortFilter(true);

    sort->setSourceModel(model);
    ui->tableWidget->setModel(sort);
    ui->tableWidget->setItemDelegate(new ItemSearchDelegate());

    ui->lineEdit->installEventFilter(this);
    ui->tableWidget->setHidden(true);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);

}


GMItemSearchListWidgetLite::~GMItemSearchListWidgetLite()
{
    delete ui;
}

bool GMItemSearchListWidgetLite::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up)
        {
            qDebug()<<"Key Up";
            if(lLoc>0)
                lLoc--;
            else
                lLoc=sort->rowCount()-1;
            qDebug()<<lLoc;
            ui->tableWidget->setCurrentIndex(sort->index(lLoc,0));

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
            ui->tableWidget->setCurrentIndex(sort->index(lLoc,0));
            if(lLoc<sort->rowCount()-1)
                lLoc++;
            else
                lLoc=0;
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
            QString text= ui->lineEdit->text();

            if(text.size()<1)
                return true;

            QString key = keyEvent->text();
            int len = text.length();

            qDebug()<<key.toLatin1();

            QString code = text;
            qDebug()<<"Reading Single text :"<<text;


            // FROM PLU MACHINE
            if(text.startsWith("99") && text.length()>10){
                code = text.mid(2,5);
                qDebug()<<"Code : "<<code;
                if(code.length()>1){
                    //                    code = QString::number(code.toInt(),'f',0);
                    qDebug()<< "code = "<<code;
                    CompoundItemDataObject item = itemHelper.getInventoryItemByBarcode(code,false);
                    if(item.BaseItem.ItemID.length()>0){
                        item.BaseItem.quantity=(text.mid(7,3)+"."+text.mid(10,3)).toFloat()/item.BaseItem.price;
                        qDebug()<<"Qtyy = "<<item.BaseItem.quantity;
                        //                        ui->lineEdit->setText(text.mid(len,ui->lineEdit->text().length()));
                        ui->lineEdit->clear();
                        //                        qDebug()<<" New Text ";
                        emit selectedItembyBarcode(item.BaseItem);
                        this->close();
                    }
                    return true;
                }
            }
            else if(text.startsWith("21") && text.length()>10){
                code = text.mid(2,5);
                qDebug()<<"Code : "<<code;
                if(code.length()>1){
                    //                    code = QString::number(code.toInt(),'f',0);
                    qDebug()<< "code = "<<code;
                    CompoundItemDataObject item = itemHelper.getInventoryItemByBarcode(code,false);
                    if(item.BaseItem.ItemID.length()>0){
                        float amt = (text.mid(7,3)+"."+text.mid(10,3)).toFloat();
                        float subt = amt *100/(100+item.BaseItem.taxRate);
                        item.BaseItem.quantity=subt/item.BaseItem.price;
                        qDebug()<<"Qtyy = "<<item.BaseItem.quantity;
                        //                        ui->lineEdit->setText(text.mid(len,ui->lineEdit->text().length()));
                        ui->lineEdit->clear();
                        //                        qDebug()<<" New Text ";
                        emit selectedItembyBarcode(item.BaseItem);
                        this->close();
                    }
                    return true;
                }
            }
            else if(text.startsWith("888",Qt::CaseInsensitive)){
                qDebug()<<text << "                        BY AB";
                QString vno = text.mid(3,text.length()-3);
                emit SOSelectedSig(QString::number(vno.toInt()));
                this->close();
                return  true;
            }
            else if(text.startsWith("!AB",Qt::CaseInsensitive)){
                qDebug()<<text << "                        BY AB";
                QString vno = text.mid(3,text.length()-3);
                emit SOSelectedSig(QString::number(vno.toInt()));
                this->close();
                return  true;
            }
            else{
                CompoundItemDataObject item = itemHelper.getInventoryItemByBarcode(code);
                if(item.BaseItem.ItemID.length()>0){
                    //                    item.BaseItem.quantity=(text.mid(6,2)+"."+text.mid(8,3)).toFloat();
                    //                    item.BaseItem.price=(text.mid(11,4)+"."+text.mid(15,2)).toFloat();
                    emit selectedItembyBarcode(item.BaseItem);
                    this->close();
                    return true;
                }
            }

            QModelIndex i = ui->tableWidget->currentIndex();
            if(i.row()>-1)
                onTablewidgetEnterpressed(i);

            return true;
        }

    }
    if( obj == ui->lineEdit){

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
    return GMItemSearchListWidgetLite::eventFilter(obj, event);
}

void GMItemSearchListWidgetLite::filterData(QString text)
{
    ui->tableWidget->setHidden(false);
    QRegExp regExp(text,Qt::CaseInsensitive);
    sort->setFilterRegExp(regExp);
    if(sort->rowCount()>0){
        ui->tableWidget->setCurrentIndex(sort->index(0,0));
    }
    //    qDebug()<<"Changed Index to " <<ui->tableWidget->currentIndex().row();

}

void GMItemSearchListWidgetLite::setData(QSqlQueryModel *modelArg)
{
    model = modelArg;
    sort = new GMItemSearchProxyModel();
    sort->setDynamicSortFilter(true);

    sort->setSourceModel(model);
    ui->tableWidget->setModel(sort);

}

void GMItemSearchListWidgetLite::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

void GMItemSearchListWidgetLite::onTablewidgetEnterpressed(QModelIndex index)
{
    qDebug()<<"From Search";
    int row = sort->mapToSource(index).row();
    QString id = model->record(row).value(0).toString();
    QString name = model->record(row).value(1).toString();
    lLoc = 0 ;
    emit selectedID(id,name);
    this->close();
}

void GMItemSearchListWidgetLite::checkData(){
    QString text = ui->lineEdit->text();
    qDebug()<<"Return Pressed  "<<text;
    if(text.length() == 0)
        return;
    QString code = text;

    if(text.startsWith("99")){
        code = text.mid(2,4);



        //        qDebug()<<"Checking code";

        CompoundItemDataObject item = itemHelper.getInventoryItemByBarcode(code);
        if(item.BaseItem.ItemID.length()>0){
            qDebug()<<"EMITTING via checkdata"<<item.BaseItem.ItemID;
            emit selectedID(item.BaseItem.ItemID,item.BaseItem.ItemName);
            this->close();
        }
    }
    else if(text.startsWith("99",Qt::CaseInsensitive)){      qDebug()<<text;
        QString vno = text.mid(2,text.length()-2);
        emit SOSelectedSig(QString::number(vno.toInt()));
        this->close();

    }
    else if(text.startsWith("888",Qt::CaseInsensitive)){      qDebug()<<"888"<<text;
        QString vno = text.mid(2,text.length()-3);
        emit SOSelectedSig(QString::number(vno.toInt()));
        this->close();

    }

}

void GMItemSearchListWidgetLite::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug()<<"INDEX ROW + "<<index.row();
    int row = sort->mapToSource(index).row();
    qDebug()<<"Clicked ROW"<<row;
    QString id = model->record(row).value(0).toString();
    QString name = model->record(row).value(1).toString();

    lLoc = 0;

    emit selectedID(id,name);
    this->close();
}
