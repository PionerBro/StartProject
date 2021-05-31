#include "calculatortreemodel.h"
#include "calculatortreeitem.h"
#include "calculatordatabase.h"

#include <QIcon>
#include <QBrush>

#include <QDebug>

extern CalculatorDatabase g_db;

CalculatorTreeModel::CalculatorTreeModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent):QAbstractItemModel(parent)
{
    m_db = &g_db;
    setSortCol(0);
    m_sqlTable = sqlTable;
    m_hHeaderData = headers;
    //setupModelData();
    editableCols.fill(false, m_hHeaderData.count());
}

CalculatorTreeModel::~CalculatorTreeModel(){
    delete m_root;
}

QVariant CalculatorTreeModel::data(const QModelIndex& index, int role)const{
    if(!index.isValid())
           return QVariant();

    int iColumn = index.column();

    switch (role) {
    case Qt::DecorationRole:
        if(iColumn == 0){
            int itemType = static_cast<CalculatorTreeItem*>(index.internalPointer())->getItemType();
            if(itemType == ItemType::Element){
                return QIcon("../Calculator/blue1.png").pixmap(QSize(25,25));
            }else if(itemType == ItemType::FolderOpen){
                return QIcon("../Calculator/blue3.jpg").pixmap(QSize(25,25));
            }else if(itemType == ItemType::FolderClose){
                return QIcon("../Calculator/yellow2.png").pixmap(QSize(25,25));
            }
        }
        return QVariant();
    case Qt::BackgroundRole:
        if(iColumn == columnCount()-1){
            if(changedRow.isEmpty())
                return QVariant();
            else if(reserveCh.value(index.row()))
                return QBrush(QColor(Qt::red));
            else if(reserveAc.value(index.row()))
                return QBrush(QColor(Qt::green));
            else
               return QVariant();
        }
        return QVariant();
    case Qt::EditRole:
    case Qt::DisplayRole:
        switch (iColumn) {
        case 0:
        case 1:
        case 2:
            return QVariant();
        default:
            //if(iColumn == columnCount()-1)
              //  return QVariant();
            if(editMode && editableCols.value(iColumn)){
                return reserveData.value(iColumn).value(index.row());
            }else{
                return static_cast<CalculatorTreeItem*>(index.internalPointer())->data(iColumn);
            }
        }
    default:
        return QVariant();
    }
}

Qt::ItemFlags CalculatorTreeModel::flags(const QModelIndex &index)const{
    if(!index.isValid())
        return Qt::NoItemFlags;
    if(editMode && editableCols.value(index.column()))
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex CalculatorTreeModel::index(int row, int column, const QModelIndex &parent)const{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    if(treeMode){
        CalculatorTreeItem* item;
        if(!parent.isValid())
            item = m_curRootItem;
        else
            item = static_cast<CalculatorTreeItem*>(parent.internalPointer());
        CalculatorTreeItem* child = item->child(row);
        if(child)
            return createIndex(row, column, child);
        else
            return QModelIndex();
    }else{
        CalculatorTreeItem* child = m_treeElements.value(row);
        if(child)
            return createIndex(row, column, child);
        else
            return QModelIndex();
    }
}

QVariant CalculatorTreeModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_hHeaderData.value(section);
    return QVariant();
}

QModelIndex CalculatorTreeModel::parent(const QModelIndex &index)const{
    if(!index.isValid())
        return QModelIndex();
    if(treeMode){
        CalculatorTreeItem *childItem = static_cast<CalculatorTreeItem*>(index.internalPointer());
        CalculatorTreeItem *parentItem = childItem->parent();
        if(parentItem == m_curRootItem)
           return QModelIndex();
        return createIndex(parentItem->row(),0, parentItem);
    }else{
        return QModelIndex();
    }
}

int CalculatorTreeModel::rowCount(const QModelIndex &parent)const{
    if(treeMode){
        CalculatorTreeItem* parentItem;
        if(parent.column()>0)
            return 0;
        if(!parent.isValid())
            parentItem = m_curRootItem;
        else
            parentItem = static_cast<CalculatorTreeItem*>(parent.internalPointer());
        return parentItem->childCount();
    }else{
        return m_treeElements.count();
    }
}

int CalculatorTreeModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent);
    return m_hHeaderData.count();
}

void CalculatorTreeModel::sethHeaderData(QVector<QVariant> &headers){
    m_hHeaderData.clear();
    m_hHeaderData<<headers;
    QVector<bool> tmp;
    tmp.fill(false, columnCount());
    for(int i = 0; i < tmp.count() && i < editableCols.count(); ++i){
        tmp[i] = editableCols.value(i);
    }
    editableCols = tmp;
}

QVector<QVariant> CalculatorTreeModel::getRowData(const QModelIndex &index) const{
    return static_cast<CalculatorTreeItem*>(index.internalPointer())->rowData();
}

void CalculatorTreeModel::setColumnEditable(int col, bool value){
    if(col >= 3 && col < editableCols.count())
        editableCols[col] = value;
}

void CalculatorTreeModel::setSortCol(int col){
    m_sortCol = col;
    for(int i = 0 ; i < m_treeFolders.count(); ++i){
        m_treeFolders.value(i)->setSortCol(col);
        m_treeFolders.value(i)->sortItem();
    }
    std::sort(m_treeElements.begin(), m_treeElements.end(), compare);
}

bool CalculatorTreeModel::getEditModeValue() const{
    return editMode;
}

CalculatorTreeItem* CalculatorTreeModel::currentRoot() const{
    return m_curRootItem;
}

void CalculatorTreeModel::setupModelData(){
    if(m_root){
        delete m_root;
        m_treeElements.clear();
        m_treeFolders.clear();
    }
    m_root = new CalculatorTreeItem({0,0,0});
    m_curRootItem = m_root;
    m_treeFolders<<m_root;
    QVector<QVector<QVariant>> dbData;
    if(selectDataBaseAll(m_sqlTable, dbData)){
        for(int i = 0; i < dbData.count(); ++i){
            QVector<QVariant> itemData = dbData.value(i);
            int folderNum = itemData.value(FieldName::ParentId).toLongLong();
            CalculatorTreeItem* item = new CalculatorTreeItem(itemData, m_treeFolders[folderNum]);
            m_treeFolders[folderNum]->appendChild(item);
            (item->getItemType()) ? m_treeFolders<<item : m_treeElements<<item;
        }
        for(int i = 0; i < m_treeFolders.count(); ++i){
            m_treeFolders.value(i)->sortItem();
        }
        std::sort(m_treeElements.begin(), m_treeElements.end(), compare);
    }
}

void CalculatorTreeModel::setCalculatorDatabase(CalculatorDatabase *db){
    m_db = db;
}

QVariant CalculatorTreeModel::getReserveData(int row, int col) const{
    QVector<QVariant> data;
    data = reserveData.value(col, {});
    if(!data.isEmpty())
        return data.value(row);
    return QVariant();
}

bool CalculatorTreeModel::isEditableCol(int col) const{
    return editableCols.value(col);
}

void CalculatorTreeModel::rootItemChanged(QModelIndex index){
    if(!index.isValid())
        return;
    CalculatorTreeItem* item = static_cast<CalculatorTreeItem*>(index.internalPointer());
    beginResetModel();
    int itemType = item->getItemType();
    if(itemType==ItemType::Element){
        QVector<QVariant> data;
        data = item->rowData();
        emit sendData(data);
    }else if(itemType==ItemType::FolderClose){
        m_curRootItem = item;
    }else if(itemType==ItemType::FolderOpen){
        m_curRootItem = item->parent();
    }
    endResetModel();
}

bool CalculatorTreeModel::createModelItem(const QVector<QVariant> &data){
    QVector<QVariant> itemData = data;
    if(createDataBaseItem(m_sqlTable, itemData)){
        beginResetModel();
        CalculatorTreeItem* item = new CalculatorTreeItem(itemData, m_curRootItem);
        m_curRootItem->appendChild(item);
        m_curRootItem->sortItem();
        if(item->getItemType() == ItemType::FolderClose){
            m_treeFolders.append(item);
        }else{
            m_treeElements.append(item);
            std::sort(m_treeElements.begin(), m_treeElements.end(), compare);
        }
        endResetModel();
        return true;
    }
    return false;
}

bool CalculatorTreeModel::updateModelItem(const QVector<QVariant> &data, const QModelIndex &index){
    if(updateDataBaseItem(m_sqlTable, data)){
        beginResetModel();
        static_cast<CalculatorTreeItem*>(index.internalPointer())->setRowData(data);
        endResetModel();
        return true;
    }
    return false;
}

bool CalculatorTreeModel::updateModelItems(const QVector<QVector<QVariant>> &data, const QVector<CalculatorTreeItem*> &items){
    if(updateDataBaseItems(m_sqlTable,data)){
        beginResetModel();
        for(int i = 0; i < items.count(); ++i){
            if(items.value(i))
                items.value(i)->setRowData(data.value(i));
        }
        endResetModel();
        return true;
    }
    return false;
}

void CalculatorTreeModel::setTreeMode(bool value){
    if(!editMode){
        beginResetModel();
        treeMode = value;
        endResetModel();
    }
}

void CalculatorTreeModel::setEditMode(bool value){
    if(!treeMode){
        beginResetModel();
        editMode = value;
        if(value){
            m_hHeaderData<<"";
            reserveCh.fill(false, m_treeElements.count());
            for(int i = 0; i < editableCols.count(); ++i){
                if(editableCols[i]){
                    QVector<QVariant> data;
                    for(int j = 0; j < m_treeElements.count(); ++j){
                        data<<m_treeElements.value(j)->data(i);
                    }
                    reserveData.insert(i, data);
                }
            }
        }else{
            m_hHeaderData.pop_back();
            changedRow.clear();
            reserveData.clear();
            reserveCh.clear();
            reserveAc.clear();
        }
        endResetModel();
    }
}

void CalculatorTreeModel::reserveDataChange(int row, int column, const QString& text){
    if(!reserveAc.isEmpty()){
        reserveAc.clear();
        changedRow.clear();
    }
    reserveData[column][row] = text;
    reserveCh[row] = true;
    for(int i = 0; i < changedRow.count(); ++i){
        if(changedRow.value(i) == row){
            return;
        }
    }
    changedRow<<row;
    emit reserveDataChanged();
}

void CalculatorTreeModel::reserveDataAccept(){
    if(editMode){
        beginResetModel();
        reserveAc.fill(false, m_treeElements.count());
        QVector<int> editCols;
        for(int j = 0; j < editableCols.count(); ++j){
            if(editableCols.value(j))
                editCols<<j;
        }
        QVector<QVector<QVariant>> updateData;
        QVector<CalculatorTreeItem*> updateItems;
        for(int i = 0; i < changedRow.count(); ++i){
            bool chFlag = false;
            int num = changedRow.value(i);
            CalculatorTreeItem* item = m_treeElements.value(num);
            QVector<QVariant> itemData = item->rowData();
            for(int j = 0; j<editCols.count(); ++j){
                int column = editCols.value(j);
                if(item->data(column).toString() != reserveData.value(column).value(num)){
                    chFlag = true;
                    itemData[column] = reserveData.value(column).value(num);
                }
            }
            if(chFlag){
                updateData<<itemData;
                updateItems<<item;
            }
        }
        if(updateModelItems(updateData, updateItems)){
            for(int i = 0; i < changedRow.count(); ++i){
                int num = changedRow.value(i);
                reserveCh[num] = false;
                reserveAc[num] = true;
            }
            emit reserveDataAccepted();
        }
        endResetModel();
    }
}

void CalculatorTreeModel::reserveDataReject(){
    if(editMode){
        beginResetModel();
        reserveData.clear();
        reserveCh.clear();
        changedRow.clear();
        for(int i = 0; i < editableCols.count(); ++i){
            if(editableCols[i]){
                QVector<QVariant> data;
                for(int j = 0; j < m_treeElements.count(); ++j){
                    data<<m_treeElements.value(j)->data(i);
                }
                reserveData.insert(i, data);
            }
        }
        reserveCh.fill(false,m_treeElements.count());
        emit reserveDataRejected();
        endResetModel();
    }
}

bool CalculatorTreeModel::selectDataBaseAll(const QString& tableName, QVector<QVector<QVariant>>& data){
    if(m_db->select(tableName, data))
        return true;
    return false;
}

bool CalculatorTreeModel::updateDataBaseItem(const QString& tableName, const QVector<QVariant>& data){
    if(m_db->updateTableItem(tableName, data))
        return true;
    return false;
}

bool CalculatorTreeModel::updateDataBaseItems(const QString& tableName, const QVector<QVector<QVariant>>& data){
    for(int i = 0; i < data.count(); ++i){
        if(!m_db->updateTableItem(tableName, data.value(i)))
            return false;
    }
    return true;
}

bool CalculatorTreeModel::createDataBaseItem(const QString& tableName, QVector<QVariant>& data){
    if(m_db->insertIntoTable(tableName, data))
        return true;
    return false;
}
