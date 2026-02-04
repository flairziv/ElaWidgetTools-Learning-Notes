#ifndef T_TREEVIEWMODEL_H
#define T_TREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QObject>

class T_TreeItem;
class T_TreeViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit T_TreeViewModel(QObject* parent = nullptr);
    ~T_TreeViewModel();
    // 必须重写的 6 个核心方法
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 可选重写（支持编辑/勾选）
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    int getItemCount() const;

private:
    QMap<QString, T_TreeItem*> _itemsMap;   // 所有节点的快速查找表
    T_TreeItem* _rootItem{nullptr};         // 根节点（不显示）
};

#endif // T_TREEVIEWMODEL_H
