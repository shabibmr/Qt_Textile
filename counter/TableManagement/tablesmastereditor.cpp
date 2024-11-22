#include "tablesmastereditor.h"
#include <QLabel>
#include <QFormLayout>
TablesMasterEditor::TablesMasterEditor(TableMasterDataModel *table, TablesMasterHelper *dbHelper, QWidget *parent)
    : QWidget{parent}
{
    this->dbHelper = dbHelper;
    this->table = table;

    initWidgets();
    setWidgets();
}

TablesMasterEditor::~TablesMasterEditor()
{

}

void TablesMasterEditor::initWidgets()
{

    QString bgcolor = "#e0f7fa";
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");


    QLabel *windowLabel = new QLabel(windowTitle);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));



    appBar->appBarLayout()->addWidget(backButton);


    //    appBar->appBarLayout()->addLayout(vbox);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignCenter);
    // appBar->appBarLayout()->addStretch(1);
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Calibri", 18, QFont::Normal));

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);
    //    layout->addStretch(1);


    deleteButton = new QtMaterialIconButton(QtMaterialTheme::icon("content", "delete"));
    deleteButton->setParent(appBar);
    deleteButton->setColor(QColor(185,10,10));
    deleteButton->setIconSize(QSize(30,30));

    appBar->appBarLayout()->addWidget(deleteButton);

    connect(deleteButton,QtMaterialIconButton::clicked,this,&TablesMasterEditor::deleteTable);
    connect(backButton,QtMaterialIconButton::clicked,this,[=](){
        this->close();
    });


    tableName = new GMLineEdit(&table->tableName);
    tableName->setMaximumWidth(5000);
    tableName->setMinimumHeight(40);

    floor = new GMLineEdit(&table->floor);
    floor->setMaximumWidth(5000);
    floor->setMinimumHeight(40);

    capacity = new QLineEdit();
    capacity->setText(QString::number(table->capacity));
    capacity->setAlignment(Qt::AlignRight);
    capacity->setMinimumHeight(40);

    QObject::connect(capacity,&QLineEdit::editingFinished,this,[=](){
        table->capacity = capacity->text().toInt();
    });

    max = new QLineEdit();
    max->setText(QString::number(table->maxCapacity));
    max->setAlignment(Qt::AlignRight);
    QObject::connect(max,&QLineEdit::editingFinished,this,[=](){
        table->maxCapacity = max->text().toInt();
    });

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(appBar);

    this->setLayout(mainLayout);

    QFormLayout *form = new QFormLayout();
    form->setVerticalSpacing(15);
    form->addRow(tr("Table Name"),tableName);
    form->addRow(tr("Capacity"), capacity);
    form->addRow(tr("Floor"),floor);

    QWidget *body=new QWidget();
    body->setLayout(form);

    mainLayout->addWidget(body);
    mainLayout->addSpacing(50);

    mainLayout->setStretch(1,1);
    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip(tr("Save Table"));
    connect(saveButton,QtMaterialFlatButton::clicked,this,&TablesMasterEditor::saveTable);

}

void TablesMasterEditor::setWidgets()
{

}

void TablesMasterEditor::saveTable()
{
    if(table->tableID.length()==0){
        dbHelper->insertTableMaster(table);
    }
    else{
        dbHelper->updateTableMaster(table);
    }
    emit closing();
    this->close();
}

void TablesMasterEditor::deleteTable()
{
    dbHelper->deleteTableMaster(table);
    emit closing();
    this->close();
}

void TablesMasterEditor::disableSave()
{
    saveButton->setDisabled(true);
}
