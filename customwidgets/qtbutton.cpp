#include "qtbutton.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QFuture>
#include <QtConcurrent>
QtButton::QtButton(QWidget *parent) :
    QToolButton(parent)
{
    this->setAutoRaise(true);
    this->setStyleSheet("border-style: outset;border-bottom-color: rgb(0, 150, 136);"
                        "border-top-color: rgb(120, 150, 136);border-left-color:  rgb(120, 150, 136);"
                        "background-color: rgb(255, 255, 255);border-right-color: rgb(0, 150, 136);"
                        "border-radius: 10px;border-width: 1px;");

    effect = new QGraphicsDropShadowEffect();
    effect->setColor(QColor(0, 0, 0, 80));
    effect->setBlurRadius(3); //Adjust accordingly
    effect->setOffset(3,3);

    uploader = new GMFileUploadHelper("","xxx.jpg",this);



}
/*QtButton::QtButton(const QString &text, QWidget *parent=0):
{

}*/

QtButton::~QtButton(){


}

void QtButton::mousePressEvent(QMouseEvent *e)
{
    this->setGraphicsEffect(effect);
    if(e->button()==Qt::RightButton)
    {
        emit rightClicked();
    }
    else
    {
        emit clicked();
    }
}

void QtButton::mouseReleaseEvent(QMouseEvent *e)
{
    effect->setEnabled(false);
}

void QtButton::setImage(QString fileName,QString label)
{
    connect(uploader,&GMFileUploadHelper::downLoaded,this,&QtButton::place,Qt::QueuedConnection);

    QFuture<void> t1 = QtConcurrent::run(uploader,&GMFileUploadHelper::readFile,QString(fileName+".png"));

    uploader->readFile(fileName+".png");
    lay= new QVBoxLayout(this);
    this->label = label;
    QLabel *ltext = new QLabel(this);
    ltext->setText(label);
    lay->addWidget(ltext);
    this->setLayout(lay);
    t1.waitForFinished();
}

void QtButton::place(QByteArray data)
{
    qDebug()<<"Data Size : "<<data.size();
    QPixmap pm;
    pm.loadFromData(data);

    lay= new QVBoxLayout(this);

    QLabel *ltext = new QLabel(this);
    ltext->setText(label);
    lay->addWidget(ltext);

    QLabel *ll = new QLabel(this);
    lay->insertWidget(0,ll);
    ll->setPixmap(pm.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ll->setScaledContents(true);
    lay->setStretch(0,1);

    ll->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ll->setFixedSize(300,300);


    //        ui->imageLabel->setPixmap(pm.scaled(150,150,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //        ui->imageLabel->setScaledContents(true);
    //        ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //        ui->imageLabel->setFixedSize(300,300);
}

