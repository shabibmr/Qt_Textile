#include "gmledgersearchlistwidgetlite.h"
#include "ui_gmledgersearchlistwidgetlite.h"

GMLedgerSearchListWidgetLite::GMLedgerSearchListWidgetLite(QSqlQueryModel *model1,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMLedgerSearchListWidgetLite)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(filterData(QString)));
    QObject::connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(checkData()));

    model = model1;
    sort = new GMLedgerSearchProxyModel();
    sort->setDynamicSortFilter(true);



    qDebug()<<"Setting sort source model";
    qDebug()<<"Sort is "<<sort;
    sort->setSourceModel(model);
    qDebug()<<"Setting table model";
    ui->tableWidget->setModel(sort);
    qDebug()<<"Setting itemdelegate";
    ui->tableWidget->setItemDelegate(new GMLedgerSearchDelegate());

    qDebug()<<"Delegate Done";
    ui->lineEdit->installEventFilter(this);
    ui->tableWidget->setHidden(true);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    qDebug()<<"GMLedgerSearchListWidgetLite Cons";


}

GMLedgerSearchListWidgetLite::~GMLedgerSearchListWidgetLite()
{
    delete ui;
}

void GMLedgerSearchListWidgetLite::setFocusExpl(QString text)
{
    ui->lineEdit->setText(text);
    ui->lineEdit->setFocus();
}

bool GMLedgerSearchListWidgetLite::eventFilter(QObject *obj, QEvent *event)
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
    return GMLedgerSearchListWidgetLite::eventFilter(obj, event);

}

void GMLedgerSearchListWidgetLite::filterData(QString text)
{
    ui->tableWidget->setHidden(false);
    QRegExp regExp(text,Qt::CaseInsensitive);
    sort->setFilterRegExp(regExp);


}

void GMLedgerSearchListWidgetLite::setData(QSqlQueryModel *modelArg)
{
    model = modelArg;
    sort = new GMLedgerSearchProxyModel();
    sort->setDynamicSortFilter(true);

    sort->setSourceModel(model);
    ui->tableWidget->setModel(sort);

}

void GMLedgerSearchListWidgetLite::checkData()
{

}

void GMLedgerSearchListWidgetLite::onTablewidgetEnterpressed(QModelIndex index)
{
    int row = sort->mapToSource(index).row();
    QString id = model->record(row).value(0).toString();
    QString name = model->record(row).value(1).toString();

    emit selectedID(id,name);
    this->close();

}

void GMLedgerSearchListWidgetLite::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString id = model->record(row).value(0).toString();
    QString name = model->record(row).value(1).toString();

    emit selectedID(id,name);
    this->close();

}
