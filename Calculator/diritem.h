#ifndef DIRITEM_H
#define DIRITEM_H

#include <QDialog>

class DirItem : public QDialog
{
    Q_OBJECT
public:
    DirItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    DirItem(const QList<QVariant> &data, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~DirItem();
private:
    void setupDirItem(const QList<QVariant>& data);
};

#endif // DIRITEM_H
