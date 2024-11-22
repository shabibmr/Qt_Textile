#include "gmgodownsearchlistwidgetlite.h"
#include "ui_gmgodownsearchlistwidgetlite.h"

GMGodownSearchListWidgetLite::GMGodownSearchListWidgetLite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMGodownSearchListWidgetLite)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(filterData(QString)));
    QObject::connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(checkData()));

    model = itemHelper.getGodownForSearchModel();
    sort = new GMGodownSearchProxyModel();
    sort->setDynamicSortFilter(true);

    sort->setSourceModel(model);
    ui->tableWidget->setModel(sort);
    ui->tableWidget->setItemDelegate(new GodownSearchDelegate());

    ui->lineEdit->installEventFilter(this);
    ui->tableWidget->setHidden(true);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);

}

GMGodownSearchListWidgetLite::~GMGodownSearchListWidgetLite()
{
    delete ui;
}

void GMGodownSearchListWidgetLite::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
        ui->lineEdit->setFocus();
}

bool GMGodownSearchListWidgetLite::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
         QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
         if (keyEvent->key() == Qt::Key_Up)
         {
             if(lLoc>0)
                 lLoc--;
             ui->tableWidget->setCurrentIndex(sort->index(lLoc,0));

             return true;
         }
         else if(keyEvent->key() == Qt::Key_Down)
         {
             ui->tableWidget->setCurrentIndex(sort->index(lLoc,0));
             if(lLoc<sort->rowCount()-1)
                 lLoc++;

             return true;
         }
         else if(keyEvent->key() == Qt::Key_Enter||keyEvent->key() == Qt::Key_Return||
                 keyEvent->key() == Qt::Key_Tab)
         {
             QModelIndex i = ui->tableWidget->currentIndex();
             if(i.row()>-1)
                 onTablewidgetEnterpressed(i);

             return true;
         }

     }
     if(obj==ui->lineEdit){

         return false;
     }
     else{

     }

     return GMGodownSearchListWidgetLite::eventFilter(obj, event);
}

void GMGodownSearchListWidgetLite::filterData(QString text)
{
    ui->tableWidget->setHidden(false);
    QRegExp regExp(text,Qt::CaseInsensitive);
    sort->setFilterRegExp(regExp);

}

void GMGodownSearchListWidgetLite::setData(QSqlQueryModel *modelArg)
{
    model = modelArg;
        sort = new GMGodownSearchProxyModel();
        sort->setDynamicSortFilter(true);

        sort->setSourceModel(model);
        ui->tableWidget->setModel(sort);

}

void GMGodownSearchListWidgetLite::checkData()
{

}

void GMGodownSearchListWidgetLite::onTablewidgetEnterpressed(QModelIndex index)
{
    int row = sort->mapToSource(index).row();
    QString id = model->record(row).value(0).toString();
    QString name = model->record(row).value(1).toString();

    emit selectedID(id,name);
    this->close();

}

void GMGodownSearchListWidgetLite::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString id = model->record(row).value(0).toString();
    QString name = model->record(row).value(1).toString();

    emit selectedID(id,name);
    this->close();

}
