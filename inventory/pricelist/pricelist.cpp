#include "pricelist.h"
#include "ui_pricelist.h"

priceList::priceList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::priceList)
{
    ui->setupUi(this);
    dbHelper = new PricelistDatabaseHelper();
    sqlModel = new QSqlQueryModel();
    sort = new GMItemSortFilterProxyModel();

    settable();
}

priceList::~priceList()
{
    delete ui;
}

void priceList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
}

void priceList::on_createNewPushButton_clicked()
{
    addPriceListWidget = new addPriceList(this);
    addPriceListWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addPriceListWidget->showMaximized();
    QObject::connect(addPriceListWidget,SIGNAL(closing()),this,SLOT(settable()));
}

void priceList::settable()
{
    sqlModel = dbHelper->getPriceListsAsQueryModel();
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(sqlModel);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    if(sqlModel->columnCount()>0)
        ui->tableView->hideColumn(0);
}





void priceList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    int col = index.column();
    addPriceListWidget = new addPriceList(this);
    addPriceListWidget->setWindowFlags(Qt::Window);
    addPriceListWidget->setAttribute(Qt::WA_DeleteOnClose);
    //addPriceListWidget->setItem(sqlModel->record(row).value(0).toString());
    int priceID = sqlModel->record(row).value(0).toInt();

    addPriceListWidget->edit(dbHelper->getPriceListByIDasPtr(sqlModel->record(row).value(0).toInt()));
    addPriceListWidget->setPriceName(sqlModel->record(row).value(1).toString());
    addPriceListWidget->setId(sqlModel->record(row).value(0).toInt());
    QObject::connect(addPriceListWidget,SIGNAL(closing()),this,SLOT(settable()));
    addPriceListWidget->showMaximized();


}
