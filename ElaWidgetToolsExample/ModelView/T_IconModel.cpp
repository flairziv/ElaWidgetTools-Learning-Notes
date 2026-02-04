#include "T_IconModel.h"

#include "ElaDef.h"
T_IconModel::T_IconModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _metaEnum = QMetaEnum::fromType<ElaIconType::IconName>();
    _rowCount = _metaEnum.keyCount() - 1;       // 计算图标总数（减 1 是因为第一个通常是 None 或无效值）
    _pIsSearchMode = false;                     // 初始化搜索模式为 false（由 Q_PROPERTY_CREATE 宏生成的成员变量）
}

T_IconModel::~T_IconModel()
{
}

int T_IconModel::rowCount(const QModelIndex& parent) const
{
    return _rowCount;
}

void T_IconModel::setSearchKeyList(QStringList list)
{
    // 通知 View 模型即将重置
    beginResetModel();
    // 更新搜索结果列表
    this->_searchKeyList = list;
    // 根据模式设置行数
    if (_pIsSearchMode)
    {
        // 搜索模式：行数 = 匹配的图标数量
        _rowCount = this->getSearchKeyList().count();
    }
    else
    {
        // 正常模式：行数 = 全部图标数量（减 1）
        _rowCount = _metaEnum.keyCount() - 1;
    }
    // 通知 View 模型重置完成，View 会重新获取数据
    endResetModel();
}

QStringList T_IconModel::getSearchKeyList()
{
    return this->_searchKeyList;
}

QVariant T_IconModel::data(const QModelIndex& index, int role) const
{
    /*
        数据角色（Role）说明：
        * Qt::DisplayRole       默认显示文本
        * Qt::DecorationRole    图标/装饰
        * Qt::UserRole          自定义数据（本例使用）
        * Qt::UserRole + 1      更多自定义数据...
    */
    // 只响应 Qt::UserRole（自定义数据角色）
    if (role == Qt::UserRole)
    {
        if (!_pIsSearchMode)
        {
            // 边界检查
            if (index.row() >= _metaEnum.keyCount() - 1)
            {
                // 返回空值
                return QVariant();
            }
            // 返回 QStringList：[枚举名, 图标字符]
            // index.row() + 1 是因为跳过第一个（通常是 None）
            return QStringList{_metaEnum.key(index.row() + 1), QChar(_metaEnum.value(index.row() + 1))};
        }
        // 搜索模式：从搜索结果列表获取
        else
        {
            QStringList iconList;
            // 边界检查
            if (index.row() >= _searchKeyList.count())
            {
                return QVariant();
            }
            // 获取枚举名
            iconList.append(_searchKeyList.at(index.row()));
            // 根据枚举名获取对应的数值，转为字符
            iconList.append(QChar(_metaEnum.keyToValue(_searchKeyList.at(index.row()).toUtf8().constData())));
            return iconList;
        }
    }
    return QVariant();
}

QString T_IconModel::getIconNameFromModelIndex(const QModelIndex& index) const
{
    QString iconName;
    if (_pIsSearchMode)
    {
        // 搜索模式：从搜索结果列表获取
        if (index.row() < _searchKeyList.count())
        {
            // 例如: "ElaIconType::Home"
            iconName = QString("ElaIconType::") + _searchKeyList.at(index.row());
        }
    }
    else
    {
        // 正常模式：从枚举元信息获取
        if (index.row() < _metaEnum.keyCount() - 1)
        {
            // 例如: "ElaIconType::Home"
            iconName = QString("ElaIconType::") + _metaEnum.key(index.row() + 1);
        }
    }
    return iconName;
}
