#ifndef QTBUTTON
#define QTBUTTON

#include <QToolButton>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <utils/gmfileuploadhelper.h>

class QtButton : public QToolButton
{
    Q_OBJECT

public:
    QGraphicsDropShadowEffect* effect;
    explicit QtButton(QWidget *parent);
    ~QtButton();
    //explicit QtButton(const QString &text, QWidget *parent=0);
    GMFileUploadHelper *uploader ;
    QVBoxLayout *lay;
    QString label;
private slots:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public slots:

    void setImage(QString fileName,QString label);

    void place(QByteArray data);

signals:
    void rightClicked();

public slots:


};


#endif // QTBUTTON

