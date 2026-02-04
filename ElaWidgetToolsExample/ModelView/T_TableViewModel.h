#ifndef T_TABLEVIEWMODEL_H
#define T_TABLEVIEWMODEL_H

#include <QAbstractTableModel>

class T_TableViewModel : public QAbstractTableModel // 继承表格模型基类
{
    Q_OBJECT
public:
    explicit T_TableViewModel(QObject* parent = nullptr);
    ~T_TableViewModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QStringList _header;            // 表头数据
    QList<QStringList> _dataList;   // 二维数据（每行是一个 QStringList）
    QList<QIcon> _iconList;         // 图标列表
};

#endif // T_TABLEVIEWMODEL_H
