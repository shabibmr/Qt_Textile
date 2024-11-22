#include "stockjournals.h"
#include "ui_stockjournals.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
stockJournals::stockJournals(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stockJournals)
{
    ui->setupUi(this);
    dbhelper = new StockJournalDatabaseHelper();
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    setTableView();
}

stockJournals::~stockJournals()
{
    delete ui;
}

void stockJournals::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void stockJournals::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void stockJournals::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateModel(fromDate,toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    float total =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        total+= model->record(i).value(4).toFloat();
    }
    //    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    //    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}

void stockJournals::on_createNewPushButton_clicked()
{
    AddStockJournalWidget = new StockJournalEditor(this);
    AddStockJournalWidget->setWindowFlags(Qt::Window);
    AddStockJournalWidget->showMaximized();
    QObject::connect(AddStockJournalWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void stockJournals::on_okPushButton_clicked()
{
    setTableView();
}

void stockJournals::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void stockJournals::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoAsPtr(voucherNo, voucherPrefix);

    AddStockJournalWidget = new StockJournalEditor(this);
    AddStockJournalWidget->setWindowFlags(Qt::Window);
    AddStockJournalWidget->showVoucher(voucher);
    AddStockJournalWidget->showMaximized();
    QObject::connect(AddStockJournalWidget,SIGNAL(closing()),this,SLOT(setTableView()));

}
