#ifndef TEMPLATELIST_H
#define TEMPLATELIST_H

#include <QWidget>

#include "voucherOperations/VoucherEditor/vouchereditor.h"

namespace Ui {
class TemplateList;
}

class TemplateList : public QWidget
{
    Q_OBJECT

public:
    explicit TemplateList(QWidget *parent = nullptr);
    ~TemplateList();

    QString windowTitle="Template";
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    QString backgroundColor = "#e0f7fa";
    QString textColor = "black";
    QString selectionColor = "#e0f7fa";
    QString highlightedColor = "#00acc1";
    QString headerBgColor= "#00bcd4"; //#0082c8
    QString iconColor = "white";
    QString titleColor = "white";

    QSize iconSize = QSize(18,18);


//    void initExtraActions();

//    void initDbHelpers();

//    void initVoucher();

//    void initGlobalObjects();

//    void setWidgets();

//    void resetWidgets();



private slots:
    void on_createNew_clicked();

private:
    Ui::TemplateList *ui;
};

#endif // TEMPLATELIST_H
