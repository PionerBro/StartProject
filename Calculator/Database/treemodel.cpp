#include "treemodel.h"

#include <QIcon>
#include <QBrush>

TreeModel::TreeModel(const QString& sqlTable, const QVector<QVariant>& headers, QObject* parent):QAbstractItemModel(parent)
{
    QSqlDatabase db = QSqlDatabase::database("QSLITE", "d");
    m_db = &db;
    setSortCol(0);
    m_sqlTable = sqlTable;
    m_hHeaderData = headers;
    //setupModelData();
    editableCols.fill(false, m_hHeaderData.count());
}

TreeModel::~TreeModel(){
    delete m_root;
}

QVariant TreeModel::data(const QModelIndex& index, int role)const{
    if(!index.isValid())
        return QVariant();

    int iColumn = index.column();

    switch (role) {
    case Qt::DecorationRole:
        if(iColumn == 0){
            int itemType = static_cast<TreeItem*>(index.internalPointer())->getItemType();
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
                return static_cast<TreeItem*>(index.internalPointer())->data(iColumn);
            }
        }
    default:
        return QVariant();
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index)const{
    if(!index.isValid())
        return Qt::NoItemFlags;

    if(editMode && editableCols.value(index.column()))
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)const{
    if(!hasIndex(row,column,parent))
        return QModelIndex();

    if(treeMode){
        TreeItem* item;
        if(!parent.isValid())
            item = m_curRootItem;
        else
            item = static_cast<TreeItem*>(parent.internalPointer());
        TreeItem* child = item->child(row);
        if(child)
            return createIndex(row, column, child);
        else
            return QModelIndex();
    }else{
        TreeItem* child = m_treeElements.value(row);
        if(child)
            return createIndex(row, column, child);
        else
            return QModelIndex();
    }
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_hHeaderData.value(section);
    return QVariant();
}

QModelIndex TreeModel::parent(const QModelIndex &index)const{
    if(!index.isValid())
        return QModelIndex();
    if(treeMode){
        TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
        TreeItem *parentItem = childItem->parent();
        if(parentItem == m_curRootItem)
           return QModelIndex();
        return createIndex(parentItem->row(),0, parentItem);
    }else{
        return QModelIndex();
    }
}

int TreeModel::rowCount(const QModelIndex &parent)const{
    if(treeMode){
        TreeItem* parentItem;
        if(parent.column()>0)
            return 0;
        if(!parent.isValid())
            parentItem = m_curRootItem;
        else
            parentItem = static_cast<TreeItem*>(parent.internalPointer());
        return parentItem->childCount();
    }else{
        return m_treeElements.count();
    }
}

int TreeModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent);
    return m_hHeaderData.count();
}

//устанавливаем заголовок, при этом сохраняя порядок изменяемых колонок
void TreeModel::sethHeaderData(QVector<QVariant> &headers){
    m_hHeaderData.clear();
    m_hHeaderData<<headers;
    QVector<bool> tmp;
    tmp.fill(false, columnCount());
    for(int i = 0; i < tmp.count() && i < editableCols.count(); ++i){
        tmp[i] = editableCols.value(i);
    }
    editableCols = tmp;
}

QVector<QVariant> TreeModel::getRowData(const QModelIndex &index) const{
    return static_cast<TreeItem*>(index.internalPointer())->rowData();
}

void TreeModel::setColumnEditable(int col, bool value){
    if(col >= 3 && col < editableCols.count())
        editableCols[col] = value;
}

void TreeModel::setSortCol(int col){
    m_sortCol = col;
    for(int i = 0 ; i < m_treeFolders.count(); ++i){
        m_treeFolders.value(i)->setSortCol(col);
        m_treeFolders.value(i)->sortItem();
    }
    std::sort(m_treeElements.begin(), m_treeElements.end(), compare);
}

bool TreeModel::getEditModeValue() const{
    return editMode;
}

TreeItem* TreeModel::currentRoot() const{
    return m_curRootItem;
}

void TreeModel::setupModelData(){
    if(m_root){
        delete m_root;
        m_treeElements.clear();
        m_treeFolders.clear();
    }
    m_root = new TreeItem({0,0,0});
    m_curRootItem = m_root;
    m_treeFolders<<m_root;
    QVector<QVector<QVariant>> dbData;
    if(selectDataBaseAll(m_sqlTable, dbData)){
        for(int i = 0; i < dbData.count(); ++i){
            QVector<QVariant> itemData = dbData.value(i);
            int folderNum = itemData.value(FieldName::ParentId).toLongLong();
            TreeItem* item = new TreeItem(itemData, m_treeFolders[folderNum]);
            m_treeFolders[folderNum]->appendChild(item);
            (item->getItemType()) ? m_treeFolders<<item : m_treeElements<<item;
        }
        for(int i = 0; i < m_treeFolders.count(); ++i){
            m_treeFolders.value(i)->sortItem();
        }
        std::sort(m_treeElements.begin(), m_treeElements.end(), compare);
    }
}

void TreeModel::setCalculatorDatabase(QSqlDatabase *db){
    m_db = db;
}

QVariant TreeModel::getReserveData(int row, int col) const{
    QVector<QVariant> data;
    data = reserveData.value(col, {});
    if(!data.isEmpty())
        return data.value(row);
    return QVariant();
}

bool TreeModel::isEditableCol(int col) const{
    return editableCols.value(col);
}

void TreeModel::rootItemChanged(QModelIndex index){
    if(!index.isValid())
        return;
    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
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

bool TreeModel::createModelItem(QVector<QVariant> &data){
    if(createDataBaseItem(m_sqlTable, data)){
        beginResetModel();
        //QVector<QVariant> itemData = data;
        TreeItem* item = new TreeItem(data, m_curRootItem);
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

bool TreeModel::updateModelItem(const QVector<QVariant> &data, const QModelIndex &index){
    if(updateDataBaseItem(m_sqlTable, data)){
        beginResetModel();
        static_cast<TreeItem*>(index.internalPointer())->setRowData(data);
        endResetModel();
        return true;
    }
    return false;
}

bool TreeModel::updateModelItems(const QVector<QVector<QVariant>> &data, const QVector<TreeItem*> &items, const QVariant& date){
    if(updateDataBaseItems(m_sqlTable,data, date)){
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

void TreeModel::setTreeMode(bool value){
    if(!editMode){
        beginResetModel();
        treeMode = value;
        endResetModel();
    }
}

void TreeModel::setEditMode(bool value){
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

void TreeModel::reserveDataChange(int row, int column, const QString& text){
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

void TreeModel::reserveDataAccept(const QVariant& date){
    if(editMode){
        beginResetModel();
        reserveAc.fill(false, m_treeElements.count());
        QVector<int> editCols;
        for(int j = 0; j < editableCols.count(); ++j){
            if(editableCols.value(j))
                editCols<<j;
        }
        QVector<QVector<QVariant>> updateData;
        QVector<TreeItem*> updateItems;
        for(int i = 0; i < changedRow.count(); ++i){
            bool chFlag = false;
            int num = changedRow.value(i);
            TreeItem* item = m_treeElements.value(num);
            QVector<QVariant> itemData = item->rowData();
            for(int j = 0; j<editCols.count(); ++j){
                int column = editCols.value(j);
                if(item->data(column) != reserveData.value(column).value(num)){
                    chFlag = true;
                    itemData[column] = reserveData.value(column).value(num);
                }
            }
            if(chFlag){
                updateData<<itemData;
                updateItems<<item;
            }
        }
        if(updateModelItems(updateData, updateItems, date)){
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

void TreeModel::reserveDataReject(){
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

bool TreeModel::selectDataBaseAll(const QString& tableName, QVector<QVector<QVariant>>& data){
    //if(m_db->select(tableName, data))
        return true;
    //return false;
}

bool TreeModel::updateDataBaseItem(const QString& tableName, const QVector<QVariant>& data){
    //if(m_db->updateTableItem(tableName, data))
        return true;
    //return false;
}

bool TreeModel::updateDataBaseItems(const QString& tableName, const QVector<QVector<QVariant>>& data, const QVariant&){
    for(int i = 0; i < data.count(); ++i){
        //if(!m_db->updateTableItem(tableName, data.value(i)))
          //  return false;
    }
    return true;
}

bool TreeModel::createDataBaseItem(const QString& tableName, QVector<QVariant>& data){
    //if(m_db->insertIntoTable(tableName, data))
        return true;
    //return false;
}

