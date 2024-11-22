#ifndef GMSEARCHLINEEDIT_H
#define GMSEARCHLINEEDIT_H

#include <QLineEdit>
#include <QStringList>

class GMSearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMSearchLineEdit(QLineEdit *parent=nullptr);
    ~GMSearchLineEdit();
    QStringList list;

signals:

public slots:
    void checkEdit();
    void setData(QStringList ptr);
};

#endif // GMSEARCHLINEEDIT_H
