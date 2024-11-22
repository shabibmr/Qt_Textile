#include "gmemployeesearchlite.h"

GMEmployeeSearchLite::GMEmployeeSearchLite(UserProfileDatabaseHelper *dbHelper, QSqlQueryModel *model1, QString text,
                                           QLineEdit *parent)
    :QLineEdit(parent)

{
    empHelper = dbHelper;
    //  items = itemsHelper->getALLInventoryObjects();
    //  qDebug()<<items.size();
    //    for(int i=0;i<itemsListPtr.size();i++){
    //        list.append(itemsListPtr[i]->ItemName);
    //    }
    // qDebug()<<"list append completed"<<QDateTime::currentDateTime().toString();

    empModel = model1;
    for(int i=0;i<empModel->rowCount();i++){
        list.append(empModel->record(i).value(1).toString());
    }

    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText(text);
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(returnPressed()),this,SLOT(checkEdit()));

    this->setFocusPolicy(Qt::StrongFocus);

    listWidget =0;
    listWidget = new GMEmployeeSearchListWidgetLite(empModel);
    //qDebug()<<"set data"<<QDateTime::currentDateTime().toString();

    listWidget->setData(empModel);
    //qDebug()<<"st data end"<<QDateTime::currentDateTime().toString();

    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );
}

void GMEmployeeSearchLite::checkEdit()
{
    qDebug()<<">>>>>Running :"<<this->text();

    if(this->text().compare("+")==0){

        qDebug()<<"plus button 47";
        emit plusButton();
        this->setFocus();
        listWidget->hide();
        this->setText("");
    }
    else if(this->text().compare("-")==0){

        qDebug()<<"plus button 47";
        emit minusButton();
        this->setFocus();
        listWidget->hide();
        this->setText("");
    }
    else if(this->text().compare("*")==0){

        qDebug()<<"plus button 47";
        emit astriskpressed();
        this->setFocus();
        listWidget->hide();
        this->setText("");
    }


    //    if(this->text().startsWith("99")){
    //        qDebug()<<"text continues"<<this->text();
    //        return;
    //    }
    if(!list.contains(this->text(),Qt::CaseInsensitive) && false){
        qDebug()<<"no match";
        this->setText("");
    }
}

void GMEmployeeSearchLite::editTextChanged(QString text)
{
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    }
}

void GMEmployeeSearchLite::itemSelected(int id, QString value)
{
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    listWidget->setHidden(true);
    this->setFocus();
    emp = empHelper->getEmployeeByID(id);
    emit SelectedEmpID(emp);
    qDebug()<<"item is Set Calling emit";
    emit ItemSet();

}

void GMEmployeeSearchLite::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(QString,QString)),this,SLOT(itemSelected(QString,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();
}

