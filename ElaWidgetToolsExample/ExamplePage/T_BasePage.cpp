#include "T_BasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaMenu.h"
#include "ElaText.h"
#include "ElaTheme.h"       // 主题管理器（全局单例 eTheme）
#include "ElaToolButton.h"
#include "ElaWindow.h"      // 主窗口类（用于回溯导航）
T_BasePage::T_BasePage(QWidget* parent)
    : ElaScrollPage(parent)
{
    // 连接主题变化信号
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
        if (!parent)        // 如果没有父窗口
        {
            update();       // 手动触发重绘以更新主题
        }
    });
    setContentsMargins(20, 5, 0, 0);    // 设置页面边距：左20、上5、右0、下0
}

T_BasePage::~T_BasePage()
{
}

void T_BasePage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);

    // GitHub 链接文本
    ElaText* subTitleText = new ElaText(this);
    subTitleText->setText("https://github.com/Liniyous/ElaWidgetTools");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);   // 允许鼠标选择复制
    subTitleText->setTextPixelSize(11);

    // Documentation 按钮（带下拉菜单）
    ElaToolButton* documentationButton = new ElaToolButton(this);
    documentationButton->setFixedHeight(35);
    documentationButton->setIsTransparent(false);   // 非透明背景
    documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);  // 图标在文字旁边
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    documentationButton->setText("Documentation");
    documentationButton->setElaIcon(ElaIconType::FileDoc);
    ElaMenu* documentationMenu = new ElaMenu(this);
    documentationMenu->addElaIconAction(ElaIconType::CardsBlank, "CardsBlank");
    documentationMenu->addElaIconAction(ElaIconType::EarthAmericas, "EarthAmericas");
    documentationButton->setMenu(documentationMenu);

    // Source 按钮（带下拉菜单）
    ElaToolButton* sourceButton = new ElaToolButton(this);
    sourceButton->setFixedHeight(35);
    sourceButton->setIsTransparent(false);
    sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sourceButton->setText("Source");
    sourceButton->setElaIcon(ElaIconType::NfcSymbol);
    ElaMenu* sourceMenu = new ElaMenu(this);
    sourceMenu->addElaIconAction(ElaIconType::FireBurner, "FireBurner");
    sourceMenu->addElaIconAction(ElaIconType::Galaxy, "Galaxy~~~~");
    sourceButton->setMenu(sourceMenu);

    // 主题切换按钮
    ElaToolButton* themeButton = new ElaToolButton(this);
    themeButton->setFixedSize(35, 35);  // 固定尺寸的方形按钮
    themeButton->setIsTransparent(false);
    themeButton->setElaIcon(ElaIconType::MoonStars);    // 月亮星星图标
    connect(themeButton, &ElaToolButton::clicked, this, [=]() {
        // 点击时切换主题：亮色→暗色，暗色→亮色
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });

    // 回溯导航按钮
    ElaToolButton* backtrackButton = new ElaToolButton(this);
    backtrackButton->setFixedSize(35, 35);
    backtrackButton->setIsTransparent(false);
    backtrackButton->setElaIcon(ElaIconType::Timer);    // 计时器图标
    connect(backtrackButton, &ElaToolButton::clicked, this, [=]() {
        // 获取主窗口并调用回溯导航
        ElaWindow* window = dynamic_cast<ElaWindow*>(this->window());   // this->window() 获取顶层窗口
        if (window)
        {
            window->backtrackNavigationNode(property("ElaPageKey").toString()); // property("ElaPageKey") 获取页面注册时的唯一标识
        }
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(backtrackButton);
    buttonLayout->addSpacing(15);

    ElaText* descText = new ElaText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}
