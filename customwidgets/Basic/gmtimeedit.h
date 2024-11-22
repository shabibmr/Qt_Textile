#ifndef GMTIMEEDIT_H
#define GMTIMEEDIT_H

#include <QObject>
#include <QDebug>
#include <QTimeEdit>

class GMTimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    GMTimeEdit();

    explicit GMTimeEdit(QTime *m, QTimeEdit* parent = Q_NULLPTR);
    virtual ~GMTimeEdit() override {}
    QTime *value;

public slots:
    void FinishedEdit();
    void setVariable(QTime *temp);

signals:
    void editFinished();
};

#endif // GMTIMEEDIT_H
