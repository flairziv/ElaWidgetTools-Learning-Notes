#include "T_ListView.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaListView.h"        // Ela 风格列表视图
#include "ElaScrollBar.h"       // Ela 风格滚动条（浮动动画）
#include "ElaText.h"
#include "T_ListViewModel.h"    // 自定义数据模型
T_ListView::T_ListView(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaListView");

    // 顶部元素
    createCustomWidget("列表视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    //ElaListView
    ElaText* listText = new ElaText("ElaListView", this);
    listText->setTextPixelSize(18);

    // 创建列表视图：
    _listView = new ElaListView(this);
    _listView->setFixedHeight(450);
    // _listView->setAlternatingRowColors(true);    // 交替行颜色，启用后奇偶行颜色不同
    _listView->setModel(new T_ListViewModel(this)); // 绑定数据模型（MVC 模式）
    
    // 创建浮动滚动条：
    ElaScrollBar* listViewFloatScrollBar = new ElaScrollBar(_listView->verticalScrollBar(), _listView);
    listViewFloatScrollBar->setIsAnimation(true);
    QHBoxLayout* listViewLayout = new QHBoxLayout();
    listViewLayout->setContentsMargins(0, 0, 10, 0);
    listViewLayout->addWidget(_listView);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaView");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(listText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(listViewLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_ListView::~T_ListView()
{
}
