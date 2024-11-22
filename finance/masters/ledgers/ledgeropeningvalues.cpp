#include "ledgeropeningvalues.h"
#include "ui_ledgeropeningvalues.h"
#include <QMessageBox>

LedgerOpeningValues::LedgerOpeningValues(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedgerOpeningValues)
{
    ui->setupUi(this);
    ledHelper = new LedgerMasterDatabaseHelper;
    ledsList = ledHelper->getAllLedgersAsPtr();

    ui->lineEdit->hide();

    ui->tableWidget->setColumnCount(2);

    int i=0;
    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Ledger Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,ItemName);

    QTableWidgetItem* ModelNumber = new QTableWidgetItem;
    ModelNumber->setText("Opening Balance");
    ui->tableWidget->setHorizontalHeaderItem(i++,ModelNumber);

     ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    setTable();


}

LedgerOpeningValues::~LedgerOpeningValues()
{
    delete ui;
}

void LedgerOpeningValues::setTable()
{
    int rowCount = ledsList.size();

    ui->tableWidget->setRowCount(rowCount);

    for(int j=0;j<rowCount;j++){

        if((text.length() == 0) || ((text.length()>0)&&
                                    (ledsList[j]->LedgerName.contains(text,Qt::CaseInsensitive)||
                                     ledsList[j]->LedgerGroupName.contains(text,Qt::CaseInsensitive)
                                     ))){
            int i=0;

            QTableWidgetItem* ItemName = new QTableWidgetItem;
            ItemName->setText(ledsList[j]->LedgerName);
            ui->tableWidget->setItem(j,i++,ItemName);

            QLineEdit* OpeningStkQty = new QLineEdit();
            OpeningStkQty->setAlignment(Qt::AlignRight);
            OpeningStkQty->setValidator(new QDoubleValidator(0,99999,2,this));
            OpeningStkQty->setProperty("row",j);
            OpeningStkQty->setText(QString::number(ledsList[j]->openingBalance));
            ui->tableWidget->setCellWidget(j,i++,OpeningStkQty);
            QObject::connect(OpeningStkQty,SIGNAL(returnPressed()),SLOT(updateValue()));


        }

    }
}

void LedgerOpeningValues::on_lineEdit_returnPressed()
{
     text = ui->lineEdit->text();
     setTable();
}

void LedgerOpeningValues::on_pushButton_2_clicked()
{
    this->close();
}

void LedgerOpeningValues::on_pushButton_clicked()
{
 QMessageBox box; box.setText("Saved"); box.exec();
}

void LedgerOpeningValues::updateValue()
{
    int row = sender()->property("row").toInt();
    QLineEdit* line = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,1));
    ledsList[row]->openingBalance = line->text().toFloat();
    ledHelper->updateOpeningValue(ledsList[row]->LedgerID,ledsList[row]->openingBalance);
    if(row<ledsList.size()-1){
        QLineEdit* qtyNext = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row+1,1));
        qtyNext->setFocus();
        qtyNext->selectAll();
    }
}


