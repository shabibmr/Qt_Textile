#ifndef PDFHELPER_H
#define PDFHELPER_H

#include <QObject>


class PdfHelper : public QObject
{
    Q_OBJECT
public:
    PdfHelper();

public slots:
    void setPrinter();
};

#endif // PDFHELPER_H
