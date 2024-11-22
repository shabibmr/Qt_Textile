#ifndef GMDATEEDIT_H
#define GMDATEEDIT_H

#include <QDateEdit>

class GMDateEdit : public QDateEdit
{
    Q_OBJECT
public:
    GMDateEdit();

    explicit GMDateEdit(QDate *m,QDateEdit* parent = Q_NULLPTR);
    virtual ~GMDateEdit() override {}
    QDate *value;

public slots:
    void FinishedEdit(QDate value);
    void setVariable(QDate *temp);

signals:
    void editFinished();

};

#endif // GMDATEEDIT_H
