#include "journal.h"
#include "ui_journal.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include <QDebug>

journal::journal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::journal)
{
    ui->setupUi(this);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    dbhelper = new JournalVoucherDatabaseHelper();

     ui->tableView->setItemDelegate(new ReportCommonDelegate());
    setTableView();
}

journal::~journal()
{
    delete ui;
}

void journal::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void journal::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNew_clicked();
    }

}

void journal::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);

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

void journal::on_createNew_clicked()
{
    addJournalEntryWidget = new JournalVoucherEditor();
    addJournalEntryWidget->setWindowFlags(Qt::Window);
    addJournalEntryWidget->showMaximized();
    connect(addJournalEntryWidget, &JournalVoucherEditor::closing, this, &journal::setTableView);
//    QObject::connect(addJournalEntryWidget,SIGNAL(closing()),this,SLOT(setTable()));
}

//void journal::on_tableWidget_doubleClicked(const QModelIndex &index)
//{
//    int r = ui->tableWidget->item(index.row(),0)->text().toInt()-1;
//    qDebug()<<"value of r"<<r;
//    addJournalEntryWidget = new JournalVoucherEditor(this);
//    addJournalEntryWidget->setWindowFlags(Qt::Window);
//    addJournalEntryWidget->showVoucher( &voucherList[r]);
//    addJournalEntryWidget->show();
//    QObject::connect(addJournalEntryWidget,SIGNAL(closing()),this,SLOT(setTable()));
//}

//void journal::setTable()
//{
//    QDate dateFrom = ui->fromDateEdit->date();
//    QDate dateTo = ui->toDateEdit->date();
//    voucherList = dbhelper->getVoucherListByDate(dateFrom,dateTo);
//    qDebug()<<"Size = "<<voucherList.size();
//    int cnt =0;
//    int i=0;
//    ui->tableWidget->setRowCount(0);
//    for(GeneralVoucherDataObject obj:voucherList){
//        ui->tableWidget->insertRow(cnt);
//        i=0;

//        QTableWidgetItem* SLNO = new QTableWidgetItem;
//        SLNO->setText(QString::number(cnt+1));
//        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,SLNO);

//        QTableWidgetItem* dateItem = new QTableWidgetItem;
//        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
//        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,dateItem);

//        QTableWidgetItem* LedgerName = new QTableWidgetItem;
//        LedgerName->setText(obj.ledgerObject.LedgerName);
//        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,LedgerName);

//        QTableWidgetItem* VchNo = new QTableWidgetItem;
//        VchNo->setText(obj.voucherNumber);
//        VchNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        VchNo->setFlags(VchNo->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,VchNo);

//        QTableWidgetItem* Amount = new QTableWidgetItem;
//        Amount->setText(QString::number(obj.grandTotal,'f',2));
//        Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        Amount->setFlags(Amount->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,Amount);

//        QToolButton* delButton = new QToolButton;
//        delButton->setIcon(QIcon(":/icons/trash.ico"));
//        delButton->setProperty("row",cnt);
//        delButton->setAutoRaise(true);

//        ui->tableWidget->setCellWidget(cnt,i++,delButton);
//        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

//        cnt++;
//    }
//}

void journal::on_okPushButton_clicked()
{
    setTableView();
}

void journal::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void journal::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

        addJournalEntryWidget = new JournalVoucherEditor(this);
        addJournalEntryWidget->setWindowFlags(Qt::Window);
        addJournalEntryWidget->showVoucher(voucher);
        addJournalEntryWidget->showMaximized();
        QObject::connect(addJournalEntryWidget,SIGNAL(closing()),this,SLOT(setTableView()));

}
