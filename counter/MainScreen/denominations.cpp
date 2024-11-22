#include "denominations.h"
#include "ui_denominations.h"

Denominations::Denominations(ShiftDataModel *shift, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Denominations)
{
    ui->setupUi(this);

    ui->save->hide();
    ui->cancel->hide();

    this->setObjectName("shiftClosingValues");
    this->setStyleSheet("QWidget#shiftClosingValues{background-color:#e0f7fa}");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);


    this->shift = shift;
    obj = new DenominationsDataModel();
    shift->denominations = obj;
}

Denominations::~Denominations()
{
    delete ui;
    ui->thousandLineEdit->setAlignment(Qt::AlignRight);
    ui->fiveHunderdLineEdit->setAlignment(Qt::AlignRight);
    ui->twoHundredLineEdit->setAlignment(Qt::AlignRight);
    ui->hundredLineEdit->setAlignment(Qt::AlignRight);
    ui->fityLineEdit->setAlignment(Qt::AlignRight);
    ui->twentyLineEdit->setAlignment(Qt::AlignRight);
    ui->tenLineEdit->setAlignment(Qt::AlignRight);
    ui->fiveLineEdit->setAlignment(Qt::AlignRight);
    ui->coinsLineEdit->setAlignment(Qt::AlignRight);

    ui->thousandLineEdit_3->setAlignment(Qt::AlignRight);
    ui->fiveHunderdLineEdit_2->setAlignment(Qt::AlignRight);
    ui->twoHundredLineEdit_2->setAlignment(Qt::AlignRight);
    ui->hundredLineEdit_2->setAlignment(Qt::AlignRight);
    ui->fityLineEdit_2->setAlignment(Qt::AlignRight);
    ui->twentyLineEdit_2->setAlignment(Qt::AlignRight);
    ui->tenLineEdit_2->setAlignment(Qt::AlignRight);
    ui->fiveLineEdit_2->setAlignment(Qt::AlignRight);
    ui->coinsLineEdit_2->setAlignment(Qt::AlignRight);

    ui->thousandLineEdit_3->setReadOnly(true);
    ui->fiveHunderdLineEdit_2->setReadOnly(true);
    ui->twoHundredLineEdit_2->setReadOnly(true);
    ui->hundredLineEdit_2->setReadOnly(true);
    ui->fityLineEdit_2->setReadOnly(true);
    ui->twentyLineEdit_2->setReadOnly(true);
    ui->tenLineEdit_2->setReadOnly(true);
    ui->fiveLineEdit_2->setReadOnly(true);
    ui->coinsLineEdit_2->setReadOnly(true);

}

void Denominations::setObj(DenominationsDataModel *value)
{
    obj = value;
}

void Denominations::on_save_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Close Shift","Are you sure you want to close Shift",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        emit totalSales(obj);
        this->close();
    }
    else {
        this->close();
    }

}

void Denominations::on_cancel_clicked()
{
    this->close();
}

void Denominations::on_thousandLineEdit_textChanged(const QString &arg1)
{
    obj->count2000 = ui->thousandLineEdit->text().toInt();
    obj->amt2000 = obj->count2000 * 2000;
    ui->thousandLineEdit_3->setText(QString::number(obj->amt2000,'f',2));
    calcTotal();
}

void Denominations::on_coinsLineEdit_textChanged(const QString &arg1)
{
    obj->countCoins = ui->coinsLineEdit->text().toFloat();
    obj->amtCoins = obj->countCoins * 1;
    ui->coinsLineEdit_2->setText(QString::number(obj->amtCoins,'f',2));
    calcTotal();
}

void Denominations::on_fiveHunderdLineEdit_textChanged(const QString &arg1)
{
    obj->count500 = ui->fiveHunderdLineEdit->text().toInt();
    obj->amt500 = obj->count500 * 500;
    ui->fiveHunderdLineEdit_2->setText(QString::number(obj->amt500,'f',2));
    calcTotal();
}

void Denominations::on_twoHundredLineEdit_textChanged(const QString &arg1)
{
    obj->count200 = ui->twoHundredLineEdit->text().toInt();
    obj->amt200 = obj->count200 * 200;
    ui->twoHundredLineEdit_2->setText(QString::number(obj->amt200,'f',2));
    calcTotal();
}

void Denominations::on_hundredLineEdit_textChanged(const QString &arg1)
{
    obj->count100 = ui->hundredLineEdit->text().toInt();
    obj->amt100 = obj->count100 * 100;
    ui->hundredLineEdit_2->setText(QString::number(obj->amt100,'f',2));
    calcTotal();
}

void Denominations::on_fityLineEdit_textChanged(const QString &arg1)
{
    obj->count50 = ui->fityLineEdit->text().toInt();
    obj->amt50 = obj->count50 * 50;
    ui->fityLineEdit_2->setText(QString::number(obj->amt50,'f',2));
    calcTotal();
}

void Denominations::on_twentyLineEdit_textChanged(const QString &arg1)
{
    obj->count20 = ui->twentyLineEdit->text().toInt();
    obj->amt20 = obj->count20 * 20;
    ui->twentyLineEdit_2->setText(QString::number(obj->amt20,'f',2));
    calcTotal();
}

void Denominations::on_tenLineEdit_textChanged(const QString &arg1)
{
    obj->count10 = ui->tenLineEdit->text().toInt();
    obj->amt10 = obj->count10 * 10;
    ui->tenLineEdit_2->setText(QString::number(obj->amt10,'f',2));
    calcTotal();
}

void Denominations::on_fiveLineEdit_textChanged(const QString &arg1)
{
    obj->count5 = ui->fiveLineEdit->text().toInt();
    obj->amt5 = obj->count5 * 5;
    ui->fiveLineEdit_2->setText(QString::number(obj->amt5,'f',2));
    calcTotal();
}

void Denominations::calcTotal()
{
    obj->total = 0;
    obj->total += obj->amt2000;
    obj->total += obj->amt500;
    obj->total += obj->amt200;
    obj->total += obj->amt100;
    obj->total += obj->amt50;
    obj->total += obj->amt20;
    obj->total += obj->amt10;
    obj->total += obj->amt5;
    obj->total += obj->amtCoins;


    ui->totalLineEdit->setText(QString::number(obj->total,'f',2));

}
