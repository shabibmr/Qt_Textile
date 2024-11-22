#include "receiptnotevouchereditor.h"
//#include "ui_receiptnotevouchereditor.h"

#include "voucherOperations/vouchereditorselector.h"

ReceiptNoteVoucherEditor::ReceiptNoteVoucherEditor(VoucherDBAbstract *db, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor(GMVoucherTypes::ReceiptNote, db, parent)
{

    setDrawerLayout();

}

ReceiptNoteVoucherEditor::~ReceiptNoteVoucherEditor()
{
//    delete ui;
}

void ReceiptNoteVoucherEditor::setDrawerLayout()
{
    QPushButton *button = new QPushButton(this);
    button->setText("Convert To Purchase Invoice");
    drawer->drawerLayout()->addWidget(button);

    QObject::connect(exportButton,&QPushButton::clicked,
                     drawer,&QtMaterialDrawer::openDrawer);

    QObject::connect(button,&QPushButton::clicked,
                     this,&ReceiptNoteVoucherEditor::exportVoucher);
}

void ReceiptNoteVoucherEditor::saveVoucher()
{
    if(ConfigurationSettingsDatabaseHelper::getValue(promptItemChecklist,false).toBool()){
        if(!itemsChecked){
            showItemChecklistDialog();
            return;
        }

    }
    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::ReceiptReady;

    InventoryLedgerTransactionsEditor::saveVoucher();
//    VoucherEditorBase::closeEditor();
    emit saveCompleted();

}


//void ReceiptNoteVoucherEditor::askForPrint()
//{
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
//                                  QMessageBox::Yes|QMessageBox::No);
//    if(reply==QMessageBox::Yes){
//        printVoucher();
//    }
//}

//void ReceiptNoteVoucherEditor::printVoucher()
//{
//    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryVoucher(voucher);
//}

void ReceiptNoteVoucherEditor::exportVoucher()
{
    if(voucher->status ==  QuotationStatus::Started){
        snackBar->addMessage("Delivery note not Saved!!!");
        return;
    }

    VoucherEditorSelector *vEditor = new VoucherEditorSelector;
    vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));


}

void ReceiptNoteVoucherEditor::showItemChecklistDialog()
{

    ItemCheckListWidget *checkList = new ItemCheckListWidget(voucher);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    checkList->setWindowFlags(Qt::Popup);
    checkList->show();

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(checkList, &ItemCheckListWidget::checkCompleted, this, [=](bool val){
        itemsChecked = val;
        saveVoucher();
    });


}
