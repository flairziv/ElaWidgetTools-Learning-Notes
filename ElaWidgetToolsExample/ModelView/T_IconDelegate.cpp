#include "T_IconDelegate.h"

#include <QPainter>

#include "ElaDef.h"
#include "ElaTheme.h"
T_IconDelegate::T_IconDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = eTheme->getThemeMode();
    // 监听主题变化，实时更新绘制颜色
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

T_IconDelegate::~T_IconDelegate()
{
}

// QStyleOptionViewItem&	包含绘制区域、状态等信息
void T_IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // 复制一份选项（避免修改原始值）
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    // 检查是否有焦点状态
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        // 移除焦点状态（避免绘制焦点虚线框）
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    // 先调用父类绘制默认背景（选中高亮等）
    QStyledItemDelegate::paint(painter, viewOption, index);
    // 从 Model 获取自定义数据（Qt::UserRole）
    QStringList iconList = index.data(Qt::UserRole).toStringList();
    // 数据格式验证：必须是 [枚举名, 图标字符]
    if (iconList.count() != 2)
    {
        return;
    }

    QString iconName = iconList.at(0);  // 如 "Home"
    QString iconValue = iconList.at(1); // 如 "\ue900"（图标 Unicode 字符）
    
    // 保存画笔状态（以便后续恢复）
    painter->save();
    // 设置抗锯齿和平滑缩放
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    // 再次保存（嵌套 save/restore 用于图标绘制）
    painter->save();
    // 设置图标字体（ElaAwesome 是 Ela 内置的图标字体）
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(22);  // 图标大小 22px
    painter->setFont(iconFont);

    // 设置颜色（根据主题自动选择）
    painter->setPen(ElaThemeColor(_themeMode, BasicText));

    // 绘制图标字符
    // 位置：水平居中偏左 11px，垂直居中偏上 11px
    painter->drawText(option.rect.x() + option.rect.width() / 2 - 11, option.rect.y() + option.rect.height() / 2 - 11, iconValue);
    // 恢复画笔状态
    painter->restore();
    
    // 文字绘制
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    QFont titlefont = painter->font();
    titlefont.setPixelSize(13);
    painter->setFont(titlefont);
    // 计算文字允许的宽度（项宽度的 80%）
    qreal rowTextWidth = option.rect.width() * 0.8;
    // 计算文字需要几行
    // horizontalAdvance() 返回文字的像素宽度
    int subTitleRow = painter->fontMetrics().horizontalAdvance(iconName) / rowTextWidth;
    // 需要换行
    if (subTitleRow > 0)
    {
        QString subTitleText = iconName;
        for (int i = 0; i < subTitleRow + 1; i++)
        {
            // elidedText 截断文字并添加省略号
            QString text = painter->fontMetrics().elidedText(subTitleText, Qt::ElideRight, rowTextWidth);
            // 如果最后 3 个字符包含省略号"…"
            if (text.right(3).contains("…"))
            {
                // 用实际字符替换省略号（特殊处理）
                text = text.replace("…", subTitleText.mid(text.length() - 1, 1));
            }
            // 从源字符串移除已绘制的部分
            subTitleText.remove(0, text.length());
            // 绘制当前行文字（水平居中）
            painter->drawText(option.rect.x() + option.rect.width() / 2 - painter->fontMetrics().horizontalAdvance(text) / 2, option.rect.y() + option.rect.height() - 10 * (subTitleRow + 1 - i), text);
        }
    }
    else
    {
        painter->drawText(option.rect.x() + option.rect.width() / 2 - painter->fontMetrics().horizontalAdvance(iconName) / 2, option.rect.y() + option.rect.height() - 20, iconName);
    }
    // 恢复画笔状态
    painter->restore();
}

QSize T_IconDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(100, 100);
}
