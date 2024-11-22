#ifndef GMLINEEDIT_H
#define GMLINEEDIT_H

#include <QLineEdit>
#include <customwidgets/Validator/myvalidator.h>

class GMLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    GMLineEdit();

    explicit GMLineEdit(QString *m,int dataType=0,int decimal=2,QLineEdit* parent = Q_NULLPTR);
    virtual ~GMLineEdit() override {}
    QString *value;

    void setVariable(QString *temp);
public slots:
    void FinishedEdit();

signals:
    void editFinished();

};

#endif // GMLINEEDIT_H
