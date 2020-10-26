#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QObject>

class QLineEdit;
class QLabel;

class Registration : public QWidget
{
    Q_OBJECT
private:
    QLineEdit* plneLogin;
    QLineEdit* plnePassword;
    QLineEdit* plnePasswordRep;
    QLabel*    plblWarning;
public:
    Registration(QWidget* parent = 0);
signals:
    void returntoWelcomePage();
    void regErorrSignal(const QString&);
private slots:
    void retPage();
    void createUser();
};

#endif // REGISTRATION_H
