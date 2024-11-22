#include "restaurantitemdetail.h"
#include "ui_restaurantitemdetail.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

RestaurantItemDetail::RestaurantItemDetail(GeneralVoucherDataObject *voucher,int row,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantItemDetail)
{
    ui->setupUi(this);
    qDebug()<<Q_FUNC_INFO<<"constru";
    getLeftWords();
    getRightWords();
    //setItem();
    allowNegativeStock = ConfigurationSettingsDatabaseHelper::getValue(allowNegativeBilling,false).toBool();
    this->voucher = voucher;
    this->row = row;
    bool allowItemDiscFlag =  ConfigurationSettingsDatabaseHelper::getValue(allowItemDiscinPOS,false).toBool();


    if(!allowItemDiscFlag){
        ui->discAmountLine->setReadOnly(true);
        ui->discPercLine->setReadOnly(true);
        ui->rateLine->setReadOnly(true);
    }
    qDebug()<<Q_FUNC_INFO<<"constru";
}

RestaurantItemDetail::~RestaurantItemDetail()
{
    delete ui;
}

void RestaurantItemDetail::setItem(CompoundItemDataObject *value)
{
    Item = value;
    ui->itemNameLineEdit->setText(Item->BaseItem.ItemName);
    ui->quantityLineEdit->setText(
        QString::number(Item->BaseItem.quantity,'f',Item->BaseItem.uomObject.decimalPoints));
    ui->descriptionLineEdit->setText(Item->BaseItem.ItemDescription);
    ui->narrationTextEdit->setPlainText(Item->BaseItem.narration);

    ui->rateLine->setText(QString::number(Item->BaseItem.price,'f',2));
    ui->discAmountLine->setValidator(new MyValidator(0,Item->BaseItem.grandTotal,2,this));
    ui->discPercLine->setValidator(new MyValidator(0,100,2,this));

    ui->discPercLine->setText(QString::number(Item->BaseItem.discountPercentage,'f',2));
    ui->discAmountLine->setText(QString::number(Item->BaseItem.discountinAmount,'f',2));

    connect(ui->discPercLine,&QLineEdit::textChanged,this,[=](){
        float dd = ui->discPercLine->text().toFloat();
        Item->BaseItem.price = Item->BaseItem.price_1 - (Item->BaseItem.price_1 *dd/100);
    });

    connect(ui->discAmountLine,&QLineEdit::textChanged,this,[=](){
        float dd = ui->discAmountLine->text().toFloat();
        Item->BaseItem.price = Item->BaseItem.price_1 - dd;

    });


}

void RestaurantItemDetail::on_saveButton_clicked()
{
    if(!allowNegativeStock)
        if(Item->BaseItem.isStockItem == 1 && ui->quantityLineEdit->text().toFloat()> Item->BaseItem.ClosingStock){
            QMessageBox box;
            box.setText("Current Stock is " + QString::number(Item->BaseItem.ClosingStock,'f',2) +
                        " for " + Item->BaseItem.ItemName + " ! ");
            box.exec();
            return;
        }

    Item->BaseItem.price = ui->rateLine->text().toFloat();

    Item->BaseItem.ItemName = ui->itemNameLineEdit->text();
    Item->BaseItem.quantity = ui->quantityLineEdit->text().toFloat();
    Item->BaseItem.ItemDescription = ui->descriptionLineEdit->text();
    Item->BaseItem.narration = ui->narrationTextEdit->toPlainText();

    if(Item->BaseItem.prevQty > Item->BaseItem.quantity && Item->BaseItem.ItemReqUuid.length()==0){
        CompoundItemDataObject c;
        c.BaseItem = Item->BaseItem;
        c.BaseItem.quantity = Item->BaseItem.prevQty - Item->BaseItem.quantity;
        emit itemDeletedFromDetail(c);
    }

    emit saveRestaurantItemDetails();
    this->close();

}

void RestaurantItemDetail::on_cancelButton_clicked()
{
    this->close();
}

void RestaurantItemDetail::getLeftWords()
{
    QStringList leftWordList;

    int cnt =0;
    for(int i=0;i<leftWordList.size();i++){
        QString name = leftWordList[i];

        QString color = "#f0ff00;color:#000000;font-weight:bold";

        QtButton* button = new QtButton(this);
        button->setStyleSheet("background-color:"+color);
//        button->
        button->setText(name);
        button->setProperty("id",name);
//        button->setFixedSize(QSize(100,40));
        connect(button, SIGNAL(clicked()), this, SLOT(on_wordButton_Clicked()));

        ui->leftLayout->addWidget(button,cnt/3,cnt%3);
        cnt++;
    }
}

void RestaurantItemDetail::getRightWords()
{
    QStringList rightWordList;

    QString list = ConfigurationSettingsDatabaseHelper::getValue(leftwords,false).toString();
    rightWordList = list.split("|");
    int cnt =0;
    for(int i=0;i<rightWordList.size();i++){
        QString name = rightWordList[i];

        if(name.length()==0){
            continue;
        }
        QString color = "#d6efa0;color:#000000;font-weight:bold";

        QtButton* button = new QtButton(this);
        button->setStyleSheet("background-color:"+color);
        QHBoxLayout *lay = new QHBoxLayout(button);
        lay->setContentsMargins(1,1,1,1);

        QLabel *label = new QLabel;
        label->setText(name);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        QFont font;
        font.setPointSize(10);

        label->setFont(font);
        lay->addWidget(label);

        button->setProperty("id",name);
        button->setFixedSize(QSize(140,50));
        connect(button, SIGNAL(clicked()), this, SLOT(on_wordButton_Clicked()));

        ui->rightLayout->addWidget(button,cnt/3,cnt%3);
        cnt++;
    }
}

void RestaurantItemDetail::on_wordButton_Clicked()
{
    QString text = sender()->property("id").toString();

    ui->narrationTextEdit->setPlainText(ui->narrationTextEdit->toPlainText()+ " "+text );
}
