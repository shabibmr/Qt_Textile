#include "payslipgenerationeditor.h"
#include "ui_payslipgenerationeditor.h"

PayslipGenerationEditor::PayslipGenerationEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayslipGenerationEditor)
{
    ui->setupUi(this);
}

PayslipGenerationEditor::~PayslipGenerationEditor()
{
    delete ui;
}

void PayslipGenerationEditor::initExtraActions()
{

}

void PayslipGenerationEditor::initDbHelpers()
{

}

void PayslipGenerationEditor::initVoucher()
{

}

void PayslipGenerationEditor::initGlobalObjects()
{

}

void PayslipGenerationEditor::setWidgets()
{

}

void PayslipGenerationEditor::resetWidgets()
{

}

void PayslipGenerationEditor::showVoucher(PayslipDataModel *obj)
{

}

void PayslipGenerationEditor::closeEditor()
{

}

void PayslipGenerationEditor::saveVoucher()
{

}

void PayslipGenerationEditor::rejectVoucher()
{

}

void PayslipGenerationEditor::deleteVoucher()
{

}

void PayslipGenerationEditor::askForPrint()
{

}

void PayslipGenerationEditor::printVoucher()
{

}

void PayslipGenerationEditor::setTable()
{

}
