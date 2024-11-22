#include "termslist.h"
#include "ui_termslist.h"

TermsList::TermsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TermsList)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->hideColumn(2);
    QTableWidgetItem* GroupName = new QTableWidgetItem;
    GroupName->setText("Ref Name");
    ui->tableWidget->setHorizontalHeaderItem(0,GroupName);

    QTableWidgetItem* parentName = new QTableWidgetItem;
    parentName->setText("Validity");
    ui->tableWidget->setHorizontalHeaderItem(1,parentName);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    dbHelper = new TermsDatabaseHelper;

    setTable();
}

TermsList::~TermsList()
{
    delete ui;
}

TermsList::setTable()
{
    QList<QMap<QString,QString>> map = dbHelper->getAllTerms();
    ui->tableWidget->setRowCount(map.size());
    qDebug()<<"Map size = "<<map.size();
    for(int ix=0;ix< map.size();ix++){
        QMap<QString,QString> mmap = map[ix];
        QMapIterator<QString, QString> i(mmap);

        while (i.hasNext()) {
            i.next();
            QString colname = i.key();
            QString value = i.value();

            QTableWidgetItem* GroupName = new QTableWidgetItem;
            GroupName->setText(colname.split("|")[1]);
            GroupName->setFlags(GroupName->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(ix,0,GroupName);

            QTableWidgetItem* parentName = new QTableWidgetItem;
            parentName->setText(value);
            parentName->setFlags(parentName->flags() ^ Qt::ItemIsEditable);

            //        qDebug()<<"parent Name = "<<obj.parentGroupName;
            ui->tableWidget->setItem(ix,1,parentName);

            QTableWidgetItem* id = new QTableWidgetItem;
            id->setText(colname.split("|")[0]);
            ui->tableWidget->setItem(ix,2,id);



        }
    }
}

void TermsList::on_createNewPushButton_clicked()
{
    TermsConditionsWidget = new TermsConditions(this);
    TermsConditionsWidget->setWindowFlags(Qt::Window);
    TermsConditionsWidget->show();
    QObject::connect(TermsConditionsWidget,SIGNAL(closing()),this,SLOT(settable()));
}

void TermsList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r= index.row();
    qDebug()<<ui->tableWidget->item(r,2)->text();
    int n = ui->tableWidget->item(r,2)->text().toInt();
    TermsConditionsWidget = new TermsConditions(this);
    TermsConditionsWidget->setWindowFlags(Qt::Window);
    TermsConditionsWidget->editTerms(n);
    TermsConditionsWidget->show();
    QObject::connect(TermsConditionsWidget,SIGNAL(closing()),this,SLOT(settable()));

}
