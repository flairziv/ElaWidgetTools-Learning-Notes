#ifndef T_ICON_H
#define T_ICON_H

#include <QMetaEnum>

#include "T_BasePage.h"
class ElaLineEdit;
class ElaListView;
class T_IconModel;
class T_IconDelegate;
class T_Icon : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Icon(QWidget* parent = nullptr);
    ~T_Icon();
    // 搜索功能实现
    Q_SLOT void onSearchEditTextEdit(const QString& searchText);

private:
    QMetaEnum _metaEnum;                // 存储 ElaIconType::IconName 枚举的元信息
    ElaLineEdit* _searchEdit{nullptr};  // 搜索输入框
    ElaListView* _iconView;             // 图标列表视图
    T_IconModel* _iconModel;            // 数据模型
    T_IconDelegate* _iconDelegate;      // 绘制代理
};

#endif // T_ICON_H
