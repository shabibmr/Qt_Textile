#include "additemadditionalrequirement.h"
#include "ui_additemadditionalrequirement.h"
#include <QModelIndex>

addItemAdditionalRequirement::addItemAdditionalRequirement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addItemAdditionalRequirement)
{
    ui->setupUi(this);
    ui->amountLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));

    ledHelper = new LedgerMasterDatabaseHelper();
    ledgersList = ledHelper->getAllLedgers();

    GMSearchLineEdit *lineEdit = new GMSearchLineEdit();
    ui->ledgerComboBox->setLineEdit(lineEdit);
    modelLedger = new QStandardItemModel(ledgersList.size(),2,this);

    for(int row=0;row<ledgersList.size();row++){
        for(int col=0;col<2;col++){
            QModelIndex index = modelLedger->index(row,col,QModelIndex());
            if(col==0){
                modelLedger->setData(index,ledgersList[row].LedgerName);
                LedNames.append(ledgersList[row].LedgerName);
            }
            else
                modelLedger->setData(index,ledgersList[row].LedgerGroupName);

        }

    }
    QTableView *ledCompView = new QTableView;
    ledCompView->verticalHeader()->hide();
    ledCompView->horizontalHeader()->hide();
    ledCompView->setFrameShape(QFrame::NoFrame);
    ledCompView->setModel(modelLedger);

    lineEdit->setData(LedNames);
    QCompleter *ledCompleter = new QCompleter(this);
    ledCompleter->setFilterMode(Qt::MatchContains);
    ledCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ledCompleter->setCompletionMode(QCompleter::PopupCompletion);
    ledCompleter->setModel(modelLedger);
    ledCompleter->setPopup(ledCompView);

    ui->ledgerComboBox->setCompleter(ledCompleter);
}

addItemAdditionalRequirement::~addItemAdditionalRequirement()
{
    delete ui;
}

void addItemAdditionalRequirement::setLedsPtr(QList<QSharedPointer <LedgerMasterDataModel>> value)
{
    ledsPtr = value;
}

void addItemAdditionalRequirement::edit(LedgerMasterDataModel ledgerObj)
{
    ledger = ledgerObj;
    ui->ledgerComboBox->setCurrentText(ledgerObj.LedgerName);
    ui->amountLineEdit->setText(QString::number(ledgerObj.amount,'f',2));
    ui->descriptionLineEdit->setText(ledgerObj.narration);
}

void addItemAdditionalRequirement::setTitle(QString text)
{
    ui->label->setText(text);
}

void addItemAdditionalRequirement::on_buttonBox_accepted()
{
    ledger.LedgerID=ledgersList[ui->ledgerComboBox->currentIndex()].LedgerID;
    ledger.LedgerName = ledgersList[ui->ledgerComboBox->currentIndex()].LedgerName;
    ledger.amount = ui->amountLineEdit->text().toFloat();
    ledger.narration = ui->descriptionLineEdit->text();

    this->close();
    emit closing(ledger);
}

void addItemAdditionalRequirement::on_buttonBox_rejected()
{
    this->close();
}
