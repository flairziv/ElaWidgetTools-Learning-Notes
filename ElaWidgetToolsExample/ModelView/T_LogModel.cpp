#include "T_LogModel.h"

T_LogModel::T_LogModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

T_LogModel::~T_LogModel()
{
}

// 返回数据行数
int T_LogModel::rowCount(const QModelIndex& parent) const
{
    return this->_logList.count();
}

// 返回指定索引的数据
QVariant T_LogModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return _logList[index.row()];
    }
    return QVariant();
}

// 设置完整日志列表
void T_LogModel::setLogList(QStringList list)
{
    beginResetModel();
    this->_logList = list;
    endResetModel();
}

// 追加单条日志
void T_LogModel::appendLogList(QString log)
{
    beginResetModel();
    this->_logList.append(log);
    endResetModel();
}

// 获取日志列表
QStringList T_LogModel::getLogList() const
{
    return this->_logList;
}
