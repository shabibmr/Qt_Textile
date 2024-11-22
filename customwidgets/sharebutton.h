#ifndef SHAREBUTTON_H
#define SHAREBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class ShareButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ShareButton(QPushButton *parent = nullptr);
    ~ShareButton();
signals:

public slots:
};

#endif // SHAREBUTTON_H
