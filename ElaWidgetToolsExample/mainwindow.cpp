#include "mainwindow.h"

#include "ElaContentDialog.h"           // 内容对话框（关闭确认弹窗）
#include "ElaDockWidget.h"              // 停靠窗口（日志面板等）
#include "ElaEventBus.h"                // 事件总线（组件间通信）
#include "ElaLog.h"                     // 日志管理器
#include "ElaMenu.h"                    // 菜单
#include "ElaMenuBar.h"                 // 菜单栏
#include "ElaNavigationRouter.h"        // 导航路由器（前进/后退）
#include "ElaProgressBar.h"             // 进度条
#include "ElaProgressRing.h"            // 环形进度环
#include "ElaStatusBar.h"               // 状态栏
#include "ElaSuggestBox.h"              // 搜索建议框
#include "ElaText.h"                    // 文本标签
#include "ElaTheme.h"                   // 主题管理器
#include "ElaToolBar.h"                 // 工具栏
#include "ElaToolButton.h"              // 工具按钮

// ===== 示例页面 =====
#include "T_About.h"
#include "T_BaseComponents.h"
#include "T_Card.h"
#include "T_Graphics.h"
#include "T_ListView.h"
#include "T_Setting.h"
#include "T_TableView.h"
#include "T_TreeView.h"

#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QMouseEvent>

// ===== Windows 专属 =====
#ifdef Q_OS_WIN
#include "ElaApplication.h"
#include "ExamplePage/T_ElaScreen.h"
#include <QTimer>
#endif

#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_Icon.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_Navigation.h"
#include "ExamplePage/T_Popup.h"
#include "ExamplePage/T_UpdateWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : ElaWindow(parent)
{
    initWindow();       // 初始化窗口基本配置

    //额外布局
    initEdgeLayout();   // 初始化边缘布局（菜单栏、工具栏、停靠窗口、状态栏）

    //中心窗口
    initContent();      // 初始化中心内容（导航页面）

    // ===== 拦截默认关闭事件，弹出确认对话框 =====
    _closeDialog = new ElaContentDialog(this);

    // 右按钮（确认关闭） → 关闭窗口
    connect(_closeDialog, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    
    // 中间按钮（最小化） → 关闭对话框 + 窗口最小化
    connect(_closeDialog, &ElaContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
    });

    // 禁止默认关闭行为，改为弹出对话框
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });

    //移动到中心
    //moveToCenter();

    //  如果你的windows版本低于Win11 调用原生Mica、Mica-Alt、Acrylic 会导致窗口绘制失效  Dwm_Blur仍可使用
    //    eTheme->setThemeMode(ElaThemeType::Dark);
    //    QTimer::singleShot(1, this, [=]() {
    //        eApp->setWindowDisplayMode(ElaApplicationType::Mica);
    //    });

    //    QTimer::singleShot(1, this, [=]() {
    //        showFullScreen();
    //    });
}

MainWindow::~MainWindow()
{
    delete this->_aboutPage;
}

void MainWindow::initWindow()
{
    setFocusPolicy(Qt::StrongFocus);                    // 接收键盘焦点
    setIsCentralStackedWidgetTransparent(true);         // 窗口图标
    setWindowIcon(QIcon(":/include/Image/Cirno.jpg"));  // 初始大小
    resize(1200, 740);

    // eTheme->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);

    // 用户信息卡片设置（左上角）
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));   // 头像
    setUserInfoCardTitle("Ela Tool");                               // 标题
    setUserInfoCardSubTitle("Liniyous@gmail.com");                  // 副标题
    setWindowTitle("ElaWidgetTool");                                // 窗口标题

    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
    // setNavigationBarWidth(260);

    // 默认中心页面
    ElaText* centralStack = new ElaText("这是一个主窗口堆栈页面", this);
    centralStack->setFocusPolicy(Qt::StrongFocus);
    centralStack->setTextPixelSize(32);
    centralStack->setAlignment(Qt::AlignCenter);
    addCentralWidget(centralStack);

    // 窗口背景图片（浅色/深色主题分别设置）
    setWindowPixmap(ElaThemeType::Light, QPixmap(":/Resource/Image/WindowBase/Miku.png"));
    setWindowPixmap(ElaThemeType::Dark, QPixmap(":/Resource/Image/WindowBase/WorldTree.jpg"));
    
     // 窗口动画背景（浅色/深色主题分别设置）
    setWindowMoviePath(ElaThemeType::Light, ":/Resource/Image/WindowBase/Miku.gif");
    setWindowMoviePath(ElaThemeType::Dark, ":/Resource/Image/WindowBase/WorldTree.gif");
    //setWindowPaintMode(ElaWindowType::PaintMode::Pixmap); // 取消注释可激活图片背景

    // 自定义AppBar菜单
    ElaMenu* appBarMenu = new ElaMenu(this);
    appBarMenu->setMenuItemHeight(27);

    // 菜单项：跳转到不同堆栈
    connect(appBarMenu->addAction("跳转到一级主要堆栈"), &QAction::triggered, this, [=]() {
        setCurrentStackIndex(0);
    });
    connect(appBarMenu->addAction("跳转到二级主要堆栈"), &QAction::triggered, this, [=]() {
        setCurrentStackIndex(1);
    });

    // 菜单项：更改页面切换特效
    connect(appBarMenu->addAction("更改页面切换特效(Scale)"), &QAction::triggered, this, [=]() {
        setStackSwitchMode(ElaWindowType::StackSwitchMode::Scale);
    });

    // 带图标的菜单项：跳转到设置页面
    connect(appBarMenu->addElaIconAction(ElaIconType::GearComplex, "自定义主窗口设置"), &QAction::triggered, this, [=]() {
        navigation(_settingKey);    // 导航到设置页面
    });

    appBarMenu->addSeparator();
    
    // 菜单项：切换主题
    connect(appBarMenu->addElaIconAction(ElaIconType::MoonStars, "更改项目主题"), &QAction::triggered, this, [=]() {
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });

    connect(appBarMenu->addAction("使用原生菜单"), &QAction::triggered, this, [=]() {
        setCustomMenu(nullptr);
    });

    // 设置为标题栏菜单
    setCustomMenu(appBarMenu);

    // 堆栈独立自定义窗口
    QWidget* centralCustomWidget = new QWidget(this);
    QHBoxLayout* centralCustomWidgetLayout = new QHBoxLayout(centralCustomWidget);
    centralCustomWidgetLayout->setContentsMargins(13, 15, 9, 6);
    
    // ===== 后退按钮 =====
    ElaToolButton* leftButton = new ElaToolButton(this);
    leftButton->setElaIcon(ElaIconType::AngleLeft);     // ← 图标
    leftButton->setEnabled(false);                      // 初始禁用
    connect(leftButton, &ElaToolButton::clicked, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();  // 后退
    });

    // ===== 前进按钮 =====
    ElaToolButton* rightButton = new ElaToolButton(this);
    rightButton->setElaIcon(ElaIconType::AngleRight);     // → 图标
    rightButton->setEnabled(false);                       // 初始禁用
    connect(rightButton, &ElaToolButton::clicked, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteForward();   // 前进
    });

    // 根据导航路由状态启用/禁用按钮
    connect(ElaNavigationRouter::getInstance(), &ElaNavigationRouter::navigationRouterStateChanged, this, [=](ElaNavigationRouterType::RouteMode routeMode) {
        switch (routeMode)
        {
        case ElaNavigationRouterType::BackValid:
        {
            leftButton->setEnabled(true);
            break;
        }
        case ElaNavigationRouterType::BackInvalid:
        {
            leftButton->setEnabled(false);
            break;
        }
        case ElaNavigationRouterType::ForwardValid:
        {
            rightButton->setEnabled(true);
            break;
        }
        case ElaNavigationRouterType::ForwardInvalid:
        {
            rightButton->setEnabled(false);
            break;
        }
        }
    });

    // ===== 搜索建议框 =====
    _windowSuggestBox = new ElaSuggestBox(this);
    _windowSuggestBox->setFixedHeight(32);
    _windowSuggestBox->setPlaceholderText("搜索关键字");

    // 点击搜索建议 → 导航到对应页面
    connect(_windowSuggestBox, &ElaSuggestBox::suggestionClicked, this, [=](const ElaSuggestBox::SuggestData& suggestData) {
        navigation(suggestData.getSuggestData().value("ElaPageKey").toString());
    });

    // ===== 系统运行指示器 =====
    ElaText* progressBusyRingText = new ElaText("系统运行中", this);
    progressBusyRingText->setIsWrapAnywhere(false);
    progressBusyRingText->setTextPixelSize(15);

    ElaProgressRing* progressBusyRing = new ElaProgressRing(this);
    progressBusyRing->setBusyingWidth(4);
    progressBusyRing->setFixedSize(28, 28);
    progressBusyRing->setIsBusying(true);       // 持续旋转（忙碌状态）

    centralCustomWidgetLayout->addWidget(leftButton);
    centralCustomWidgetLayout->addWidget(rightButton);
    centralCustomWidgetLayout->addWidget(_windowSuggestBox);
    centralCustomWidgetLayout->addStretch();
    centralCustomWidgetLayout->addWidget(progressBusyRingText);
    centralCustomWidgetLayout->addWidget(progressBusyRing);
    setCentralCustomWidget(centralCustomWidget);
}

void MainWindow::initEdgeLayout()
{
    //菜单栏
    ElaMenuBar* menuBar = new ElaMenuBar(this);
    menuBar->setFixedHeight(30);

    // 包裹在自定义组件中
    QWidget* customWidget = new QWidget(this);
    customWidget->setFixedWidth(500);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    // this->setMenuBar(menuBar);

    // 放置在标题栏中间区域
    this->setCustomWidget(ElaAppBarType::MiddleArea, customWidget);

    // 添加菜单项
    menuBar->addElaIconAction(ElaIconType::AtomSimple, "动作菜单");

    // 带子菜单的图标菜单
    ElaMenu* iconMenu = menuBar->addMenu(ElaIconType::Aperture, "图标菜单");
    iconMenu->setMenuItemHeight(27);
    iconMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::SelectAll);
    iconMenu->addElaIconAction(ElaIconType::Copy, "复制");
    iconMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    iconMenu->addSeparator();
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    iconMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    menuBar->addSeparator();

    ElaMenu* shortCutMenu = new ElaMenu("快捷菜单(&A)", this);
    shortCutMenu->setMenuItemHeight(27);
    shortCutMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Find);
    shortCutMenu->addElaIconAction(ElaIconType::Copy, "复制");
    shortCutMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");
    shortCutMenu->addSeparator();
    shortCutMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    shortCutMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    menuBar->addMenu(shortCutMenu);

    menuBar->addMenu("样例菜单(&B)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&C)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&E)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&F)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&G)")->addElaIconAction(ElaIconType::ArrowRotateRight, "样例选项");

    //工具栏
    ElaToolBar* toolBar = new ElaToolBar("工具栏", this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);   // 允许顶部/底部
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);    // 仅显示图标
    toolBar->setIconSize(QSize(25, 25));
    // toolBar->setFloatable(false);
    // toolBar->setMovable(false);
    ElaToolButton* toolButton1 = new ElaToolButton(this);
    toolButton1->setElaIcon(ElaIconType::BadgeCheck);
    toolBar->addWidget(toolButton1);
    ElaToolButton* toolButton2 = new ElaToolButton(this);
    toolButton2->setElaIcon(ElaIconType::ChartUser);
    toolBar->addWidget(toolButton2);
    toolBar->addSeparator();
    ElaToolButton* toolButton3 = new ElaToolButton(this);
    toolButton3->setElaIcon(ElaIconType::Bluetooth);
    toolButton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton3->setText("Bluetooth");
    toolBar->addWidget(toolButton3);

    // 图标+文字的工具按钮
    ElaToolButton* toolButton4 = new ElaToolButton(this);
    toolButton4->setElaIcon(ElaIconType::BringFront);
    toolBar->addWidget(toolButton4);
    toolBar->addSeparator();
    ElaToolButton* toolButton5 = new ElaToolButton(this);
    toolButton5->setElaIcon(ElaIconType::ChartSimple);
    toolBar->addWidget(toolButton5);
    ElaToolButton* toolButton6 = new ElaToolButton(this);
    toolButton6->setElaIcon(ElaIconType::FaceClouds);
    toolBar->addWidget(toolButton6);
    ElaToolButton* toolButton8 = new ElaToolButton(this);
    toolButton8->setElaIcon(ElaIconType::Aperture);
    toolBar->addWidget(toolButton8);
    ElaToolButton* toolButton9 = new ElaToolButton(this);
    toolButton9->setElaIcon(ElaIconType::ChartMixed);
    toolBar->addWidget(toolButton9);
    ElaToolButton* toolButton10 = new ElaToolButton(this);
    toolButton10->setElaIcon(ElaIconType::Coins);
    toolBar->addWidget(toolButton10);
    ElaToolButton* toolButton11 = new ElaToolButton(this);
    toolButton11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton11->setElaIcon(ElaIconType::AlarmPlus);
    toolButton11->setText("AlarmPlus");
    toolBar->addWidget(toolButton11);
    ElaToolButton* toolButton12 = new ElaToolButton(this);
    toolButton12->setElaIcon(ElaIconType::Crown);
    toolBar->addWidget(toolButton12);
    QAction* test = new QAction(this);
    test->setMenu(new QMenu(this));

    // 无限进度条
    ElaProgressBar* progressBar = new ElaProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setFixedWidth(350);
    toolBar->addWidget(progressBar);

    this->addToolBar(Qt::TopToolBarArea, toolBar);  // 添加到窗口顶部

    //停靠窗口
    ElaDockWidget* logDockWidget = new ElaDockWidget("日志信息", this);
    logDockWidget->setWidget(new T_LogWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    resizeDocks({logDockWidget}, {200}, Qt::Horizontal);

    ElaDockWidget* updateDockWidget = new ElaDockWidget("更新内容", this);
    updateDockWidget->setWidget(new T_UpdateWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    resizeDocks({updateDockWidget}, {200}, Qt::Horizontal);

    //状态栏
    ElaStatusBar* statusBar = new ElaStatusBar(this);
    ElaText* statusText = new ElaText("初始化成功！", this);
    statusText->setTextPixelSize(14);
    statusBar->addWidget(statusText);
    this->setStatusBar(statusBar);
}

// 导航页面注册
void MainWindow::initContent()
{
    _homePage = new T_Home(this);
#ifdef Q_OS_WIN
    _elaScreenPage = new T_ElaScreen(this);
#endif
    _iconPage = new T_Icon(this);
    _baseComponentsPage = new T_BaseComponents(this);
    _graphicsPage = new T_Graphics(this);
    _navigationPage = new T_Navigation(this);
    _popupPage = new T_Popup(this);
    _cardPage = new T_Card(this);
    _listViewPage = new T_ListView(this);
    _tableViewPage = new T_TableView(this);
    _treeViewPage = new T_TreeView(this);
    _settingPage = new T_Setting(this);

    QString testKey_1;
    QString testKey_2;

    // ===== 一级页面节点 =====
    addPageNode("HOME", _homePage, ElaIconType::House);

    // ===== Windows DXGI 分组（条件编译） =====
#ifdef Q_OS_WIN
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);  // 可展开节点
    QString dxgiCategoryKey;
    addCategoryNode("Windows-DXGI", dxgiCategoryKey, _elaDxgiKey);  // 分类标题
    addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
#endif

    // ===== Controls 分组 =====
    QString controlCategoryKey;
    addCategoryNode("Controls", controlCategoryKey);    // 分类标题
    // navigation(elaScreenWidget->property("ElaPageKey").toString());
    addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);

    // ===== View 分组（嵌套展开节点） =====
    addExpanderNode("ElaView", _viewKey, ElaIconType::CameraViewfinder);
    QString viewCategoryKey;
    addCategoryNode("View Content", viewCategoryKey, _viewKey);
    addPageNode("ElaListView", _listViewPage, _viewKey, 9, ElaIconType::List);
    addPageNode("ElaTableView", _tableViewPage, _viewKey, ElaIconType::Table);
    addPageNode("ElaTreeView", _treeViewPage, _viewKey, ElaIconType::ListTree);
    expandNavigationNode(_viewKey);     // 默认展开 View 分组

    // ===== 独立页面 =====
    addPageNode("ElaGraphics", _graphicsPage, 9, ElaIconType::Paintbrush);
    addPageNode("ElaCard", _cardPage, ElaIconType::Cards);

    // ===== Custom 分组 =====
    QString customKey;
    addCategoryNode("Custom", customKey);
    addPageNode("ElaNavigation", _navigationPage, ElaIconType::LocationArrow);
    addPageNode("ElaPopup", _popupPage, ElaIconType::Envelope);
    addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontCase);
    addExpanderNode("TEST_EXPAND_NODE1", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE2", testKey_2, testKey_1, ElaIconType::Acorn);
    addPageNode("TEST_NODE3", new QWidget(this), testKey_2, ElaIconType::Acorn);
    for (int i = 0; i < 10; i++)
    {
        addExpanderNode(QString("TEST_EXPAND_NODE%1").arg(i + 4), testKey_1, testKey_2, ElaIconType::Acorn);
    }
    addExpanderNode("TEST_EXPAND_NODE14", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE5", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE16", testKey_1, ElaIconType::Acorn);

    // ===== 底部固定节点 =====
    // About 页面（独立窗口，不在导航堆栈中）
    addFooterNode("About", nullptr, _aboutKey, 0, ElaIconType::User);
    _aboutPage = new T_About();

    _aboutPage->hide();
    // 点击 About 节点 → 弹出独立窗口
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            _aboutPage->moveToCenter();
            _aboutPage->show();
        }
    });

    // Setting 页面（在导航堆栈中）
    addFooterNode("Setting", _settingPage, _settingKey, 0, ElaIconType::GearComplex);

    // ===== 用户卡片点击 → 跳转到首页 =====
    connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
        this->navigation(_homePage->property("ElaPageKey").toString());
    });

    // ===== 首页卡片跳转（页面间通信） =====
#ifdef Q_OS_WIN
    connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() {
        this->navigation(_elaScreenPage->property("ElaPageKey").toString());
    });
#endif
    connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() {
        this->navigation(_baseComponentsPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() {
        this->navigation(_graphicsPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaIconNavigation, this, [=]() {
        this->navigation(_iconPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &T_Home::elaCardNavigation, this, [=]() {
        this->navigation(_cardPage->property("ElaPageKey").toString());
    });

    // 将所有导航页面的建议数据添加到搜索框
    _windowSuggestBox->addSuggestion(getNavigationSuggestDataList());
    qDebug() << "已注册的事件列表" << ElaEventBus::getInstance()->getRegisteredEventsName();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (getCurrentNavigationIndex() != 2)
    {
        switch (event->button())
        {
        case Qt::BackButton:    // 鼠标侧键后退
        {
            this->setCurrentStackIndex(0);
            break;
        }
        case Qt::ForwardButton: // 鼠标侧键前进
        {
            this->setCurrentStackIndex(1);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    ElaWindow::mouseReleaseEvent(event);    // 调用基类事件处理
}
