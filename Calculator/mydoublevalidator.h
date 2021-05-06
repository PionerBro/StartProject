#ifndef MYDOUBLEVALIDATOR_H
#define MYDOUBLEVALIDATOR_H

#include <QValidator>

class MyDoubleValidator : public QValidator
{
    Q_OBJECT
public:
    MyDoubleValidator(double bottom, double top, int decimals, QObject* parent = nullptr);

    virtual QValidator::State validate(QString& input, int &pos) const override;
    virtual void fixup(QString& input) const override;
private:
    double b;
    double t;
    int m_dec;

    bool validateChars(QStringView str, QByteArray *buff, int &digPointNum) const;
};

#endif // MYDOUBLEVALIDATOR_H
