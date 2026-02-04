#include "T_Popup.h"

#include "ElaCalendar.h"        // 日历组件
#include "ElaCalendarPicker.h"  // 日历选择器（带弹出日历）
#include "ElaCheckBox.h"        // 复选框
#include "ElaColorDialog.h"     // 颜色选择对话框
#include "ElaDrawerArea.h"      // 抽屉面板（可展开/折叠）
#include "ElaKeyBinder.h"       // 快捷键绑定器
#include "ElaMenu.h"            // 菜单
#include "ElaPushButton.h"      // 按钮
#include "ElaRoller.h"          // 单列滚轮选择器
#include "ElaRollerPicker.h"    // 多列滚轮选择器
#include "ElaScrollPageArea.h"  // 滚动区域
#include "ElaText.h"            // 文本
#include "ElaToggleSwitch.h"    // 开关
#include "ElaToolButton.h"      // 工具按钮
#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

T_Popup::T_Popup(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaPopup");

    // 顶部元素
    createCustomWidget("一些常用的弹出组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaPopup");

    // 创建工具按钮
    _toolButton = new ElaToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);   // 图标在上，文字在下
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _toolButton->setText("ElaToolButton");

    // 创建下拉菜单
    ElaMenu* menu = new ElaMenu(this);
    menu->addElaIconAction(ElaIconType::JackOLantern, "JackOLantern");
    menu->addElaIconAction(ElaIconType::LacrosseStick, "LacrosseStick");
    _toolButton->setMenu(menu);
    _toolButton->setElaIcon(ElaIconType::Broom);

    // 布局：标签 + 按钮
    ElaScrollPageArea* toolButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* toolButtonLayout = new QHBoxLayout(toolButtonArea);
    ElaText* toolButtonText = new ElaText("ElaToolButton", this);
    toolButtonText->setTextPixelSize(15);
    toolButtonLayout->addWidget(toolButtonText);
    toolButtonLayout->addWidget(_toolButton);
    toolButtonLayout->addStretch();

    // 创建颜色对话框
    _colorDialog = new ElaColorDialog(this);

    // 显示当前颜色的 RGB 文本
    ElaText* colorText = new ElaText(_colorDialog->getCurrentColorRGB(), this);
    colorText->setTextPixelSize(15);

    // 设置按钮在浅色/深色主题下的各种状态颜色（全部设为当前选中色）
    ElaPushButton* colorDialogButton = new ElaPushButton(this);
    colorDialogButton->setFixedSize(35, 35);
    colorDialogButton->setLightDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightPressColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkPressColor(_colorDialog->getCurrentColor());
    
    // 点击按钮打开颜色选择对话框
    connect(colorDialogButton, &ElaPushButton::clicked, this, [=]() {
        _colorDialog->exec();
    });

    // 颜色选择完成后更新按钮颜色和文本
    connect(_colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        colorDialogButton->setLightDefaultColor(color);
        colorDialogButton->setLightHoverColor(color);
        colorDialogButton->setLightPressColor(color);
        colorDialogButton->setDarkDefaultColor(color);
        colorDialogButton->setDarkHoverColor(color);
        colorDialogButton->setDarkPressColor(color);
        colorText->setText(_colorDialog->getCurrentColorRGB());
    });

    ElaScrollPageArea* colorDialogArea = new ElaScrollPageArea(this);
    QHBoxLayout* colorDialogLayout = new QHBoxLayout(colorDialogArea);
    ElaText* colorDialogText = new ElaText("ElaColorDialog", this);
    colorDialogText->setTextPixelSize(15);
    colorDialogLayout->addWidget(colorDialogText);
    colorDialogLayout->addWidget(colorDialogButton);
    colorDialogLayout->addWidget(colorText);
    colorDialogLayout->addStretch();

    // 独立日历组件（直接显示）
    _calendar = new ElaCalendar(this);

    // 日历选择器（点击弹出日历）
    _calendarPicker = new ElaCalendarPicker(this);

    // 布局
    ElaScrollPageArea* calendarPickerArea = new ElaScrollPageArea(this);
    QHBoxLayout* calendarPickerLayout = new QHBoxLayout(calendarPickerArea);
    ElaText* calendarPickerText = new ElaText("ElaCalendarPicker", this);
    calendarPickerText->setTextPixelSize(15);
    calendarPickerLayout->addWidget(calendarPickerText);
    calendarPickerLayout->addWidget(_calendarPicker);
    calendarPickerLayout->addStretch();

    // 创建快捷键绑定器
    _keyBinder = new ElaKeyBinder(this);

    // 布局
    ElaScrollPageArea* keyBinderArea = new ElaScrollPageArea(this);
    QHBoxLayout* keyBinderLayout = new QHBoxLayout(keyBinderArea);
    ElaText* keyBinderText = new ElaText("ElaKeyBinder", this);
    keyBinderText->setTextPixelSize(15);
    keyBinderLayout->addWidget(keyBinderText);
    keyBinderLayout->addWidget(_keyBinder);
    keyBinderLayout->addStretch();

    // 创建滚轮选择器
    _roller = new ElaRoller(this);

    // 生成 1-100 的选项列表
    QStringList rollerItemList;
    for (int i = 0; i < 100; i++)
    {
        rollerItemList.append(QString::number(i + 1));
    }
    _roller->setItemList(rollerItemList);   // 设置选项

    // 容器布局（固定高度 220px）
    ElaScrollPageArea* rollerArea = new ElaScrollPageArea(this);
    rollerArea->setFixedHeight(220);
    QHBoxLayout* rollerLayout = new QHBoxLayout(rollerArea);
    ElaText* rollerText = new ElaText("ElaRoller", this);
    rollerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerText);
    rollerLayout->addWidget(_roller);
    rollerLayout->addSpacing(30);

    ElaText* rollerPickerText = new ElaText("ElaRollerPicker", this);
    rollerPickerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerPickerText);

    // 获取当前时间
    QTime currentTime = QTime::currentTime();
    QString currentHour = QString("%1").arg(currentTime.hour(), 2, 10, QChar('0')); // 补零格式化
    QString currentMinute = QString("%1").arg(currentTime.minute(), 2, 10, QChar('0'));
    
    // ===== 时间选择器 1：时 + 分 + AM/PM =====
    _timeRollerPicker = new ElaRollerPicker(this);

    // 生成小时列表 00-23
    QStringList hourItemList;
    for (int i = 0; i < 24; i++)
    {
        hourItemList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }

    // 生成分钟列表 00-60
    QStringList minuteList;
    for (int i = 0; i < 61; i++)
    {
        minuteList.append(QString("%1").arg(i, 2, 10, QChar('0')));
    }

    // 添加三列滚轮
    _timeRollerPicker->addRoller(hourItemList);         // 第一列：小时
    _timeRollerPicker->addRoller(minuteList);           // 第二列：分钟
    _timeRollerPicker->addRoller({"AM", "PM"}, false);  // 第三列：AM/PM（不循环滚动）

    // 设置当前值
    _timeRollerPicker->setCurrentData({currentHour,
                                       currentMinute,
                                       currentTime.hour() >= 12 ? "PM" : "AM"});

    // ===== 时间选择器 2：时 + 分（自定义宽度） =====
    _clockRollerPicker = new ElaRollerPicker(this);
    _clockRollerPicker->addRoller(hourItemList);
    _clockRollerPicker->addRoller(minuteList);
    _clockRollerPicker->setRollerWidth(0, 135); // 第一列宽度 135px
    _clockRollerPicker->setRollerWidth(1, 135); // 第二列宽度 135px
    _clockRollerPicker->setCurrentData({currentHour,
                                        currentMinute});

    QVBoxLayout* rollerPickerLayout = new QVBoxLayout();
    rollerPickerLayout->addWidget(_timeRollerPicker);
    rollerPickerLayout->addWidget(_clockRollerPicker);
    rollerLayout->addLayout(rollerPickerLayout);
    rollerLayout->addStretch();

    // 创建抽屉组件
    _drawer = new ElaDrawerArea(this);

    // ===== 抽屉头部（标题栏） =====
    QWidget* drawerHeader = new QWidget(this);
    QHBoxLayout* drawerHeaderLayout = new QHBoxLayout(drawerHeader);
    
    // 图标
    ElaText* drawerIcon = new ElaText(this);
    drawerIcon->setTextPixelSize(15);
    drawerIcon->setElaIcon(ElaIconType::MessageArrowDown);
    drawerIcon->setFixedSize(25, 25);

    // 标题文本
    ElaText* drawerText = new ElaText("ElaDrawer", this);
    drawerText->setTextPixelSize(15);

    // 开关控制展开/折叠
    ElaToggleSwitch* drawerSwitch = new ElaToggleSwitch(this);
    ElaText* drawerSwitchText = new ElaText("关", this);
    drawerSwitchText->setTextPixelSize(15);

    // 开关状态变化时展开/折叠抽屉
    connect(drawerSwitch, &ElaToggleSwitch::toggled, this, [=](bool toggled) {
        if (toggled)
        {
            drawerSwitchText->setText("开");
            _drawer->expand();      // 展开
        }
        else
        {
            drawerSwitchText->setText("关");
            _drawer->collapse();    // 折叠
        }
    });

    // 抽屉状态变化时同步开关状态
    connect(_drawer, &ElaDrawerArea::expandStateChanged, this, [=](bool isExpand) {
        drawerSwitch->setIsToggled(isExpand);
    });

    drawerHeaderLayout->addWidget(drawerIcon);
    drawerHeaderLayout->addWidget(drawerText);
    drawerHeaderLayout->addStretch();
    drawerHeaderLayout->addWidget(drawerSwitchText);
    drawerHeaderLayout->addWidget(drawerSwitch);

    // 设置抽屉头部
    _drawer->setDrawerHeader(drawerHeader);

    // ===== 抽屉内容（三个子项） =====
    QWidget* drawerWidget1 = new QWidget(this);
    drawerWidget1->setFixedHeight(75);
    QHBoxLayout* drawerWidget1Layout = new QHBoxLayout(drawerWidget1);
    ElaCheckBox* drawerCheckBox1 = new ElaCheckBox("测试窗口1", this);
    drawerWidget1Layout->addSpacing(60);
    drawerWidget1Layout->addWidget(drawerCheckBox1);

    QWidget* drawerWidget2 = new QWidget(this);
    drawerWidget2->setFixedHeight(75);
    QHBoxLayout* drawerWidget2Layout = new QHBoxLayout(drawerWidget2);
    ElaCheckBox* drawerCheckBox2 = new ElaCheckBox("测试窗口2", this);
    drawerWidget2Layout->addSpacing(60);
    drawerWidget2Layout->addWidget(drawerCheckBox2);

    QWidget* drawerWidget3 = new QWidget(this);
    drawerWidget3->setFixedHeight(75);
    QHBoxLayout* drawerWidget3Layout = new QHBoxLayout(drawerWidget3);
    ElaCheckBox* drawerCheckBox3 = new ElaCheckBox("测试窗口3", this);
    drawerWidget3Layout->addSpacing(60);
    drawerWidget3Layout->addWidget(drawerCheckBox3);

    // 添加到抽屉
    _drawer->addDrawer(drawerWidget1);
    _drawer->addDrawer(drawerWidget2);
    _drawer->addDrawer(drawerWidget3);

    // 整体布局组装
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addWidget(colorDialogArea);
    centerVLayout->addWidget(calendarPickerArea);
    centerVLayout->addWidget(_calendar);
    centerVLayout->addWidget(keyBinderArea);
    centerVLayout->addWidget(_drawer);
    centerVLayout->addWidget(rollerArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup()
{
}
