#include "itemlabeldata.h"
#include "ui_itemlabeldata.h"

ItemLabelData::ItemLabelData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemLabelData)
{
    ui->setupUi(this);

    label = new ItemLabelDataModel();
    dbHelper = new ItemLabelDatabaseHelper();

}

ItemLabelData::~ItemLabelData()
{
    delete ui;
}

void ItemLabelData::on_pushButton_clicked()
{
    label->Energy = ui->energyLineEdit->text();
    label->Carbohydrate = ui->carbohydrateGLineEdit->text();
    label->Fat = ui->fatGLineEdit->text();
    label->Protein = ui->proteinGLineEdit->text();
    label->Sugar = ui->sugarGLineEdit->text();
    label->TransFat = ui->transFatLineEdit->text();
    label->SaturatedFat = ui->saturatedFatLineEdit->text();
    label->Cholestrol = ui->cholestrolLineEdit->text();
    label->CrudeFibre = ui->crudeFibreLineEdit->text();
    label->DietaryFibre = ui->dietaryFibreLineEdit->text();
    label->MonoUnsaturatedFat = ui->monoUnsaturatedFatLineEdit->text();
    label->PolyUnsaturatedFat = ui->polyunsaturatedFatLineEdit->text();

    label->MRP = ui->mRPLineEdit->text().toFloat();
    label->NetWeight = ui->netWeightLineEdit->text().toFloat();
    label->Unit = ui->unitLineEdit->text();
    if(ui->nonVegCheckBox->isChecked())
        label->isVeg = false;
    label->Ingredients = ui->ingredientsPlainText->toPlainText();
    label->AllergenAdvice = ui->allergenAdviceText->toPlainText();

    label->convertNutritionFactsJson(label);

    dbHelper->insertLabelData(label);

    this->close();

}

void ItemLabelData::on_pushButton_2_clicked()
{
    this->close();
}
