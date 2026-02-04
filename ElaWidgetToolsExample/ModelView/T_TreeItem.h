#ifndef T_TREEITEM_H
#define T_TREEITEM_H

#include <QModelIndex>
#include <QObject>

#include "ElaProperty.h"
class T_TreeItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(QList<T_TreeItem*>, ChildrenItems)    // 子节点列表
    Q_PROPERTY_CREATE(bool, IsChecked)                      // 是否勾选
    Q_PRIVATE_CREATE(T_TreeItem*, ParentItem)               // 父节点指针
public:
    explicit T_TreeItem(QString itemTitle, T_TreeItem* parent = nullptr);
    ~T_TreeItem();
    QString getItemKey() const;     // 获取唯一标识
    QString getItemTitle() const;   // 获取显示标题

    void setChildChecked(bool isChecked);   // 递归设置子节点勾选状态
    Qt::CheckState getChildCheckState();    // 获取子节点的复选状态（三态）

    void appendChildItem(T_TreeItem* childItem);    // 添加子节点
    bool getIsHasChild() const;                     // 是否有子节点
    int getRow() const;                             // 获取在父节点中的行索引

private:
    QString _itemKey = "";      // 唯一标识（UUID）
    QString _itemTitle = "";    // 显示标题
    bool _isExpanded{false};    // 是否展开
};

#endif // T_TREEITEM_H
