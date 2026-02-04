#ifndef T_ICONDELEGATE_H
#define T_ICONDELEGATE_H

#include <QStyledItemDelegate>      // 带样式的项代理基类

#include "ElaDef.h"
class T_IconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit T_IconDelegate(QObject* parent = nullptr);
    ~T_IconDelegate() override;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override; // 绘制每个项的外观
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;    // 返回每个项的尺寸

private:
    ElaThemeType::ThemeMode _themeMode; // 当前主题模式（亮色/暗色）
};

#endif // T_ICONDELEGATE_H
