#include "numberpad.h"
#include "ui_numberpad.h"

NumberPad::NumberPad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NumberPad)
{
    ui->setupUi(this);
    text="";
}

NumberPad::~NumberPad()
{
    delete ui;
}

void NumberPad::on_B1_clicked()
{
    text +="1";
    emit clicked(text);
    text ="";
}

void NumberPad::on_B2_clicked()
{
    text +="2";
    emit clicked(text);
    text ="";
}

void NumberPad::on_B3_clicked()
{
    text +="3";
    emit clicked(text);
    text ="";
}

void NumberPad::on_B4_clicked()
{
    text +="4";
    emit clicked(text);
    text ="";

}

void NumberPad::on_B5_clicked()
{
    text +="5";
    emit clicked(text);
    text ="";

}

void NumberPad::on_B6_clicked()
{
    text +="6";
    emit clicked(text);
    text ="";

}

void NumberPad::on_B7_clicked()
{
    text +="7";
    emit clicked(text);
    text ="";

}

void NumberPad::on_B8_clicked()
{
    text +="8";
    emit clicked(text);
    text ="";

}

void NumberPad::on_B9_clicked()
{
    text +="9";
    emit clicked(text);
    text ="";

}

void NumberPad::on_B0_clicked()
{
    text +="0";
    if(!text.contains(".")){
        emit clicked(text);
        text ="";
    }
}

void NumberPad::on_BDOT_clicked()
{
    if(!text.contains(".")){
        text +=".";
    }
}

void NumberPad::on_BDEL_clicked()
{
    emit deleted();
}

void NumberPad::on_BClear_clicked()
{
    emit clearAll();
}

void NumberPad::on_BENTER_clicked()
{
    emit plus();
}

void NumberPad::on_B00_clicked()
{
    text +="00";
    if(!text.contains(".")){
        emit clicked(text);
    }
}

void NumberPad::on_BMinus_clicked()
{
    emit minus();
}
