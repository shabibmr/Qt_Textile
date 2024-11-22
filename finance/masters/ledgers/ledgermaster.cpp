#include "ledgermaster.h"
#include "ui_ledgermaster.h"

ledgerMaster::ledgerMaster(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ledgerMaster)
{
    ui->setupUi(this);
    dbHelper = new LedgerMasterDatabaseHelper();
    //    ui->tableWidget->setColumnCount(3);
    //    ui->tableWidget->hideColumn(2);

    sort = new GMItemSortFilterProxyModel();
    setTableView();
}

ledgerMaster::~ledgerMaster()
{
    delete ui;
}

void ledgerMaster::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void ledgerMaster::setTableView()
{
    ledModel = dbHelper->getAllLedgersModel();
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(ledModel);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //    ui->tableWidget->setItemDelegate(new LedgerReportDelegate());
    ui->tableView->hideColumn(0);

    ui->searchLine->setFocus();

}

void ledgerMaster::on_createNewPushButton_clicked()
{
    addLedgerWidget = new addLedger(this);
    addLedgerWidget->setWindowFlags(Qt::Window);
    addLedgerWidget->show();
    QObject::connect(addLedgerWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}


void ledgerMaster::searchTextChanged(QString x)
{
    this->x=x;
    setTableView();
}


void ledgerMaster::on_tableView_doubleClicked(const QModelIndex &index)
{
    int r = sort->mapToSource(index).row();

    QString ledgerID = ledModel->record(r).value(0).toString();

    addLedgerWidget = new addLedger(this);
    addLedgerWidget->setWindowFlags(Qt::Window);
    addLedgerWidget->setTitle("Update Ledger");
    addLedgerWidget->setLedger(dbHelper->getLedgerPtrById(ledgerID));
    addLedgerWidget->edit();
    addLedgerWidget->show();
    QObject::connect(addLedgerWidget,SIGNAL(closing()),this,SLOT(setTableView()));

}

void ledgerMaster::on_searchLine_textChanged(const QString &arg1)
{
    QRegExp regExp(ui->searchLine->text(),Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);

}
