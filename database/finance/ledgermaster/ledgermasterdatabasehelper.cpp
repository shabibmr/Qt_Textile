#include "ledgermasterdatabasehelper.h"
#include "ledgermasterdatabasehelper.h"

QString LedgerMasterDatabaseHelper::Ledger_Master_TableName = "Ledger_Master";
QString LedgerMasterDatabaseHelper::Ledger_Master_slno = "SL_NO";
QString LedgerMasterDatabaseHelper::Ledger_Master_LedgerID="Ledger_ID";
QString LedgerMasterDatabaseHelper::Ledger_Master_LedgerName="Ledger_Name";
QString LedgerMasterDatabaseHelper::Ledger_Master_LedgerGroupId="Group_Id";
QString LedgerMasterDatabaseHelper::Ledger_Master_openingBalance="Opening_Balance";
QString LedgerMasterDatabaseHelper::Ledger_Master_openingBalanceDate="Opening_Balance_Date";

QString LedgerMasterDatabaseHelper::Ledger_Master_closingBalance="Closing_Balance";     //calulate
QString LedgerMasterDatabaseHelper::Ledger_Master_totalTurnover="Turn_Over";      //calulate

QString LedgerMasterDatabaseHelper::Ledger_Master_LedgerType="Ledger_Type";
QString LedgerMasterDatabaseHelper::Ledger_Master_narration="Narration";
QString LedgerMasterDatabaseHelper::Ledger_Master_City="City";
QString LedgerMasterDatabaseHelper::Ledger_Master_Address="Address";
QString LedgerMasterDatabaseHelper::Ledger_Master_emailAddress="Email";
QString LedgerMasterDatabaseHelper::Ledger_Master_phoneNumber="Phone_Number";
QString LedgerMasterDatabaseHelper::Ledger_Master_mobileNumber="Mobile_Number";
QString LedgerMasterDatabaseHelper::Ledger_Master_website="Website";

QString LedgerMasterDatabaseHelper::Ledger_Master_ContactPersonName="Contact_Person_Name";
QString LedgerMasterDatabaseHelper::Ledger_Master_ContactPersonNumber="Contant_Person_Number";
QString LedgerMasterDatabaseHelper::Ledger_Master_PoBox="PoBox";
QString LedgerMasterDatabaseHelper::Ledger_Master_Country="Country";
QString LedgerMasterDatabaseHelper::Ledger_Master_TRN = "TRN";
QString LedgerMasterDatabaseHelper::Ledger_Master_Fax = "Fax";
QString LedgerMasterDatabaseHelper::Ledger_Master_ParentCompany = "ParentCompany";

QString LedgerMasterDatabaseHelper::Ledger_Master_Type_Of_Supply = "Type_Of_Supply";
QString LedgerMasterDatabaseHelper::Ledger_Master_Tax_Rate = "Tax_Rate";
QString LedgerMasterDatabaseHelper::Ledger_Master_Default_Tax_Ledger = "Default_Tax_Ledger";

QString LedgerMasterDatabaseHelper::Ledger_Master_defaultPriceList="Default_Price_List_Id";
QString LedgerMasterDatabaseHelper::Ledger_Master_DBName="DbName";
QString LedgerMasterDatabaseHelper::Ledger_Master_State = "State";
QString LedgerMasterDatabaseHelper::Ledger_Master_Birth_Date = "Birth_Date";
QString LedgerMasterDatabaseHelper::Ledger_Master_Credit_Period = "Credit_Period";
QString LedgerMasterDatabaseHelper::Ledger_Master_isIndividual = "isIndividual";

QString LedgerMasterDatabaseHelper::Ledger_Master_isFrequent = "isFrequent";
QString LedgerMasterDatabaseHelper::Ledger_Master_DiscountPercentage = "Discount_Percentage";
QString LedgerMasterDatabaseHelper::Ledger_Master_Credit_Limit= "Credit_Limit";

QString LedgerMasterDatabaseHelper::Ledger_Master_Last_Updated="Last_Modified";
QString LedgerMasterDatabaseHelper::Ledger_Master_Distance="Distance";
QString LedgerMasterDatabaseHelper::Ledger_Master_DeliveryRatePerKM="DeliveryRatePer";


LedgerMasterDatabaseHelper::LedgerMasterDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

LedgerMasterDatabaseHelper::~LedgerMasterDatabaseHelper()
{

}

int LedgerMasterDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int LedgerMasterDatabaseHelper::insertLedgerMasterItem(LedgerMasterDataModel *obj,int ContactId)
{

    //    this->startTransaction();
    qDebug()<<"Started insert ledger";
    QMap<QString,QVariant> map;

    if(obj->LedgerID ==""){

        obj->LedgerID = obj->LedgerGroupId +LoginValues::voucherPrefix + getMaxID();
    }
    map.insert(Ledger_Master_LedgerID,obj->LedgerID);
    map.insert(Ledger_Master_LedgerName,obj->LedgerName);
    map.insert(Ledger_Master_LedgerGroupId,obj->LedgerGroupId);
    map.insert(Ledger_Master_Address,obj->Address);
    map.insert(Ledger_Master_City,obj->City);
    map.insert(Ledger_Master_Country,obj->Country);
    map.insert(Ledger_Master_emailAddress,obj->emailAddress);
    map.insert(Ledger_Master_phoneNumber,obj->phoneNumber);
    map.insert(Ledger_Master_mobileNumber,obj->mobileNumber);
    map.insert(Ledger_Master_ContactPersonName,obj->ContactPersonName);
    map.insert(Ledger_Master_ContactPersonNumber,obj->ContactPersonNumber);
    map.insert(Ledger_Master_openingBalance,QString::number(obj->openingBalance,'f',2));
    map.insert(Ledger_Master_openingBalanceDate,obj->openingBalanceDate.toString("yyyy-MM-dd"));
    map.insert(Ledger_Master_closingBalance,QString::number(obj->closingBalance,'f',2));
    map.insert(Ledger_Master_totalTurnover,QString::number(obj->totalTurnover,'f',2));
    map.insert(Ledger_Master_emailAddress,obj->emailAddress);
    map.insert(Ledger_Master_defaultPriceList,obj->defaultPriceListID);
    map.insert(Ledger_Master_LedgerType,obj->LedgerType);
    map.insert(Ledger_Master_TRN,obj->TRN);
    map.insert(Ledger_Master_State,obj->State);
    map.insert(Ledger_Master_Credit_Period,QString::number(obj->Credit_Period));
    map.insert(Ledger_Master_isIndividual,QString::number(obj->isIndividual));
    map.insert(Ledger_Master_Birth_Date,obj->Birth_Date.toString("yyyy-MM-dd"));
    map.insert(Ledger_Master_Fax,obj->fax);
    map.insert(Ledger_Master_ParentCompany,obj->parentCompany);
    map.insert(Ledger_Master_Type_Of_Supply,obj->TypeOfSupply);
    map.insert(Ledger_Master_Tax_Rate,QString::number(obj->taxRate,'f',2));
    map.insert(Ledger_Master_Default_Tax_Ledger,obj->DefaultTaxLedger);
    map.insert(Ledger_Master_narration,obj->narration);
    map.insert(Ledger_Master_PoBox,obj->PoBox);
    map.insert(Ledger_Master_isFrequent, obj->isFrequent);
    map.insert(Ledger_Master_DiscountPercentage, obj->discountPercent);
    map.insert(Ledger_Master_Credit_Limit, obj->Credit_Limit);
    map.insert(Ledger_Master_Last_Updated, QDateTime::currentDateTime());



    this->startTransaction();

    if(db->insertRecordsPlus(map,Ledger_Master_TableName)){

        if(obj->openingBalance != 0){
            TransactionHelper *itemTransaction = new TransactionHelper();
            GeneralVoucherDataObject *transModel = new GeneralVoucherDataObject();
            LedgerMasterDataModel ledger;

            ledger.LedgerID = obj->LedgerID;
            ledger.amount = obj->openingBalance;
            if(obj->openingBalance >0){
                ledger.crAmount = obj->openingBalance;
            }
            else
                ledger.drAmount = -1 * obj->openingBalance;

            qDebug()<<Q_FUNC_INFO<<ledger.crAmount<<ledger.drAmount<<obj->openingBalance;

            //            ledger.AgainstLedger = ?

            transModel->ledgersList.append(ledger);

            transModel->voucherNumber = "0";
            transModel->VoucherPrefix = LoginValues::voucherPrefix;
            transModel->VoucherDate = obj->openingBalanceDate;
            transModel->voucherType = "OPENING BALANCE";
            transModel->narration = " Opening Balance";

            itemTransaction->insertTransaction(transModel);
        }


        if(ContactId>0){
            AddressBookDatabaseHelper* addHelper = new AddressBookDatabaseHelper;
            addHelper->updateLedgerID(obj->LedgerID,ContactId);

            if(obj->LedgerGroupId == "0x2x15" || obj->LedgerGroupId == "0x5x19"){
                BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
                billHelper->deleteBillwiseRecordsByLedger(obj->LedgerID);
                for(BillwiseRecordDataModel* a:obj->mapList){
                    //                    if(a->LedgerID.length()==0)
                    a->LedgerID = obj->LedgerID;
                    billHelper->insertBillwiseRecords(a);
                }
            }

        }
    }


    return commit();

}

int LedgerMasterDatabaseHelper::updateLedgerMasterItem(LedgerMasterDataModel *obj, QString oldID)
{
    QMap<QString,QVariant> map;
    map.insert(Ledger_Master_LedgerID,obj->LedgerID);
    map.insert(Ledger_Master_LedgerName,obj->LedgerName);
    map.insert(Ledger_Master_LedgerGroupId,obj->LedgerGroupId);
    map.insert(Ledger_Master_Address,obj->Address);
    map.insert(Ledger_Master_City,obj->City);
    map.insert(Ledger_Master_Country,obj->Country);
    map.insert(Ledger_Master_emailAddress,obj->emailAddress);
    map.insert(Ledger_Master_phoneNumber,obj->phoneNumber);
    map.insert(Ledger_Master_mobileNumber,obj->mobileNumber);
    map.insert(Ledger_Master_ContactPersonName,obj->ContactPersonName);
    map.insert(Ledger_Master_ContactPersonNumber,obj->ContactPersonNumber);
    map.insert(Ledger_Master_openingBalance,QString::number(obj->openingBalance,'f',2));
    map.insert(Ledger_Master_openingBalanceDate,obj->openingBalanceDate.toString("yyyy-MM-dd"));
    map.insert(Ledger_Master_closingBalance,QString::number(obj->closingBalance,'f',2));
    map.insert(Ledger_Master_totalTurnover,QString::number(obj->totalTurnover,'f',2));
    map.insert(Ledger_Master_emailAddress,obj->emailAddress);
    map.insert(Ledger_Master_defaultPriceList,obj->defaultPriceListID);
    map.insert(Ledger_Master_LedgerType,obj->LedgerType);
    map.insert(Ledger_Master_TRN,obj->TRN);
    map.insert(Ledger_Master_State,obj->State);
    map.insert(Ledger_Master_Credit_Period,QString::number(obj->Credit_Period));
    map.insert(Ledger_Master_isIndividual,QString::number(obj->isIndividual));
    map.insert(Ledger_Master_Birth_Date,obj->Birth_Date.toString("yyyy-MM-dd"));
    map.insert(Ledger_Master_Fax,obj->fax);
    map.insert(Ledger_Master_ParentCompany,obj->parentCompany);
    map.insert(Ledger_Master_Type_Of_Supply,obj->TypeOfSupply);
    map.insert(Ledger_Master_Tax_Rate,QString::number(obj->taxRate,'f',2));
    map.insert(Ledger_Master_Default_Tax_Ledger,obj->DefaultTaxLedger);
    map.insert(Ledger_Master_narration,obj->narration);
    map.insert(Ledger_Master_PoBox,obj->PoBox);
    map.insert(Ledger_Master_isFrequent, obj->isFrequent);
    map.insert(Ledger_Master_DiscountPercentage, obj->discountPercent);
    map.insert(Ledger_Master_Credit_Limit, obj->Credit_Limit);
    map.insert(Ledger_Master_Last_Updated, QDateTime::currentDateTime());

    QMap<QString,QVariant> map1;
    map1.insert(Ledger_Master_LedgerID,oldID);
    this->startTransaction();

    db->updateRecordsPlus(map,map1,Ledger_Master_TableName);


    TransactionHelper *itemTransaction = new TransactionHelper();
    GeneralVoucherDataObject *transModel = new GeneralVoucherDataObject();
    transModel->voucherNumber = "0";
    transModel->VoucherPrefix = LoginValues::voucherPrefix;
    transModel->voucherType = "OPENING BALANCE";

    QMap<QString,QVariant> mapDel;

    mapDel.insert(itemTransaction->Transaction_Helper_VoucherNo,"0");
    mapDel.insert(itemTransaction->Transaction_Helper_VoucherType,"OPENING BALANCE");
    mapDel.insert(itemTransaction->Transaction_Helper_VoucherPrefix,LoginValues::voucherPrefix);
    mapDel.insert(itemTransaction->Transaction_Helper_LedgerID,obj->LedgerID);

    db->deleteRecordsPlus(mapDel,itemTransaction->Transaction_Helper_TableName);

    if(obj->openingBalance != 0){
        LedgerMasterDataModel ledger;

        ledger.LedgerID = obj->LedgerID;
        ledger.amount = obj->openingBalance;
        if(obj->openingBalance >0){
            ledger.crAmount = obj->openingBalance;
        }
        else
            ledger.drAmount = -1 * obj->openingBalance;
        qDebug()<<Q_FUNC_INFO<<ledger.crAmount<<ledger.drAmount<<obj->openingBalance;

        //            ledger.AgainstLedger = ?

        transModel->ledgersList.append(ledger);

        transModel->VoucherDate = obj->openingBalanceDate;
        transModel->narration = " Opening Balance";

        itemTransaction->insertTransaction(transModel);
    }


    if(obj->LedgerGroupId == "0x2x15" || obj->LedgerGroupId == "0x5x19"){
        BillwiseRecordsDatabaseHelper* billHelper = new BillwiseRecordsDatabaseHelper;
        billHelper->deleteBillwiseRecordsByLedger(oldID);
        for(BillwiseRecordDataModel* a:obj->mapList){
            billHelper->insertBillwiseRecords(a);
        }
    }

    return commit();

}

int LedgerMasterDatabaseHelper::deleteLedgerMasterItem(QString oldID)
{
    QMap<QString,QVariant> map1;
    map1.insert(Ledger_Master_LedgerID,oldID);
    this->startTransaction();

    db->deleteRecordsPlus(map1,Ledger_Master_TableName);

    return commit();
}




int LedgerMasterDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        flag=true;
        return 0;
    }
}

int LedgerMasterDatabaseHelper::updateLedgerTypeByGroup(QString GroupId, QString LedgerType)
{
    QMap<QString,QVariant> map;
    map.insert(Ledger_Master_LedgerType,LedgerType);
    QMap<QString,QVariant> map1;
    map1.insert(Ledger_Master_LedgerGroupId,GroupId);

    db->updateRecordsPlus(map,map1,Ledger_Master_TableName);

}

QString LedgerMasterDatabaseHelper::getMaxID()
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QSqlQuery query(db);
    int id =0;
    QString qStr = "select max(sl_no) from "+Ledger_Master_TableName;
    query.exec(qStr);
    while(query.next()){
        id = query.value(0).toInt();
    }
    return QString::number(id+1);
}

QString LedgerMasterDatabaseHelper::getLedgerNameByID(QString id)
{
    if(id == "0"){
        return "";
    }
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select "+Ledger_Master_LedgerName+" from "+Ledger_Master_TableName+
            " where "+ Ledger_Master_LedgerID +" like :id";
    //  qDebug()<<qStr <<id;
    QSqlQuery query(db);
    QString name = "";

    query.prepare(qStr);
    query.bindValue(":id",id);
    query.exec();

    if(query.next()){
        name = query.value(0).toString();
    }

    return name;
}

QString LedgerMasterDatabaseHelper::getLedgerIDByName(QString ledgerName)
{
    QSqlDatabase db = QSqlDatabase::database(DatabaseValues::connectionString);
    QString qStr = "Select "+ Ledger_Master_LedgerID+" from "+Ledger_Master_TableName+
            " where "+ Ledger_Master_LedgerName +" like :id";
    //  qDebug()<<qStr <<id;
    QSqlQuery query(db);
    QString name = "";

    query.prepare(qStr);
    query.bindValue(":id",ledgerName);
    query.exec();

    if(query.next()){
        name = query.value(0).toString();
    }
    return name;
}

QList<LedgerMasterDataModel> LedgerMasterDatabaseHelper::getAllLedgers()
{
    QList<LedgerMasterDataModel> data;
    AccountGroupMasterDatabaseHelper *accHelper = new AccountGroupMasterDatabaseHelper();

    PricelistDatabaseHelper priceHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList +", "
            +Ledger_Master_mobileNumber +", "
            +Ledger_Master_PoBox+", "
            +Ledger_Master_narration+", "
            +Ledger_Master_Fax

            + " FROM "+ Ledger_Master_TableName;
    if(query.exec(qStr)){
        while(query.next()){
            LedgerMasterDataModel obj;

            obj.LedgerID = query.value(0).toString();
            obj.LedgerName =query.value(1).toString();
            obj.LedgerType = query.value(2).toString();
            obj.LedgerGroupId =query.value(3).toString();
            obj.LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj.narration =query.value(4).toString();
            obj.openingBalance = query.value(5).toFloat();
            obj.openingBalanceDate = query.value(6).toDate();
            obj.closingBalance = query.value(7).toFloat();
            obj.totalTurnover = query.value(8).toFloat();
            obj.phoneNumber = query.value(9).toString();
            obj.Address = query.value(10).toString();
            obj.City = query.value(11).toString();
            obj.Country = query.value(12).toString();
            obj.emailAddress = query.value(13).toString();
            obj.website = query.value(14).toString();
            obj.ContactPersonName = query.value(15).toString();
            obj.ContactPersonNumber = query.value(16).toString();
            obj.PoBox = query.value(17).toString();
            obj.TRN = query.value(18).toString();
            obj.taxRate = query.value(19).toFloat();
            obj.TypeOfSupply = query.value(20).toString();
            obj.DefaultTaxLedger = query.value(21).toString();
            obj.defaultPriceListID = PricelistDatabaseHelper::getPriceListNameByID(query.value(22).toString());
            obj.mobileNumber = query.value(23).toString();
            obj.PoBox = query.value(24).toString();
            obj.narration = query.value(25).toString();
            obj.fax = query.value(26).toString();

            qDebug()<<obj.fax;
            data.append(obj);
        }

    }
    else{
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }

    return data;
}

QSqlQueryModel *LedgerMasterDatabaseHelper::getAllLedgersModel()
{
    AccountGroupMasterDatabaseHelper accHelper;
    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + " as 'Ledger Id', "
            +Ledger_Master_LedgerName+ " as 'Ledger Name', "
            + "(SELECT " + accHelper.Accounts_Group_GroupName + " FROM " + accHelper.Accounts_Group_TableName
            + " WHERE " + accHelper.Accounts_Group_GroupId + " = LED." + Ledger_Master_LedgerGroupId + ") as 'Group' "
            + " FROM "+ Ledger_Master_TableName + " LED";

    QSqlQueryModel* model = new QSqlQueryModel;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;

}

QList<QSharedPointer<LedgerMasterDataModel>> LedgerMasterDatabaseHelper::getAllLedgersAsPtr()
{
    QList<QSharedPointer<LedgerMasterDataModel>> data;
    AccountGroupMasterDatabaseHelper *accHelper = new AccountGroupMasterDatabaseHelper();

    PricelistDatabaseHelper priceHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", led."
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList +", "
            + "(Select "+ accHelper->Accounts_Group_GroupName+ " from "+accHelper->Accounts_Group_TableName
            + " WHERE "+ accHelper->Accounts_Group_GroupId +"= led. "
            + Ledger_Master_LedgerGroupId
            + ") AS GroupName" +", "
            + "(Select "+ Ledger_Master_LedgerName+ " from "+Ledger_Master_TableName
            + " WHERE "+ Ledger_Master_LedgerID +"= led."
            + Ledger_Master_Default_Tax_Ledger
            + ") AS TaxLedger"+", "
            + "(Select "+ priceHelper.PriceListName+ " from "+priceHelper.PriceLists_TableName
            + " WHERE "+ priceHelper.PriceListID +"= led. "
            + Ledger_Master_defaultPriceList
            + ") AS PricelistName, "
            + Ledger_Master_isFrequent + ", "
            + Ledger_Master_DiscountPercentage+", "
            + Ledger_Master_mobileNumber
            + " FROM "+ Ledger_Master_TableName + " led "
            + " ORDER BY "+Ledger_Master_LedgerName;;
    //+ accHelper->Accounts_Group_TableName + " Acc";
    ///  qDebug()<<qStr;
    if(query.exec(qStr)){
        LedgerMasterDataModel *obj;
        qDebug()<<"Led Size from db helper = "<<query.size();
        while(query.next()){
            obj= new LedgerMasterDataModel();
            obj->LedgerID = query.value(0).toString();
            obj->LedgerName =query.value(1).toString();
            obj->LedgerType = query.value(2).toString();
            obj->LedgerGroupId =query.value(3).toString();
            obj->narration =query.value(4).toString();
            obj->openingBalance = query.value(5).toFloat();
            obj->openingBalanceDate = query.value(6).toDate();
            obj->closingBalance = query.value(7).toFloat();
            obj->totalTurnover = query.value(8).toFloat();
            obj->phoneNumber = query.value(9).toString();
            obj->Address = query.value(10).toString();
            obj->City = query.value(11).toString();
            obj->Country = query.value(12).toString();
            obj->emailAddress = query.value(13).toString();
            obj->website = query.value(14).toString();
            obj->ContactPersonName = query.value(15).toString();
            obj->ContactPersonNumber = query.value(16).toString();
            obj->PoBox = query.value(17).toString();
            obj->TRN = query.value(18).toString();
            obj->taxRate = query.value(19).toFloat();
            obj->TypeOfSupply = query.value(20).toString();
            obj->DefaultTaxLedger = query.value(24).toString();

            obj->defaultPriceListID = query.value(25).toString();
            obj->isFrequent = query.value(26).toBool();
            obj->discountPercent = query.value(27).toFloat();
            obj->mobileNumber = query.value(28).toString();
            obj->LedgerGroupName = query.value(23).toString();

            data.push_back(QSharedPointer<LedgerMasterDataModel>(obj));
        }
    }
    else{
        qDebug()<<"Ledger error301";
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return data;
}



QList<LedgerMasterDataModel> LedgerMasterDatabaseHelper::getAllLedgersUnderGroup(QString groupID)
{
    QList<LedgerMasterDataModel> data;
    PricelistDatabaseHelper priceHelper;
    AccountGroupMasterDatabaseHelper *accHelper= new AccountGroupMasterDatabaseHelper();
    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList + ", "
            +Ledger_Master_isFrequent + ", "
            +Ledger_Master_DiscountPercentage +", "
            + Ledger_Master_mobileNumber
            + " FROM "+ Ledger_Master_TableName
            + " WHERE "+ Ledger_Master_LedgerGroupId + "=:groupid";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    query.bindValue(":groupid",groupID);

    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            LedgerMasterDataModel obj;
            obj.LedgerID = query.value(0).toString();
            obj.LedgerName =query.value(1).toString();
            obj.LedgerType = query.value(2).toString();
            obj.LedgerGroupId =query.value(3).toString();
            obj.LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj.narration =query.value(4).toString();
            obj.openingBalance = query.value(5).toFloat();
            obj.openingBalanceDate = query.value(6).toDate();
            obj.closingBalance = query.value(7).toFloat();
            obj.totalTurnover = query.value(8).toFloat();
            obj.phoneNumber = query.value(9).toString();
            obj.Address = query.value(10).toString();
            obj.City = query.value(11).toString();
            obj.Country = query.value(12).toString();
            obj.emailAddress = query.value(13).toString();
            obj.website = query.value(13).toString();
            obj.ContactPersonName = query.value(13).toString();
            obj.ContactPersonNumber = query.value(14).toString();
            obj.PoBox = query.value(15).toString();
            obj.TRN = query.value(16).toString();
            obj.taxRate = query.value(17).toFloat();
            obj.TypeOfSupply = query.value(18).toString();
            obj.DefaultTaxLedger = query.value(19).toString();

            obj.defaultPriceListID = PricelistDatabaseHelper::getPriceListNameByID(query.value(20).toString());
            obj.isFrequent = query.value(21).toBool();
            obj.discountPercent = query.value(22).toFloat();
            obj.mobileNumber = query.value(23).toString();
            data.append(obj);
        }
    }

    return data;
}

QList<LedgerMasterDataModel> LedgerMasterDatabaseHelper::getAllLedgersUnderGroupRecursive(QString groupID)
{
    QList<LedgerMasterDataModel> data;
    PricelistDatabaseHelper priceHelper;
    AccountGroupMasterDatabaseHelper *accHelper= new AccountGroupMasterDatabaseHelper();
    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList + ", "
            + Ledger_Master_isFrequent + ", "
            + Ledger_Master_DiscountPercentage +", "
            + Ledger_Master_mobileNumber
            + " FROM "+ Ledger_Master_TableName
            + " WHERE "+ Ledger_Master_LedgerGroupId + " IN ";
    qStr += "(SELECT  GROUP_ID FROM    (SELECT * FROM  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  ORDER BY PARENT_ID, GROUP_ID) GROUPS_SORTED," ;
    qStr += " (SELECT @PV := '" + groupID + "') INITIALISATION WHERE FIND_IN_SET(PARENT_ID, @PV) "+
            " AND     LENGTH(@PV := CONCAT(@PV, ',', GROUP_ID))) AS ACC_GROUP";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    query.bindValue(":groupid",groupID);

    if(!query.exec()){
        qDebug()<<query.lastError();
    }
    else{
        while(query.next()){
            LedgerMasterDataModel obj;
            obj.LedgerID = query.value(0).toString();
            obj.LedgerName =query.value(1).toString();
            obj.LedgerType = query.value(2).toString();
            obj.LedgerGroupId =query.value(3).toString();
            obj.LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj.narration =query.value(4).toString();
            obj.openingBalance = query.value(5).toFloat();
            obj.openingBalanceDate = query.value(6).toDate();
            obj.closingBalance = query.value(7).toFloat();
            obj.totalTurnover = query.value(8).toFloat();
            obj.phoneNumber = query.value(9).toString();
            obj.Address = query.value(10).toString();
            obj.City = query.value(11).toString();
            obj.Country = query.value(12).toString();
            obj.emailAddress = query.value(13).toString();
            obj.website = query.value(13).toString();
            obj.ContactPersonName = query.value(13).toString();
            obj.ContactPersonNumber = query.value(14).toString();
            obj.PoBox = query.value(15).toString();
            obj.TRN = query.value(16).toString();
            obj.taxRate = query.value(17).toFloat();
            obj.TypeOfSupply = query.value(18).toString();
            obj.DefaultTaxLedger = query.value(19).toString();

            obj.defaultPriceListID = PricelistDatabaseHelper::getPriceListNameByID(query.value(20).toString());
            obj.isFrequent = query.value(21).toBool();
            obj.discountPercent = query.value(22).toFloat();
            obj.mobileNumber = query.value(23).toString();
            data.append(obj);
        }
    }

    return data;
}

QString LedgerMasterDatabaseHelper::getAllLedgerIDsUnderGroupRecursiveAsString(QString groupID)
{
    QString data;
    PricelistDatabaseHelper priceHelper;

    QString qStr = " ";
    qStr += " select ledger.ledger_id FROM " +Ledger_Master_TableName + " ledger inner join ";
    qStr += " (with recursive cte (group_id, group_name, parent_id, ledger_id) as ";
    qStr += " ( select grp.group_id,grp.group_name,grp.parent_id, ledger.ledger_id FROM " + Ledger_Master_TableName + "";
    qStr += " ledger left outer join  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  grp on ledger.group_id = grp.group_id union all ";
    qStr += " select p.group_id, p.group_name,p.parent_id, ledger_id from  " + AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +"  p inner ";
    qStr += "  join cte on p.group_id = cte.parent_id ) select cte.group_name, cte.ledger_id,";
    qStr += " cte.group_id from cte where cte.parent_id = :groupid ) ledgerGrp1 on ";
    qStr += " ledgerGrp1.ledger_id = ledger.ledger_id union select ledger2.ledger_id FROM " + Ledger_Master_TableName + " ledger2 ";
    qStr += " where ledger2.group_id = :groupid ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    qDebug()<<"Recusive : "<<qStr << groupID;
    query.bindValue(":groupid",groupID);

    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){
            data+= "'"+query.value(0).toString()+"',";
        }
    }

    data.truncate(data.lastIndexOf(","));

    return data;
}

QList<LedgerMasterDataModel> LedgerMasterDatabaseHelper::getAllLedgersUnderGroupByName(QString groupName)
{


    QList<LedgerMasterDataModel> data;
    PricelistDatabaseHelper priceHelper;
    AccountGroupMasterDatabaseHelper *accHelper = new AccountGroupMasterDatabaseHelper();
    QString groupID =accHelper->getGroupIDByName(groupName);
    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList+ ", "
            + Ledger_Master_isFrequent + ", "
            + Ledger_Master_DiscountPercentage +", "
            + Ledger_Master_mobileNumber
            + " FROM "+ Ledger_Master_TableName
            + " WHERE "+ Ledger_Master_LedgerGroupId + "=:groupid";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    query.bindValue(":groupid",groupID);
    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<qStr;
        qDebug()<<groupID;
    }
    else{
        while(query.next()){
            LedgerMasterDataModel obj;

            obj.LedgerID = query.value(0).toString();
            obj.LedgerName =query.value(1).toString();
            obj.LedgerType = query.value(2).toString();
            obj.LedgerGroupId =query.value(3).toString();
            obj.LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj.narration =query.value(4).toString();
            obj.openingBalance = query.value(5).toFloat();
            obj.openingBalanceDate = query.value(6).toDate();
            obj.closingBalance = query.value(7).toFloat();
            obj.totalTurnover = query.value(8).toFloat();
            obj.phoneNumber = query.value(9).toString();
            obj.Address = query.value(10).toString();
            obj.City = query.value(11).toString();
            obj.Country = query.value(12).toString();
            obj.emailAddress = query.value(13).toString();
            obj.website = query.value(13).toString();
            obj.ContactPersonName = query.value(13).toString();
            obj.ContactPersonNumber = query.value(14).toString();
            obj.PoBox = query.value(15).toString();
            obj.TRN = query.value(16).toString();
            obj.taxRate = query.value(17).toFloat();
            obj.TypeOfSupply = query.value(18).toString();
            obj.DefaultTaxLedger = query.value(19).toString();
            obj.defaultPriceListID = PricelistDatabaseHelper::getPriceListNameByID(query.value(20).toString());
            obj.isFrequent = query.value(21).toBool();
            obj.discountPercent = query.value(22).toFloat();
            obj.mobileNumber = query.value(23).toString();
            data.append(obj);
        }
    }

    return data;
}

LedgerMasterDataModel LedgerMasterDatabaseHelper::getLastInsertedLedger()
{
    LedgerMasterDataModel obj;
    AccountGroupMasterDatabaseHelper *accHelper =new AccountGroupMasterDatabaseHelper;
    PricelistDatabaseHelper *priceHelper = new PricelistDatabaseHelper;
    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList+ ", "
            + Ledger_Master_isFrequent + ", "
            + Ledger_Master_DiscountPercentage +", "

            + " FROM "+ Ledger_Master_TableName
            + " ORDER BY "+ Ledger_Master_slno + " DESC LIMIT 1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);



    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            obj.LedgerID = query.value(0).toString();
            obj.LedgerName =query.value(1).toString();
            obj.LedgerType = query.value(2).toString();
            obj.LedgerGroupId =query.value(3).toString();
            obj.LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj.narration =query.value(4).toString();
            obj.openingBalance = query.value(5).toFloat();
            obj.openingBalanceDate = query.value(6).toDate();
            obj.closingBalance = query.value(7).toFloat();
            obj.totalTurnover = query.value(8).toFloat();
            obj.phoneNumber = query.value(9).toString();
            obj.Address = query.value(10).toString();
            obj.City = query.value(11).toString();
            obj.Country = query.value(12).toString();
            obj.emailAddress = query.value(13).toString();
            obj.website = query.value(14).toString();
            obj.ContactPersonName = query.value(15).toString();
            obj.ContactPersonNumber = query.value(16).toString();
            obj.PoBox = query.value(17).toString();
            obj.TRN = query.value(18).toString();
            obj.taxRate = query.value(19).toFloat();
            obj.TypeOfSupply = query.value(20).toString();
            obj.DefaultTaxLedger = query.value(21).toString();

            obj.defaultPriceListID = PricelistDatabaseHelper::getPriceListNameByID(query.value(22).toString());
            obj.isFrequent = query.value(23).toBool();
            obj.discountPercent = query.value(24).toFloat();
            obj.mobileNumber = query.value(25).toString();


        }
    }
    return obj;

}

QSqlQueryModel* LedgerMasterDatabaseHelper::getLedgerQueryString(QStringList groupList)
{
    AccountGroupMasterDatabaseHelper accHlper;
    QString qStr = "SELECT ";
    qStr += Ledger_Master_LedgerID + ", ";
    qStr += Ledger_Master_LedgerName +", ";
    qStr += Ledger_Master_LedgerGroupId+", ";
    qStr += "(SELECT "+accHlper.Accounts_Group_GroupName+" FROM ";
    qStr += AccountGroupMasterDatabaseHelper::Accounts_Group_TableName +" agm ";
    qStr += " WHERE agm."+accHlper.Accounts_Group_GroupId +"=lm.";
    qStr += Ledger_Master_LedgerGroupId+")";
    qStr +=  " FROM "+ Ledger_Master_TableName +" lm ";

    //    if(groupID.length()>0)
    //        qStr= " WHERE "+ Ledger_Master_LedgerGroupId + "='"+groupID+"'";

    if(groupList.size()>0){
        QString grpList ;
        for(QString groupID:groupList)
            grpList += "'" + groupID + "',";
        grpList.truncate(grpList.lastIndexOf(QChar(',')));

        qStr += " WHERE " + Ledger_Master_LedgerGroupId + " IN (" + grpList +")";
    }
    qStr += " ORDER BY "+Ledger_Master_LedgerName ;


    QSqlQueryModel* model = new QSqlQueryModel;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

QSqlQueryModel *LedgerMasterDatabaseHelper::getLedgersUnderGroups(QStringList groupList)
{
    QString grpList ;
    for(QString groupID:groupList)
        grpList += "'" + groupID + "',";
    grpList.truncate(grpList.lastIndexOf(QChar('\'')));
    grpList += "'";

    AccountGroupMasterDatabaseHelper* accHelper = new AccountGroupMasterDatabaseHelper();

    QString qStr = "SELECT ";
    qStr += "led." + Ledger_Master_LedgerID + ", ";
    qStr += "led." + Ledger_Master_LedgerName +", ";
    qStr += "led." + Ledger_Master_LedgerGroupId+" ";
    qStr +=  " FROM "+ Ledger_Master_TableName + " led ";
    qStr += " inner join ";
    qStr += " ( with recursive cte ( " +  accHelper->Accounts_Group_GroupId + ", "  ;
    qStr += accHelper->Accounts_Group_GroupName + ", "  ;
    qStr += accHelper->Accounts_Group_ParentGroupID + ", " + Ledger_Master_LedgerID + ") as (";
    qStr += " select grp." + accHelper->Accounts_Group_GroupId + ", grp." + accHelper->Accounts_Group_GroupName + ", ";
    qStr += " grp." + accHelper->Accounts_Group_ParentGroupID + ", ledger." + Ledger_Master_LedgerID;
    qStr += " from " + Ledger_Master_TableName + " ledger left outer join " + accHelper->Accounts_Group_TableName + " grp ";
    qStr += "  on ledger." + Ledger_Master_LedgerGroupId + " = grp." + accHelper->Accounts_Group_GroupId;
    qStr += " union all " ;
    qStr += " select p." + accHelper->Accounts_Group_GroupId + ", p." + accHelper->Accounts_Group_GroupName + ", ";
    qStr += " p." + accHelper->Accounts_Group_ParentGroupID + ", '' as " + Ledger_Master_LedgerID;
    qStr += " from " + accHelper->Accounts_Group_TableName + " p inner join cte ";
    qStr += " on p." + accHelper->Accounts_Group_GroupId + " = cte." + accHelper->Accounts_Group_ParentGroupID + " )" ;
    qStr += " select cte." + accHelper->Accounts_Group_GroupName + ", cte." + Ledger_Master_LedgerID;
    qStr += " from cte where cte." + accHelper->Accounts_Group_GroupId + " in (" + grpList + ")) groups ";
    qStr += " ON led." + Ledger_Master_LedgerID + " = groups." + Ledger_Master_LedgerID;
    qStr += " ORDER BY "+Ledger_Master_LedgerName ;

    QSqlQueryModel* model = new QSqlQueryModel;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;

}

QSqlQueryModel *LedgerMasterDatabaseHelper::getLedForModeofPay()
{
    QString qStr;

    qStr = " SELECT ";
    qStr += Ledger_Master_LedgerID +",";
    qStr += Ledger_Master_LedgerName +",";
    qStr += Ledger_Master_DiscountPercentage +", ";
    qStr += " COALESCE("+Ledger_Master_defaultPriceList +",0), ";
    qStr += Ledger_Master_Credit_Limit ;
    qStr += " FROM " + Ledger_Master_TableName;
    qStr += " WHERE "+ Ledger_Master_isFrequent +" =1";
    qStr += " ORDER BY "+Ledger_Master_slno;

    QSqlQueryModel* model = new QSqlQueryModel;
    model = DatabaseHelper::getQueryModel(qStr);
    qDebug()<<qStr;

    return model;
}

QSqlQueryModel *LedgerMasterDatabaseHelper::getLedgerQueryModel(QString groupID)
{
    QString qStr = "SELECT ";
    qStr += Ledger_Master_LedgerID + ", ";
    qStr += Ledger_Master_LedgerName +", ";
    qStr += Ledger_Master_LedgerGroupId+" ";
    qStr +=  " FROM "+ Ledger_Master_TableName;
    qStr += " ORDER BY "+Ledger_Master_LedgerName ;

    if(groupID.length()>0)
        qStr= " WHERE "+ Ledger_Master_LedgerGroupId + "='"+groupID+"'";



    QSqlQueryModel* model = new QSqlQueryModel;
    model = DatabaseHelper::getQueryModel(qStr);

    return model;
}

void LedgerMasterDatabaseHelper::updateOpeningValue(QString ledgerID, float OpeningValue)
{
    QMap<QString,QVariant> map;
    QMap<QString,QVariant> clause;
    clause.insert(Ledger_Master_LedgerID,ledgerID);
    map.insert(Ledger_Master_openingBalance,OpeningValue);
    db->updateRecordsPlus(map,clause,Ledger_Master_TableName);
}

QStringList LedgerMasterDatabaseHelper::getLedgersListHavingDbName()
{
    QStringList ledgerNames;
    QString qStr = " SELECT " + Ledger_Master_LedgerName + " FROM " + Ledger_Master_TableName ;
    qStr += " WHERE " + Ledger_Master_DBName + " is not NULL AND length(" + Ledger_Master_DBName + ") >0 ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            ledgerNames.append(query.value(0).toString());
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return ledgerNames;

}

QStringList LedgerMasterDatabaseHelper::getDbNamesList()
{
    QStringList dbNames;
    QString qStr = " SELECT " + Ledger_Master_DBName + " FROM " + Ledger_Master_TableName ;
    qStr += " WHERE " + Ledger_Master_DBName + " is not NULL AND length(" + Ledger_Master_DBName + ") >0 ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(query.exec()){
        while(query.next()){
            dbNames.append(query.value(0).toString());
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return dbNames;

}

QString LedgerMasterDatabaseHelper::getDbNameByLedgerName(QString ledgerName)
{
    QString dbName;
    QString qStr = " SELECT " + Ledger_Master_DBName  + " FROM " + Ledger_Master_TableName ;
    qStr += " WHERE " + Ledger_Master_LedgerName + " = '" + ledgerName + "' ";

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);
    if(query.exec()){
        if(query.first()){
            dbName = (query.value(0).toString());
        }
    }

    else{
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }

    return dbName;
}



LedgerMasterDataModel LedgerMasterDatabaseHelper::getLedgerObjectByID(QString id)
{

    LedgerMasterDataModel obj;
    AccountGroupMasterDatabaseHelper *accHelper =new AccountGroupMasterDatabaseHelper;
    PricelistDatabaseHelper *priceHelper = new PricelistDatabaseHelper;

    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList + ", "
            +Ledger_Master_DBName+ ", "
            + Ledger_Master_isFrequent + ", "
            + Ledger_Master_DiscountPercentage +", "
            + Ledger_Master_Credit_Limit +", "
            + Ledger_Master_mobileNumber
            + " FROM "+ Ledger_Master_TableName
            + " WHERE "+ Ledger_Master_LedgerID + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    query.bindValue(":id",id);

    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            obj.LedgerID = query.value(0).toString();
            obj.LedgerName =query.value(1).toString();
            obj.LedgerType = query.value(2).toString();
            obj.LedgerGroupId = query.value(3).toString();
            obj.LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj.narration =query.value(4).toString();
            obj.openingBalance = query.value(5).toFloat();
            obj.openingBalanceDate = query.value(6).toDate();
            obj.closingBalance = query.value(7).toFloat();
            obj.totalTurnover = query.value(8).toFloat();
            obj.phoneNumber = query.value(9).toString();
            obj.Address = query.value(10).toString();
            obj.City = query.value(11).toString();
            obj.Country = query.value(12).toString();
            obj.emailAddress = query.value(13).toString();
            obj.website = query.value(14).toString();
            obj.ContactPersonName = query.value(15).toString();
            obj.ContactPersonNumber = query.value(16).toString();
            obj.PoBox = query.value(17).toString();
            obj.TRN = query.value(18).toString();
            obj.taxRate = query.value(19).toFloat();
            obj.TypeOfSupply = query.value(20).toString();
            obj.DefaultTaxLedger = query.value(21).toString();

            obj.defaultPriceListID = PricelistDatabaseHelper::getPriceListNameByID(query.value(22).toString());
            //            obj.mapList = billHelper->getBillMappingsByLedger(query.value(0).toString(), QDate::currentDate());
            obj.dbName = query.value(23).toString();
            obj.isFrequent = query.value(24).toBool();
            obj.discountPercent = query.value(25).toFloat();
            obj.Credit_Limit = query.value(26).toFloat();
            obj.mobileNumber = query.value(27).toString();
            //qDebug()<<"query size ledger"<<query.record().count();
        }
    }

    //    qDebug()<<"OBJ nme : "<<obj.LedgerName;
    return obj;

}

LedgerMasterDataModel *LedgerMasterDatabaseHelper::getLedgerPtrById(QString id)
{
    LedgerMasterDataModel *obj = new LedgerMasterDataModel();
    AccountGroupMasterDatabaseHelper *accHelper =new AccountGroupMasterDatabaseHelper;
    PricelistDatabaseHelper *priceHelper = new PricelistDatabaseHelper;
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;

    QString qStr = "SELECT "
            +Ledger_Master_LedgerID + ", "
            +Ledger_Master_LedgerName+ ", "
            +Ledger_Master_LedgerType+ ", "
            +Ledger_Master_LedgerGroupId+ ", "
            +Ledger_Master_narration+", "
            +Ledger_Master_openingBalance+", "
            +Ledger_Master_openingBalanceDate+", "
            +Ledger_Master_closingBalance+", "
            +Ledger_Master_totalTurnover+", "
            +Ledger_Master_phoneNumber+", "
            +Ledger_Master_Address+", "
            +Ledger_Master_City+", "
            +Ledger_Master_Country+", "
            +Ledger_Master_emailAddress+", "
            +Ledger_Master_website+", "
            +Ledger_Master_ContactPersonName +", "
            +Ledger_Master_ContactPersonNumber +", "
            +Ledger_Master_PoBox +", "
            +Ledger_Master_TRN +", "
            +Ledger_Master_Tax_Rate +", "
            +Ledger_Master_Type_Of_Supply +", "
            +Ledger_Master_Default_Tax_Ledger +", "
            +Ledger_Master_defaultPriceList+ ", "
            + Ledger_Master_isFrequent + ", "
            + Ledger_Master_DiscountPercentage +", "
            + Ledger_Master_mobileNumber+", "
            + Ledger_Master_Credit_Limit

            + " FROM "+ Ledger_Master_TableName
            + " WHERE "+ Ledger_Master_LedgerID + "=:id";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);


    query.bindValue(":id",id);

    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
    }
    else{
        while(query.next()){

            obj->LedgerID = query.value(0).toString();
            obj->LedgerName =query.value(1).toString();
            obj->LedgerType = query.value(2).toString();
            obj->LedgerGroupId =query.value(3).toString();
            obj->LedgerGroupName = accHelper->getGroupNameByID(query.value(3).toString());
            obj->narration =query.value(4).toString();
            obj->openingBalance = query.value(5).toFloat();
            obj->openingBalanceDate = query.value(6).toDate();
            obj->closingBalance = query.value(7).toFloat();
            obj->totalTurnover = query.value(8).toFloat();
            obj->phoneNumber = query.value(9).toString();
            obj->Address = query.value(10).toString();
            obj->City = query.value(11).toString();
            obj->Country = query.value(12).toString();
            obj->emailAddress = query.value(13).toString();
            obj->website = query.value(14).toString();
            obj->ContactPersonName = query.value(15).toString();
            obj->ContactPersonNumber = query.value(16).toString();
            obj->PoBox = query.value(17).toString();
            obj->TRN = query.value(18).toString();
            obj->taxRate = query.value(19).toFloat();
            obj->TypeOfSupply = query.value(20).toString();
            obj->DefaultTaxLedger = query.value(21).toString();

            obj->defaultPriceListID = query.value(22).toString();
            obj->isFrequent = query.value(23).toBool();
            obj->discountPercent = query.value(24).toFloat();
            obj->mobileNumber = query.value(25).toString();
            obj->Credit_Limit = query.value(26).toFloat();

            //            obj.mapList = billHelper->getBillMappingsByLedger(query.value(0).toString(), QDate::currentDate());
//            obj->mapList = billHelper->getOpeningBalanceMapping(obj->LedgerID);
            if(obj->mapList.size() > 1 || ( obj->mapList.size() ==1 && obj->mapList[0]->MethodOfAdjustment != "ON ACC")){
                obj->hasBillwiseMappings = true;
                qDebug()<<"Ledger has billwise mappings for opening balance";
            }
            else
                qDebug()<<"No billwise mappings";

        }
    }

    return obj;

}

QMap<QString, QString> LedgerMasterDatabaseHelper::getLedgersAsMap()
{
    QMap<QString,QString> map;


    return map;
}
