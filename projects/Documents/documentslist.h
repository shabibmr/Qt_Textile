#ifndef DOCUMENTSLIST_H
#define DOCUMENTSLIST_H

#include <QWidget>

namespace Ui {
class DocumentsList;
}

class DocumentsList : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentsList(QWidget *parent = 0);
    ~DocumentsList();

private slots:
    void on_okButton_clicked();

private:
    Ui::DocumentsList *ui;
};

#endif // DOCUMENTSLIST_H
