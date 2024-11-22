#include "gmcombobox.h"

GMComboBox::GMComboBox(QString *m, QComboBox *parent) : QComboBox(parent)
{

    gHelper = new GodownDatabaseHelper();
    QObject::connect(this,&QComboBox::currentTextChanged,
                     this,&GMComboBox::ItemSelected);

    value =m;
    this->setEditable(true);
    this->addItem("Select");
    this->setCurrentIndex(0);

}

GMComboBox::~GMComboBox()
{

}

void GMComboBox::getGodownList()
{

//    this->addItem("");
    isGodown = true;
    //    QList<GodownDataObject* > godownList = gHelper->getAllGodowns();
    godowns = gHelper->getGodownForSearchAsMap();

    QMapIterator<QString, QString> iter(godowns);

    iter.toFront();
    while(iter.hasNext()){
        iter.next();
        this->addItem(iter.value(),iter.key());
    }

}

void GMComboBox::setValue(QString *temp)
{

    value = temp;
    qDebug()<<"set"<<*temp;
    if(isGodown)
        this->setCurrentText(gHelper->getGodownNameById(*temp));
    else
        this->setCurrentText(*temp);


}

void GMComboBox::ItemSelected(QString temp)
{
    if(isGodown)
        *value = godowns.key(this->currentText());
    //    emit editFinished();
    else {
        if(this->currentIndex() > 0)
            *value = this->currentText();
    }
    emit selected();

}
