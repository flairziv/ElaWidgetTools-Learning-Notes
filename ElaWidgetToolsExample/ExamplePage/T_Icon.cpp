#include "T_Icon.h"

#include <QApplication>
#include <QClipboard>           // 剪贴板操作
#include <QHeaderView>
#include <QStandardItemModel>
#include <QVBoxLayout>

#include "ElaLineEdit.h"        // 搜索输入框
#include "ElaListView.h"        // 列表视图 (V)
#include "ElaMessageBar.h"      // 消息提示
#include "T_IconDelegate.h"     // 绘制代理 (D)
#include "T_IconModel.h"        // 数据模型 (M)
T_Icon::T_Icon(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaIcon");
    // 顶部元素
    createCustomWidget("一堆常用图标被放置于此，左键单击以复制其枚举");

    // 获取枚举元信息（用于搜索功能）
    _metaEnum = QMetaEnum::fromType<ElaIconType::IconName>();

    // 创建 ElaListView（视图）
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 5, 0);
    centralWidget->setWindowTitle("ElaIcon");

    // ═══════════════════════════════════════════════════
    // 创建列表视图 (View)
    // ═══════════════════════════════════════════════════
    _iconView = new ElaListView(this);
    _iconView->setIsTransparent(true);              // 透明背景

    // 关键配置：设置为图标模式
    _iconView->setFlow(QListView::LeftToRight);     // 从左到右排列
    _iconView->setViewMode(QListView::IconMode);    // 图标模式（类似桌面图标）
    _iconView->setResizeMode(QListView::Adjust);    // 窗口变化时自动重排

    // 监听点击事件
    connect(_iconView, &ElaListView::clicked, this, [=](const QModelIndex& index) {
        // ① 从 Model 获取图标名称
        // 内部调用: _iconModel->getIconNameFromModelIndex(index)
        // 返回: "ElaIconType::Home"
        QString iconName = _iconModel->getIconNameFromModelIndex(index);
        if (iconName.isEmpty())
        {
            return;
        }
        // ② 复制到系统剪贴板
        qApp->clipboard()->setText(iconName);
        // ③ 显示成功提示
        ElaMessageBar::success(ElaMessageBarType::Top, "复制完成", iconName + "已被复制到剪贴板", 1000, this);
    });

    // 创建数据模型 (Model)
    _iconModel = new T_IconModel(this);
    // 创建绘制代理 (Delegate)
    _iconDelegate = new T_IconDelegate(this);

    // 将 M 和 D 关联到 V
    _iconView->setModel(_iconModel);
    _iconView->setItemDelegate(_iconDelegate);
    _iconView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建搜索框
    _searchEdit = new ElaLineEdit(this);
    _searchEdit->setPlaceholderText("搜索图标");
    _searchEdit->setFixedSize(300, 35);

    // 连接搜索信号
    // textEdited   用户手动输入时（每输入一个字符触发一次）
    // focusIn  输入框获得焦点时
    connect(_searchEdit, &ElaLineEdit::textEdited, this, &T_Icon::onSearchEditTextEdit);
    connect(_searchEdit, &ElaLineEdit::focusIn, this, &T_Icon::onSearchEditTextEdit);

    centerVLayout->addSpacing(13);
    centerVLayout->addWidget(_searchEdit);
    centerVLayout->addWidget(_iconView);
    this->addCentralWidget(centralWidget, true, true, 0);
}

T_Icon::~T_Icon()
{
}

void T_Icon::onSearchEditTextEdit(const QString& searchText)
{
    // 情况1：搜索框为空 → 显示所有图标
    if (searchText.isEmpty())
    {
        _iconModel->setIsSearchMode(false);             // 关闭搜索模式
        _iconModel->setSearchKeyList(QStringList());    // 清空搜索列表
        _iconView->clearSelection();                    // 清除选中
        _iconView->viewport()->update();                // 刷新视图
        return;
    }
    // 情况2：有搜索词 → 筛选匹配的图标
    QStringList searchKeyList;

    // 遍历所有枚举（从 1 开始跳过 None）
    for (int i = 1; i < _metaEnum.keyCount(); i++)
    {
        QString key = QString(_metaEnum.key(i));    // 如 "Home", "HomePage", "HomeAlt"
        // 不区分大小写匹配
        if (key.contains(searchText, Qt::CaseInsensitive))
        {
            searchKeyList.append(key);
        }
    }

    // 更新 Model 状态
    _iconModel->setIsSearchMode(true);              // 开启搜索模式
    _iconModel->setSearchKeyList(searchKeyList);    // 设置搜索结果
    _iconView->clearSelection();                    // 清除选中
    _iconView->scrollTo(_iconModel->index(0, 0));   // 滚动到第一个结果
    _iconView->viewport()->update();                // 刷新视图
}
