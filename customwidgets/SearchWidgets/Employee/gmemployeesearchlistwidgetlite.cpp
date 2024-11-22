#include "gmemployeesearchlistwidgetlite.h"
#include "ui_gmemployeesearchlistwidgetlite.h"

#include <QMessageBox>

GMEmployeeSearchListWidgetLite::GMEmployeeSearchListWidgetLite(QSqlQueryModel *model1,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMEmployeeSearchListWidgetLite)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(filterData(QString)));
    QObject::connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(checkData()));

    model = model1;
    sort = new GMEmployeeSearchProxyModel();
    sort->setDynamicSortFilter(true);

    sort->setSourceModel(model);
    ui->tableWidget->setModel(sort);
    ui->tableWidget->setItemDelegate(new EmployeeSearchDelegate());

    ui->lineEdit->installEventFilter(this);
    ui->tableWidget->setHidden(true);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);

}


GMEmployeeSearchListWidgetLite::~GMEmployeeSearchListWidgetLite()
{
    delete ui;
}

bool GMEmployeeSearchListWidgetLite::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up)
        {
            if(lLoc>0)
                lLoc--;
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
    return GMEmployeeSearchListWidgetLite::eventFilter(obj, event);
}

void GMEmployeeSearchListWidgetLite::filterData(QString text)
{
    ui->tableWidget->setHidden(false);
    QRegExp regExp(text,Qt::CaseInsensitive);
    sort->setFilterRegExp(regExp);
    if(sort->rowCount()>0){
        ui->tableWidget->setCurrentIndex(sort->index(0,0));
    }
//    qDebug()<<"Changed Index to " <<ui->tableWidget->currentIndex().row();

}

void GMEmployeeSearchListWidgetLite::setData(QSqlQueryModel *modelArg)
{
    model = modelArg;
    sort = new GMEmployeeSearchProxyModel();
    sort->setDynamicSortFilter(true);

    sort->setSourceModel(model);
    ui->tableWidget->setModel(sort);

}

void GMEmployeeSearchListWidgetLite::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

void GMEmployeeSearchListWidgetLite::onTablewidgetEnterpressed(QModelIndex index)
{
    qDebug()<<"From Search";
    int row = sort->mapToSource(index).row();
    int id = model->record(row).value(0).toInt();
    QString name = model->record(row).value(1).toString();
    lLoc = 0 ;
    emit selectedID(id,name);
    this->close();
}

void GMEmployeeSearchListWidgetLite::checkData(){
    QString text = ui->lineEdit->text();
    qDebug()<<"Return Pressed  "<<text;
    if(text.length() == 0)
        return;
    QString code = text;

}

void GMEmployeeSearchListWidgetLite::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug()<<"INDEX ROW + "<<index.row();
    int row = sort->mapToSource(index).row();
    qDebug()<<"Clicked ROW"<<row;
    int id = model->record(row).value(0).toInt();
    QString name = model->record(row).value(1).toString();

    lLoc = 0;

    emit selectedID(id,name);
    this->close();
}
