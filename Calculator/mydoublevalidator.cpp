#include "mydoublevalidator.h"
#include "limits"

#include <QDebug>
#include <QLineEdit>

MyDoubleValidator::MyDoubleValidator(double bottom, double top, int decimals, QObject* parent):QValidator(parent)
{
        b = bottom;
        t = top;
        m_dec = decimals;
}

static int numDigits(qlonglong n){
    if(n==0)
        return 1;
    return (int)std::log10(double(n)) + 1;
}

static qlonglong pow10(int exp){
    qlonglong result = 1;
    for(int i = 0; i < exp; ++i)
        result *= 10;
    return result;
}

QValidator::State MyDoubleValidator::validate(QString &input, int &) const{
    QByteArray buff;
    int digPointNum = -1;
    if(!validateChars(input, &buff, digPointNum))
        return QValidator::Invalid;

    if(digPointNum != -1)
        input[digPointNum]='.';

    if(buff.isEmpty())
        return  QValidator::Intermediate;

    if(b >= 0 && buff.startsWith('-'))
        return QValidator::Invalid;

    if(t < 0 && buff.startsWith('+'))
        return QValidator::Invalid;

    bool ok = false;
    double i = buff.toDouble(&ok);
    if(i == std::numeric_limits<double>::quiet_NaN())
        return QValidator::Invalid;
    if(!ok)
        return QValidator::Intermediate;

    if(digPointNum==(input.size()-1 - m_dec) && input.size()>(1+m_dec))
        return QValidator::Acceptable;

    if(i >= b && i <= t)
        return QValidator::Intermediate;


    double max = qMax(qAbs(b),qAbs(t));
    if((qlonglong)max < LLONG_MAX){
        qlonglong n = pow10(numDigits(qlonglong(max)));
        // In order to get the highest possible number in the intermediate
        // range we need to get 10 to the power of the number of digits
        // after the decimal's and subtract that from the top number.
        //
        // For example, where q->dec == 2 and with a range of 0.0 - 9.0
        // then the minimum possible number is 0.00 and the maximum
        // possible is 9.99. Therefore 9.999 and 10.0 should be seen as
        // invalid.
        if(qAbs(i) > (n - std::pow(10, -m_dec)))
            return QValidator::Invalid;
    }

    return  QValidator::Intermediate;
}

void MyDoubleValidator::fixup(QString &input) const{
    QByteArray buff;
    int digPointNum = -1;

    if(!validateChars(input, &buff, digPointNum))
        return ;


    if(input.isEmpty()){
        input.append("0.");
        for(int i = 0; i<m_dec; ++i)
            input.append('0');
        return;
    }


    int inputSize = input.size();
    if(digPointNum != -1){
        if(digPointNum == 0){
            if(inputSize==1){
                input.clear();
                input.append("0.");
                for(int i = 0; i<m_dec; ++i)
                    input.append('0');
                return;
            }else{
                input.prepend('0');
            }
        }
        inputSize = input.size();
        ++digPointNum;
        for(int j = 0; j < m_dec; ++j){
            if(digPointNum == inputSize-1-j){
                for(int i = 0; i<m_dec-j;++i)
                    input.append('0');
                return;
            }
        }
    }else{
        qDebug()<<input << buff;
        input.append(".00");
        //QLineEdit* lne = static_cast<QLineEdit*>(sender());

        //static_cast<QLineEdit*>(sender())->setPlaceholderText(input);
    }
}

char digitToCLocale(QChar in){
    if(in.unicode() >= '0' && in.unicode() <= '9')
        return in.toLatin1();
    if(in == QLatin1Char('+'))
        return '+';
    if(in == QLatin1Char('-') || in == QChar(0x2212))
        return '-';
    if(in == QLatin1Char('.') || in == QLatin1Char(','))
        return '.';
    return 0;
}

bool MyDoubleValidator::validateChars(QStringView str, QByteArray *buff, int& digPointNum) const{
    buff->clear();
    buff->reserve(str.length());

//    bool lastWasDigit = false;
    int decPointCnt = 0;
    bool dec = false;
    int decDigitCnt = 0;

    for(qsizetype i = 0; i < str.size(); ++i){
        char c = digitToCLocale(str.at(i));

        if(c >= '0' && c <= '9'){
            if(dec && m_dec != -1 && m_dec < ++decDigitCnt)
                return false;
//            lastWasDigit = true;
        }else{
            switch (c) {
                case '.':
                    if(++decPointCnt > 1)
                        return false;
                    if(m_dec == 0)
                        return false;
                    digPointNum = i;
                    dec = true;
                    break;
                case '+':
                case '-':
                    if(i!=0)
                        return false;
                    break;
                default:
                    return false;
            }
//            lastWasDigit = false;
        }
        buff->append(c);
    }
    return true;
}
