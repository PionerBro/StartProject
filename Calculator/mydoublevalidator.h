#ifndef MYDOUBLEVALIDATOR_H
#define MYDOUBLEVALIDATOR_H

#include <QValidator>

class MyDoubleValidator : public QValidator
{
    Q_OBJECT
public:

    enum DigitalPoint{
        Dot,
        Comma
    };
    Q_ENUM(DigitalPoint)

    enum DecVersion{
        Short,
        Expanded
    };
    Q_ENUM(DecVersion)

    MyDoubleValidator(double bottom, double top, int decimals, QObject* parent = nullptr, DigitalPoint digPoint = DigitalPoint::Dot, DecVersion decV = DecVersion::Short);

    virtual QValidator::State validate(QString& input, int &pos) const override;
    virtual void fixup(QString& input) const override;
private:
    double b;
    double t;
    int m_dec;
    DigitalPoint m_digPoint;
    DecVersion m_decVersion;

    bool validateChars(QStringView str, QByteArray *buff, int &digPointNum) const;
};

#endif // MYDOUBLEVALIDATOR_H
