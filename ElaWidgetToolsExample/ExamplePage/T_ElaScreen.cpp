#include "T_ElaScreen.h"

#ifdef Q_OS_WIN // #ifdef Q_OS_WIN 表示这段代码仅在 Windows 下编译，因为 DXGI 是 Windows 独有的图形接口。
#include "ElaComboBox.h"
#include "ElaDxgiManager.h"     // DXGI 管理器（核心：屏幕捕获控制）
#include "ElaLineEdit.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaScreen");

    // 顶部元素
    createCustomWidget("DXGI录制组件被放置于此，可在此界面预览录制效果");

    // 获取 DXGI 管理器单例（全局唯一实例）
    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();

    // 设置捕获区域大小（1920x1080 像素）
    dxgiManager->setGrabArea(1920, 1080);

    // 创建滚动区域容器（固定高度 700px）
    ElaScrollPageArea* dxgiScreenArea = new ElaScrollPageArea(this);
    dxgiScreenArea->setFixedHeight(700);
    QHBoxLayout* dxgiScreenLayout = new QHBoxLayout(dxgiScreenArea);
    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setFixedSize(1200, 678);
    dxgiScreenLayout->addWidget(_dxgiScreen);

    // 显卡选择标签
    ElaText* dxText = new ElaText("显卡选择", this);
    dxText->setTextPixelSize(15);

    // 显卡下拉框
    _dxComboBox = new ElaComboBox(this);
    _dxComboBox->addItems(dxgiManager->getDxDeviceList());      // 获取所有显卡列表
    _dxComboBox->setCurrentIndex(dxgiManager->getDxDeviceID()); // 设置当前选中的显卡

    // 屏幕选择标签
    ElaText* outputText = new ElaText("屏幕选择", this);
    outputText->setTextPixelSize(15);

    // 屏幕下拉框
    _outputComboBox = new ElaComboBox(this);
    _outputComboBox->addItems(dxgiManager->getOutputDeviceList());      // 获取所有显示器列表
    _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID()); // 设置当前选中的屏幕

    connect(_dxComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        // 切换显卡
        dxgiManager->setDxDeviceID(index);

        // 切换显卡后，需要刷新屏幕列表
        /**
         * 在 clear() 和 addItems() 过程中会触发 currentIndexChanged 信号
         * 用 blockSignals(true/false) 临时阻塞信号，避免不必要的回调
         */
        _outputComboBox->blockSignals(true);    // 暂时阻塞信号，防止触发额外的回调
        _outputComboBox->clear();               // 清空旧的屏幕列表
        _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
        _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
        _outputComboBox->blockSignals(false);   // 恢复信号
        
        _dxgiScreen->update();  // 刷新显示
    });
    connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged), this, [=](int index) {
        // 切换显卡
        dxgiManager->setOutputDeviceID(index);
        _dxgiScreen->update();
    });

    // 创建切换按钮
    ElaToggleButton* startButton = new ElaToggleButton("捕获", this);
    connect(startButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled)
        {
            dxgiManager->startGrabScreen(); // 开始捕获
        }
        else
        {
            dxgiManager->stopGrabScreen();  // 停止捕获
            _dxgiScreen->update();          // 刷新显示（清空画面）
        }
    });

    QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    comboBoxLayout->addWidget(dxText);
    comboBoxLayout->addWidget(_dxComboBox);
    comboBoxLayout->addSpacing(10);
    comboBoxLayout->addWidget(outputText);
    comboBoxLayout->addWidget(_outputComboBox);
    comboBoxLayout->addWidget(startButton);
    comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaScreen");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(dxgiScreenArea);
    addCentralWidget(centralWidget, false, true);
}

T_ElaScreen::~T_ElaScreen()
{
}
#endif
