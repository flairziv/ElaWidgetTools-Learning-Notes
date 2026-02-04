#ifndef T_ICONMODEL_H
#define T_ICONMODEL_H

#include <QAbstractListModel>
#include <QMetaEnum>

#include "ElaProperty.h"
class T_IconModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY_CREATE(bool, IsSearchMode)   // 自动生成 _pIsSearchMode 和 get/set 方法
public:
    explicit T_IconModel(QObject* parent = nullptr);
    ~T_IconModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    void setSearchKeyList(QStringList list);    // 设置搜索结果
    QStringList getSearchKeyList();             // 获取搜索结果

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;     //返回单元格数据
    QString getIconNameFromModelIndex(const QModelIndex& index) const;                      // 获取完整枚举名

private:
    QMetaEnum _metaEnum;        // 枚举元信息
    QStringList _searchKeyList; // 搜索结果列表
    int _rowCount;              // 当前行数
};

#endif // T_ICONMODEL_H
