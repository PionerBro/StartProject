#ifndef DIRITEM_H
#define DIRITEM_H

#include <QDialog>

class QLineEdit;
class QComboBox;

class DirItem : public QDialog
{
    Q_OBJECT
public:
    enum DirType{
        Element,
        Folder
    };
    Q_ENUM(DirType)

    DirItem(qlonglong rootItemNum, QList<QVariant> &data, int type,  QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    DirItem(QList<QVariant> &data, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~DirItem();
private:
    void setupDirItem(const QList<QVariant>& data);
    void setupDirFolder(const QList<QVariant>& data);
private slots:
    void slotOkClicked();
private:
    QPushButton* btnOk;
    QList<QVariant>& m_data;

    qlonglong  num;
    qlonglong  parentNum;
    qlonglong  dirNum;
    QLineEdit* nameEdit;
    QComboBox* unitBox;
    QLineEdit* priceEdit;
};

#endif // DIRITEM_H
