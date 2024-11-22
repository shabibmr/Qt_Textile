#ifndef GMCPBAR_H
#define GMCPBAR_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QTimer>

class GMCPBar : public QWidget
{
    Q_OBJECT
public:
    explicit GMCPBar(QWidget *parent = nullptr);
    void upd(qreal pp) {
        if (p == pp) return;
        p = pp;
        update();
    }

signals:

private:
    qreal p; // progress 0.0 to 1.0


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // GMCPBAR_H
