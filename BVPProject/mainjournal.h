#ifndef MAINJOURNAL_H
#define MAINJOURNAL_H

#include <QDockWidget>

class QTableWidget;
class Order;

class MainJournal : public QDockWidget
{
    Q_OBJECT
private:
    int row = 0;
    const int col = 6;
    QTableWidget* m_journal;

public:
    explicit MainJournal(QWidget *parent = nullptr);
protected:
    virtual void closeEvent(QCloseEvent* pe);
private slots:
    void slotOpenDoc(int);
    void slotNewDoc(QStringList);
signals:

};

#endif // MAINJOURNAL_H
