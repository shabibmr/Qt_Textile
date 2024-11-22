#include "tablereservationeditor.h"
#include <QVBoxLayout>

TableReservationEditor::TableReservationEditor(TableReservationDataModel *reservation, TableReservationDatabaseHelper *dbHelper, QWidget *parent)
  : QWidget{parent}
{
    initWidgets();
    setWidgets();
}

TableReservationEditor::~TableReservationEditor()
{

}

void TableReservationEditor::initWidgets()
{
    entryDate = new GMDateEdit(&reservation->addedDate);
    resDate = new GMDateEdit(&reservation->resDate);
    fromTime = new GMTimeEdit(&reservation->fromTime);
    toTime = new GMTimeEdit(&reservation->toTime);

    tableName = new GMMaterialLineEdit(&reservation->table->tableName);
    narration = new GMPlainTextEdit(&reservation->narration);
}

void TableReservationEditor::setWidgets()
{
    QVBoxLayout *mainLay = new QVBoxLayout();
    this->setLayout(mainLay);


    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip(tr("Save Reservation"));
}

void TableReservationEditor::saveReservation()
{
    if(reservation->reservationID.length()==0){
        dbHelper->insertReservation(reservation);
    }
    else{
        dbHelper->updateReservation(reservation);
    }
}

void TableReservationEditor::deleteReservation()
{
    dbHelper->deleteReservation(reservation);
}

void TableReservationEditor::disableSave()
{
    saveButton->setDisabled(true);
}
