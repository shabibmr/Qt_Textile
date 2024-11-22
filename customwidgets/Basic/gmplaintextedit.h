#ifndef GMPLAINTEXTEDIT_H
#define GMPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class GMPlainTextEdit : public QPlainTextEdit
{


    Q_OBJECT
public:
    GMPlainTextEdit();

    explicit GMPlainTextEdit(QString *m,QPlainTextEdit* parent = Q_NULLPTR);
    virtual ~GMPlainTextEdit() override {}
    QString *value;

    void setVariable(QString *temp);
public slots:
    void FinishedEdit();

signals:
    void editFinished();

};

#endif // GMPLAINTEXTEDIT_H
