#include "addconversion.h"
#include "ui_addconversion.h"
#include <QMessageBox>

addConversion::addConversion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addConversion)
{
    ui->setupUi(this);
    this->setWindowTitle("Add UOM Conversion");
    QString bgcolor = "#e0f7fa";
   this->setObjectName("background");
   this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    ui->multipleLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));

    dbHelper = new UomDataBaseHelper();
    map = dbHelper->getUOMForSearch();

    QStringList UOMList;
    for(int i:map.keys()){
        ui->unitComboBox->addItem(map.value(i), i);

        UOMList<<map.value(i);
    }
    QCompleter *UOMCompleter = new QCompleter(UOMList,this);
    UOMCompleter->setFilterMode(Qt::MatchContains);
    UOMCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    UOMCompleter->setCompletionMode(QCompleter::PopupCompletion);

//    ui->subUnitComboBox->setEnabled(true);

    ui->unitComboBox->setCompleter(UOMCompleter);

    GMSearchLineEdit* UOMLine = new GMSearchLineEdit;
    UOMLine->setPlaceholderText("Search...");
    UOMLine->setData(map.values());
    ui->unitComboBox->setLineEdit(UOMLine);

    for(int j:map.keys()){
        ui->subUnitComboBox->addItem(map.value(j), j);

        UOMList<<map.value(j);
    }

    QCompleter *UOMCompleter1 = new QCompleter(UOMList,this);
    UOMCompleter1->setFilterMode(Qt::MatchContains);
    UOMCompleter1->setCaseSensitivity(Qt::CaseInsensitive);
    UOMCompleter1->setCompletionMode(QCompleter::PopupCompletion);

    ui->subUnitComboBox->setCompleter(UOMCompleter1);

    GMSearchLineEdit* UOMLine1 = new GMSearchLineEdit;
    UOMLine1->setPlaceholderText("Search...");
    UOMLine1->setData(map.values());
    ui->subUnitComboBox->setLineEdit(UOMLine1);


}

addConversion::~addConversion()
{
    delete ui;
}

void addConversion::setBaseUnit(int baseUnit, QString Item, QString ItemCode)
{
    invItem = Item;
    itemCode = ItemCode;
    ui->subUnitComboBox->setCurrentIndex(baseUnit);
    ui->subUnitComboBox->setEditable(false);
    ui->subUnitComboBox->setEnabled(false);

    ui->unitComboBox->removeItem(baseUnit);
}

void addConversion::setData(UomConversionDataModel *obj, QString code)
{
    qDebug()<<"conversion set data called";

    itemCode = code;

    ui->subUnitComboBox->setCurrentText(obj->BaseUnit);
    ui->unitComboBox->setCurrentText(obj->ToUnit);
    ui->multipleLineEdit->setText(QString::number(obj->conValue,'f',3));
    ui->narrationLineEdit->setText(obj->Narration);
    ui->barCodeLineEdit->setText(obj->barCode);
    convId = obj->_id;
}

void addConversion::on_buttonBox_accepted()
{
    UomConversionDataModel* uom = new UomConversionDataModel();
    qDebug()<< "Base Unit"<<ui->unitComboBox->currentData();
    uom->_id = convId;
    uom->itemID = invItem;
    uom->BaseUnit = ui->subUnitComboBox->currentData().toString();
    uom->ToUnit = ui->unitComboBox->currentData().toString();
    uom->conValue = ui->multipleLineEdit->text().toFloat();
    uom->Narration = ui->narrationLineEdit->text();
    uom->barCode = ui->barCodeLineEdit->text();
//    dbHelper->insertUOMConversion(item);

    if(dbHelper->checkBarCodeExists(uom)){
        QMessageBox box; box.setText("Barcode exists.");
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        box.exec();
        return;
    }

    if(uom->_id == ""){
        dbHelper->insertUOMConversion(uom);
    }
    else{
        dbHelper->updateUOMConversion(uom,convId);
    }
    this->close();
    emit closing();

}

void addConversion::on_buttonBox_rejected()
{
    this->close();
}


void addConversion::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void addConversion::disableSave()
{
    ui->buttonBox->hide();
}



void addConversion::on_pushButton_clicked()
{
    ui->barCodeLineEdit->setText(itemCode + "0" + ui->unitComboBox->currentData().toString());
}
