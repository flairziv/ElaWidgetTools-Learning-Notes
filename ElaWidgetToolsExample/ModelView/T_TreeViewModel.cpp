#include "T_TreeViewModel.h"

#include <QIcon>

#include "T_TreeItem.h"
T_TreeViewModel::T_TreeViewModel(QObject* parent)
    : QAbstractItemModel{parent}
{
    _rootItem = new T_TreeItem("root"); // 创建不可见的根节点
    for (int i = 0; i < 20; i++)    // 第 1 层：20 个节点
    {
        T_TreeItem* level1Item = new T_TreeItem(QString("Lv1--TreeItem%1").arg(i + 1), _rootItem);
        for (int j = 0; j < 6; j++) // 第 2 层：每个父节点 6 个子节点
        {
            T_TreeItem* level2Item = new T_TreeItem(QString("Lv2--TreeItem%1").arg(j + 1), level1Item);
            for (int k = 0; k < 6; k++) // 第 3 层
            {
                T_TreeItem* level3Item = new T_TreeItem(QString("Lv3--TreeItem%1").arg(k + 1), level2Item);
                for (int l = 0; l < 6; l++) // 第 4 层
                {
                    T_TreeItem* level4Item = new T_TreeItem(QString("Lv4--TreeItem%1").arg(l + 1), level3Item);
                    level3Item->appendChildItem(level4Item);
                    _itemsMap.insert(level4Item->getItemKey(), level4Item);
                }
                level2Item->appendChildItem(level3Item);
                _itemsMap.insert(level3Item->getItemKey(), level3Item);
            }
            level1Item->appendChildItem(level2Item);
            _itemsMap.insert(level2Item->getItemKey(), level2Item);
        }
        _rootItem->appendChildItem(level1Item);
        _itemsMap.insert(level1Item->getItemKey(), level1Item);
    }
}

T_TreeViewModel::~T_TreeViewModel()
{
    delete _rootItem;
}

// 返回父节点索引 (第 41-58 行) ⭐ 关键方法
QModelIndex T_TreeViewModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }

    // 从索引中取出节点指针
    T_TreeItem* childItem = static_cast<T_TreeItem*>(child.internalPointer());
    T_TreeItem* parentItem = childItem->getParentItem();

    // 如果父节点是根节点，返回无效索引
    if (parentItem == _rootItem)
    {
        return QModelIndex();
    }
    else if (parentItem == nullptr)
    {
        return QModelIndex();
    }

    // 创建父节点的索引
    return createIndex(parentItem->getRow(), 0, parentItem);
}

// 创建节点索引 (第 60-85 行) ⭐ 关键方法
QModelIndex T_TreeViewModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    T_TreeItem* parentItem;
    if (!parent.isValid())
    {
        parentItem = _rootItem; // 无效父索引表示根节点
    }
    else
    {
        parentItem = static_cast<T_TreeItem*>(parent.internalPointer());
    }
    T_TreeItem* childItem = nullptr;
    if (parentItem->getChildrenItems().count() > row)
    {
        childItem = parentItem->getChildrenItems().at(row);
    }
    if (childItem)
    {
        return createIndex(row, column, childItem); // 将指针存入索引
    }
    return QModelIndex();
}

// TreeView 的 rowCount 是相对于父节点的，不是全局行数
int T_TreeViewModel::rowCount(const QModelIndex& parent) const
{
    T_TreeItem* parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        parentItem = _rootItem;
    }
    else
    {
        parentItem = static_cast<T_TreeItem*>(parent.internalPointer());
    }
    return parentItem->getChildrenItems().count();
}

int T_TreeViewModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant T_TreeViewModel::data(const QModelIndex& index, int role) const
{
    // 1. 显示文本
    if (role == Qt::DisplayRole)
    {
        return static_cast<T_TreeItem*>(index.internalPointer())->getItemTitle();
    }
    // 2. 显示图标
    else if (role == Qt::DecorationRole)
    {
        return QIcon(":/Resource/Image/Cirno.jpg");
    }
    // 3. 复选框状态（三态）
    else if (role == Qt::CheckStateRole)
    {
        T_TreeItem* item = static_cast<T_TreeItem*>(index.internalPointer());
        if (item->getIsHasChild())
        {
            return item->getChildCheckState();  // 有子节点：返回三态
        }
        else
        {
            // 叶子节点：二态
            return item->getIsChecked() ? Qt::Checked : Qt::Unchecked;
        }
        return Qt::Unchecked;
    }
    return QVariant();
}

bool T_TreeViewModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::CheckStateRole)
    {
        T_TreeItem* item = static_cast<T_TreeItem*>(index.internalPointer());
        item->setIsChecked(!item->getIsChecked());      // 切换当前节点状态
        item->setChildChecked(item->getIsChecked());    // 递归设置子节点
        Q_EMIT dataChanged(QModelIndex(), QModelIndex(), {role});   // 通知视图刷新
        return true;
    }
    return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags T_TreeViewModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsUserCheckable;   // 添加可勾选标志
    return flags;
}

QVariant T_TreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return QString("ElaTreeView-Example-4Level");
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int T_TreeViewModel::getItemCount() const
{
    return this->_itemsMap.count();
}
