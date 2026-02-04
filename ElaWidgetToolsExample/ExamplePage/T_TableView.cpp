#include "T_TableView.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QVBoxLayout>

#include "ElaTableView.h"       // Ela 风格表格视图
#include "ElaText.h"
#include "T_TableViewModel.h"   // 自定义表格数据模型
T_TableView::T_TableView(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaTableView");

    // 顶部元素
    createCustomWidget("表格视图被放置于此，可在此界面体验其效果并按需添加进项目中");

    //ElaTableView
    ElaText* tableText = new ElaText("ElaTableView", this);
    tableText->setTextPixelSize(18);
    _tableView = new ElaTableView(this);
    
    // 浮动滚动条 - TableView 也可以像 ListView 一样添加浮动滚动条
    // ElaScrollBar* tableViewFloatScrollBar = new ElaScrollBar(_tableView->verticalScrollBar(), _tableView);
    // tableViewFloatScrollBar->setIsAnimation(true);
    
    QFont tableHeaderFont = _tableView->horizontalHeader()->font(); // 获取表头当前字体
    tableHeaderFont.setPixelSize(16);                               // 设置字号 16px
    _tableView->horizontalHeader()->setFont(tableHeaderFont);       // 应用新字体
    
    _tableView->setModel(new T_TableViewModel(this));       // 绑定数据模型
    _tableView->setAlternatingRowColors(true);              // 交替行颜色
    _tableView->setIconSize(QSize(38, 38));                 // 图标尺寸 38×38
    _tableView->verticalHeader()->setHidden(true);          // 隐藏行号表头
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // 可拖动调整列宽
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);    // 整行选中
    _tableView->horizontalHeader()->setMinimumSectionSize(60);          // 最小列宽 60px
    _tableView->verticalHeader()->setMinimumSectionSize(46);            // 最小行高 46px
    _tableView->setFixedHeight(450);                                    // 固定高度 450px
    
    // 行 - 列宽设置（延迟执行）监听 tableViewShow 信号，当 TableView 真正显示时再设置列宽
    connect(_tableView, &ElaTableView::tableViewShow, this, [=]() {
        _tableView->setColumnWidth(0, 60);
        _tableView->setColumnWidth(1, 205);
        _tableView->setColumnWidth(2, 170);
        _tableView->setColumnWidth(3, 150);
        _tableView->setColumnWidth(4, 60);
    });
    
    QHBoxLayout* tableViewLayout = new QHBoxLayout();
    tableViewLayout->setContentsMargins(0, 0, 10, 0);
    tableViewLayout->addWidget(_tableView);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaView");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(tableText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(tableViewLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_TableView::~T_TableView()
{
}
