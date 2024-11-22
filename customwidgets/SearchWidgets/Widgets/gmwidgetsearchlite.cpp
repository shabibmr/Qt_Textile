#include "gmwidgetsearchlite.h"

GMWidgetSearchLite::GMWidgetSearchLite(QString text,
                                           QLineEdit *parent)
    :QLineEdit(parent)

{
    widgetHelper = new UiSettingsDatabaseHelper;


    widModel = widgetHelper->getAllScreensForSearchModel();
    for(int i=0;i<widModel->rowCount();i++){
        list.append(widModel->record(i).value(1).toString());
    }

    this->setFrame(QFrame::NoFrame);
    this->setPlaceholderText(text);
    QObject::connect(this,SIGNAL(textChanged(QString)),this,SLOT(checkEdit()));
    QObject::connect(this,SIGNAL(textEdited(QString)),this,SLOT(editTextChanged(QString)));
    QObject::connect(this,SIGNAL(returnPressed()),this,SLOT(checkEdit()));

    this->setFocusPolicy(Qt::StrongFocus);

    listWidget =0;
    listWidget = new GMWidgetSearchListWidgetLite(widModel);
    //qDebug()<<"set data"<<QDateTime::currentDateTime().toString();

    listWidget->setData(widModel);
    //qDebug()<<"st data end"<<QDateTime::currentDateTime().toString();

    listWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup );
}

void GMWidgetSearchLite::checkEdit()
{

    if(!list.contains(this->text(),Qt::CaseInsensitive) && false){
        qDebug()<<"no match";
        this->setText("");
    }
}

void GMWidgetSearchLite::editTextChanged(QString text)
{
    if(listWidget->isHidden()){
        QPoint point1 = mapToGlobal(this->pos());
        listWidget->move(point1);
        listWidget->setMaximumWidth(this->width());
        listWidget->setHidden(false);
        listWidget->setFocus();
        listWidget->setFocusExpl(text);
        QObject::connect(listWidget,SIGNAL(selectedID(int,QString)),this,SLOT(itemSelected(int,QString)));
        QObject::connect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    }
}

void GMWidgetSearchLite::itemSelected(int id, QString value)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<id<<value;
    this->setText(value);
    QObject::disconnect(listWidget,SIGNAL(selectedID(int,QString)),this,SLOT(itemSelected(int,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));

    listWidget->setHidden(true);
    this->setFocus();
    widObj = widgetHelper->getUiConfigById(id);
    emit SelectedWidget(widObj);
    QSqlQueryModel *model = widgetHelper->getScreenIdByName(value);
    emit uiSelected(model->record(0).value(0).toInt(), model->record(0).value(1).toBool());
    qDebug()<<"item is Set Calling emit";
    emit ItemSet();

}

void GMWidgetSearchLite::noneSelected()
{
    QObject::disconnect(listWidget,SIGNAL(selectedID(int,QString)),this,SLOT(itemSelected(int,QString)));
    QObject::disconnect(listWidget,SIGNAL(nonSelected()),this,SLOT(noneSelected()));
    listWidget->hide();
    listWidget=0;
    this->setFocus();
    emit clearItem();
}

