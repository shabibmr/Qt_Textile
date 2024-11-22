#include "billwisemappingeditor.h"
#include <QScreen>
#include <QtMath>
#include "voucherOperations/vouchereditorselector.h"
#include "customwidgets/Misc/customshadoweffect.h"

BillwiseMappingEditor::BillwiseMappingEditor(LedgerMasterDataModel *ledger, GeneralVoucherDataObject *voucher, QWidget *parent)
    : QWidget(parent)
{

    this->setWindowTitle("Set bill mapping");
    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:#ffffff}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    this->setFixedWidth(1000);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);




    this->voucher = voucher;
    this->ledger = ledger;
    dbHelper= new BillwiseRecordsDatabaseHelper;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<this->ledger->mapList.size();

    setWidgets();
    calcValues();

}

void BillwiseMappingEditor::setWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    clearDuesButton = new QtMaterialFlatButton("Clear Dues");
    clearDuesButton->setForegroundColor(QColor("#00bcd4"));
    connect(clearDuesButton, &QtMaterialFlatButton::clicked, this,&BillwiseMappingEditor::clearDues);
    clearDuesButton->hide();


    newRefEdit = new GMMaterialLineEdit(&newRef);
    newRefEdit->setLabel("New Reference");
    newRefAmtEdit = new GMMaterialNumberEdit(&newRefAmount);
    newRefAmtEdit->setLabel("Amount");

    connect(newRefEdit, &GMMaterialLineEdit::editFinished, this, &BillwiseMappingEditor::addNewReferenceMap);
    connect(newRefAmtEdit, &GMMaterialNumberEdit::editFinished, this, &BillwiseMappingEditor::addNewReferenceMap);

    QHBoxLayout *newRefLayout = new QHBoxLayout;
    newRefLayout->addWidget(newRefEdit);
    newRefLayout->addWidget(newRefAmtEdit);
    newRefLayout->addStretch(1);
    newRefLayout->addWidget(clearDuesButton);


    QVBoxLayout *vLayout = new QVBoxLayout;
    this->setLayout(vLayout);

    QFrame *frame = new QFrame(this);
    vLayout->addWidget(frame);

    QVBoxLayout *vLayout1 = new QVBoxLayout;
    frame->setLayout(vLayout1);

    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(30.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor("#1a3966")); //#1a5050
    frame->setAutoFillBackground(true);
    frame->setGraphicsEffect(bodyShadow);

    frame->setStyleSheet("QFrame{background-color:#ffffff}");


    vLayout1->addLayout(newRefLayout);

    qModel = dbHelper->getPendingBillsModelByLedger(ledger->LedgerID, voucher);
    initMapList();
    model = new GMBillwiseMappingModel(qModel,  ledger);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(model, &GMBillwiseMappingModel::mapChanged, this, &BillwiseMappingEditor::calcValues);

    tableView = new ReportsTableView();
    //    tableView->lastRow->hide();
    tableView->setItemDelegate(new BillwiseMappingDelegate(qModel));
    tableView->setModel(model);

    tableView->setColumnWidth(0,10);
    tableView->setColumnWidth(8,10);
    tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);


    vLayout1->addWidget(tableView);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    QLabel *labelEntered = new QLabel("Entered: ");
    QLabel *labelSelected = new QLabel("Selected: ");
    QLabel *labelOnAccount = new QLabel("On Account: ");

    amountEntered   = new QLabel(QString::number(qFabs(ledger->amount),'f', 2));
    amountSelected  = new QLabel(QString::number(qFabs(selected),'f', 2));
    amountOnAccount = new QLabel(QString::number(qFabs(onAccount),'f', 2));

    QVBoxLayout *vLay1 = new QVBoxLayout;
    QVBoxLayout *vLay2 = new QVBoxLayout;
    QVBoxLayout *vLay3 = new QVBoxLayout;
    QHBoxLayout *hLay = new QHBoxLayout;
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    vLay1->addWidget(labelEntered);
    vLay1->addWidget(amountEntered);

    vLay2->addWidget(labelSelected);
    vLay2->addWidget(amountSelected);

    //    vLay3->addWidget(labelOnAccount);
    //    vLay3->addWidget(amountOnAccount);

    hLay->addLayout(vLay1);
    hLay->addLayout(vLay2);
    hLay->addLayout(vLay3);

    vLayout1->addLayout(hLay);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setCorner(Qt::BottomRightCorner);
    saveButton->setXOffset(0);
    saveButton->setYOffset(0);
    saveButton->setParent(this);
    saveButton->setToolTip("Save");
    connect(saveButton, &QtMaterialFloatingActionButton::clicked, this, &BillwiseMappingEditor::saveMapping);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    cancelButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("navigation", "cancel"));
    cancelButton->setMini(true);
    cancelButton->setCorner(Qt::TopRightCorner);
    cancelButton->setXOffset(0);
    cancelButton->setYOffset(0);
    cancelButton->setParent(this);
    cancelButton->setToolTip("Cancel");
    connect(cancelButton, &QtMaterialFloatingActionButton::clicked, this, &BillwiseMappingEditor::clearMapping);
    qDebug()<<Q_FUNC_INFO<<__LINE__;


    // 9895606066
}

void BillwiseMappingEditor::calcValues()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    onAccount = 0;
    selected = 0;

    for(int i=0; i<ledger->mapList.size(); i++){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<ledger->mapList[i]->CreditAmount<<ledger->mapList[i]->DebitAmount;
        selected += ledger->mapList[i]->CreditAmount - ledger->mapList[i]->DebitAmount;
    }

    amountSelected  ->setText(QString::number(qFabs(selected),'f', 2) + (selected > 0? " Cr": " Dr"));
    //    amountOnAccount ->setText(QString::number(qFabs(onAccount),'f', 2) + (onAccount > 0? " Cr": " Dr"));

}

void BillwiseMappingEditor::initMapList()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(ledger->mapList.size() == 0){
        for(int i=0; i< qModel->rowCount(); i++){
            //            selected += qModel->record(i).value(6).toFloat();
            if(qFabs(qModel->record(i).value(6).toFloat()) > 0 /*&& qModel->record(i).value(7).toString() != "ON ACC"*/){
                BillwiseRecordDataModel *map = new BillwiseRecordDataModel;
                map->LedgerID = ledger->LedgerID;
                map->RefVoucherNo = qModel->record(i).value(1).toString();
                map->RefType = qModel->record(i).value(2).toString();
                map->RefPrefix = qModel->record(i).value(3).toString();
                map->MethodOfAdjustment = qModel->record(i).value(7).toString();

                if(qModel->record(i).value(6).toFloat() > 0)
                    map->CreditAmount = qModel->record(i).value(6).toFloat();
                else
                    map->DebitAmount = qFabs(qModel->record(i).value(6).toFloat());

                qDebug()<<Q_FUNC_INFO<<__LINE__;
                ledger->mapList.append(map);
            }
        }
    }
    else{
        qDebug()<<Q_FUNC_INFO<<__LINE__<<ledger->mapList.size();
    }

}

void BillwiseMappingEditor::addNewReferenceMap()
{
    if(newRef.length() > 0 && newRefAmount > 0){
        bool newRefFound = false;

        for(int i=0; i<ledger->mapList.size(); i++){
            if(ledger->mapList[i]->MethodOfAdjustment == "NEW REF"){
                if(ledger->crAmount > 0)
                    ledger->mapList[i]->CreditAmount = newRefAmount;
                else
                    ledger->mapList[i]->DebitAmount = newRefAmount;

                newRefFound = true;
            }
        }

        if(!newRefFound){
            BillwiseRecordDataModel *map = new BillwiseRecordDataModel;
            map->LedgerID = ledger->LedgerID;
            map->RefVoucherNo = newRef;
            map->RefType = "New Reference";
            map->RefPrefix = "";
            map->MethodOfAdjustment = "NEW REF";
            if(ledger->crAmount > 0)
                map->CreditAmount = newRefAmount;
            else
                map->DebitAmount = newRefAmount;

            qDebug()<<Q_FUNC_INFO<<__LINE__;
            ledger->mapList.append(map);

        }

        calcValues();
    }

}

void BillwiseMappingEditor::clearDues()
{
    GeneralVoucherDatabaseHelper vHelper;
    voucher->ledgersList.append(*ledger);
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->status = QuotationStatus::Started;

    if(qFabs(selected) > 0){
        if(selected > 0){
            voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher);
            voucher->voucherNumber = vHelper.getNextVoucherNoByType(voucher->voucherType,
                                                                    voucher->VoucherPrefix,
                                                                    GMVoucherTypes::getVoucherMainTablename(GMVoucherTypes::ReceiptVoucher));
            voucher->ledgersList[0].crAmount =  selected;
            voucher->ledgersList[0].amount =  selected;
        }
        else{
            voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher);
            voucher->voucherNumber = vHelper.getNextVoucherNoByType(voucher->voucherType,
                                                                    voucher->VoucherPrefix,
                                                                    GMVoucherTypes::getVoucherMainTablename(GMVoucherTypes::PaymentVoucher));

            voucher->ledgersList[0].drAmount = qFabs(selected);
            voucher->ledgersList[0].amount = qFabs(selected);
        }
    }

    VoucherEditorSelector *vEditor = new VoucherEditorSelector;
    connect(vEditor, &VoucherEditorSelector::refresh, this, &BillwiseMappingEditor::resetWidgets);
    vEditor->openExternalVoucher(voucher);
    this->close();
}

void BillwiseMappingEditor::showClearDues()
{
    clearDuesFlag = true;
    //    clearDuesButton->show();

}

void BillwiseMappingEditor::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qModel = dbHelper->getPendingBillsModelByLedger(ledger->LedgerID, voucher);
    initMapList();
    model = new GMBillwiseMappingModel(qModel,  ledger);
    connect(model, &GMBillwiseMappingModel::mapChanged, this, &BillwiseMappingEditor::calcValues);

    tableView->setItemDelegate(new BillwiseMappingDelegate(qModel));
    tableView->setAvlcols(QList<int> {4,5,6});
    tableView->setModel(model);
    tableView->setColumns();

    tableView->setColumnWidth(0,10);
    tableView->setColumnWidth(1,20);
    tableView->setColumnWidth(2,15);
    tableView->setColumnWidth(3,40);
    tableView->setColumnWidth(8,10);
    tableView->setColumnWidth(7,30);
    //    tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

    calcValues();
}

void BillwiseMappingEditor::saveMapping()
{
    if(clearDuesFlag)
        clearDues();

    else
    {
        if(qFabs((selected) - (ledger->crAmount - ledger->drAmount)) > 0){
            QMessageBox box; box.setText("Selected amount " + QString::number(selected, 'f',2 )+
                                         " not matching with entered amount " + QString::number(ledger->crAmount - ledger->drAmount , 'f',2) );
            box.exec();
            return;
        }
        else
            this->close();
    }
}

void BillwiseMappingEditor::clearMapping()
{

    ledger->mapList.clear();
    qDebug()<<Q_FUNC_INFO<<ledger->mapList.size();

    // set default on account
    BillwiseRecordDataModel *refObj = new BillwiseRecordDataModel();
    refObj->MethodOfAdjustment = "ON ACC";
    refObj->Amount = ledger->amount;
    if(ledger->amount>0){
        refObj->CreditAmount = ledger->amount;
        refObj->DebitAmount = 0;
    }
    else {
        refObj->DebitAmount = -1*ledger->amount;
        refObj->CreditAmount = 0;
    }
    refObj->LedgerID = ledger->LedgerID;
    refObj->VoucherDate = voucher->VoucherDate;
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ledger->mapList.append(refObj);

    this->close();

}
