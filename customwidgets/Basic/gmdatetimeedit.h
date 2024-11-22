#ifndef GMDATETIMEEDIT_H
#define GMDATETIMEEDIT_H

#include <QDateTimeEdit>

class GMDateTimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    GMDateTimeEdit();

    explicit GMDateTimeEdit(QDateTime *m,
                            QString displayFormat ="dd-MM-yyyy hh:mm:ss",
                            QDateTimeEdit* parent = Q_NULLPTR);
    virtual ~GMDateTimeEdit() override {}
    QDateTime *value;

public slots:
    void FinishedEdit();
    void setVariable(QDateTime *temp);

signals:
    void editFinished();

};

#endif // GMDATETIMEEDIT_H
