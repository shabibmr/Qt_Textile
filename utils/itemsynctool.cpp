#include "itemsynctool.h"
#include "ui_itemsynctool.h"

ItemSyncTool::ItemSyncTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemSyncTool)
{
    ui->setupUi(this);

    itemHelper = new SalesInventoryItemDatabaseHelper();

    //    selectCurrentDb();
    setWidgets();
}

ItemSyncTool::~ItemSyncTool()
{
    delete ui;
}

bool ItemSyncTool::selectCurrentDb()
{
    CompanySelectorWidget *compSel = new CompanySelectorWidget(this);
    compSel->setWindowTitle("Select Branch to sync");
    connect(compSel, &CompanySelectorWidget::companySwitched, this, [=](QString val){
        currentDb = val;
        return true;
    });
    compSel->showMaximized();

}

bool ItemSyncTool::selectRefdb()
{
    CompanySelectorWidget *compSel = new CompanySelectorWidget(this);
    compSel->setWindowTitle("Select Reference Branch");
//    compSel->setSwitchDB(false);
    connect(compSel, &CompanySelectorWidget::companySwitched, this, [=](QString val){
        refDB = val;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<currentDb<<refDB;
        return true;

    });
    compSel->show();
}

void ItemSyncTool::syncMissingItems()
{
    //    selectCurrentDb();
    //    if(selectRefdb()){

    qDebug()<<Q_FUNC_INFO<<__LINE__<<currentDb<<refDB;

    missingItems= itemHelper->getMissingItemsFromRefDb(refDB, currentDb);

    if(missingItems.size() > 0){
        QMessageBox::StandardButton reply = QMessageBox::Yes;
        reply = QMessageBox::question(this, "Sync items? ", QString::number(missingItems.size()) +
                                      " new items found. Do you want to add them?",
                                      QMessageBox::Yes|QMessageBox::No);

        if(reply==QMessageBox::Yes)
        {

            showItemEditor();
        }
    }

    else {
        showMessage("No missing items found. ");
    }
    //    }

}

void ItemSyncTool::syncItemIdsWithDiffNames()
{
    QSqlQueryModel *model = itemHelper->getSameItemIdForDiffItemsBetweenDbs(currentDb, refDB);
    if(model->rowCount() > 0){
        for (int i=0; i<model->rowCount(); i++) {
            QString currName = model->record(i).value(2).toString();
            QString refName = model->record(i).value(0).toString();
            QString currId = model->record(i).value(3).toString();
            QString refId = model->record(i).value(1).toString();

            // ask user if they're the same items
            QMessageBox::StandardButton reply = QMessageBox::Yes;
            reply = QMessageBox::question(this, "Sync items? ",  " Found : ? "
                                                                 "\n  Current name   : " + currName +
                                          " \n Reference name : " + refName +
                                          " \n Are these the same items? ",
                                          QMessageBox::Yes|QMessageBox::No);
            // if yes, rename in master only
            if(reply==QMessageBox::Yes){
                if(itemHelper->updateItemName(model->record(i).value(0).toString() , model->record(i).value(1).toString())){
                    showMessage("Item name updated ");
                }
            }
            // if no, check if same name exists in ref db
            else if(reply==QMessageBox::No){
                QString itemID = itemHelper->checkItemNameInRefDb(model->record(i).value(2).toString(), refDB);
                // if yes, step 3
                if(itemID.length() > 0 && itemID != "0"){
                    showMessage("Item name exists in ref Db with id " + itemID +" . complete step 3 for " + currName + " and come back. ");
                }
                // if no, step 1
                else {
                    QString newId = itemHelper->createItemID(itemHelper->getInventoryItemByID(currId).GroupID);
                    if(itemHelper->updateItemIdInMasters(currId, newId))
                        if(itemHelper->updateItemIdInTrans(currId, newId))

                            showMessage("New Id generated for " + currName + " - " + newId +
                                        "\n complete step 3 for " + refName + " and come back.");


                }
            }

        }
    }

    else{
        showMessage("No items found");
    }
}

void ItemSyncTool::removeDuplicates()
{
    dupsList = itemHelper->getDuplicateItems();
    if(dupsList.size() > 0){
        showDuplicateDialog();

    }
    else {
        showMessage("No duplicates found. ");
    }
}

void ItemSyncTool::syncItemsWithSameName()
{


    QSqlQueryModel *model = itemHelper->getDuplicateItemsBetweenDbs(currentDb, refDB);
    if(model->rowCount() > 0){
        for (int i=0; i<model->rowCount(); i++) {
            QMessageBox::StandardButton reply = QMessageBox::Yes;
            reply = QMessageBox::question(this, "Sync items? ",  " Duplicates found!"
                                                                 "\n  Current name   : " + model->record(i).value(2).toString() +
                                          " \n Reference name : " + model->record(i).value(0).toString() +
                                          " \n Keep Reference item? ",
                                          QMessageBox::Yes|QMessageBox::No);

            if(reply==QMessageBox::Yes)
            {
                if( !itemHelper->checkItemExists(model->record(i).value(1).toString())){
                    itemHelper->updateItemIdInTrans(model->record(i).value(3).toString(),
                                                    model->record(i).value(1).toString());
                    itemHelper->updateItemIdInMasters(model->record(i).value(3).toString(),
                                                      model->record(i).value(1).toString());
                }
                else {

                    // item id already exists for diff item name. go to step 4
                    showMessage("Item id " + model->record(i).value(1).toString() +
                                " already exists. Complete option 4 and come back ");
                }
            }

        }
    }

    else {
        showMessage("No items found");
    }
}

void ItemSyncTool::selectOption1()
{
    CompanySelectorWidget *compSel = new CompanySelectorWidget(this);
    compSel->setWindowTitle("Select Reference Branch");
//    compSel->setSwitchDB(false);
    connect(compSel, &CompanySelectorWidget::companySwitched, this, [=](QString val){
        refDB = val;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<currentDb<<refDB;
        syncMissingItems();

    });
    compSel->show();


}

void ItemSyncTool::selectOption2()
{
    removeDuplicates();
}

void ItemSyncTool::selectOption3()
{
    CompanySelectorWidget *compSel = new CompanySelectorWidget(this);
    compSel->setWindowTitle("Select Reference Branch");
//    compSel->setSwitchDB(false);
    connect(compSel, &CompanySelectorWidget::companySwitched, this, [=](QString val){
        refDB = val;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<currentDb<<refDB;
        syncItemsWithSameName();

    });
    compSel->show();
}

void ItemSyncTool::selectOption4()
{
    CompanySelectorWidget *compSel = new CompanySelectorWidget(this);
    compSel->setWindowTitle("Select Reference Branch");
//    compSel->setSwitchDB(false);
    connect(compSel, &CompanySelectorWidget::companySwitched, this, [=](QString val){
        refDB = val;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<currentDb<<refDB;
        syncItemIdsWithDiffNames();

    });
    compSel->show();
}

void ItemSyncTool::showMessage(QString text)
{
    QMessageBox box;
    box.setText(text);
    box.exec();
}

void ItemSyncTool::showItemEditor()
{
    AddItems *additemWidget = new AddItems(this);
    additemWidget->setWindowFlags(Qt::Window);
    additemWidget->setAttribute(Qt::WA_DeleteOnClose);
    additemWidget->setItem(missingItems[0]);
    connect(additemWidget, &AddItems::closing, this, [=](){
        missingItems.removeAt(0);
        if(missingItems.size() > 0)
            showItemEditor();
        else {
            showMessage("Completed all items");
        }
    });
    additemWidget->edit();
    additemWidget->showMaximized();

}

void ItemSyncTool::showDuplicateDialog()
{
    QMapIterator<int, QList<QPair<QString, QString> > >i(dupsList);

    i.next();
    QList<QPair<QString, QString> > itemsList = i.value();

    DuplicateItemsDialog *dialog = new DuplicateItemsDialog;
    for(QPair<QString, QString> pair: itemsList){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<pair.first<<pair.second;
        dialog->addItemToLayout(pair.first, pair.second);
    }
    connect(dialog, &DuplicateItemsDialog::itemSelected, this, [=](QString keepID){
        for(QPair<QString, QString> pair: itemsList){
            QString oldID = pair.first;
            if(oldID != keepID){
                if(itemHelper->updateItemIdInTrans(oldID, keepID))
                    itemHelper->deleteItemFromMasters(oldID);
            }

            dupsList.remove(i.key());
            if(i.hasNext())
                showDuplicateDialog();
        }
    });

    dialog->show();

}

void ItemSyncTool::setWidgets()
{
    //    QtMaterialFlatButton *select = new QtMaterialFlatButton("Select Branch");
    //    connect(select, &QtMaterialFlatButton::clicked, this,[=](){
    ////        selectCurrentDb();
    //        select->setText("Branch selected - " + currentDb);
    //    });


    QtMaterialFlatButton *opt1 = new QtMaterialFlatButton("Copy missing items");
    connect(opt1, &QtMaterialFlatButton::clicked, this, &ItemSyncTool::selectOption1);

    QtMaterialFlatButton *opt2 = new QtMaterialFlatButton("Remove duplicates");
    connect(opt2, &QtMaterialFlatButton::clicked, this, &ItemSyncTool::selectOption2);

    QtMaterialFlatButton *opt3 = new QtMaterialFlatButton("Sync similar names");
    connect(opt3, &QtMaterialFlatButton::clicked, this, &ItemSyncTool::selectOption3);

    QtMaterialFlatButton *opt4 = new QtMaterialFlatButton("Sync item IDs");
    connect(opt4, &QtMaterialFlatButton::clicked, this, &ItemSyncTool::selectOption4);

    //    ui->verticalLayout->addWidget(select);
    ui->verticalLayout->addWidget(opt1);
    ui->verticalLayout->addWidget(opt2);
    ui->verticalLayout->addWidget(opt3);
    ui->verticalLayout->addWidget(opt4);

}
