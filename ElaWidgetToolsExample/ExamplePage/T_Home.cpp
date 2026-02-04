#include "T_Home.h"

#include <QDebug>
#include <QDesktopServices>     // 打开外部链接
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaAcrylicUrlCard.h"  // 亚克力效果的 URL 卡片
#include "ElaFlowLayout.h"      // 流式布局（自动换行）
#include "ElaImageCard.h"       // 图片卡片
#include "ElaMenu.h"            // 右键菜单
#include "ElaMessageBar.h"      // 消息提示条
#include "ElaNavigationRouter.h"// 导航路由
#include "ElaPopularCard.h"     // 热门卡片
#include "ElaScrollArea.h"      // 滚动区域
#include "ElaText.h"            // 文本组件
#include "ElaToolTip.h"
T_Home::T_Home(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("Home");

    setTitleVisible(false);     // 隐藏 ElaScrollPage 的默认标题
    setContentsMargins(2, 2, 0, 0);

    // 标题卡片区域
    // 副标题
    ElaText* desText = new ElaText("FluentUI For QWidget", this);
    desText->setTextPixelSize(18);
    // 主标题
    ElaText* titleText = new ElaText("ElaWidgetTools", this);
    titleText->setTextPixelSize(35);

    // 标题垂直布局
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(30, 10, 0, 0);
    titleLayout->addWidget(desText);
    titleLayout->addWidget(titleText);

    // 背景图片卡片
    ElaImageCard* backgroundCard = new ElaImageCard(this);
    backgroundCard->setBorderRadius(10);    // 圆角10px
    backgroundCard->setFixedHeight(340);    // 固定高度340px
    backgroundCard->setCardImage(QImage(":/Resource/Image/Home_Background.png"));

    /*
        ElaAcrylicUrlCard 特点：
        * 亚克力模糊背景效果
        * 点击自动打开 URL
        * 支持图标、标题、副标题
    */
    // GitHub 卡片
    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setCardPixmapSize(QSize(62, 62)); // 图标大小
    urlCard1->setFixedSize(195, 225);           // 卡片尺寸
    urlCard1->setTitlePixelSize(17);            // 标题字号
    urlCard1->setTitleSpacing(25);              // 标题与图标的间距
    urlCard1->setSubTitleSpacing(13);           // 副标题间距
    urlCard1->setUrl("https://github.com/Liniyous/ElaWidgetTools");     // 点击跳转的 URL
    urlCard1->setCardPixmap(QPixmap(":/Resource/Image/github.png"));    // 图标
    urlCard1->setTitle("ElaTool Github");       // 标题
    urlCard1->setSubTitle("Use ElaWidgetTools To Create A Cool Project");   // 副标题
    
    // 为卡片添加工具提示
    ElaToolTip* urlCard1ToolTip = new ElaToolTip(urlCard1);
    urlCard1ToolTip->setToolTip("https://github.com/Liniyous/ElaWidgetTools");
    
    // BiliBili 卡片（类似配置）
    ElaAcrylicUrlCard* urlCard2 = new ElaAcrylicUrlCard(this);
    urlCard2->setCardPixmapSize(QSize(62, 62));
    urlCard2->setFixedSize(195, 225);
    urlCard2->setTitlePixelSize(17);
    urlCard2->setTitleSpacing(25);
    urlCard2->setSubTitleSpacing(13);
    urlCard2->setUrl("https://space.bilibili.com/21256707");
    urlCard2->setCardPixmap(QPixmap(":/Resource/Image/Moon.jpg"));
    urlCard2->setTitle("ElaWidgetTool");
    urlCard2->setSubTitle("80985@qq.com");

    // 创建滚动区域容器
    ElaScrollArea* cardScrollArea = new ElaScrollArea(this);
    cardScrollArea->setWidgetResizable(true);   // 内容自适应大小
    cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏水平滚动条
    cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);     // 隐藏垂直滚动条
    cardScrollArea->setIsGrabGesture(true, 0);  // 启用手势拖拽
    cardScrollArea->setIsOverShoot(Qt::Horizontal, true);   // 启用水平方向弹性过度滚动
    
    // 滚动区域内的容器
    QWidget* cardScrollAreaWidget = new QWidget(this);
    cardScrollAreaWidget->setStyleSheet("background-color:transparent;");   // 透明背景
    cardScrollArea->setWidget(cardScrollAreaWidget);

    // URL 卡片水平布局
    QHBoxLayout* urlCardLayout = new QHBoxLayout();
    urlCardLayout->setSpacing(15);
    urlCardLayout->setContentsMargins(30, 0, 0, 6);
    urlCardLayout->addWidget(urlCard1);
    urlCardLayout->addWidget(urlCard2);
    urlCardLayout->addStretch();        // 右侧弹性空间

    // 垂直布局：让卡片在底部
    QVBoxLayout* cardScrollAreaWidgetLayout = new QVBoxLayout(cardScrollAreaWidget);
    cardScrollAreaWidgetLayout->setContentsMargins(0, 0, 0, 0);
    cardScrollAreaWidgetLayout->addStretch();
    cardScrollAreaWidgetLayout->addLayout(urlCardLayout);

    // 将标题和滚动区域组合到背景卡片中
    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundCard);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->addLayout(titleLayout);
    backgroundLayout->addWidget(cardScrollArea);

    // 推荐卡片
    ElaText* flowText = new ElaText("热门免费应用", this);
    flowText->setTextPixelSize(20);

    // 创建 6 张热门卡片（ElaPopularCard）：
    QHBoxLayout* flowTextLayout = new QHBoxLayout();
    flowTextLayout->setContentsMargins(33, 0, 0, 0);
    flowTextLayout->addWidget(flowText);
    // ElaFlowLayout
    ElaPopularCard* homeCard = new ElaPopularCard(this);
    connect(homeCard, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        // 点击打开 URL
        QDesktopServices::openUrl(QUrl("https://github.com/Liniyous/ElaWidgetTools"));
    });
    homeCard->setCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
    homeCard->setTitle("ElaWidgetTool");
    homeCard->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard->setInteractiveTips("免费下载");
    homeCard->setDetailedText("ElaWidgetTools致力于为QWidget用户提供一站式的外观和实用功能解决方案,只需数十MB内存和极少CPU占用以支持高效而美观的界面开发");
    homeCard->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard1 = new ElaPopularCard(this);
    connect(homeCard1, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaScreenNavigation();
    });
    homeCard1->setTitle("ElaScreen");
    homeCard1->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard1->setCardPixmap(QPixmap(":/Resource/Image/control/AutomationProperties.png"));
    homeCard1->setInteractiveTips("免费使用");
    homeCard1->setDetailedText("使用ElaDxgiManager获取屏幕的实时数据，以QImage的形式处理数据，支持切换采集设备和输出设备。");
    homeCard1->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard2 = new ElaPopularCard(this);
    connect(homeCard2, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaSceneNavigation();
    });
    homeCard2->setTitle("ElaScene");
    homeCard2->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard2->setCardPixmap(QPixmap(":/Resource/Image/control/Canvas.png"));
    homeCard2->setInteractiveTips("免费使用");
    homeCard2->setDetailedText("使用ElaScene封装的高集成度API进行快速拓扑绘图开发，对基于连接的网络拓扑特化处理。");
    homeCard2->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard3 = new ElaPopularCard(this);
    connect(homeCard3, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaBaseComponentNavigation();
    });
    homeCard3->setTitle("ElaBaseComponent");
    homeCard3->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard3->setCardPixmap(QPixmap(":/Resource/Image/control/StandardUICommand.png"));
    homeCard3->setInteractiveTips("免费使用");
    homeCard3->setDetailedText("添加ElaBaseComponent页面中的基础组件到你的项目中以进行快捷开发，使用方便，结构整洁，API规范");
    homeCard3->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard4 = new ElaPopularCard(this);
    connect(homeCard4, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaCardNavigation();
    });
    homeCard4->setTitle("ElaCard");
    homeCard4->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard4->setCardPixmap(QPixmap(":/Resource/Image/control/FlipView.png"));
    homeCard4->setInteractiveTips("免费使用");
    homeCard4->setDetailedText("使用ElaCard系列组件，包括促销卡片和促销卡片视窗来快速建立循环动画。");
    homeCard4->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard5 = new ElaPopularCard(this);
    connect(homeCard5, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaIconNavigation();
    });
    homeCard5->setTitle("ElaIcon");
    homeCard5->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard5->setCardPixmap(QPixmap(":/Resource/Image/control/CommandBarFlyout.png"));
    homeCard5->setInteractiveTips("免费使用");
    homeCard5->setDetailedText("在该界面快速挑选你喜欢的图标应用到项目中，以枚举的形式使用它");
    homeCard5->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    /*
        * ElaFlowLayout 特点：
        * 根据窗口宽度自动换行
        * 支持动画效果
        * 类似 CSS 的 flex-wrap: wrap
    */
    ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
    flowLayout->setContentsMargins(30, 0, 0, 0);
    flowLayout->setIsAnimation(true);
    flowLayout->addWidget(homeCard);
    flowLayout->addWidget(homeCard1);
    flowLayout->addWidget(homeCard2);
    flowLayout->addWidget(homeCard3);
    flowLayout->addWidget(homeCard4);
    flowLayout->addWidget(homeCard5);

    // 菜单
    _homeMenu = new ElaMenu(this);
    ElaMenu* checkMenu = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu->addAction("查看1");
    checkMenu->addAction("查看2");
    checkMenu->addAction("查看3");
    checkMenu->addAction("查看4");

    ElaMenu* checkMenu1 = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu1->addAction("查看1");
    checkMenu1->addAction("查看2");
    checkMenu1->addAction("查看3");
    checkMenu1->addAction("查看4");

    ElaMenu* checkMenu2 = checkMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu2->addAction("查看1");
    checkMenu2->addAction("查看2");
    checkMenu2->addAction("查看3");
    checkMenu2->addAction("查看4");

    // QKeySequence key = QKeySequence(Qt::CTRL | Qt::Key_S);

    _homeMenu->addSeparator();
    _homeMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    _homeMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    QAction* action = _homeMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    connect(action, &QAction::triggered, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });

    _homeMenu->addElaIconAction(ElaIconType::Copy, "复制");
    _homeMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setSpacing(0);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(backgroundCard);
    centerVLayout->addSpacing(20);
    centerVLayout->addLayout(flowTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(flowLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget);

    // 初始化提示
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success", "初始化成功!", 2000);
    qDebug() << "初始化成功";
}

T_Home::~T_Home()
{
}

void T_Home::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::RightButton:
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        _homeMenu->popup(event->globalPosition().toPoint());
#else
        _homeMenu->popup(event->globalPos());
#endif
        break;
    }
    default:
    {
        break;
    }
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
