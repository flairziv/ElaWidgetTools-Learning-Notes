#ifndef T_LOGMODEL_H
#define T_LOGMODEL_H

#include <QAbstractListModel>

class T_LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit T_LogModel(QObject* parent = nullptr);
    ~T_LogModel();
    void setLogList(QStringList list);      // 设置完整日志列表
    void appendLogList(QString log);        // 追加单条日志
    QStringList getLogList() const;         // 获取日志列表

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _logList;       // 存储日志的字符串列表
};

#endif // T_LOGMODEL_H
