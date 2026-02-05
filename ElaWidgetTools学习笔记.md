# ElaWidgetTools 组件学习笔记

## 目录

[TOC]

---

## 一、项目基础配置

### 1.1 CMakeLists.txt 关键配置

```cmake
# 添加 ElaWidgetTools 子目录
add_subdirectory(ElaWidgetTools-main/ElaWidgetTools)

# 链接库
target_link_libraries(你的项目名 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ElaWidgetTools
)
```

### 1.2 main.cpp 初始化

```cpp
#include <QApplication>
#include "ElaApplication.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eApp->init();  // 初始化 Ela（字体、主题等）
    MainWindow w;
    w.show();
    return a.exec();
}
```

### 1.3 MainWindow 继承 ElaWindow

```cpp
// mainwindow.h
#include "ElaWindow.h"

class MainWindow : public ElaWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
```

继承 `ElaWindow` 后自动获得：

| 自带功能     | 说明                        |
| ------------ | --------------------------- |
| 标题栏       | 自定义无边框，含最小/最大/关闭按钮 |
| 左侧导航栏   | 通过 `addPageNode()` 添加页面     |
| 用户信息卡   | 左上角，可设置头像、名称          |
| 页面堆栈     | 点击导航自动切换页面              |
| 主题支持     | 自动响应深色/浅色切换             |

---

## 二、页面创建模板

每个页面都遵循相同的模式：

```cpp
// 1. 创建页面容器
QWidget* page = new QWidget(this);

// 2. 创建布局
QVBoxLayout* layout = new QVBoxLayout(page);
layout->setContentsMargins(30, 30, 30, 30);  // 页边距（左、上、右、下）
layout->setSpacing(15);                       // 组件间距

// 3. 创建组件并添加到布局
layout->addWidget(组件);
layout->addStretch();   // 弹性空间（把组件挤到顶部）

// 4. 注册到导航栏
addPageNode("页面名", page, ElaIconType::图标);
```

**三者关系：**

```
page (QWidget)       → 容器，一张空白画布
  └── layout (QVBoxLayout) → 排版规则，管理组件位置
        └── 组件            → 实际显示的内容
```

**布局常用方法：**

| 方法                               | 作用             |
| ---------------------------------- | ---------------- |
| `setContentsMargins(左,上,右,下)` | 设置页边距        |
| `setSpacing(15)`                   | 设置组件间距      |
| `addWidget(组件)`                  | 添加组件          |
| `addSpacing(20)`                   | 添加额外空白间距  |
| `addStretch()`                     | 添加弹性空间      |

---

## 三、已学习的组件

### 3.1 ElaPushButton（按钮）

**头文件：** `#include "ElaPushButton.h"`

**创建：**

```cpp
ElaPushButton* btn = new ElaPushButton("按钮文字", page);
```

**信号连接：**

```cpp
connect(btn, &ElaPushButton::clicked, this, []() {
    // 点击后执行的代码
});
```

**常用方法：**

| 方法                      | 作用       |
| ------------------------- | ---------- |
| `setText("文字")`         | 设置按钮文字 |
| `setBorderRadius(15)`    | 设置圆角大小 |
| `setEnabled(false)`      | 禁用按钮     |
| `setFixedSize(120, 40)`  | 固定按钮大小 |

**常用信号：**

| 信号        | 触发时机     |
| ----------- | ------------ |
| `clicked()` | 按钮被点击时 |

---

### 3.2 ElaMessageBar（消息提示）

**头文件：** `#include "ElaMessageBar.h"`

**调用方式（静态方法，无需创建对象）：**

```cpp
ElaMessageBar::success(ElaMessageBarType::TopRight, "标题", "内容", 3000, this);
```

**参数说明：**

```cpp
ElaMessageBar::success(
    ElaMessageBarType::TopRight,  // 显示位置
    "成功",                        // 标题
    "操作已完成!",                  // 内容
    3000,                          // 显示时间（毫秒）
    this                           // 父窗口
);
```

**四种消息类型：**

| 方法              | 颜色    | 用途     |
| ----------------- | ------- | -------- |
| `success()`       | 绿色 ✓  | 操作成功 |
| `warning()`       | 黄色 ⚠  | 警告提醒 |
| `error()`         | 红色 ✗  | 错误提示 |
| `information()`   | 蓝色 ℹ  | 普通提示 |

**显示位置：**

| 枚举值                           | 位置     |
| -------------------------------- | -------- |
| `ElaMessageBarType::TopLeft`     | 左上角   |
| `ElaMessageBarType::Top`         | 顶部居中 |
| `ElaMessageBarType::TopRight`    | 右上角   |
| `ElaMessageBarType::BottomLeft`  | 左下角   |
| `ElaMessageBarType::Bottom`      | 底部居中 |
| `ElaMessageBarType::BottomRight` | 右下角   |

---

### 3.3 ElaText（文本标签）

**头文件：** `#include "ElaText.h"`

**创建：**

```cpp
ElaText* text = new ElaText("显示的文字", page);
```

**设置字号（两种方式）：**

```cpp
// 方式1：手动设置像素大小
text->setTextPixelSize(28);

// 方式2：使用预设样式
text->setTextStyle(ElaTextType::Title);
```

**预设样式：**

| 样式                      | 效果       | 适用场景       |
| ------------------------- | ---------- | -------------- |
| `ElaTextType::Title`      | 大号粗体   | 页面主标题     |
| `ElaTextType::Subtitle`   | 中号       | 副标题、分组   |
| `ElaTextType::Body`       | 正常大小   | 正文内容       |
| `ElaTextType::Caption`    | 小号灰色   | 说明、注释     |

**常用方法：**

| 方法                     | 作用             |
| ------------------------ | ---------------- |
| `setText("文字")`        | 设置/修改文本内容 |
| `setTextPixelSize(18)`  | 设置字体大小      |
| `setTextStyle(...)`     | 使用预设样式      |

---

### 3.4 ElaLineEdit（输入框）

**头文件：** `#include "ElaLineEdit.h"`

**创建：**

```cpp
ElaLineEdit* input = new ElaLineEdit(page);
input->setPlaceholderText("请输入内容...");  // 占位提示文字
input->setFixedWidth(300);                   // 固定宽度
```

**获取输入内容：**

```cpp
QString content = input->text();  // 获取用户输入的文字
```

**实时监听输入变化：**

```cpp
connect(input, &ElaLineEdit::textChanged, this, [](const QString& text) {
    // text 就是当前输入框的内容，每次输入变化都会触发
});
```

**常用方法：**

| 方法                          | 作用           |
| ----------------------------- | -------------- |
| `text()`                      | 获取输入内��   |
| `setText("内容")`             | 设置输入内容   |
| `setPlaceholderText("提示")`  | 设置占位提示   |
| `clear()`                     | 清空输入框     |
| `setReadOnly(true)`           | 设置为只读     |
| `setFixedWidth(300)`          | 固定宽度       |

**常用信号：**

| 信号                       | 触发时机                 |
| -------------------------- | ------------------------ |
| `textChanged(QString)`     | 内容改变时（实时触发）   |
| `returnPressed()`          | 按下回车键时             |
| `editingFinished()`        | 编辑完成（失去焦点/回车）|

---

### 3.5 ElaCheckBox（复选框）

**头文件：** `#include "ElaCheckBox.h"`

**创建：**

```cpp
ElaCheckBox* check = new ElaCheckBox("选项文字", page);
check->setChecked(true);  // 设置默认选中
```

**判断是否选中：**

```cpp
if (check->isChecked()) {
    // 当前是选中状态
}
```

**监听状态变化：**

```cpp
connect(check, &ElaCheckBox::stateChanged, this, [](int state) {
    if (state == Qt::Checked) {
        // 选中了
    } else {
        // 取消选中
    }
});
```

**state 的值：**

| 值                   | 含义   |
| -------------------- | ------ |
| `Qt::Unchecked` (0)  | 未选中 |
| `Qt::Checked` (2)    | 已选中 |

**常用方法：**

| 方法                 | 作用                     |
| -------------------- | ------------------------ |
| `setChecked(true)`   | 设置选中状态             |
| `isChecked()`        | 判断是否选中，返回 bool  |
| `setText("文字")`    | 设置显示文字             |

**常用信号：**

| 信号                 | 触发时机                 |
| -------------------- | ------------------------ |
| `stateChanged(int)`  | 状态改变时（参数是新状态）|
| `clicked()`          | 被点击时                 |

**实用技巧 - 获取多个选中项：**

```cpp
QStringList selected;
if (check1->isChecked()) selected << "选项1";
if (check2->isChecked()) selected << "选项2";
if (check3->isChecked()) selected << "选项3";
QString result = selected.join("、");  // "选项1、选项2"
```

---

### 3.6 ElaRadioButton（单选按钮）

**头文件：** `#include "ElaRadioButton.h"`

**创建：**

```cpp
ElaRadioButton* radio = new ElaRadioButton("选项文字", page);
radio->setChecked(true);  // 设置默认选中
```

**特点：同一父对象下的单选按钮自动互斥（选一个，其他自动取消）**

**使用 QButtonGroup 分组（多组独立互斥）：**

```cpp
#include <QButtonGroup>

// 性别组
QButtonGroup* genderGroup = new QButtonGroup(this);
genderGroup->addButton(male, 0);    // 参数：按钮, id
genderGroup->addButton(female, 1);

// 年龄组（与性别组独立）
QButtonGroup* ageGroup = new QButtonGroup(this);
ageGroup->addButton(age1, 0);
ageGroup->addButton(age2, 1);
ageGroup->addButton(age3, 2);
```

**获取选中项：**

```cpp
int id = genderGroup->checkedId();  // 返回选中按钮的 id
```

**常用方法：**

| 方法                 | 作用                   |
| -------------------- | ---------------------- |
| `setChecked(true)`   | 设置选中状态           |
| `isChecked()`        | 判断是否选中           |
| `setText("文字")`    | 设置显示文字           |

**QButtonGroup 常用方法：**

| 方法                     | 作用                     |
| ------------------------ | ------------------------ |
| `addButton(btn, id)`     | 添加按钮并指定 id        |
| `checkedId()`            | 获取选中按钮的 id        |
| `checkedButton()`        | 获取选中的按钮对象       |

**ElaCheckBox 与 ElaRadioButton 的区别：**

| | ElaCheckBox | ElaRadioButton |
|---|-------------|----------------|
| 选择方式 | 多选 | 单选 |
| 互斥性 | 不互斥 | 同组互斥 |
| 适用场景 | 多选题、开关选项 | 单选题、二选一 |

**⚠️ 重要经验：**

> 使用 ElaRadioButton 时，**建议始终使用 QButtonGroup 分组**，避免互斥冲突。

**问题场景：**

```cpp
// 错误做法：多组单选按钮在同一父对象下
ElaRadioButton* radio1 = new ElaRadioButton("A", page);
ElaRadioButton* radio2 = new ElaRadioButton("B", page);
ElaRadioButton* male = new ElaRadioButton("男", page);
ElaRadioButton* female = new ElaRadioButton("女", page);

radio1->setChecked(true);
male->setChecked(true);  // 这会取消 radio1 的选中！
```

**原因：** 所有 RadioButton 父对象都是 page，Qt 认为它们是同一组。

**正确做法：**

```cpp
// 每组都用 QButtonGroup 明确分组
QButtonGroup* group1 = new QButtonGroup(this);
group1->addButton(radio1, 0);
group1->addButton(radio2, 1);

QButtonGroup* group2 = new QButtonGroup(this);
group2->addButton(male, 0);
group2->addButton(female, 1);

// 现在两组独立互斥
radio1->setChecked(true);  // 正常选中
male->setChecked(true);    // 不会影响 radio1
```

---

### 3.7 ElaComboBox（下拉框）

**头文件：** `#include "ElaComboBox.h"`

**创建：**

```cpp
ElaComboBox* combo = new ElaComboBox(page);
combo->setFixedWidth(200);
```

**添加选项：**

```cpp
// 逐个添加
combo->addItem("选项 1");
combo->addItem("选项 2");

// 批量添加
combo->addItems({"北京", "上海", "广州", "深圳"});
```

**设置/获取选中项：**

```cpp
combo->setCurrentIndex(2);           // 设置选中第3项（索引从0开始）

int index = combo->currentIndex();   // 获取选中的索引
QString text = combo->currentText(); // 获取选中的文字
```

**监听选择变化：**

```cpp
// 方式1: 通过索引监听
connect(combo, QOverload<int>::of(&ElaComboBox::currentIndexChanged),
        this, [](int index) {
    // index 是新选中项的索引
});

// 方式2: 通过文字监听
connect(combo, QOverload<const QString&>::of(&ElaComboBox::currentIndexChanged),
        this, [](const QString& text) {
    // text 是新选中项的文字
});

// 方式3: 更简洁的写法（推荐）
connect(combo, &ElaComboBox::currentTextChanged,
        this, [](const QString& text) {
    // 不需要 QOverload，因为 currentTextChanged 只有一个版本
});
```

**为什么需要 QOverload？**

因为 `currentIndexChanged` 有两个重载版本（int 和 QString），编译器不知道你要连接哪个，所以用 `QOverload<参数类型>::of()` 来指定。

**常用方法：**

| 方法                  | 作用               |
| --------------------- | ------------------ |
| `addItem("文字")`     | 添加单个选项       |
| `addItems({...})`     | 批量添加选项       |
| `currentIndex()`      | 获取选中项索引     |
| `currentText()`       | 获取选中项文字     |
| `setCurrentIndex(n)`  | 设置选中项         |
| `itemText(n)`         | 获取指定索引的文字 |
| `count()`             | 获取选项总数       |
| `clear()`             | 清空所有选项       |

**常用信号：**

| 信号                            | 触发时机               |
| ------------------------------- | ---------------------- |
| `currentIndexChanged(int)`      | 选择改变时（参数是索引）|
| `currentTextChanged(QString)`   | 选择改变时（参数是文字）|

---

### 3.8 ElaSlider（滑动条）

**头文件：** `#include "ElaSlider.h"`

**创建：**

```cpp
ElaSlider* slider = new ElaSlider(page);
slider->setOrientation(Qt::Horizontal);  // 水平方向（或 Qt::Vertical 垂直）
slider->setRange(0, 100);                // 范围 0~100
slider->setValue(30);                    // 初始值
slider->setFixedWidth(400);
```

**监听值变化：**

```cpp
connect(slider, &ElaSlider::valueChanged, this, [](int value) {
    qDebug() << "当前值:" << value;
});
```

**常用方法：**

| 方法                          | 作用             |
| ----------------------------- | ---------------- |
| `setRange(min, max)`          | 设置范围         |
| `setValue(n)`                 | 设置当前值       |
| `value()`                     | 获取当前值       |
| `setOrientation(Qt::...)`     | 设置方向         |

**常用信号：**

| 信号                 | 触发时机       |
| -------------------- | -------------- |
| `valueChanged(int)`  | 值改变时       |
| `sliderPressed()`    | 开始拖动时     |
| `sliderReleased()`   | 停止拖动时     |

---

### 3.9 ElaProgressBar（进度条）

**头文件：** `#include "ElaProgressBar.h"`

**创建：**

```cpp
ElaProgressBar* progress = new ElaProgressBar(page);
progress->setMinimum(0);
progress->setMaximum(100);
progress->setValue(70);
progress->setFixedWidth(400);
```

**常用方法：**

| 方法                 | 作用             |
| -------------------- | ---------------- |
| `setMinimum(n)`      | 设置最小值       |
| `setMaximum(n)`      | 设置最大值       |
| `setValue(n)`        | 设置当前值       |
| `value()`            | 获取当前值       |

**联动示例（滑动条控制进度条）：**

```cpp
connect(slider, &ElaSlider::valueChanged, this, [progress](int value) {
    progress->setValue(value);
});
```

**实用函数 qMin / qMax：**

```cpp
int newVal = qMin(value + 10, 100);  // 取较小值，防止超过100
int newVal = qMax(value - 10, 0);    // 取较大值，防止低于0
```

---

### 3.10 ElaToggleSwitch（开关）

**头文件：** `#include "ElaToggleSwitch.h"`

**创建：**

```cpp
ElaToggleSwitch* toggleSwitch = new ElaToggleSwitch(page);
```

**设置/获取状态：**

```cpp
toggleSwitch->setIsToggled(true);   // 设置为开启状态
bool isOn = toggleSwitch->getIsToggled();  // 获取当前状态
```

**监听状态变化：**

```cpp
connect(toggleSwitch, &ElaToggleSwitch::toggled, this, [](bool checked) {
    if (checked) {
        qDebug() << "开关已打开";
    } else {
        qDebug() << "开关已关闭";
    }
});
```

**常用方法：**

| 方法                    | 作用           |
| ----------------------- | -------------- |
| `setIsToggled(bool)`    | 设置开关状态   |
| `getIsToggled()`        | 获取开关状态   |
| `setDisabled(true)`     | 禁用开关       |

**常用信号：**

| 信号               | 触发时机                    |
| ------------------ | --------------------------- |
| `toggled(bool)`    | 状态改变时（参数是新状态）  |

**实用示例（用开关控制其他组件的禁用状态）：**

```cpp
ElaToggleSwitch* disableSwitch = new ElaToggleSwitch(page);
ElaPushButton* btn = new ElaPushButton("按钮", page);

connect(disableSwitch, &ElaToggleSwitch::toggled, this, [btn](bool checked) {
    btn->setDisabled(checked);  // 开关打开时禁用按钮
});
```

---

### 3.11 ElaToggleButton（切换按钮）

**头文件：** `#include "ElaToggleButton.h"`

**创建：**

```cpp
ElaToggleButton* toggleBtn = new ElaToggleButton("按钮文字", page);
toggleBtn->setFixedWidth(120);
```

**设置/获取状态：**

```cpp
toggleBtn->setIsToggled(true);   // 设置为按下状态
bool isPressed = toggleBtn->getIsToggled();
```

**监听状态变化：**

```cpp
connect(toggleBtn, &ElaToggleButton::toggled, this, [](bool checked) {
    qDebug() << "按钮状态:" << (checked ? "按下" : "弹起");
});
```

**常用方法：**

| 方法                       | 作用           |
| -------------------------- | -------------- |
| `setText("文字")`          | 设置按钮文字   |
| `setIsToggled(bool)`       | 设置按下状态   |
| `getIsToggled()`           | 获取按下状态   |
| `setBorderRadius(8)`       | 设置圆角半径   |

**ElaToggleSwitch 与 ElaToggleButton 的区别：**

| | ElaToggleSwitch | ElaToggleButton |
|---|-----------------|-----------------|
| 外观 | iOS 风格滑动开关 | 普通按钮样式 |
| 适用场景 | 设置项开关 | 需要切换状态的按钮 |
| 文字 | 无文字 | 可设置文字 |

---

### 3.12 ElaSpinBox（数字输入框）

**头文件：** `#include "ElaSpinBox.h"`

**创建：**

```cpp
ElaSpinBox* spinBox = new ElaSpinBox(page);
spinBox->setRange(0, 100);     // 设置范围
spinBox->setValue(50);         // 设置初始值
spinBox->setSingleStep(5);     // 设置步长
```

**按钮模式：**

```cpp
// 设置按钮显示模式
spinBox->setButtonMode(ElaSpinBoxType::Inline);    // 内嵌模式（默认）
spinBox->setButtonMode(ElaSpinBoxType::Compact);   // 紧凑模式
spinBox->setButtonMode(ElaSpinBoxType::Side);      // 侧边模式
spinBox->setButtonMode(ElaSpinBoxType::PMSide);    // 加减号侧边模式
```

**按钮模式说明：**

| 模式                           | 效果                     |
| ------------------------------ | ------------------------ |
| `ElaSpinBoxType::Inline`       | 按钮在输入框右侧内部     |
| `ElaSpinBoxType::Compact`      | 按钮紧凑排列             |
| `ElaSpinBoxType::Side`         | 按钮在两侧               |
| `ElaSpinBoxType::PMSide`       | 加减号在两侧             |

**监听值变化：**

```cpp
connect(spinBox, QOverload<int>::of(&ElaSpinBox::valueChanged),
        this, [](int value) {
    qDebug() << "当前值:" << value;
});
```

**常用方法（继承自 QSpinBox）：**

| 方法                    | 作用           |
| ----------------------- | -------------- |
| `setRange(min, max)`    | 设置范围       |
| `setValue(n)`           | 设置当前值     |
| `value()`               | 获取当前值     |
| `setSingleStep(n)`      | 设置步长       |
| `setPrefix("$")`        | 设置前缀       |
| `setSuffix(" 元")`      | 设置后缀       |

---

### 3.13 ElaDoubleSpinBox（浮点数输入框）

**头文件：** `#include "ElaDoubleSpinBox.h"`

**创建：**

```cpp
ElaDoubleSpinBox* doubleSpinBox = new ElaDoubleSpinBox(page);
doubleSpinBox->setRange(0.0, 100.0);   // 设置范围
doubleSpinBox->setValue(3.14);         // 设置初始值
doubleSpinBox->setSingleStep(0.1);     // 设置步长
doubleSpinBox->setDecimals(2);         // 设置小数位数
```

**按钮模式（与 ElaSpinBox 相同）：**

```cpp
doubleSpinBox->setButtonMode(ElaSpinBoxType::Inline);
```

**常用方法（继承自 QDoubleSpinBox）：**

| 方法                    | 作用             |
| ----------------------- | ---------------- |
| `setRange(min, max)`    | 设置范围         |
| `setValue(n)`           | 设置当前值       |
| `value()`               | 获取当前值       |
| `setDecimals(n)`        | 设置小数位数     |
| `setSingleStep(n)`      | 设置步长         |

---

### 3.14 ElaProgressRing（环形进度条）

**头文件：** `#include "ElaProgressRing.h"`

**创建：**

```cpp
ElaProgressRing* ring = new ElaProgressRing(page);
ring->setRange(0, 100);
ring->setValue(75);
```

**显示模式：**

```cpp
// 显示数值
ring->setValue(50);

// 显示百分比
ring->setValueDisplayMode(ElaProgressRingType::ValueDisplayMode::Percent);

// 忙碌/加载动画模式
ring->setIsBusying(true);

// 透明背景的忙碌模式
ring->setIsBusying(true);
ring->setIsTransparent(true);
```

**常用方法：**

| 方法                                  | 作用                     |
| ------------------------------------- | ------------------------ |
| `setRange(min, max)`                  | 设置范围                 |
| `setValue(n)`                         | 设置当前值               |
| `setIsBusying(true)`                  | 设置忙碌动画模式         |
| `setIsTransparent(true)`              | 设置透明背景             |
| `setValueDisplayMode(...)`            | 设置值显示模式           |
| `setValuePixelSize(n)`                | 设置数字字体大小         |
| `setBusyingWidth(n)`                  | 设置忙碌动画宽度         |

**ValueDisplayMode 枚举：**

| 枚举值                                           | 效果       |
| ------------------------------------------------ | ---------- |
| `ElaProgressRingType::ValueDisplayMode::Value`   | 显示数值   |
| `ElaProgressRingType::ValueDisplayMode::Percent` | 显示百分比 |

**实用示例（多种进度环效果）：**

```cpp
// 普通进度环
ElaProgressRing* normalRing = new ElaProgressRing(page);
normalRing->setValue(30);

// 百分比显示
ElaProgressRing* percentRing = new ElaProgressRing(page);
percentRing->setValue(50);
percentRing->setValueDisplayMode(ElaProgressRingType::ValueDisplayMode::Percent);

// 加载动画
ElaProgressRing* busyRing = new ElaProgressRing(page);
busyRing->setIsBusying(true);

// 透明加载动画
ElaProgressRing* transparentRing = new ElaProgressRing(page);
transparentRing->setIsBusying(true);
transparentRing->setIsTransparent(true);
```

---

### 3.15 ElaMultiSelectComboBox（多选下拉框）

**头文件：** `#include "ElaMultiSelectComboBox.h"`

**创建：**

```cpp
ElaMultiSelectComboBox* multiCombo = new ElaMultiSelectComboBox(page);
multiCombo->addItems({"选项1", "选项2", "选项3", "选项4", "选项5"});
```

**设置默认选中项：**

```cpp
// 方式1：通过文字列表
multiCombo->setCurrentSelection({"选项1", "选项3"});

// 方式2：通过索引列表
multiCombo->setCurrentSelection(QList<int>{0, 2});

// 方式3：通过单个文字
multiCombo->setCurrentSelection("选项1");

// 方式4：通过单个索引
multiCombo->setCurrentSelection(0);
```

**获取选中项：**

```cpp
QStringList selectedTexts = multiCombo->getCurrentSelection();       // 获取选中的文字列表
QList<int> selectedIndexes = multiCombo->getCurrentSelectionIndex(); // 获取选中的索引列表
```

**监听选择变化：**

```cpp
// 方式1：获取选中的文字列表
connect(multiCombo, &ElaMultiSelectComboBox::currentTextListChanged,
        this, [](const QStringList& selectedList) {
    qDebug() << "选中项:" << selectedList.join(", ");
});

// 方式2：获取每个选项的选中状态
connect(multiCombo, &ElaMultiSelectComboBox::itemSelectionChanged,
        this, [](const QVector<bool>& itemSelection) {
    // itemSelection[i] 表示第 i 项是否被选中
});
```

**常用方法：**

| 方法                                | 作用                   |
| ----------------------------------- | ---------------------- |
| `addItems({...})`                   | 添加选项               |
| `setCurrentSelection(...)`          | 设置选中项             |
| `getCurrentSelection()`             | 获取选中的文字列表     |
| `getCurrentSelectionIndex()`        | 获取选中的索引列表     |
| `setBorderRadius(n)`                | 设置圆角半径           |

**常用信号：**

| 信号                                    | 触发时机                   |
| --------------------------------------- | -------------------------- |
| `currentTextListChanged(QStringList)`   | 选中项改变时               |
| `itemSelectionChanged(QVector<bool>)`   | 任一项选中状态改变时       |

**ElaComboBox 与 ElaMultiSelectComboBox 的区别：**

| | ElaComboBox | ElaMultiSelectComboBox |
|---|-------------|------------------------|
| 选择方式 | 单选 | 多选 |
| 返回值 | 单个文字/索引 | 文字列表/索引列表 |
| 适用场景 | 下拉单选 | 下拉多选（如标签选择） |

---

### 3.16 ElaMessageButton（消息按钮）

**头文件：** `#include "ElaMessageButton.h"`

**创建：**

```cpp
ElaMessageButton* msgBtn = new ElaMessageButton("按钮文字", page);
msgBtn->setBarTitle("标题");
msgBtn->setBarText("这是消息内容");
```

**设置消息类型：**

```cpp
msgBtn->setMessageMode(ElaMessageBarType::Success);      // 成功（绿色）
msgBtn->setMessageMode(ElaMessageBarType::Information);  // 信息（蓝色）
msgBtn->setMessageMode(ElaMessageBarType::Warning);      // 警告（黄色）
msgBtn->setMessageMode(ElaMessageBarType::Error);        // 错误（红色）
```

**设置消息显示位置：**

```cpp
msgBtn->setPositionPolicy(ElaMessageBarType::TopRight);    // 右上角
msgBtn->setPositionPolicy(ElaMessageBarType::TopLeft);     // 左上角
msgBtn->setPositionPolicy(ElaMessageBarType::BottomRight); // 右下角
msgBtn->setPositionPolicy(ElaMessageBarType::BottomLeft);  // 左下角
```

**设置显示时间：**

```cpp
msgBtn->setDisplayMsec(3000);  // 显示 3 秒后自动消失
```

**常用方法：**

| 方法                           | 作用               |
| ------------------------------ | ------------------ |
| `setBarTitle("标题")`          | 设置消息标题       |
| `setBarText("内容")`           | 设置消息内容       |
| `setMessageMode(...)`          | 设置消息类型       |
| `setPositionPolicy(...)`       | 设置显示位置       |
| `setDisplayMsec(ms)`           | 设置显示时间       |
| `setBorderRadius(n)`           | 设置圆角半径       |

**完整示例：**

```cpp
// 成功消息按钮
ElaMessageButton* successBtn = new ElaMessageButton("Success", page);
successBtn->setBarTitle("操作成功");
successBtn->setBarText("数据已保存！");
successBtn->setMessageMode(ElaMessageBarType::Success);
successBtn->setPositionPolicy(ElaMessageBarType::TopRight);
successBtn->setDisplayMsec(2500);

// 错误消息按钮
ElaMessageButton* errorBtn = new ElaMessageButton("Error", page);
errorBtn->setBarTitle("错误");
errorBtn->setBarText("网络连接失败，请重试");
errorBtn->setMessageMode(ElaMessageBarType::Error);
errorBtn->setPositionPolicy(ElaMessageBarType::BottomRight);
```

---

### 3.17 ElaPlainTextEdit（多行文本编辑框）

**头文件：** `#include "ElaPlainTextEdit.h"`

**创建：**

```cpp
ElaPlainTextEdit* edit = new ElaPlainTextEdit(page);
edit->setPlainText("这是初始内容");
edit->setPlaceholderText("请输入内容...");
```

**带初始文本创建：**

```cpp
ElaPlainTextEdit* edit = new ElaPlainTextEdit("默认文本内容", page);
```

**获取/设置内容：**

```cpp
QString content = edit->toPlainText();  // 获取全部文本
edit->setPlainText("新内容");            // 设置文本
edit->appendPlainText("追加内容");       // 追加文本
edit->clear();                           // 清空内容
```

**监听内容变化：**

```cpp
connect(edit, &ElaPlainTextEdit::textChanged, this, [edit]() {
    qDebug() << "内容:" << edit->toPlainText();
});
```

**常用方法（继承自 QPlainTextEdit）：**

| 方法                          | 作用             |
| ----------------------------- | ---------------- |
| `toPlainText()`               | 获取全部文本     |
| `setPlainText("文本")`        | 设置文本内容     |
| `appendPlainText("文本")`     | 追加文本         |
| `setPlaceholderText("提示")`  | 设置占位提示     |
| `clear()`                     | 清空内容         |
| `setReadOnly(true)`           | 设置只读         |

**ElaLineEdit 与 ElaPlainTextEdit 的区别：**

| | ElaLineEdit | ElaPlainTextEdit |
|---|-------------|------------------|
| 行数 | 单行 | 多行 |
| 适用场景 | 用户名、密码、搜索 | 备注、描述、代码 |
| 换行 | 不支持 | 支持 |

---

### 3.18 ElaIconButton（图标按钮）

**头文件：** `#include "ElaIconButton.h"`

**创建（使用内置图标）：**

```cpp
ElaIconButton* iconBtn = new ElaIconButton(ElaIconType::Home, page);
```

**创建（指定图标大小）：**

```cpp
ElaIconButton* iconBtn = new ElaIconButton(ElaIconType::Search, 20, page);
```

**创建（指定图标和按钮大小）：**

```cpp
ElaIconButton* iconBtn = new ElaIconButton(ElaIconType::Settings, 18, 40, 40, page);
// 参数：图标类型, 图标像素大小, 按钮宽度, 按钮高度, 父对象
```

**创建（使用自定义图片）：**

```cpp
ElaIconButton* iconBtn = new ElaIconButton(QPixmap(":/icon.png"), page);
```

**更换图标：**

```cpp
iconBtn->setAwesome(ElaIconType::User);      // 更换为内置图标
iconBtn->setPixmap(QPixmap(":/new.png"));    // 更换为自定义图片
```

**设置颜色：**

```cpp
// 浅色主题下的颜色
iconBtn->setLightIconColor(QColor(50, 50, 50));       // 图标颜色
iconBtn->setLightHoverColor(QColor(200, 200, 200));   // 悬停背景色
iconBtn->setLightHoverIconColor(QColor(0, 0, 0));     // 悬停图标颜色

// 深色主题下的颜色
iconBtn->setDarkIconColor(QColor(200, 200, 200));
iconBtn->setDarkHoverColor(QColor(60, 60, 60));
iconBtn->setDarkHoverIconColor(QColor(255, 255, 255));
```

**常用方法：**

| 方法                              | 作用                 |
| --------------------------------- | -------------------- |
| `setAwesome(ElaIconType::...)`    | 设置内置图标         |
| `setPixmap(QPixmap)`              | 设置自定义图片       |
| `setBorderRadius(n)`              | 设置圆角半径         |
| `setIsSelected(true)`             | 设置选中状态         |
| `setOpacity(0.8)`                 | 设置透明度           |

**常用信号（继承自 QPushButton）：**

| 信号        | 触发时机     |
| ----------- | ------------ |
| `clicked()` | 按钮被点击时 |

**实用示例（工具栏图标按钮）：**

```cpp
QHBoxLayout* toolBar = new QHBoxLayout();

ElaIconButton* homeBtn = new ElaIconButton(ElaIconType::Home, 18, 32, 32, page);
ElaIconButton* searchBtn = new ElaIconButton(ElaIconType::Search, 18, 32, 32, page);
ElaIconButton* settingsBtn = new ElaIconButton(ElaIconType::Gear, 18, 32, 32, page);

toolBar->addWidget(homeBtn);
toolBar->addWidget(searchBtn);
toolBar->addWidget(settingsBtn);
toolBar->addStretch();

connect(homeBtn, &ElaIconButton::clicked, this, []() {
    qDebug() << "点击了首页";
});
```

---

### 3.19 ElaScrollPageArea（滚动页面区域）

**头文件：** `#include "ElaScrollPageArea.h"`

**创建：**

```cpp
ElaScrollPageArea* area = new ElaScrollPageArea(page);
```

**添加内容：**

```cpp
ElaScrollPageArea* area = new ElaScrollPageArea(page);
QHBoxLayout* layout = new QHBoxLayout(area);

ElaText* label = new ElaText("组件名称", page);
label->setTextPixelSize(15);

ElaToggleSwitch* toggle = new ElaToggleSwitch(page);

layout->addWidget(label);
layout->addWidget(toggle);
layout->addStretch();
```

**常用方法：**

| 方法                   | 作用           |
| ---------------------- | -------------- |
| `setBorderRadius(n)`   | 设置圆角半径   |
| `setFixedHeight(n)`    | 固定高度       |

**使用场景：**

ElaScrollPageArea 是一个带有圆角背景的容器组件，常用于：
- 将相关的组件组合在一起
- 为组件组提供统一的背景
- 在设置页面中作为选项行的容器

**完整示例（设置选项行）：**

```cpp
// 创建一个设置选项行
ElaScrollPageArea* optionArea = new ElaScrollPageArea(page);
QHBoxLayout* optionLayout = new QHBoxLayout(optionArea);

ElaText* optionLabel = new ElaText("启用通知", page);
optionLabel->setTextPixelSize(15);

ElaToggleSwitch* optionSwitch = new ElaToggleSwitch(page);

optionLayout->addWidget(optionLabel);
optionLayout->addStretch();
optionLayout->addWidget(optionSwitch);
```

---

### 3.20 ElaToolButton（工具按钮）

**头文件：** `#include "ElaToolButton.h"`

**创建：**

```cpp
ElaToolButton* toolBtn = new ElaToolButton(page);
toolBtn->setText("按钮文字");
toolBtn->setElaIcon(ElaIconType::Home);
```

**设置图标：**

```cpp
// 使用 Ela 内置图标
toolBtn->setElaIcon(ElaIconType::FileDoc);
toolBtn->setElaIcon(ElaIconType::MoonStars);
toolBtn->setElaIcon(ElaIconType::Timer);
```

**按钮样式：**

```cpp
// 图标在文字旁边
toolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

// 仅显示图标
toolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

// 仅显示文字
toolBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);

// 图标在文字下方
toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
```

**透明/非透明背景：**

```cpp
toolBtn->setIsTransparent(true);   // 透明背景（适合工具栏）
toolBtn->setIsTransparent(false);  // 非透明背景（有背景色）
```

**关联下拉菜单：**

```cpp
ElaMenu* menu = new ElaMenu(page);
menu->addElaIconAction(ElaIconType::CardsBlank, "选项1");
menu->addElaIconAction(ElaIconType::EarthAmericas, "选项2");
toolBtn->setMenu(menu);  // 关联菜单，点击按钮弹出菜单
```

**设置尺寸：**

```cpp
toolBtn->setFixedSize(35, 35);    // 固定宽高（方形按钮）
toolBtn->setFixedHeight(35);      // 只固定高度
toolBtn->setFixedWidth(120);      // 只固定宽度
```

**信号连接：**

```cpp
connect(toolBtn, &ElaToolButton::clicked, this, [=]() {
    // 点击后执行的代码
});
```

**常用方法：**

| 方法                             | 作用                     |
| -------------------------------- | ------------------------ |
| `setText("文字")`                | 设置按钮文字             |
| `setElaIcon(ElaIconType::...)`   | 设置 Ela 内置图标        |
| `setToolButtonStyle(...)`        | 设置按钮样式             |
| `setIsTransparent(bool)`         | 设置背景透明             |
| `setMenu(ElaMenu*)`              | 关联下拉菜单             |
| `setFixedSize(w, h)`             | 设置固定尺寸             |
| `setEnabled(false)`              | 禁用按钮                 |

**ToolButtonStyle 枚举：**

| 样式                             | 效果           |
| -------------------------------- | -------------- |
| `Qt::ToolButtonIconOnly`         | 仅图标         |
| `Qt::ToolButtonTextOnly`         | 仅文字         |
| `Qt::ToolButtonTextBesideIcon`   | 图标在文字旁边 |
| `Qt::ToolButtonTextUnderIcon`    | 图标在文字上方 |

**实用示例（主题切换按钮）：**

```cpp
#include "ElaToolButton.h"
#include "ElaTheme.h"

ElaToolButton* themeBtn = new ElaToolButton(page);
themeBtn->setFixedSize(35, 35);
themeBtn->setIsTransparent(false);
themeBtn->setElaIcon(ElaIconType::MoonStars);

connect(themeBtn, &ElaToolButton::clicked, this, [=]() {
    // 切换亮色/暗色主题
    eTheme->setThemeMode(
        eTheme->getThemeMode() == ElaThemeType::Light
            ? ElaThemeType::Dark
            : ElaThemeType::Light
    );
});
```

**实用示例（带下拉菜单的按钮）：**

```cpp
ElaToolButton* docBtn = new ElaToolButton(page);
docBtn->setFixedHeight(35);
docBtn->setIsTransparent(false);
docBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
docBtn->setText("Documentation");
docBtn->setElaIcon(ElaIconType::FileDoc);

ElaMenu* docMenu = new ElaMenu(page);
docMenu->addElaIconAction(ElaIconType::Book, "使用指南");
docMenu->addElaIconAction(ElaIconType::Code, "API 文档");
docMenu->addElaIconAction(ElaIconType::CircleQuestion, "常见问题");
docBtn->setMenu(docMenu);
```

**ElaToolButton 与 ElaPushButton 的区别：**

| | ElaToolButton | ElaPushButton |
|---|---------------|---------------|
| 继承自 | QToolButton | QPushButton |
| 图标支持 | 原生支持 setElaIcon | 需要 setIcon |
| 菜单支持 | 原生支持 setMenu | 不支持下拉菜单 |
| 样式变体 | 4 种 ToolButtonStyle | 固定样式 |
| 透明模式 | 支持 setIsTransparent | 不支持 |
| 适用场景 | 工具栏、带菜单按钮 | 普通操作按钮 |

---

### 3.21 ElaMenu（菜单）

**头文件：** `#include "ElaMenu.h"`

**创建：**

```cpp
ElaMenu* menu = new ElaMenu(this);
```

**添加菜单项：**

```cpp
// 普通菜单项
menu->addAction("菜单项1");

// 带图标的菜单项
menu->addElaIconAction(ElaIconType::BoxCheck, "排序方式");

// 带图标和快捷键的菜单项
menu->addElaIconAction(ElaIconType::BoxCheck, "保存", QKeySequence::Save);

// 分隔线
menu->addSeparator();
```

**子菜单（嵌套）：**

```cpp
ElaMenu* subMenu = menu->addMenu(ElaIconType::Cubes, "查看");
subMenu->addAction("查看1");
subMenu->addAction("查看2");

// 多级嵌套
ElaMenu* subSubMenu = subMenu->addMenu(ElaIconType::Cubes, "更多");
subSubMenu->addAction("选项A");
```

**监听菜单项点击：**

```cpp
QAction* action = menu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
connect(action, &QAction::triggered, this, [=]() {
    qDebug() << "撤销操作";
});
```

**弹出菜单（右键菜单）：**

```cpp
// 在鼠标位置弹出
void MyPage::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        menu->popup(event->globalPosition().toPoint());  // Qt6
#else
        menu->popup(event->globalPos());                   // Qt5
#endif
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
```

**关联到 ElaToolButton：**

```cpp
ElaToolButton* btn = new ElaToolButton(page);
btn->setMenu(menu);  // 点击按钮弹出菜单
```

**常用方法：**

| 方法                                          | 作用                   |
| --------------------------------------------- | ---------------------- |
| `addAction("文字")`                           | 添加普通菜单项         |
| `addElaIconAction(icon, "文字")`              | 添加带图标的菜单项     |
| `addElaIconAction(icon, "文字", shortcut)`    | 添加带图标和快捷键的项 |
| `addMenu(icon, "文字")`                       | 添加子菜单             |
| `addSeparator()`                              | 添加分隔线             |
| `popup(QPoint)`                               | 在指定位置弹出菜单     |

---

### 3.22 ElaImageCard（图片卡片）

**头文件：** `#include "ElaImageCard.h"`

**创建：**

```cpp
ElaImageCard* imageCard = new ElaImageCard(page);
imageCard->setCardImage(QImage(":/Resource/Image/background.png"));
imageCard->setBorderRadius(10);       // 圆角 10px
imageCard->setFixedHeight(340);       // 固定高度
```

**常用方法：**

| 方法                         | 作用             |
| ---------------------------- | ---------------- |
| `setCardImage(QImage)`       | 设置卡片背景图片 |
| `setBorderRadius(n)`         | 设置圆角半径     |
| `setFixedHeight(n)`          | 固定高度         |
| `setFixedSize(w, h)`         | 固定尺寸         |

**使用场景：** 首页大背景图、Banner 展示区域。可在其上叠加布局放置其他控件。

**叠加布局示例：**

```cpp
ElaImageCard* bg = new ElaImageCard(page);
bg->setCardImage(QImage(":/bg.png"));
bg->setFixedHeight(340);
bg->setBorderRadius(10);

// 在图片卡片上叠加内容
QVBoxLayout* bgLayout = new QVBoxLayout(bg);
ElaText* title = new ElaText("标题文字", page);
title->setTextPixelSize(35);
bgLayout->addWidget(title);
bgLayout->addStretch();
```

---

### 3.23 ElaAcrylicUrlCard（亚克力链接卡片）

**头文件：** `#include "ElaAcrylicUrlCard.h"`

**创建：**

```cpp
ElaAcrylicUrlCard* urlCard = new ElaAcrylicUrlCard(page);
urlCard->setFixedSize(195, 225);                                    // 卡片尺寸
urlCard->setCardPixmap(QPixmap(":/Resource/Image/github.png"));     // 卡片图标
urlCard->setCardPixmapSize(QSize(62, 62));                          // 图标大小
urlCard->setTitle("GitHub");                                        // 标题
urlCard->setSubTitle("点击访问项目主页");                              // 副标题
urlCard->setUrl("https://github.com/Liniyous/ElaWidgetTools");     // 点击跳转的 URL
```

**间距调整：**

```cpp
urlCard->setTitlePixelSize(17);     // 标题字体大小
urlCard->setTitleSpacing(25);       // 标题与图标的间距
urlCard->setSubTitleSpacing(13);    // 副标题与标题的间距
```

**常用方法：**

| 方法                           | 作用                       |
| ------------------------------ | -------------------------- |
| `setUrl("https://...")`        | 设置点击跳转的 URL         |
| `setCardPixmap(QPixmap)`       | 设置卡片图标               |
| `setCardPixmapSize(QSize)`     | 设置图标大小               |
| `setTitle("标题")`             | 设置标题                   |
| `setSubTitle("副标题")`        | 设置副标题                 |
| `setTitlePixelSize(n)`         | 设置标题字号               |
| `setTitleSpacing(n)`           | 设置标题与图标间距         |
| `setSubTitleSpacing(n)`        | 设置副标题间距             |

**特点：**
- 亚克力（磨砂玻璃）背景效果
- 点击自动调用系统浏览器打开 URL
- 适合用于项目链接、社交媒体入口

---

### 3.24 ElaPopularCard（热门应用卡片）

**头文件：** `#include "ElaPopularCard.h"`

**创建：**

```cpp
ElaPopularCard* card = new ElaPopularCard(page);
card->setCardPixmap(QPixmap(":/Resource/Image/app.jpg"));           // 应用图标
card->setTitle("应用名称");                                          // 标题
card->setSubTitle("5.0⭐ 实用程序与工具");                            // 副标题（评分/分类）
card->setInteractiveTips("免费下载");                                // 按钮文字
card->setDetailedText("这是一段详细的应用描述...");                    // 详细描述
card->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC_7+.png"));  // 右下角标图片
```

**监听按钮点击：**

```cpp
connect(card, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
    // 点击卡片按钮后的操作
    QDesktopServices::openUrl(QUrl("https://github.com/..."));
});
```

**常用方法：**

| 方法                              | 作用                   |
| --------------------------------- | ---------------------- |
| `setCardPixmap(QPixmap)`          | 设置应用图标           |
| `setTitle("标题")`                | 设置应用名称           |
| `setSubTitle("副标题")`           | 设置评分/分类信息      |
| `setInteractiveTips("文字")`      | 设置按钮文字           |
| `setDetailedText("描述")`         | 设置详细描述           |
| `setCardFloatPixmap(QPixmap)`     | 设置右下角标图片       |

**常用信号：**

| 信号                           | 触发时机         |
| ------------------------------ | ---------------- |
| `popularCardButtonClicked()`   | 卡片按钮被点击时 |

**使用场景：** 仿 Microsoft Store 的热门应用卡片，适合展示推荐项目、应用列表。

---

### 3.25 ElaFlowLayout（流式布局）

**头文件：** `#include "ElaFlowLayout.h"`

**创建：**

```cpp
// 参数：父布局/对象, 水平间距, 垂直间距
ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
flowLayout->setContentsMargins(30, 0, 0, 0);
```

**启用动画：**

```cpp
flowLayout->setIsAnimation(true);  // 窗口缩放时卡片平滑移动
```

**添加组件：**

```cpp
flowLayout->addWidget(card1);
flowLayout->addWidget(card2);
flowLayout->addWidget(card3);
// 根据窗口宽度自动换行排列
```

**常用方法：**

| 方法                         | 作用                       |
| ---------------------------- | -------------------------- |
| `addWidget(widget)`          | 添加子控件                 |
| `setIsAnimation(true)`       | 启用布局变化动画           |
| `setContentsMargins(...)`    | 设置边距                   |

**特点：**
- 类似 CSS 的 `display: flex; flex-wrap: wrap`
- 子控件按从左到右排列，空间不足时自动换行
- 支持动画过渡效果

**对比普通布局：**

| | QHBoxLayout | QVBoxLayout | ElaFlowLayout |
|---|-------------|-------------|---------------|
| 排列方向 | 水平 | 垂直 | 水平，自动换行 |
| 换行 | 不换行 | 不换行 | 自动换行 |
| 自适应 | 固定方向 | 固定方向 | 根据窗口宽度自适应 |

---

### 3.26 ElaScrollArea（滚动区域）

**头文件：** `#include "ElaScrollArea.h"`

**创建：**

```cpp
ElaScrollArea* scrollArea = new ElaScrollArea(page);
scrollArea->setWidgetResizable(true);  // 内容自适应大小
```

**隐藏滚动条：**

```cpp
scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏水平滚动条
scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    // 隐藏垂直滚动条
```

**手势与弹性效果：**

```cpp
scrollArea->setIsGrabGesture(true, 0);                   // 启用鼠标拖拽手势
scrollArea->setIsOverShoot(Qt::Horizontal, true);         // 水平方向弹性过度滚动
scrollArea->setIsOverShoot(Qt::Vertical, true);           // 垂直方向弹性过度滚动
```

**设置内容：**

```cpp
QWidget* content = new QWidget();
content->setStyleSheet("background-color:transparent;");
scrollArea->setWidget(content);

// 在 content 上添加布局和控件
QVBoxLayout* contentLayout = new QVBoxLayout(content);
contentLayout->addWidget(widget1);
contentLayout->addWidget(widget2);
```

**常用方法：**

| 方法                                    | 作用                   |
| --------------------------------------- | ---------------------- |
| `setWidget(QWidget*)`                   | 设置滚动内容           |
| `setWidgetResizable(true)`              | 内容自适应大小         |
| `setHorizontalScrollBarPolicy(...)`     | 设置水平滚动条策略     |
| `setVerticalScrollBarPolicy(...)`       | 设置垂直滚动条策略     |
| `setIsGrabGesture(true, 0)`             | 启用手势拖拽           |
| `setIsOverShoot(direction, true)`       | 启用弹性过度滚动       |

**滚动条策略：**

| 枚举值                       | 效果         |
| ---------------------------- | ------------ |
| `Qt::ScrollBarAlwaysOff`     | 始终隐藏     |
| `Qt::ScrollBarAlwaysOn`      | 始终显示     |
| `Qt::ScrollBarAsNeeded`      | 需要时显示   |

---

### 3.27 ElaToolTip（工具提示）

**头文件：** `#include "ElaToolTip.h"`

**创建：**

```cpp
// 为任意控件添加工具提示
ElaToolTip* toolTip = new ElaToolTip(targetWidget);  // 传入目标控件
toolTip->setToolTip("鼠标悬停时显示的提示文字");
```

**完整示例：**

```cpp
ElaAcrylicUrlCard* card = new ElaAcrylicUrlCard(page);
card->setTitle("GitHub");

// 为卡片添加悬停提示
ElaToolTip* tip = new ElaToolTip(card);
tip->setToolTip("https://github.com/Liniyous/ElaWidgetTools");
```

**常用方法：**

| 方法                    | 作用                   |
| ----------------------- | ---------------------- |
| `setToolTip("文字")`    | 设置提示文字           |

**特点：**
- 鼠标悬停在目标控件上时自动显示
- 相比 Qt 原生 `setToolTip()`，样式与 Ela 主题统一

---

### 3.28 ElaScrollPageArea 与 ElaScrollArea 对比

这两个组件名称相似，但用途完全不同：

| | ElaScrollPageArea | ElaScrollArea |
|---|-------------------|---------------|
| **类型** | 静态容器（不可滚动） | 滚动容器（可滚动） |
| **继承自** | QWidget | QScrollArea |
| **主要用途** | 为一组控件提供带圆角的背景 | 当内容超出时提供滚动功能 |
| **外观** | 带圆角、有背景色的卡片 | 透明，仅提供滚动能力 |

**ElaScrollPageArea 示例（设置选项行）：**

```cpp
ElaScrollPageArea* area = new ElaScrollPageArea(this);
QHBoxLayout* layout = new QHBoxLayout(area);

ElaText* label = new ElaText("启用通知", this);
ElaToggleSwitch* toggle = new ElaToggleSwitch(this);

layout->addWidget(label);
layout->addStretch();
layout->addWidget(toggle);
```

**ElaScrollArea 示例（可滚动卡片列表）：**

```cpp
ElaScrollArea* scrollArea = new ElaScrollArea(this);
scrollArea->setWidgetResizable(true);
scrollArea->setIsGrabGesture(true, 0);  // 手势拖拽

QWidget* content = new QWidget();
scrollArea->setWidget(content);

QHBoxLayout* layout = new QHBoxLayout(content);
layout->addWidget(card1);
layout->addWidget(card2);  // 内容超出时可滚动
```

**简单记忆：**
- **ElaScrollPageArea** = "Area"（区域/背景容器）
- **ElaScrollArea** = "Scroll"（滚动容器）

---

### 3.29 ElaScrollPage 多页面堆叠

ElaScrollPage 支持在单个页面内部创建多个可切换的子视图。

**添加多个中心部件：**

```cpp
// 添加主内容（索引 0）
addCentralWidget(mainContent, true, true, 0);

// 添加额外视图（索引 1、2...）
ElaText* detailView = new ElaText("详情视图", this);
detailView->setAlignment(Qt::AlignCenter);
addCentralWidget(detailView);

ElaText* emptyState = new ElaText("暂无数据", this);
emptyState->setAlignment(Qt::AlignCenter);
addCentralWidget(emptyState);
```

**`addCentralWidget` 参数说明：**

```cpp
addCentralWidget(widget, isWidgetResizeable, isVerticalGrabGesture, baseMargin);
// widget              — 要添加的部件
// isWidgetResizeable  — 内容是否可调整大小（默认 true）
// isVerticalGrabGesture — 是否启用垂直手势滚动（默认 true）
// baseMargin          — 基础边距（默认 0）
```

**切换视图（页面内导航）：**

```cpp
this->navigation(0);  // 显示主内容（索引 0）
this->navigation(1);  // 显示详情视图（索引 1）
this->navigation(2);  // 显示空状态（索引 2）
```

**完整示例：**

```cpp
// 在构造函数中
QWidget* mainContent = new QWidget(this);
// ... 添加主内容的控件 ...
addCentralWidget(mainContent, true, true, 0);

ElaText* detailView = new ElaText("详情页面", this);
QFont font = detailView->font();
font.setPixelSize(32);
detailView->setFont(font);
detailView->setAlignment(Qt::AlignCenter);
addCentralWidget(detailView);

// 在事件处理中切换
void MyPage::onShowDetail() {
    this->navigation(1);  // 切换到详情视图
}

void MyPage::onBackToMain() {
    this->navigation(0);  // 返回主内容
}
```

**两层导航的区别：**

| 层级 | 方法 | 作用范围 |
|------|------|----------|
| **ElaWindow 导航** | `navigation(pageKey)` | 切换左侧导航栏对应的**不同页面** |
| **ElaScrollPage 导航** | `this->navigation(index)` | 切换**当前页面内部**的堆叠视图 |

**使用场景：**
- 向导式流程（多步骤表单）
- 点击某项后显示详情
- 空状态/加载状态切换
- 列表视图/网格视图切换

---

### 3.30 T_Icon - 图标展示页（MVC 完整示例）

**头文件：** `#include "T_Icon.h"`

T_Icon 是一个完整的 MVC 模式示例，展示如何使用自定义 Model 和 Delegate 实现图标展示和搜索功能。

#### 架构概览

```
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│   T_IconModel   │ ←── │   ElaListView   │ ──→ │ T_IconDelegate  │
│   (数据模型)     │     │   (视图)         │     │  (绘制代理)      │
└─────────────────┘     └─────────────────┘     └─────────────────┘
```

#### T_Icon 控制器类

**功能：** 组装 Model、View、Delegate，协调搜索功能和点击事件

**核心成员变量：**

```cpp
class T_Icon : public T_BasePage {
    QMetaEnum _metaEnum;              // 存储枚举的元信息
    ElaLineEdit* _searchEdit;          // 搜索输入框
    ElaListView* _iconView;            // 列表视图
    T_IconModel* _iconModel;           // 数据模型
    T_IconDelegate* _iconDelegate;     // 绘制代理
};
```

**构造函数关键步骤：**

```cpp
T_Icon::T_Icon(QWidget* parent) : T_BasePage(parent)
{
    // 1. 获取枚举元信息（Qt 反射机制）
    _metaEnum = QMetaEnum::fromType<ElaIconType::IconName>();

    // 2. 创建并配置 ElaListView
    _iconView = new ElaListView(this);
    _iconView->setIsTransparent(true);
    _iconView->setFlow(QListView::LeftToRight);      // 水平排列
    _iconView->setViewMode(QListView::IconMode);     // 图标模式
    _iconView->setResizeMode(QListView::Adjust);     // 自动调整

    // 3. 处理点击事件 - 复制图标枚举名到剪贴板
    connect(_iconView, &ElaListView::clicked, this, [=](const QModelIndex& index) {
        QString iconName = _iconModel->getIconNameFromModelIndex(index);
        qApp->clipboard()->setText(iconName);
        ElaMessageBar::success(ElaMessageBarType::Top, "复制完成",
                               iconName + "已被复制到剪贴板", 1000, this);
    });

    // 4. 创建 Model 和 Delegate
    _iconModel = new T_IconModel(this);
    _iconDelegate = new T_IconDelegate(this);
    _iconView->setModel(_iconModel);
    _iconView->setItemDelegate(_iconDelegate);

    // 5. 创建搜索框并连接信号
    _searchEdit = new ElaLineEdit(this);
    _searchEdit->setPlaceholderText("搜索图标");
    connect(_searchEdit, &ElaLineEdit::textEdited, this, &T_Icon::onSearchEditTextEdit);
}
```

**搜索功能实现：**

```cpp
void T_Icon::onSearchEditTextEdit(const QString& searchText)
{
    // 搜索框为空 - 显示所有图标
    if (searchText.isEmpty()) {
        _iconModel->setIsSearchMode(false);
        _iconModel->setSearchKeyList(QStringList());
        _iconView->viewport()->update();
        return;
    }

    // 筛选匹配的图标
    QStringList searchKeyList;
    for (int i = 1; i < _metaEnum.keyCount(); i++) {
        QString key = QString(_metaEnum.key(i));
        if (key.contains(searchText, Qt::CaseInsensitive)) {
            searchKeyList.append(key);
        }
    }

    // 更新 Model
    _iconModel->setIsSearchMode(true);
    _iconModel->setSearchKeyList(searchKeyList);
    _iconView->scrollTo(_iconModel->index(0, 0));  // 滚动到第一个结果
}
```

#### T_IconModel 数据模型

**继承自：** `QAbstractListModel`

**职责：** 为 ElaListView 提供图标数据

**核心方法：**

```cpp
class T_IconModel : public QAbstractListModel {
    Q_PROPERTY_CREATE(bool, IsSearchMode)  // 自动生成 getter/setter

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void setSearchKeyList(QStringList list);
    QString getIconNameFromModelIndex(const QModelIndex& index) const;
};
```

**rowCount() - 返回行数：**

```cpp
int T_IconModel::rowCount(const QModelIndex& parent) const
{
    return _rowCount;  // 搜索模式返回匹配数，否则返回全部图标数
}
```

**data() - 返回单元格数据：**

```cpp
QVariant T_IconModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole) {
        if (!_pIsSearchMode) {
            // 返回 [枚举名, 图标字符]
            return QStringList{
                _metaEnum.key(index.row() + 1),
                QChar(_metaEnum.value(index.row() + 1))
            };
        } else {
            // 搜索模式：从搜索结果列表获取
            return QStringList{
                _searchKeyList.at(index.row()),
                QChar(_metaEnum.keyToValue(_searchKeyList.at(index.row()).toUtf8().constData()))
            };
        }
    }
    return QVariant();
}
```

**数据格式：**

```
QStringList { "Home", "\ue900" }
               ↑         ↑
           枚举名    图标 Unicode 字符
```

**setSearchKeyList() - 更新搜索结果：**

```cpp
void T_IconModel::setSearchKeyList(QStringList list)
{
    beginResetModel();  // 通知 View 数据即将重置

    _searchKeyList = list;
    if (_pIsSearchMode) {
        _rowCount = list.count();  // 搜索模式：行数 = 匹配数
    } else {
        _rowCount = _metaEnum.keyCount() - 1;  // 正常模式：全部图标
    }

    endResetModel();  // 通知 View 重新获取数据
}
```

#### T_IconDelegate 绘制代理

**继承自：** `QStyledItemDelegate`

**职责：** 自定义绘制每个图标项

**核心方法：**

```cpp
class T_IconDelegate : public QStyledItemDelegate {
    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};
```

**sizeHint() - 返回项尺寸：**

```cpp
QSize T_IconDelegate::sizeHint(const QStyleOptionViewItem& option,
                               const QModelIndex& index) const
{
    return QSize(100, 100);  // 每个图标项固定 100×100 像素
}
```

**paint() - 绘制项的外观：**

```cpp
void T_IconDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                           const QModelIndex& index) const
{
    // 1. 获取数据
    QStringList iconList = index.data(Qt::UserRole).toStringList();
    QString iconName = iconList.at(0);   // 如 "Home"
    QString iconValue = iconList.at(1);  // 如 "\ue900"

    // 2. 保存画笔状态
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    // 3. 绘制图标
    painter->save();
    QFont iconFont = QFont("ElaAwesome");
    iconFont.setPixelSize(22);
    painter->setFont(iconFont);
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    painter->drawText(
        option.rect.x() + option.rect.width() / 2 - 11,   // 水平居中
        option.rect.y() + option.rect.height() / 2 - 11,  // 垂直居中
        iconValue
    );
    painter->restore();

    // 4. 绘制文字
    painter->setPen(ElaThemeColor(_themeMode, BasicText));
    QFont titlefont = painter->font();
    titlefont.setPixelSize(13);
    painter->setFont(titlefont);

    qreal rowTextWidth = option.rect.width() * 0.8;
    int subTitleRow = painter->fontMetrics().horizontalAdvance(iconName) / rowTextWidth;

    if (subTitleRow > 0) {
        // 多行文字（换行处理）
        QString subTitleText = iconName;
        for (int i = 0; i < subTitleRow + 1; i++) {
            QString text = painter->fontMetrics().elidedText(
                subTitleText, Qt::ElideRight, rowTextWidth);
            subTitleText.remove(0, text.length());
            painter->drawText(
                option.rect.x() + option.rect.width() / 2 -
                    painter->fontMetrics().horizontalAdvance(text) / 2,
                option.rect.y() + option.rect.height() - 10 * (subTitleRow + 1 - i),
                text
            );
        }
    } else {
        // 单行文字
        painter->drawText(
            option.rect.x() + option.rect.width() / 2 -
                painter->fontMetrics().horizontalAdvance(iconName) / 2,
            option.rect.y() + option.rect.height() - 20,
            iconName
        );
    }

    painter->restore();  // 恢复画笔状态
}
```

**关键概念 - save()/restore() 机制：**

```
painter->save();          // 📸 存档：记住当前设置
    painter->setFont(...); // 修改字体
    painter->drawText(...);// 绘制文字
painter->restore();       // ⏪ 读档：恢复之前的设置
```

- **save()** 保存当前画笔状态（字体、颜色、笔宽等）
- **restore()** 恢复到上一次 save 时的状态
- 嵌套使用时遵循栈结构（先进后出）
- **必须使用**，否则修改的设置会影响后续绘制

#### ElaListView 配置

**关键配置项：**

| 方法                              | 作用                 |
|-----------------------------------|----------------------|
| `setIsTransparent(true)`          | 设置透明背景         |
| `setFlow(QListView::LeftToRight)` | 水平排列，满行换行   |
| `setViewMode(QListView::IconMode)`| 图标模式显示         |
| `setResizeMode(QListView::Adjust)`| 窗口变化时自动重排   |
| `setModel(model)`                 | 设置数据模型         |
| `setItemDelegate(delegate)`       | 设置绘制代理         |
| `scrollTo(index)`                 | 滚动到指定项         |

**scrollTo() 的作用：**

```cpp
_iconView->scrollTo(_iconModel->index(0, 0));  // 滚动到第一个结果
```

**为什么需要？** 搜索后数据量变少，但滚动位置不变。如果用户之前滚动到底部，搜索后可能看到空白区域：

```
搜索前（500 个图标，用户已滚动到底部）：
┌─────────────────────────────┐
│  图标 470   图标 471  ...   │  ← 用户视口在底部
└─────────────────────────────┘

搜索 "home" 后只有 5 个结果：

不调用 scrollTo()：              调用 scrollTo() 后：
┌─────────────────────────────┐  ┌─────────────────────────────┐
│      （空白区域）            │  │  🏠 Home    🏠 HomeAlt      │
│  视口还在原位置 ❌           │  │  视口自动滚到第一个结果 ✅   │
└─────────────────────────────┘  └─────────────────────────────┘
```

**scrollTo() 的滚动模式：**

```cpp
scrollTo(index, QAbstractItemView::EnsureVisible);     // 默认：确保可见
scrollTo(index, QAbstractItemView::PositionAtTop);     // 定位到视口顶部
scrollTo(index, QAbstractItemView::PositionAtCenter);  // 定位到视口中央
scrollTo(index, QAbstractItemView::PositionAtBottom);  // 定位到视口底部
```

#### 完整数据流

```
用户输入搜索词 "home"
    ↓
T_Icon::onSearchEditTextEdit() 遍历枚举筛选
    ↓
_iconModel->setIsSearchMode(true)
_iconModel->setSearchKeyList([匹配项])
    ↓
Model 触发 beginResetModel() / endResetModel()
    ↓
ElaListView 收到通知，调用：
  - rowCount() → 获取新的行数
  - data(index) → 获取每行数据
  - delegate->paint() → 绘制每一项
    ↓
用户点击图标 → clicked 信号
    ↓
调用 _iconModel->getIconNameFromModelIndex()
    ↓
复制枚举名到剪贴板 + 显示提示
```

#### QMetaEnum 反射机制

```cpp
QMetaEnum metaEnum = QMetaEnum::fromType<ElaIconType::IconName>();

// 常用方法
int count = metaEnum.keyCount();           // 枚举成员总数
QString name = metaEnum.key(1);            // 第 1 个枚举名 → "Home"
int value = metaEnum.value(1);             // 第 1 个枚举值 → 0xe900
int val = metaEnum.keyToValue("Home");     // 名称转数值 → 0xe900
```

**作用：** 在运行时动态遍历枚举，无需硬编码图标列表

#### 核心知识点总结

| 概念 | 说明 |
|------|------|
| **MVC 模式** | Model（数据）+ View（显示）+ Delegate（绘制） |
| **QAbstractListModel** | 列表数据模型基类，必须重写 rowCount() 和 data() |
| **QStyledItemDelegate** | 自定义项绘制代理，重写 paint() 和 sizeHint() |
| **beginResetModel/endResetModel** | 通知 View 数据已改变，需要重新请求 |
| **Qt::UserRole** | 自定义数据角色，避免与标准角色冲突 |
| **QMetaEnum** | Qt 反射机制，运行时访问枚举信息 |
| **save/restore** | 画笔状态管理，嵌套使用遵循栈结构 |
| **ElaListView** | Ela 风格列表视图，支持 IconMode 图标模式 |

#### 使用场景

- **列表/网格展示**：需要自定义每项外观时
- **搜索过滤**：动态改变数据源时
- **主题切换**：根据主题自适应颜色
- **图标/图片展示**：需要混合图形和文字显示

---

### 3.31 ElaLCDNumber（LCD 数字显示）

**头文件：** `#include "ElaLCDNumber.h"`

LCD 风格的数字显示组件，支持自动时钟模式。

**创建：**

```cpp
ElaLCDNumber* lcd = new ElaLCDNumber(this);
lcd->setIsUseAutoClock(true);          // 开启自动时钟模式
lcd->setIsTransparent(false);           // 非透明背景（有背景色）
lcd->setAutoClockFormat("hh:mm:ss");   // 设置时间格式
lcd->setFixedHeight(100);               // 固定高度
```

**效果：**

```
┌─────────────────────────────────────────┐
│                                         │
│            1 2 : 3 4 : 5 6              │  ← 7 段数码管风格
│                                         │
└─────────────────────────────────────────┘
```

**手动显示数值：**

```cpp
lcd->setIsUseAutoClock(false);  // 关闭自动时钟
lcd->display(12345);             // 显示整数
lcd->display(3.14159);           // 显示浮点数
lcd->display("HELLO");           // 显示文字
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setIsUseAutoClock(true)` | 开启自动时钟模式 |
| `setAutoClockFormat("hh:mm:ss")` | 设置时间格式 |
| `setIsTransparent(bool)` | 设置是否透明背景 |
| `display(int/double/QString)` | 手动显示数值或文字 |
| `setDigitCount(n)` | 设置显示位数 |

**时间格式说明：**

| 格式 | 效果 |
|------|------|
| `"hh:mm:ss"` | 12:34:56 |
| `"hh:mm"` | 12:34 |
| `"yyyy-MM-dd"` | 2024-01-15 |

**使用场景：** 时钟显示、计数器、倒计时、仪表盘数值显示

---

### 3.32 ElaPromotionCard（促销卡片）

**头文件：** `#include "ElaPromotionCard.h"`

带图片和多层文字的促销/推广卡片组件。

**创建：**

```cpp
ElaPromotionCard* card = new ElaPromotionCard(this);
card->setFixedSize(600, 300);                                    // 固定尺寸
card->setCardPixmap(QPixmap(":/image.png"));                    // 设置图片
card->setCardTitle("标签");                                      // 小标签
card->setPromotionTitle("促销大字");                             // 促销标题
card->setTitle("主标题");                                        // 主标题
card->setSubTitle("描述文字，可以是较长的说明...");              // 副标题
```

**布局结构：**

```
┌───────────────────────────────────────────────────────────┐
│  ┌─────────────┐                                          │
│  │             │   CardTitle: "标签"       ← 小标签       │
│  │    图片      │                                          │
│  │   (Pixmap)  │   PromotionTitle: "促销"  ← 促销大字     │
│  │             │                                          │
│  │             │   Title: "主标题"         ← 主标题       │
│  │             │                                          │
│  └─────────────┘   SubTitle: "描述..."     ← 副标题       │
└───────────────────────────────────────────────────────────┘
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setCardPixmap(QPixmap)` | 设置卡片左侧图片 |
| `setCardTitle(QString)` | 设置小标签 |
| `setPromotionTitle(QString)` | 设置促销标题（醒目大字） |
| `setTitle(QString)` | 设置主标题 |
| `setSubTitle(QString)` | 设置副标题/描述 |
| `setHorizontalCardPixmapRatio(qreal)` | 设置图片水平占比 |
| `setVerticalCardPixmapRatio(qreal)` | 设置图片垂直占比 |

**使用场景：** 产品展示、广告卡片、文章预览卡片、应用推荐

---

### 3.33 ElaPromotionView（轮播视图）

**头文件：** `#include "ElaPromotionView.h"`

自动轮播多个 ElaPromotionCard 的容器组件。

**创建：**

```cpp
ElaPromotionView* carousel = new ElaPromotionView(this);

// 创建多个卡片
ElaPromotionCard* card1 = new ElaPromotionCard(this);
card1->setCardPixmap(QPixmap(":/slide1.png"));
card1->setCardTitle("Slide 1");
card1->setTitle("第一张幻灯片");

ElaPromotionCard* card2 = new ElaPromotionCard(this);
card2->setCardPixmap(QPixmap(":/slide2.png"));
card2->setCardTitle("Slide 2");
card2->setTitle("第二张幻灯片");

// 添加到轮播视图
carousel->appendPromotionCard(card1);
carousel->appendPromotionCard(card2);

// 开启自动滚动
carousel->setIsAutoScroll(true);
```

**轮播效果：**

```
时刻 0：                          时刻 1（自动滚动后）：
┌─────────────────────────┐      ┌─────────────────────────┐
│  [Card1]  Card2  Card3  │  →   │  Card1  [Card2]  Card3  │
│     ●       ○      ○    │      │    ○      ●       ○     │
└─────────────────────────┘      └─────────────────────────┘
        当前显示                        自动切换到下一个
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `appendPromotionCard(card)` | 添加卡片到轮播 |
| `setIsAutoScroll(true)` | 开启自动轮播 |
| `setAutoScrollInterval(3000)` | 设置轮播间隔（毫秒） |
| `setCurrentIndex(int)` | 手动切换到指定卡片 |
| `getCurrentIndex()` | 获取当前显示的卡片索引 |

**完整示例：**

```cpp
#include "ElaPromotionView.h"
#include "ElaPromotionCard.h"

// 创建轮播视图
ElaPromotionView* carousel = new ElaPromotionView(this);

// 批量创建卡片
QStringList images = {":/img1.png", ":/img2.png", ":/img3.png", ":/img4.png"};
QStringList titles = {"产品A", "产品B", "产品C", "产品D"};

for (int i = 0; i < images.size(); i++) {
    ElaPromotionCard* card = new ElaPromotionCard(this);
    card->setCardPixmap(QPixmap(images[i]));
    card->setCardTitle(titles[i]);
    card->setPromotionTitle("热销");
    card->setTitle(QString("商品 %1").arg(i + 1));
    card->setSubTitle("限时优惠，点击查看详情");
    carousel->appendPromotionCard(card);
}

// 开启自动轮播，间隔 3 秒
carousel->setIsAutoScroll(true);
carousel->setAutoScrollInterval(3000);
```

**使用场景：** 首页轮播图、产品推荐、新闻轮播、广告展示

---

### 3.34 T_Card 组件组合示例

T_Card 页面展示了三个卡片组件的组合使用：

```cpp
#include "ElaLCDNumber.h"
#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"

// 1. LCD 时钟
ElaLCDNumber* lcd = new ElaLCDNumber(this);
lcd->setIsUseAutoClock(true);
lcd->setFixedHeight(100);

// 2. 单个促销卡片
ElaPromotionCard* card = new ElaPromotionCard(this);
card->setFixedSize(600, 300);
card->setCardPixmap(QPixmap(":/product.png"));
card->setPromotionTitle("限时特惠");
card->setTitle("产品名称");
card->setSubTitle("产品描述...");

// 3. 轮播视图
ElaPromotionView* carousel = new ElaPromotionView(this);
for (int i = 0; i < 4; i++) {
    ElaPromotionCard* slideCard = new ElaPromotionCard(this);
    slideCard->setCardPixmap(QPixmap(QString(":/slide%1.png").arg(i)));
    slideCard->setTitle(QString("Slide %1").arg(i + 1));
    carousel->appendPromotionCard(slideCard);
}
carousel->setIsAutoScroll(true);

// 布局
QVBoxLayout* layout = new QVBoxLayout(this);
layout->addWidget(lcd);
layout->addSpacing(20);
layout->addWidget(card);
layout->addSpacing(20);
layout->addWidget(carousel);
```

**整体布局效果：**

```
┌─────────────────────────────────────────────────────────┐
│ ┌─────────────────────────────────────────────────────┐ │
│ │           12:34:56  (ElaLCDNumber)                  │ │
│ └─────────────────────────────────────────────────────┘ │
│                     ↓ 20px 间距                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │  ┌────────┐  限时特惠                               │ │
│ │  │  图片   │  产品名称                               │ │
│ │  └────────┘  产品描述...  (ElaPromotionCard)        │ │
│ └─────────────────────────────────────────────────────┘ │
│                     ↓ 20px 间距                         │
│ ┌─────────────────────────────────────────────────────┐ │
│ │  ← [卡片1] [卡片2] [卡片3] [卡片4] →                 │ │
│ │        (ElaPromotionView 自动轮播)                   │ │
│ └─────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────┘
```

---

### 3.35 ElaScrollBar（浮动动画滚动条）

**头文件：** `#include "ElaScrollBar.h"`

Ela 风格的滚动条组件，支持浮动覆盖和滚动动画效果。

**两种创建方式：**

```cpp
// 方式 1：浮动模式 - 附加到现有滚动条上方（推荐）
ElaScrollBar* floatBar = new ElaScrollBar(
    listView->verticalScrollBar(),  // 目标滚动条
    listView                         // 父组件
);
floatBar->setIsAnimation(true);      // 开启滚动动画

// 方式 2：替换模式 - 完全替换原生滚动条
listView->setVerticalScrollBar(new ElaScrollBar(this));
```

**浮动 vs 替换的区别：**

```
浮动模式（两层叠加）：              替换模式（只有一层）：
┌─────────────────────┬──┐        ┌─────────────────────┬──┐
│                     │▓▓│        │                     │▓▓│
│  原生滚动条在下层    │▓▓│        │  完全替换原生滚动条  │▓▓│
│  ElaScrollBar 浮动   │▓▓│        │                     │▓▓│
│  在上层覆盖          │▓▓│        │                     │▓▓│
└─────────────────────┴──┘        └─────────────────────┴──┘
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setIsAnimation(true)` | 开启滚动动画效果 |
| `setIsHideByAlpha(true)` | 通过透明度隐藏（渐隐效果） |

**使用场景：** 美化列表/表格/文本框的滚动体验，添加平滑滚动动画

---

### 3.36 T_ListView - 列表视图示例

**头文件：** `#include "T_ListView.h"`

T_ListView 展示了 ElaListView + 自定义 Model + 浮动滚动条的组合使用。

#### 架构概览

```
┌─────────────────┐    setModel()   ┌─────────────────────┐
│   ElaListView   │ ←─────────────→ │   T_ListViewModel   │
│   (列表视图)     │                 │   (数据模型)         │
└────────┬────────┘                 └─────────────────────┘
         │ 附加
         ↓
┌─────────────────┐
│  ElaScrollBar   │
│ (浮动动画滚动条) │
└─────────────────┘
```

#### T_ListViewModel 数据模型

**继承自：** `QAbstractListModel`

**职责：** 为 ElaListView 提供文本列表数据

```cpp
class T_ListViewModel : public QAbstractListModel {
private:
    QStringList _dataList;  // 存储列表数据

protected:
    // 返回数据总行数
    int rowCount(const QModelIndex& parent) const override {
        return _dataList.count();
    }

    // 返回指定索引的数据
    QVariant data(const QModelIndex& index, int role) const override {
        if (role == Qt::DisplayRole) {       // 只响应显示角色
            return _dataList[index.row()];   // 返回对应行的字符串
        }
        return QVariant();
    }
};
```

**与 T_IconModel 的对比：**

| 对比项 | T_ListViewModel | T_IconModel |
|--------|-----------------|-------------|
| 数据角色 | `Qt::DisplayRole`（标准文本） | `Qt::UserRole`（自定义数据） |
| 数据格式 | `QString`（单个字符串） | `QStringList`（枚举名 + 图标字符） |
| 搜索功能 | 无 | 有（beginResetModel/endResetModel） |
| Delegate | 不需要（默认绘制即可） | 需要自定义（T_IconDelegate） |

#### 完整构造流程

```cpp
// 1. 创建标题文本
ElaText* listText = new ElaText("ElaListView", this);
listText->setTextPixelSize(18);

// 2. 创建列表视图并绑定数据模型
_listView = new ElaListView(this);
_listView->setFixedHeight(450);
_listView->setModel(new T_ListViewModel(this));  // MVC 绑定

// 3. 创建浮动滚动条（关键技巧）
ElaScrollBar* floatScrollBar = new ElaScrollBar(
    _listView->verticalScrollBar(), _listView);
floatScrollBar->setIsAnimation(true);

// 4. 布局：右边距 10px 为滚动条预留空间
QHBoxLayout* listViewLayout = new QHBoxLayout();
listViewLayout->setContentsMargins(0, 0, 10, 0);
listViewLayout->addWidget(_listView);
```

#### 布局结构

```
┌─────────────────────────────────────────────────────┐
│  "ElaListView"  (ElaText, 18px)                     │
├─────────────────────────────────────────────────────┤
│                    10px 间距                         │
├─────────────────────────────────────────────────────┤
│  ┌───────────────────────────────────────────┬───┐  │
│  │  最轻一个灵魂 送出玫瑰星光                │ ┃ │  │
│  │  最轻一句提问 答案是生命的存亡            │ ┃ │  │
│  │  最轻一颗麦种 由云帆托起                  │ ┃ │  │
│  │  ...                                      │ ┃ │  │
│  │                                           │ ┃ │  │
│  │  (ElaListView, 高度 450px)                │ ┃ │  │
│  └───────────────────────────────────────────┴───┘  │
│                                         右边距 10px  │
├─────────────────────────────────────────────────────┤
│                   stretch (弹性空间)                 │
└─────────────────────────────────────────────────────┘
```

#### 数据流

```
T_ListViewModel 构造时
    ↓
_dataList.append("歌词1")
_dataList.append("歌词2") × 40 行
    ↓
ElaListView 请求数据：
  rowCount() → 返回 40
  data(0, DisplayRole) → "最轻一个灵魂..."
  data(1, DisplayRole) → "最轻一句提问..."
  ...
    ↓
ElaListView 使用默认 Delegate 绘制文本
    ↓
ElaScrollBar 浮动在滚动条上方，提供动画效果
```

#### 核心知识点

| 概念 | 说明 |
|------|------|
| **Qt::DisplayRole** | 标准数据角色（值为 0），用于文本显示 |
| **浮动滚动条** | 通过传入目标 scrollBar 创建，覆盖在原滚动条上方 |
| **右边距预留** | `setContentsMargins(0, 0, 10, 0)` 为浮动滚动条留出空间 |
| **默认 Delegate** | 当只需显示文本时，无需自定义 Delegate |

---

### 3.37 ElaTableView（表格视图）

**头文件：** `#include "ElaTableView.h"`

Ela 风格的表格视图组件，用于显示二维表格数据。

**创建与配置：**

```cpp
#include "ElaTableView.h"
#include <QHeaderView>

ElaTableView* tableView = new ElaTableView(this);

// 绑定数据模型
tableView->setModel(new MyTableModel(this));

// 表头字体设置
QFont headerFont = tableView->horizontalHeader()->font();
headerFont.setPixelSize(16);
tableView->horizontalHeader()->setFont(headerFont);

// 常用配置
tableView->setAlternatingRowColors(true);                        // 交替行颜色
tableView->setIconSize(QSize(38, 38));                           // 图标尺寸
tableView->verticalHeader()->setHidden(true);                    // 隐藏行号
tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  // 整行选中
tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // 可拖动列宽
```

**表格结构：**

```
┌──────┬───────────────────────┬────────────┬──────────┬──────┐
│ 预览 │        歌名           │    歌手    │   专辑   │ 时长 │  ← 列表头
├──────┼───────────────────────┼────────────┼──────────┼──────┤
│ 🎵   │ 夜航星(Night Voyager) │ 不才/三体  │ 章北海传 │05:03 │  ← 奇数行
│ 🎵   │ 玫瑰少年              │ 五月天     │ 玫瑰少年 │03:55 │  ← 偶数行（交替色）
│ 🎵   │ Collapsing World      │ Lightscape │ ...      │03:10 │
└──────┴───────────────────────┴────────────┴──────────┴──────┘
  ↑ 行号被隐藏
```

**延迟设置列宽：**

```cpp
// 监听 tableViewShow 信号，确保布局完成后再设置列宽
connect(tableView, &ElaTableView::tableViewShow, this, [=]() {
    tableView->setColumnWidth(0, 60);   // 预览列
    tableView->setColumnWidth(1, 205);  // 歌名列
    tableView->setColumnWidth(2, 170);  // 歌手列
    tableView->setColumnWidth(3, 150);  // 专辑列
    tableView->setColumnWidth(4, 60);   // 时长列
});
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setModel(model)` | 绑定数据模型 |
| `setAlternatingRowColors(true)` | 奇偶行颜色交替 |
| `setIconSize(QSize)` | 设置图标显示尺寸 |
| `setSelectionBehavior(SelectRows)` | 整行选中模式 |
| `setColumnWidth(col, width)` | 设置列宽 |
| `horizontalHeader()` | 获取列表头对象 |
| `verticalHeader()` | 获取行表头对象 |

**QHeaderView 常用配置：**

| 方法 | 作用 |
|------|------|
| `setHidden(true)` | 隐藏表头 |
| `setFont(font)` | 设置表头字体 |
| `setSectionResizeMode(Interactive)` | 允许拖动调整 |
| `setMinimumSectionSize(60)` | 设置最小宽度/高度 |

**使用场景：** 音乐列表、文件管理、数据报表、商品列表

---

### 3.38 QAbstractTableModel（表格数据模型）

**头文件：** `#include <QAbstractTableModel>`

表格数据模型基类，用于为 ElaTableView/QTableView 提供二维数据。

#### ListView vs TableView 模型对比

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    QAbstractListModel (一维)                            │
├─────────────────────────────────────────────────────────────────────────┤
│  必须重写：rowCount(), data()                                            │
└─────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────┐
│                    QAbstractTableModel (二维)                           │
├─────────────────────────────────────────────────────────────────────────┤
│  必须重写：rowCount(), columnCount(), data(), headerData()              │
└─────────────────────────────────────────────────────────────────────────┘
```

#### 类定义示例

```cpp
class T_TableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit T_TableViewModel(QObject* parent = nullptr);
    ~T_TableViewModel();

    // 必须重写的 4 个方法
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList _header;            // 表头数据
    QList<QStringList> _dataList;   // 二维数据
    QList<QIcon> _iconList;         // 图标列表
};
```

#### 数据存储结构

```cpp
// 表头
_header << "预览" << "歌名" << "歌手" << "专辑" << "时长";

// 行数据（每行是一个 QStringList）
QStringList row0;
row0 << "夜航星" << "不才/三体" << "章北海传" << "05:03";
_dataList.append(row0);

// 图标（缩放到指定尺寸）
_iconList.append(QIcon(QPixmap(":/image.jpg")
    .scaled(38, 38, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
```

**数据结构示意：**

```
_header = ["预览", "歌名", "歌手", "专辑", "时长"]
              0       1       2       3       4     ← 列索引

_dataList = [
    ["夜航星", "不才", "章北海传", "05:03"],  ← 行 0（注意：不含图标）
    ["玫瑰少年", "五月天", "玫瑰少年", "03:55"], ← 行 1
    ...
]

_iconList = [🎵, 🎵, 🎵, ...]  ← 与 _dataList 一一对应
```

#### 核心方法实现

**rowCount() - 返回行数：**

```cpp
int T_TableViewModel::rowCount(const QModelIndex& parent) const
{
    return 100;  // 可以返回固定值，数据循环显示
}
```

**columnCount() - 返回列数：**

```cpp
int T_TableViewModel::columnCount(const QModelIndex& parent) const
{
    return _header.count();  // 返回 5
}
```

**data() - 返回单元格数据：**

```cpp
QVariant T_TableViewModel::data(const QModelIndex& index, int role) const
{
    // 1. 文本显示（列 1-4）
    if (role == Qt::DisplayRole && index.column() != 0) {
        return _dataList[index.row() % 9][index.column() - 1];
    }
    // 2. 图标显示（列 0）
    else if (role == Qt::DecorationRole && index.column() == 0) {
        return _iconList[index.row() % 9];
    }
    // 3. 图标居中
    else if (role == Qt::DecorationPropertyRole) {
        return Qt::AlignCenter;
    }
    // 4. 时长列文本居中
    else if (role == Qt::TextAlignmentRole && index.column() == 4) {
        return Qt::AlignCenter;
    }
    return QVariant();
}
```

**headerData() - 返回表头数据：**

```cpp
QVariant T_TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return _header[section];  // 返回对应列的表头文字
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
```

#### 数据角色详解

| 角色 | 值 | 用途 | 典型使用 |
|------|-----|------|---------|
| `Qt::DisplayRole` | 0 | 文本显示 | 歌名、歌手、专辑、时长 |
| `Qt::DecorationRole` | 1 | 图标/图片 | 预览列的专辑封面 |
| `Qt::TextAlignmentRole` | 7 | 文本对齐 | 时长列居中 |
| `Qt::DecorationPropertyRole` | - | 图标属性 | 图标居中对齐 |

#### 数据索引映射技巧

```cpp
// 循环复用数据：row % 9 使 100 行循环显示 9 条数据
_dataList[index.row() % 9]

// 列索引偏移：_dataList 不含图标列，所以 column - 1
_dataList[row][index.column() - 1]

// 完整映射：
// 表格 index(5, 2) → _dataList[5 % 9][2 - 1] → _dataList[5][1] → 第 6 首歌的歌手
```

---

### 3.39 T_TableView - 表格视图示例

T_TableView 展示了 ElaTableView + QAbstractTableModel 的完整组合使用。

#### 架构概览

```
┌─────────────────┐    setModel()   ┌─────────────────────┐
│  ElaTableView   │ ←─────────────→ │  T_TableViewModel   │
│   (表格视图)     │                 │   (表格数据模型)     │
└─────────────────┘                 └─────────────────────┘
        │                                    │
        │ 请求                               │ 返回
        ↓                                    ↓
┌─────────────────────────────────────────────────────────┐
│  columnCount() → 5                                       │
│  headerData(0~4) → ["预览", "歌名", "歌手", "专辑", "时长"]│
│  rowCount() → 100                                        │
│  data(row, col) → 根据角色返回文本/图标/对齐方式          │
└─────────────────────────────────────────────────────────┘
```

#### 完整数据流

```
ElaTableView 初始化
    ↓
调用 Model 方法获取结构：
  columnCount() → 5 列
  rowCount() → 100 行
  headerData(0~4) → 表头文字
    ↓
渲染每个单元格，调用 data(index, role)：
  ┌─────────────────────────────────────────────────────────────┐
  │ (row, 0) + DecorationRole → _iconList[row % 9]  → 显示图标  │
  │ (row, 1~4) + DisplayRole → _dataList[row%9][col-1] → 显示文本│
  │ (row, 4) + TextAlignmentRole → Qt::AlignCenter → 文本居中   │
  └─────────────────────────────────────────────────────────────┘
    ↓
tableViewShow 信号触发 → 设置各列宽度
    ↓
渲染完成
```

#### 核心知识点

| 概念 | 说明 |
|------|------|
| **QAbstractTableModel** | 表格模型基类，需重写 4 个方法 |
| **columnCount()** | 返回列数（ListView 不需要） |
| **headerData()** | 返回表头数据（ListView 不需要） |
| **Qt::DecorationRole** | 用于显示图标 |
| **Qt::TextAlignmentRole** | 控制文本对齐 |
| **tableViewShow 信号** | 延迟设置列宽的时机 |
| **row % N** | 数据循环复用技巧 |
| **QHeaderView** | 控制表头样式 |

#### 与 ListView 对比

| 对比项 | ListView | TableView |
|--------|----------|-----------|
| 数据维度 | 一维（只有行） | 二维（行 + 列） |
| 基类 | `QAbstractListModel` | `QAbstractTableModel` |
| 必须重写 | `rowCount()`, `data()` | + `columnCount()`, `headerData()` |
| 数据存储 | `QStringList` | `QList<QStringList>` |
| 表头 | 无 | 有（行表头 + 列表头） |
| 典型用途 | 简单列表、图标网格 | 数据表格、文件列表 |

---

### 3.40 ElaTreeView（树形视图）

**头文件：** `#include "ElaTreeView.h"`

Ela 风格的树形视图组件，用于显示层级结构数据（如文件树、目录结构）。

**创建与配置：**

```cpp
#include "ElaTreeView.h"
#include "ElaScrollBar.h"

ElaTreeView* treeView = new ElaTreeView(this);

// 浮动滚动条
ElaScrollBar* scrollBar = new ElaScrollBar(treeView->verticalScrollBar(), treeView);
scrollBar->setIsAnimation(true);

// 表头字体
QFont headerFont = treeView->header()->font();
headerFont.setPixelSize(16);
treeView->header()->setFont(headerFont);

// 绑定模型
treeView->setModel(new T_TreeViewModel(this));
treeView->setFixedHeight(450);
```

**树形结构示意：**

```
ElaTreeView-Example-4Level    (表头)
├─────────────────────────────────────
│ ▶ ☐ 🎵 Lv1--TreeItem1
│ ▼ ▣ 🎵 Lv1--TreeItem2        ← 展开状态，部分勾选
│   ├─ ▶ ☑ 🎵 Lv2--TreeItem1   ← 全部勾选
│   ├─ ▶ ☐ 🎵 Lv2--TreeItem2   ← 未勾选
│   └─ ...
│ ▶ ☐ 🎵 Lv1--TreeItem3
│ ...
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setModel(model)` | 绑定树形数据模型 |
| `expandAll()` | 展开所有节点 |
| `collapseAll()` | 收起所有节点 |
| `setItemHeight(height)` | 设置行高 |
| `setHeaderMargin(margin)` | 设置表头边距 |
| `setIndentation(width)` | 设置层级缩进宽度 |

**缩进效果示意：**

```
Indentation = 20:                 Indentation = 40:
├─ Item1                          ├─ Item1
│  ├─ SubItem1                    │    ├─ SubItem1
│  └─ SubItem2                    │    └─ SubItem2
└─ Item2                          └─ Item2
```

---

### 3.41 T_TreeItem（树节点数据类）

**头文件：** `#include "T_TreeItem.h"`

树节点数据类，用于存储单个节点的数据和父子关系。

#### 类定义

```cpp
class T_TreeItem : public QObject
{
    Q_OBJECT
public:
    explicit T_TreeItem(QString itemTitle, T_TreeItem* parent = nullptr);
    ~T_TreeItem();

    QString getItemKey() const;       // 获取唯一标识 (UUID)
    QString getItemTitle() const;     // 获取显示标题

    void appendChildItem(T_TreeItem* childItem);  // 添加子节点
    bool getIsHasChild() const;                   // 是否有子节点
    int getRow() const;                           // 在父节点中的行索引

    // 复选框相关
    void setChildChecked(bool isChecked);     // 递归设置子节点勾选
    Qt::CheckState getChildCheckState();      // 获取三态复选状态

private:
    QString _itemKey;              // 唯一标识
    QString _itemTitle;            // 显示标题
    bool _pIsChecked;              // 是否勾选
    T_TreeItem* _pParentItem;      // 父节点指针
    QList<T_TreeItem*> _pChildrenItems;  // 子节点列表
};
```

#### 数据结构

```
                    ┌─────────────────┐
                    │   T_TreeItem    │
                    ├─────────────────┤
                    │ _itemKey (UUID) │
                    │ _itemTitle      │
                    │ _isChecked      │
                    │ _parentItem ────┼──→ 指向父节点
                    │ _childrenItems ─┼──→ [子节点1, 子节点2, ...]
                    └─────────────────┘
```

#### 构造函数

```cpp
T_TreeItem::T_TreeItem(QString itemTitle, T_TreeItem* parent)
    : QObject{parent}
{
    // 生成唯一标识
    _itemKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _itemTitle = itemTitle;
    _pParentItem = parent;
    _pIsChecked = false;
}
```

#### 三态复选框

```
┌─────────────────────────────────────────────────────────────┐
│  状态                  │  外观  │  含义                      │
├─────────────────────────────────────────────────────────────┤
│  Qt::Unchecked         │  ☐   │  未勾选（子节点全部未选）    │
│  Qt::PartiallyChecked  │  ▣   │  部分勾选（子节点部分选中）  │
│  Qt::Checked           │  ☑   │  完全勾选（子节点全部选中）  │
└─────────────────────────────────────────────────────────────┘

示例：
    ▣ 父节点 (PartiallyChecked)
      ├── ☑ 子节点1 (Checked)
      │     ├── ☑ 孙节点1
      │     └── ☑ 孙节点2
      └── ☐ 子节点2 (Unchecked)
            └── ☐ 孙节点3
```

#### getChildCheckState() - 获取三态

```cpp
Qt::CheckState T_TreeItem::getChildCheckState()
{
    bool isAllChecked = true;
    bool isAnyChecked = false;

    for (auto node : _pChildrenItems) {
        if (node->getIsChecked()) {
            isAnyChecked = true;
        } else {
            isAllChecked = false;
        }
        // 递归检查子节点状态
        Qt::CheckState childState = node->getChildCheckState();
        if (childState == Qt::PartiallyChecked) {
            isAllChecked = false;
            isAnyChecked = true;
            break;
        }
    }

    if (_pChildrenItems.count() > 0) {
        if (isAllChecked) return Qt::Checked;
        if (isAnyChecked) return Qt::PartiallyChecked;
        return Qt::Unchecked;
    }
    return Qt::Checked;
}
```

#### setChildChecked() - 递归勾选

```cpp
void T_TreeItem::setChildChecked(bool isChecked)
{
    for (auto node : _pChildrenItems) {
        node->setIsChecked(isChecked);
        node->setChildChecked(isChecked);  // 递归
    }
}
```

---

### 3.42 QAbstractItemModel（树形数据模型基类）

**头文件：** `#include <QAbstractItemModel>`

树形数据模型基类，比 TableModel 更复杂，需要额外处理父子关系。

#### 三种模型对比

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    QAbstractListModel (一维)                            │
├─────────────────────────────────────────────────────────────────────────┤
│  必须重写：rowCount(), data()                                            │
└─────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────┐
│                    QAbstractTableModel (二维)                           │
├─────────────────────────────────────────────────────────────────────────┤
│  必须重写：rowCount(), columnCount(), data(), headerData()              │
└─────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────┐
│                    QAbstractItemModel (树形)                            │
├─────────────────────────────────────────────────────────────────────────┤
│  必须重写：rowCount(), columnCount(), data(), headerData()              │
│           + parent(), index()  ⭐ 新增                                  │
│  可选重写：setData(), flags()  (支持编辑/勾选)                           │
└─────────────────────────────────────────────────────────────────────────┘
```

#### T_TreeViewModel 类定义

```cpp
class T_TreeViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    // 必须重写的 6 个方法
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // 可选重写（支持勾选）
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    T_TreeItem* _rootItem{nullptr};         // 根节点（不可见）
    QMap<QString, T_TreeItem*> _itemsMap;   // 快速查找表
};
```

#### 构造函数 - 构建 4 层树

```cpp
T_TreeViewModel::T_TreeViewModel(QObject* parent)
    : QAbstractItemModel{parent}
{
    _rootItem = new T_TreeItem("root");  // 不可见的根节点

    for (int i = 0; i < 20; i++) {  // 第 1 层：20 个
        T_TreeItem* lv1 = new T_TreeItem(QString("Lv1--TreeItem%1").arg(i+1), _rootItem);

        for (int j = 0; j < 6; j++) {  // 第 2 层：每个父节点 6 个
            T_TreeItem* lv2 = new T_TreeItem(QString("Lv2--TreeItem%1").arg(j+1), lv1);

            for (int k = 0; k < 6; k++) {  // 第 3 层
                T_TreeItem* lv3 = new T_TreeItem(QString("Lv3--TreeItem%1").arg(k+1), lv2);

                for (int l = 0; l < 6; l++) {  // 第 4 层
                    T_TreeItem* lv4 = new T_TreeItem(QString("Lv4--TreeItem%1").arg(l+1), lv3);
                    lv3->appendChildItem(lv4);
                }
                lv2->appendChildItem(lv3);
            }
            lv1->appendChildItem(lv2);
        }
        _rootItem->appendChildItem(lv1);
    }
}
// 总节点数 = 20 + 120 + 720 + 4320 = 5180
```

**树结构示意：**

```
_rootItem (不可见)
    ├── Lv1--TreeItem1
    │     ├── Lv2--TreeItem1
    │     │     ├── Lv3--TreeItem1
    │     │     │     ├── Lv4--TreeItem1
    │     │     │     └── ... (共 6 个)
    │     │     └── ... (共 6 个)
    │     └── ... (共 6 个)
    └── ... (共 20 个)
```

#### parent() - 返回父节点索引 ⭐

```cpp
QModelIndex T_TreeViewModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    T_TreeItem* childItem = static_cast<T_TreeItem*>(child.internalPointer());
    T_TreeItem* parentItem = childItem->getParentItem();

    if (parentItem == _rootItem || parentItem == nullptr)
        return QModelIndex();  // 根节点返回无效索引

    return createIndex(parentItem->getRow(), 0, parentItem);
}
```

#### index() - 创建节点索引 ⭐

```cpp
QModelIndex T_TreeViewModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    T_TreeItem* parentItem;
    if (!parent.isValid()) {
        parentItem = _rootItem;  // 无效索引 = 根节点
    } else {
        parentItem = static_cast<T_TreeItem*>(parent.internalPointer());
    }

    T_TreeItem* childItem = parentItem->getChildrenItems().at(row);
    if (childItem) {
        return createIndex(row, column, childItem);  // 存储指针到索引
    }
    return QModelIndex();
}
```

**index() 与 parent() 的关系：**

```
index(row, col, parent) → 根据父索引和行号，创建子节点索引
parent(child)           → 根据子节点索引，返回父节点索引

两个方法互为逆操作，TreeView 通过它们遍历整棵树
```

#### rowCount() - 返回子节点数量

```cpp
int T_TreeViewModel::rowCount(const QModelIndex& parent) const
{
    T_TreeItem* parentItem;
    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<T_TreeItem*>(parent.internalPointer());
    }
    return parentItem->getChildrenItems().count();
}
```

**注意：** TreeView 的 rowCount 是**相对于父节点**的子节点数量

#### data() - 返回节点数据

```cpp
QVariant T_TreeViewModel::data(const QModelIndex& index, int role) const
{
    T_TreeItem* item = static_cast<T_TreeItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->getItemTitle();  // 显示标题
    }
    else if (role == Qt::DecorationRole) {
        return QIcon(":/Resource/Image/Cirno.jpg");  // 图标
    }
    else if (role == Qt::CheckStateRole) {
        if (item->getIsHasChild()) {
            return item->getChildCheckState();  // 三态
        } else {
            return item->getIsChecked() ? Qt::Checked : Qt::Unchecked;
        }
    }
    return QVariant();
}
```

#### setData() - 处理勾选

```cpp
bool T_TreeViewModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role == Qt::CheckStateRole) {
        T_TreeItem* item = static_cast<T_TreeItem*>(index.internalPointer());
        item->setIsChecked(!item->getIsChecked());
        item->setChildChecked(item->getIsChecked());  // 递归设置子节点
        emit dataChanged(QModelIndex(), QModelIndex(), {role});
        return true;
    }
    return QAbstractItemModel::setData(index, value, role);
}
```

#### flags() - 启用复选框

```cpp
Qt::ItemFlags T_TreeViewModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsUserCheckable;  // 添加可勾选标志
    return flags;
}
```

---

### 3.43 T_TreeView - 树形视图示例

T_TreeView 展示了 ElaTreeView + T_TreeItem + T_TreeViewModel 的完整组合使用。

#### 架构概览

```
┌─────────────────┐         ┌─────────────────────┐
│   ElaTreeView   │ ←─────→ │   T_TreeViewModel   │
│   (树形视图)     │         │   (树形数据模型)     │
└─────────────────┘         └──────────┬──────────┘
                                       │ 管理
                                       ↓
                            ┌─────────────────────┐
                            │     T_TreeItem      │
                            │   (树节点数据类)     │
                            └──────────┬──────────┘
                                       │ 递归包含
                                       ↓
                            ┌─────────────────────┐
                            │   子节点 (递归)      │
                            └─────────────────────┘
```

#### 完整数据流

```
ElaTreeView 初始化
    ↓
rowCount(QModelIndex()) → _rootItem 子节点数 → 20
    ↓
index(0~19, 0, QModelIndex()) → 创建第一层节点索引
    ↓
data(index, DisplayRole) → "Lv1--TreeItem1"
data(index, DecorationRole) → QIcon(...)
data(index, CheckStateRole) → Qt::Unchecked
    ↓
用户展开节点时：
  rowCount(expandedIndex) → 获取子节点数量
  index(row, 0, expandedIndex) → 创建子节点索引
    ↓
用户勾选节点时：
  setData(index, value, CheckStateRole)
    → item->setIsChecked(!isChecked)
    → item->setChildChecked(...)  // 递归
    → emit dataChanged(...)       // 刷新视图
```

#### 布局结构

```
┌─────────────────────────────────────────────────────────────────────────────┐
│  "ElaTreeView"  (ElaText, 18px)                                             │
├─────────────────────────────────────────────────────────────────────────────┤
│                                  10px 间距                                   │
├────────────────────────────┬────────────────────────────────────────────────┤
│   设置面板                  │   ElaTreeView                                  │
│  ┌────────────────────┐    │  ┌────────────────────────────────────────┐   │
│  │ 数据条数：5180      │    │  │ ElaTreeView-Example-4Level    (表头)   │   │
│  ├────────────────────┤    │  ├────────────────────────────────────────┤   │
│  │ ItemHeight ════○══ │    │  │ ▶ ☐ 🎵 Lv1--TreeItem1                 │   │
│  ├────────────────────┤    │  │ ▼ ▣ 🎵 Lv1--TreeItem2                 │   │
│  │ HeaderMargin ══○══ │    │  │   ├─ ▶ ☑ 🎵 Lv2--TreeItem1            │   │
│  ├────────────────────┤    │  │   ├─ ▶ ☐ 🎵 Lv2--TreeItem2            │   │
│  │ Indentation ═══○══ │    │  │   └─ ...                               │   │
│  ├────────────────────┤    │  │ ▶ ☐ 🎵 Lv1--TreeItem3                 │   │
│  │ [展开全部][收起全部] │    │  │ ...                                    │   │
│  └────────────────────┘    │  └────────────────────────────────────────┘   │
└────────────────────────────┴────────────────────────────────────────────────┘
```

#### 核心知识点

| 概念 | 说明 |
|------|------|
| **QAbstractItemModel** | 树形模型基类，多 `parent()` 和 `index()` |
| **T_TreeItem** | 树节点类，存储父子关系和勾选状态 |
| **internalPointer()** | QModelIndex 存储的自定义指针 |
| **createIndex()** | 创建包含指针的 QModelIndex |
| **parent()** | 返回父节点索引，根返回无效索引 |
| **index()** | 根据父索引和行号创建子节点索引 |
| **Qt::CheckStateRole** | 复选框状态角色 |
| **三态复选框** | Unchecked / PartiallyChecked / Checked |
| **setChildChecked()** | 递归设置子节点勾选 |
| **expandAll() / collapseAll()** | 展开/收起所有节点 |

#### 三种 View 模型对比

| 对比项 | ListView | TableView | TreeView |
|--------|----------|-----------|----------|
| 基类 | `QAbstractListModel` | `QAbstractTableModel` | `QAbstractItemModel` |
| 数据维度 | 一维 | 二维 | 层级 |
| 必须重写 | 2 个方法 | 4 个方法 | 6 个方法 |
| parent() | ✗ | ✗ | ✓ |
| index() | ✗ | ✗ | ✓ |
| 数据类 | 不需要 | 不需要 | 需要（T_TreeItem） |
| 复选框 | 可选 | 可选 | 常用（三态） |
| 典型用途 | 简单列表 | 数据表格 | 文件树、目录结构 |

---

## 四、核心概念总结

### 4.1 信号槽机制 (Signal & Slot)

Qt 最核心的通信机制：

```cpp
connect(发送者, &类名::信号, 接收者, Lambda或槽函数);
```

```
┌──────────┐   信号   ┌──────────┐
│  发送者   │ ───────► │  接收者   │
│ (按钮等)  │          │ (处理函数) │
└──────────┘          └──────────┘
```

### 4.2 Lambda 表达式

```cpp
// 无捕获，无参数
[]() { qDebug() << "hello"; }

// 捕获 this
[this]() { this->doSomething(); }

// 捕获多个变量
[input, text, this]() { text->setText(input->text()); }

// 带参数
[](const QString& text) { qDebug() << text; }
```

**捕获规则：Lambda 内部用到外部的哪个变量，就在 `[]` 中写哪个。**

### 4.3 头文件引入规则

```
用什么组件 → 引入对应头文件
组件名和头文件名一致
```

| 组件                    | 头文件                              |
| ----------------------- | ----------------------------------- |
| ElaPushButton           | `#include "ElaPushButton.h"`        |
| ElaText                 | `#include "ElaText.h"`              |
| ElaLineEdit             | `#include "ElaLineEdit.h"`          |
| ElaMessageBar           | `#include "ElaMessageBar.h"`        |
| ElaCheckBox             | `#include "ElaCheckBox.h"`          |
| ElaComboBox             | `#include "ElaComboBox.h"`          |
| ElaRadioButton          | `#include "ElaRadioButton.h"`       |
| ElaSlider               | `#include "ElaSlider.h"`            |
| ElaProgressBar          | `#include "ElaProgressBar.h"`       |
| ElaToggleSwitch         | `#include "ElaToggleSwitch.h"`      |
| ElaToggleButton         | `#include "ElaToggleButton.h"`      |
| ElaSpinBox              | `#include "ElaSpinBox.h"`           |
| ElaDoubleSpinBox        | `#include "ElaDoubleSpinBox.h"`     |
| ElaProgressRing         | `#include "ElaProgressRing.h"`      |
| ElaMultiSelectComboBox  | `#include "ElaMultiSelectComboBox.h"` |
| ElaMessageButton        | `#include "ElaMessageButton.h"`     |
| ElaPlainTextEdit        | `#include "ElaPlainTextEdit.h"`     |
| ElaIconButton           | `#include "ElaIconButton.h"`        |
| ElaScrollPageArea       | `#include "ElaScrollPageArea.h"`    |
| ElaToolButton           | `#include "ElaToolButton.h"`        |
| ElaMenu                 | `#include "ElaMenu.h"`              |
| ElaImageCard            | `#include "ElaImageCard.h"`         |
| ElaAcrylicUrlCard       | `#include "ElaAcrylicUrlCard.h"`    |
| ElaPopularCard          | `#include "ElaPopularCard.h"`       |
| ElaFlowLayout           | `#include "ElaFlowLayout.h"`        |
| ElaScrollArea           | `#include "ElaScrollArea.h"`        |
| ElaToolTip              | `#include "ElaToolTip.h"`           |
| ElaNavigationRouter     | `#include "ElaNavigationRouter.h"`  |
| ...                     | 以此类推                            |

---

## 五、编译与打包

### 5.1 使用 Qt Creator 编译 ElaWidgetTools

#### 编译器选择

| 编译器 | 推荐程度 | 说明 |
|--------|----------|------|
| **MSVC** | ⭐⭐⭐ 推荐 | Windows 原生支持更好，DirectX 兼容性最佳 |
| MinGW64 | ⭐⭐ 可用 | CMake 中有支持，但可能遇到边缘问题 |

**注意：Qt 版本必须与编译器匹配**

| 编译器 | 需要的 Qt 版本 |
|--------|---------------|
| MSVC 2019 | `msvc2019_64` |
| MinGW 64-bit | `mingw_64` |

#### 编译步骤

1. **打开项目**：`文件` → `打开文件或项目` → 选择根目录的 `CMakeLists.txt`

2. **配置 Kit**：选择对应的编译套件
   - MSVC: `Desktop Qt 5.15.2 MSVC2019 64bit`
   - MinGW: `Desktop Qt 5.15.2 MinGW 64-bit`

3. **修改 Qt 路径**：在 `CMakeLists.txt` 中修改为你的实际路径：
   ```cmake
   SET(QT_SDK_DIR "D:/Qt/5.15.2/msvc2019_64" CACHE PATH "QT SDK DIR" FORCE)
   ```

4. **构建**：点击构建按钮或按 `Ctrl+B`

#### 编译产物位置

编译完成后，产物在构建目录的 `ElaWidgetTools` 子文件夹中：

```
<构建目录>/ElaWidgetTools/
├── ElaWidgetTools.dll    ← 动态链接库（运行时需要）
├── ElaWidgetTools.lib    ← 导入库（编译链接时需要）
└── ElaWidgetTools.exp    ← 导出文件（链接器中间产物）
```

Qt Creator 构建目录通常格式为：
```
项目目录/build/Desktop_Qt_5_15_0_MSVC2019_64bit-Release/
```

---

### 5.2 打包 SDK 供其他项目使用

#### SDK 目录结构

```
SDK/
├── include/          ← 头文件（72个）
│   ├── ElaWindow.h
│   ├── ElaWidget.h
│   └── ...
├── lib/
│   └── ElaWidgetTools.lib
└── bin/
    └── ElaWidgetTools.dll
```

#### 打包步骤

1. **创建目录结构**：
   ```bash
   mkdir SDK/include SDK/lib SDK/bin
   ```

2. **复制头文件**：
   ```bash
   cp ElaWidgetTools/*.h SDK/include/
   ```

3. **复制库文件**：
   ```bash
   cp <构建目录>/ElaWidgetTools/ElaWidgetTools.lib SDK/lib/
   cp <构建目录>/ElaWidgetTools/ElaWidgetTools.dll SDK/bin/
   ```

---

### 5.3 在自己的项目中使用 SDK

#### CMake 配置示例

```cmake
cmake_minimum_required(VERSION 3.5)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Qt 配置
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# ElaWidgetTools SDK 路径（修改为你的实际路径）
set(ELA_SDK_DIR "E:/path/to/SDK")

add_executable(${PROJECT_NAME} main.cpp)

# 添加头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${ELA_SDK_DIR}/include
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    ${ELA_SDK_DIR}/lib/ElaWidgetTools.lib
)

# 编译后自动复制 DLL 到输出目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ELA_SDK_DIR}/bin/ElaWidgetTools.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
```

#### 运行时 DLL 配置

程序运行时需要找到 `ElaWidgetTools.dll`，有三种方式：

| 方式 | 操作 |
|------|------|
| **推荐** | 将 `ElaWidgetTools.dll` 复制到 `.exe` 同目录 |
| 方式二 | 将 `SDK/bin` 添加到系统 PATH 环境变量 |
| 方式三 | CMake 中添加 `add_custom_command` 自动复制 |

#### 代码使用示例

```cpp
#include <QApplication>
#include "ElaWindow.h"
#include "ElaApplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eApp->init();  // 初始化 Ela

    ElaWindow w;
    w.show();

    return a.exec();
}
```

---

### 5.4 文件说明

| 文件类型 | 用途 | 使用时机 |
|----------|------|----------|
| `.h` 头文件 | 类声明、接口定义 | 编译时 `#include` |
| `.lib` 导入库 | 链接符号信息 | 编译链接时 |
| `.dll` 动态库 | 实际代码实现 | 程序运行时 |

---

## 六、导航系统

### 6.1 导航节点类型

ElaWindow 提供了四种导航节点类型：

| 节点类型 | 方法 | 说明 |
|----------|------|------|
| **PageNode** | `addPageNode()` | 页面节点，点击后导航到对应页面 |
| **CategoryNode** | `addCategoryNode()` | 分类节点，仅作为标题分隔，不可点击 |
| **ExpanderNode** | `addExpanderNode()` | 展开节点，可折叠/展开的容器 |
| **FooterNode** | `addFooterNode()` | 页脚节点，显示在导航栏底部 |

**示例：**

```cpp
// 添加页面节点
addPageNode("HOME", _homePage, ElaIconType::House);

// 添加分类节点（不可点击的标题）
QString categoryKey;
addCategoryNode("Controls", categoryKey);

// 添加展开节点（可折叠容器）
QString expanderKey;
addExpanderNode("ElaView", expanderKey, ElaIconType::CameraViewfinder);

// 在展开节点下添加子页面
addPageNode("ElaListView", _listViewPage, expanderKey, 9, ElaIconType::List);

// 添加页脚节点
addFooterNode("Setting", _settingPage, _settingKey, 0, ElaIconType::GearComplex);
```

---

### 6.2 导航方法

#### 基本导航

```cpp
// 通过页面 Key 导航
navigation(_settingKey);

// 获取页面的 Key
QString pageKey = _homePage->property("ElaPageKey").toString();
navigation(pageKey);
```

#### 导航路由器（前进/后退）

```cpp
#include "ElaNavigationRouter.h"

// 后退
ElaNavigationRouter::getInstance()->navigationRouteBack();

// 前进
ElaNavigationRouter::getInstance()->navigationRouteForward();

// 监听路由状态变化
connect(ElaNavigationRouter::getInstance(),
        &ElaNavigationRouter::navigationRouterStateChanged,
        this, [=](ElaNavigationRouterType::RouteMode routeMode) {
    switch (routeMode) {
        case ElaNavigationRouterType::BackValid:
            // 可以后退
            break;
        case ElaNavigationRouterType::BackInvalid:
            // 不可后退
            break;
        case ElaNavigationRouterType::ForwardValid:
            // 可以前进
            break;
        case ElaNavigationRouterType::ForwardInvalid:
            // 不可前进
            break;
    }
});
```

---

### 6.3 导航回溯（backtrackNavigationNode）

**头文件：** `#include "ElaWindow.h"`

#### 作用

`backtrackNavigationNode()` 方法用于 **导航到指定页面并清除该页面之后的所有导航历史**。

#### 与普通导航的区别

| 方法 | 效果 |
|------|------|
| `navigation(key)` | 跳转到页面，**保留**导航历史 |
| `backtrackNavigationNode(key)` | 跳转到页面，**清除**该页面之后的历史 |

#### 场景说明

假设导航历史为：
```
Home → BaseComponents → ListView → TableView → TreeView
                                                  ↑
                                              当前位置
```

**使用 `navigation("ListView")` 后：**
```
Home → BaseComponents → ListView → TableView → TreeView → ListView
                                                              ↑
                                              （历史保留，追加新记录）
```

**使用 `backtrackNavigationNode("ListView")` 后：**
```
Home → BaseComponents → ListView
                           ↑
           （ListView 之后的历史被清除）
```

#### 使用方法

```cpp
#include "ElaWindow.h"

// 获取主窗口
ElaWindow* window = dynamic_cast<ElaWindow*>(this->window());
if (window) {
    // 获取当前页面的 Key
    QString pageKey = property("ElaPageKey").toString();

    // 回溯到当前页面（清除后续历史）
    window->backtrackNavigationNode(pageKey);
}
```

#### 完整示例（回溯按钮）

```cpp
#include "ElaToolButton.h"
#include "ElaWindow.h"

// 创建回溯按钮
ElaToolButton* backtrackBtn = new ElaToolButton(page);
backtrackBtn->setFixedSize(35, 35);
backtrackBtn->setIsTransparent(false);
backtrackBtn->setElaIcon(ElaIconType::Timer);

connect(backtrackBtn, &ElaToolButton::clicked, this, [=]() {
    // this->window() 获取当前控件所属的顶层窗口
    ElaWindow* window = dynamic_cast<ElaWindow*>(this->window());
    if (window) {
        // property("ElaPageKey") 获取页面注册时的唯一标识
        window->backtrackNavigationNode(property("ElaPageKey").toString());
    }
});
```

#### 适用场景

- 用户想"重新开始"某个操作流程
- 清理无效的导航路径
- 实现"重置到某个节点"的功能
- 类似浏览器的"回到书签并清除后续历史"

---

### 6.4 页面 Key 机制

每个页面在通过 `addPageNode()` 注册时，ElaWindow 会自动为其设置一个 `ElaPageKey` 属性作为唯一标识。

**获取页面 Key：**

```cpp
// 方式1：通过页面对象获取
QString key = _homePage->property("ElaPageKey").toString();

// 方式2：在页面类内部获取自己的 Key
QString myKey = property("ElaPageKey").toString();

// 方式3：addPageNode 时传入引用获取
QString pageKey;
addPageNode("PageName", page, pageKey, ElaIconType::Home);
// pageKey 现在包含生成的 Key
```

**使用页面 Key：**

```cpp
// 导航到指定页面
navigation(pageKey);

// 回溯到指定页面
backtrackNavigationNode(pageKey);
```

---

## 七、导航组件

### 3.44 ElaBreadcrumbBar（面包屑导航栏）

**头文件：** `#include "ElaBreadcrumbBar.h"`

面包屑导航栏，用于显示当前页面在层级结构中的位置路径，支持点击返回上级。

**基本用法：**

```cpp
// 创建面包屑导航栏
ElaBreadcrumbBar* breadcrumbBar = new ElaBreadcrumbBar(this);

// 设置面包屑列表
QStringList breadcrumbList;
breadcrumbList << "首页" << "分类" << "商品详情" << "评论";
breadcrumbBar->setBreadcrumbList(breadcrumbList);
```

**动态生成示例：**

```cpp
// 批量生成路径项
QStringList pathList;
for (int i = 0; i < 20; i++) {
    pathList << QString("Item%1").arg(i + 1);
}
breadcrumbBar->setBreadcrumbList(pathList);
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setBreadcrumbList(QStringList)` | 设置/重置面包屑路径列表 |

**显示效果：**

```
首页 > 分类 > 商品详情 > 评论
       ↑ 点击可返回到该层级
```

**使用场景：** 文件管理器路径、电商分类导航、多级菜单定位、后台管理系统层级导航

---

### 3.45 ElaPivot（枢轴导航）

**头文件：** `#include "ElaPivot.h"`

枢轴导航组件，类似于水平排列的选项卡标题，带有下划线滑动动画效果。常用于页面内的内容分类切换。

**基本用法：**

```cpp
// 创建枢轴导航
ElaPivot* pivot = new ElaPivot(this);

// 设置样式
pivot->setPivotSpacing(8);    // 项目之间的间距（像素）
pivot->setMarkWidth(75);       // 下划线标记的宽度（像素）

// 添加导航项
pivot->appendPivot("本地歌曲");
pivot->appendPivot("下载歌曲");
pivot->appendPivot("下载视频");
pivot->appendPivot("正在下载");

// 设置默认选中项（从 0 开始）
pivot->setCurrentIndex(0);
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `appendPivot(QString)` | 添加一个导航项 |
| `setPivotSpacing(int)` | 设置项目之间的间距 |
| `setMarkWidth(int)` | 设置下划线标记宽度 |
| `setCurrentIndex(int)` | 设置当前选中项索引 |

**显示效果：**

```
  本地歌曲    下载歌曲    下载视频    正在下载
  ═════════
     ↑ 下划线会随选中项滑动
```

**信号：**

```cpp
// 当选中项改变时触发
connect(pivot, &ElaPivot::pivotClicked, this, [=](int index) {
    qDebug() << "选中了第" << index << "项";
    // 切换对应的内容页面
});
```

**使用场景：** 音乐播放器分类、视频网站频道切换、设置页面分组、仪表盘视图切换

---

### 3.46 ElaTabWidget（标签页控件）

**头文件：** `#include "ElaTabWidget.h"`

标签页控件，类似浏览器的多标签页，支持动态添加/关闭标签，可设置图标。

**基本用法：**

```cpp
// 创建标签页控件
ElaTabWidget* tabWidget = new ElaTabWidget(this);
tabWidget->setFixedHeight(600);           // 设置固定高度
tabWidget->setIsTabTransparent(true);     // 设置标签栏透明背景

// 添加带图标的标签页
ElaText* page1 = new ElaText("页面1内容", this);
page1->setTextPixelSize(32);
page1->setAlignment(Qt::AlignCenter);
tabWidget->addTab(page1, QIcon(":/image/icon.png"), "标签1");

// 添加不带图标的标签页
ElaText* page2 = new ElaText("页面2内容", this);
page2->setTextPixelSize(32);
page2->setAlignment(Qt::AlignCenter);
tabWidget->addTab(page2, "标签2");
```

**批量创建标签页：**

```cpp
for (int i = 0; i < 5; i++) {
    ElaText* page = new ElaText(QString("内容%1").arg(i), this);
    page->setTextPixelSize(32);
    page->setAlignment(Qt::AlignCenter);
    tabWidget->addTab(page, QString("标签%1").arg(i));
}
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `addTab(QWidget*, QString)` | 添加标签页（无图标） |
| `addTab(QWidget*, QIcon, QString)` | 添加标签页（带图标） |
| `setIsTabTransparent(bool)` | 设置标签栏是否透明 |
| `setFixedHeight(int)` | 设置控件固定高度 |
| `setCurrentIndex(int)` | 切换到指定标签页 |
| `count()` | 获取标签页数量 |

**显示效果：**

```
┌──────────┬──────────┬──────────┬──────────┬───┐
│ 🖼 标签1  │  标签2   │  标签3   │  标签4   │ + │
├──────────┴──────────┴──────────┴──────────┴───┤
│                                               │
│              页面内容区域                       │
│                                               │
│                                               │
└───────────────────────────────────────────────┘
```

**使用场景：** 浏览器多标签页、代码编辑器多文件、聊天软件多会话、文档查看器

---

### 3.47 T_Navigation - 导航组件示例

T_Navigation 页面展示了三种导航组件的组合使用。

**头文件引用：**

```cpp
#include "ElaBreadcrumbBar.h"   // 面包屑导航
#include "ElaPivot.h"           // 枢轴导航
#include "ElaTabWidget.h"       // 标签页控件
#include "ElaPushButton.h"      // 按钮
#include "ElaScrollPageArea.h"  // 滚动区域
#include "ElaText.h"            // 文本标签
```

**完整构造流程：**

```cpp
T_Navigation::T_Navigation(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("ElaNavigation");
    createCustomWidget("导航组件展示页面");

    // ========== 1. ElaBreadcrumbBar 面包屑导航 ==========
    ElaText* breadcrumbBarText = new ElaText("ElaBreadcrumbBar", this);
    breadcrumbBarText->setTextPixelSize(18);

    _breadcrumbBar = new ElaBreadcrumbBar(this);
    QStringList breadcrumbBarList;
    for (int i = 0; i < 20; i++) {
        breadcrumbBarList << QString("Item%1").arg(i + 1);
    }
    _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);

    // 还原按钮
    ElaPushButton* resetButton = new ElaPushButton("还原", this);
    resetButton->setFixedSize(60, 32);
    connect(resetButton, &ElaPushButton::clicked, this, [=]() {
        _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);
    });

    // 面包屑标题布局
    QHBoxLayout* breadcrumbBarTextLayout = new QHBoxLayout();
    breadcrumbBarTextLayout->addWidget(breadcrumbBarText);
    breadcrumbBarTextLayout->addSpacing(15);
    breadcrumbBarTextLayout->addWidget(resetButton);
    breadcrumbBarTextLayout->addStretch();

    // 面包屑容器
    ElaScrollPageArea* breadcrumbBarArea = new ElaScrollPageArea(this);
    QVBoxLayout* breadcrumbBarLayout = new QVBoxLayout(breadcrumbBarArea);
    breadcrumbBarLayout->addWidget(_breadcrumbBar);

    // ========== 2. ElaPivot 枢轴导航 ==========
    ElaText* pivotText = new ElaText("ElaPivot", this);
    pivotText->setTextPixelSize(18);

    _pivot = new ElaPivot(this);
    _pivot->setPivotSpacing(8);
    _pivot->setMarkWidth(75);
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->setCurrentIndex(0);

    ElaScrollPageArea* pivotArea = new ElaScrollPageArea(this);
    QVBoxLayout* pivotLayout = new QVBoxLayout(pivotArea);
    pivotLayout->addWidget(_pivot);

    // ========== 3. ElaTabWidget 标签页控件 ==========
    ElaText* tabWidgetText = new ElaText("ElaTabWidget", this);
    tabWidgetText->setTextPixelSize(18);

    _tabWidget = new ElaTabWidget(this);
    _tabWidget->setFixedHeight(600);
    _tabWidget->setIsTabTransparent(true);

    // 第一个标签页（带图标）
    ElaText* page1 = new ElaText("新标签页", this);
    page1->setTextPixelSize(32);
    page1->setAlignment(Qt::AlignCenter);
    _tabWidget->addTab(page1, QIcon(":/Resource/Image/Cirno.jpg"), "新标签页");

    // 批量添加标签页
    for (int i = 0; i < 5; i++) {
        ElaText* page = new ElaText(QString("新标签页%1").arg(i), this);
        page->setTextPixelSize(32);
        page->setAlignment(Qt::AlignCenter);
        _tabWidget->addTab(page, QString("新标签页%1").arg(i));
    }

    // ========== 整体布局 ==========
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);

    centerVLayout->addLayout(breadcrumbBarTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(breadcrumbBarArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(pivotText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(pivotArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(tabWidgetText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(_tabWidget);
    centerVLayout->addStretch();

    addCentralWidget(centralWidget, true, false, 0);
}
```

**整体布局结构：**

```
┌─────────────────────────────────────────────────────────────┐
│  ElaBreadcrumbBar              [还原]                        │
├─────────────────────────────────────────────────────────────┤
│  Item1 > Item2 > Item3 > ... > Item20                       │
│  (面包屑导航，点击可返回上级)                                  │
├─────────────────────────────────────────────────────────────┤
│                        ↓ 15px 间距                           │
├─────────────────────────────────────────────────────────────┤
│  ElaPivot                                                   │
├─────────────────────────────────────────────────────────────┤
│  本地歌曲  下载歌曲  下载视频  正在下载  ...                   │
│  ════════                                                   │
│  (枢轴导航，带下划线动画)                                      │
├─────────────────────────────────────────────────────────────┤
│                        ↓ 15px 间距                           │
├─────────────────────────────────────────────────────────────┤
│  ElaTabWidget                                               │
├─────────────────────────────────────────────────────────────┤
│  ┌──────────┬──────────┬──────────┬──────────┐              │
│  │🖼新标签页 │ 新标签页0 │ 新标签页1 │ 新标签页2│              │
│  ├──────────┴──────────┴──────────┴──────────┤              │
│  │                                           │              │
│  │            标签页内容区域                   │              │
│  │                                           │              │
│  └───────────────────────────────────────────┘              │
└─────────────────────────────────────────────────────────────┘
```

**三种导航组件对比：**

| 组件 | 用途 | 特点 | 适用场景 |
|------|------|------|----------|
| **ElaBreadcrumbBar** | 层级路径导航 | 显示位置路径，点击返回 | 文件管理、分类导航 |
| **ElaPivot** | 平级分类切换 | 水平排列，下划线动画 | 内容分类、视图切换 |
| **ElaTabWidget** | 多标签页管理 | 独立页面，可关闭 | 多文档、多会话 |

---

## 八、弹出/交互组件

### 3.48 ElaColorDialog（颜色选择对话框）

**头文件：** `#include "ElaColorDialog.h"`

模态颜色选择对话框，支持多种颜色选取方式，提供浅色/深色主题适配。

**基本用法：**

```cpp
// 创建颜色对话框
ElaColorDialog* colorDialog = new ElaColorDialog(this);

// 模态弹出
colorDialog->exec();

// 获取当前颜色
QColor color = colorDialog->getCurrentColor();
QString rgb = colorDialog->getCurrentColorRGB();  // 返回 RGB 文本（如 "#FF5733"）
```

**与按钮联动示例：**

```cpp
// 创建颜色预览按钮
ElaPushButton* colorBtn = new ElaPushButton(this);
colorBtn->setFixedSize(35, 35);

// 设置按钮颜色（需要同时设置浅色/深色主题的三种状态）
colorBtn->setLightDefaultColor(colorDialog->getCurrentColor());
colorBtn->setLightHoverColor(colorDialog->getCurrentColor());
colorBtn->setLightPressColor(colorDialog->getCurrentColor());
colorBtn->setDarkDefaultColor(colorDialog->getCurrentColor());
colorBtn->setDarkHoverColor(colorDialog->getCurrentColor());
colorBtn->setDarkPressColor(colorDialog->getCurrentColor());

// 点击按钮弹出对话框
connect(colorBtn, &ElaPushButton::clicked, this, [=]() {
    colorDialog->exec();
});

// 颜色选择完成后同步更新按钮颜色
connect(colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
    colorBtn->setLightDefaultColor(color);
    colorBtn->setLightHoverColor(color);
    colorBtn->setLightPressColor(color);
    colorBtn->setDarkDefaultColor(color);
    colorBtn->setDarkHoverColor(color);
    colorBtn->setDarkPressColor(color);
});
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `exec()` | 模态弹出对话框 |
| `getCurrentColor()` | 获取当前选中的 QColor |
| `getCurrentColorRGB()` | 获取当前颜色的 RGB 字符串 |

**信号：**

| 信号 | 参数 | 触发时机 |
|------|------|----------|
| `colorSelected` | `const QColor& color` | 用户确认选择颜色后 |

**按钮颜色状态说明：**

```
ElaPushButton 有 6 种颜色状态需要设置：
┌──────────┬───────────┬───────────┬───────────┐
│          │  Default  │   Hover   │   Press   │
├──────────┼───────────┼───────────┼───────────┤
│ Light 主题│ 默认颜色   │ 悬停颜色   │ 按下颜色   │
│ Dark 主题 │ 默认颜色   │ 悬停颜色   │ 按下颜色   │
└──────────┴───────────┴───────────┴───────────┘
```

**使用场景：** 主题配色、绘图工具调色、UI 个性化设置

---

### 3.49 ElaCalendar（日历组件）

**头文件：** `#include "ElaCalendar.h"`

完整的日历视图，直接嵌入到界面中显示。

**基本用法：**

```cpp
// 创建日历组件（直接嵌入页面显示）
ElaCalendar* calendar = new ElaCalendar(this);

// 添加到布局
layout->addWidget(calendar);
```

**显示效果：**

```
┌──────────────────────────────────────┐
│        ◀   2026 年 2 月    ▶         │
├──────────────────────────────────────┤
│  日   一   二   三   四   五   六     │
│                          1    2      │
│  3    4    5    6    7    8    9      │
│  10   11   12   13   14   15   16    │
│  17   18   19   20   21   22   23    │
│  24   25   26   27   28              │
└──────────────────────────────────────┘
```

**使用场景：** 日程管理、事件查看、考勤面板

---

### 3.50 ElaCalendarPicker（日历选择器）

**头文件：** `#include "ElaCalendarPicker.h"`

紧凑型日期选择器，外观为输入框，点击后弹出日历进行日期选择。

**基本用法：**

```cpp
// 创建日历选择器
ElaCalendarPicker* calendarPicker = new ElaCalendarPicker(this);

// 添加到布局
layout->addWidget(calendarPicker);
```

**ElaCalendar vs ElaCalendarPicker：**

| 组件 | 外观 | 适用场景 |
|------|------|----------|
| `ElaCalendar` | 完整日历，直接显示在页面 | 日程面板、需要常驻日历的场景 |
| `ElaCalendarPicker` | 输入框样式，点击弹出日历 | 表单中选择日期、节省空间 |

```
ElaCalendarPicker 交互流程：
┌────────────────┐       点击       ┌──────────────────┐
│ 2026-02-04   ▼ │  ──────────→    │ ┌──────────────┐ │
└────────────────┘                  │ │  弹出日历    │ │
                                    │ │  选择日期    │ │
                                    │ └──────────────┘ │
                                    └──────────────────┘
```

**使用场景：** 表单日期输入、预约时间、筛选日期范围

---

### 3.51 ElaKeyBinder（快捷键绑定器）

**头文件：** `#include "ElaKeyBinder.h"`

快捷键录入组件，点击后可捕获用户按下的键盘组合键。

**基本用法：**

```cpp
// 创建快捷键绑定器
ElaKeyBinder* keyBinder = new ElaKeyBinder(this);

// 添加到布局
layout->addWidget(keyBinder);
```

**交互流程：**

```
1. 初始状态            2. 点击后进入录入状态        3. 按下快捷键完成录入
┌──────────────┐      ┌──────────────────┐        ┌──────────────────┐
│  点击录入     │  →   │  请按下快捷键...  │   →    │  Ctrl+Shift+A    │
└──────────────┘      └──────────────────┘        └──────────────────┘
```

**使用场景：** 软件快捷键设置、游戏按键绑定、自定义热键

---

### 3.52 ElaRoller（单列滚轮选择器）

**头文件：** `#include "ElaRoller.h"`

类似 iOS 风格的单列滚轮选择器，通过上下滚动来选择项目。

**基本用法：**

```cpp
// 创建滚轮选择器
ElaRoller* roller = new ElaRoller(this);

// 设置选项列表
QStringList itemList;
for (int i = 0; i < 100; i++) {
    itemList.append(QString::number(i + 1));
}
roller->setItemList(itemList);
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setItemList(QStringList)` | 设置选项列表 |

**显示效果：**

```
       ┌───────┐
       │  ...  │  ← 上方项目（模糊）
       │   5   │
  ────►│   6   │◄──── 当前选中（高亮）
       │   7   │
       │  ...  │  ← 下方项目（模糊）
       └───────┘
```

**使用场景：** 数字选择、年份选择、简单列表选择

---

### 3.53 ElaRollerPicker（多列滚轮选择器）

**头文件：** `#include "ElaRollerPicker.h"`

多列滚轮选择器，可组合多个滚轮列实现复杂选择（如时间、日期等）。

**基本用法 - 时间选择器（时 + 分 + AM/PM）：**

```cpp
ElaRollerPicker* timePicker = new ElaRollerPicker(this);

// 生成小时列表 00-23
QStringList hourList;
for (int i = 0; i < 24; i++) {
    hourList.append(QString("%1").arg(i, 2, 10, QChar('0')));  // 补零：0 → "00"
}

// 生成分钟列表 00-60
QStringList minuteList;
for (int i = 0; i < 61; i++) {
    minuteList.append(QString("%1").arg(i, 2, 10, QChar('0')));
}

// 添加滚轮列
timePicker->addRoller(hourList);                    // 第一列：小时（默认循环滚动）
timePicker->addRoller(minuteList);                  // 第二列：分钟（默认循环滚动）
timePicker->addRoller({"AM", "PM"}, false);         // 第三列：AM/PM（false = 不循环）

// 设置初始值
QTime now = QTime::currentTime();
timePicker->setCurrentData({
    QString("%1").arg(now.hour(), 2, 10, QChar('0')),
    QString("%1").arg(now.minute(), 2, 10, QChar('0')),
    now.hour() >= 12 ? "PM" : "AM"
});
```

**自定义列宽：**

```cpp
ElaRollerPicker* clockPicker = new ElaRollerPicker(this);
clockPicker->addRoller(hourList);
clockPicker->addRoller(minuteList);
clockPicker->setRollerWidth(0, 135);   // 第一列宽度 135px
clockPicker->setRollerWidth(1, 135);   // 第二列宽度 135px
clockPicker->setCurrentData({currentHour, currentMinute});
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `addRoller(QStringList, bool loop=true)` | 添加一列滚轮，loop 控制是否循环 |
| `setRollerWidth(int index, int width)` | 设置指定列的宽度 |
| `setCurrentData(QStringList)` | 设置各列的当前选中值 |

**`QString::arg` 补零格式化说明：**

```cpp
QString("%1").arg(i, 2, 10, QChar('0'))
//               │  │  │   │
//               │  │  │   └── 填充字符：'0'
//               │  │  └────── 进制：10（十进制）
//               │  └───────── 最小宽度：2 位
//               └──────────── 要格式化的值
// 结果：0→"00"  5→"05"  12→"12"
```

**两种选择器对比效果：**

```
timePicker（三列）:              clockPicker（两列+自定义宽度）:
┌──────┬──────┬──────┐          ┌───────────┬───────────┐
│  14  │  30  │  PM  │          │    14     │    30     │
└──────┴──────┴──────┘          └───────────┴───────────┘
  时     分    上下午                 时          分
```

**ElaRoller vs ElaRollerPicker：**

| 组件 | 列数 | 适用场景 |
|------|------|----------|
| `ElaRoller` | 单列 | 简单数字/选项选择 |
| `ElaRollerPicker` | 多列 | 时间选择、日期选择、多维度选择 |

**使用场景：** 时间选择器、日期选择器、地址选择器（省/市/区）

---

### 3.54 ElaDrawerArea（抽屉面板）

**头文件：** `#include "ElaDrawerArea.h"`

可展开/折叠的抽屉面板，类似手风琴效果。由头部（Header）和多个抽屉内容项（Drawer）组成。

**基本用法：**

```cpp
// 创建抽屉组件
ElaDrawerArea* drawer = new ElaDrawerArea(this);

// ===== 1. 创建抽屉头部 =====
QWidget* header = new QWidget(this);
QHBoxLayout* headerLayout = new QHBoxLayout(header);

// 图标
ElaText* icon = new ElaText(this);
icon->setElaIcon(ElaIconType::MessageArrowDown);
icon->setFixedSize(25, 25);

// 标题
ElaText* title = new ElaText("ElaDrawer", this);
title->setTextPixelSize(15);

// 开关
ElaToggleSwitch* sw = new ElaToggleSwitch(this);

headerLayout->addWidget(icon);
headerLayout->addWidget(title);
headerLayout->addStretch();
headerLayout->addWidget(sw);

drawer->setDrawerHeader(header);   // 设置头部

// ===== 2. 添加抽屉内容项 =====
for (int i = 0; i < 3; i++) {
    QWidget* item = new QWidget(this);
    item->setFixedHeight(75);
    QHBoxLayout* itemLayout = new QHBoxLayout(item);
    ElaCheckBox* checkBox = new ElaCheckBox(QString("测试窗口%1").arg(i + 1), this);
    itemLayout->addSpacing(60);         // 左缩进
    itemLayout->addWidget(checkBox);
    drawer->addDrawer(item);            // 添加到抽屉
}

// ===== 3. 展开/折叠控制 =====
connect(sw, &ElaToggleSwitch::toggled, this, [=](bool toggled) {
    if (toggled) {
        drawer->expand();    // 展开
    } else {
        drawer->collapse();  // 折叠
    }
});

// 抽屉状态变化时同步开关
connect(drawer, &ElaDrawerArea::expandStateChanged, this, [=](bool isExpand) {
    sw->setIsToggled(isExpand);
});
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setDrawerHeader(QWidget*)` | 设置抽屉头部（始终显示） |
| `addDrawer(QWidget*)` | 添加一个抽屉内容项 |
| `expand()` | 展开抽屉 |
| `collapse()` | 折叠抽屉 |

**信号：**

| 信号 | 参数 | 触发时机 |
|------|------|----------|
| `expandStateChanged` | `bool isExpand` | 展开/折叠状态改变时 |

**显示效果：**

```
折叠状态：                         展开状态（带动画过渡）：
┌──────────────────────────┐      ┌──────────────────────────┐
│ ▼ ElaDrawer    关  [OFF] │      │ ▲ ElaDrawer    开  [ON]  │
└──────────────────────────┘      ├──────────────────────────┤
                                  │      ☐ 测试窗口1          │
                                  ├──────────────────────────┤
                                  │      ☐ 测试窗口2          │
                                  ├──────────────────────────┤
                                  │      ☐ 测试窗口3          │
                                  └──────────────────────────┘
```

**使用场景：** 设置分组、FAQ 问答、侧边栏菜单、高级选项折叠

---

### 3.55 T_Popup - 弹出组件示例

T_Popup 页面展示了七种弹出/交互组件的组合使用。

**完整构造流程：**

```cpp
T_Popup::T_Popup(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("ElaPopup");
    createCustomWidget("弹出组件展示页面");
    QWidget* centralWidget = new QWidget(this);

    // ========== 1. ElaToolButton 带菜单按钮 ==========
    _toolButton = new ElaToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    _toolButton->setText("ElaToolButton");
    _toolButton->setElaIcon(ElaIconType::Broom);

    ElaMenu* menu = new ElaMenu(this);
    menu->addElaIconAction(ElaIconType::JackOLantern, "JackOLantern");
    menu->addElaIconAction(ElaIconType::LacrosseStick, "LacrosseStick");
    _toolButton->setMenu(menu);

    // ========== 2. ElaColorDialog 颜色对话框 ==========
    _colorDialog = new ElaColorDialog(this);
    ElaPushButton* colorBtn = new ElaPushButton(this);
    colorBtn->setFixedSize(35, 35);
    connect(colorBtn, &ElaPushButton::clicked, this, [=]() {
        _colorDialog->exec();
    });
    connect(_colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        // 更新按钮颜色...
    });

    // ========== 3. ElaCalendar + ElaCalendarPicker ==========
    _calendar = new ElaCalendar(this);
    _calendarPicker = new ElaCalendarPicker(this);

    // ========== 4. ElaKeyBinder 快捷键绑定 ==========
    _keyBinder = new ElaKeyBinder(this);

    // ========== 5. ElaRoller 单列滚轮 ==========
    _roller = new ElaRoller(this);
    QStringList rollerItemList;
    for (int i = 0; i < 100; i++) {
        rollerItemList.append(QString::number(i + 1));
    }
    _roller->setItemList(rollerItemList);

    // ========== 6. ElaRollerPicker 多列滚轮 ==========
    _timeRollerPicker = new ElaRollerPicker(this);
    _timeRollerPicker->addRoller(hourList);
    _timeRollerPicker->addRoller(minuteList);
    _timeRollerPicker->addRoller({"AM", "PM"}, false);

    _clockRollerPicker = new ElaRollerPicker(this);
    _clockRollerPicker->addRoller(hourList);
    _clockRollerPicker->addRoller(minuteList);
    _clockRollerPicker->setRollerWidth(0, 135);
    _clockRollerPicker->setRollerWidth(1, 135);

    // ========== 7. ElaDrawerArea 抽屉面板 ==========
    _drawer = new ElaDrawerArea(this);
    // 设置头部 + 添加 3 个抽屉项...

    // ========== 整体布局 ==========
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
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
```

**整体布局结构：**

```
┌─────────────────────────────────────────────────────────────┐
│  ElaToolButton        [🧹 ElaToolButton ▼]                  │
├─────────────────────────────────────────────────────────────┤
│  ElaColorDialog       [■] #FF5733                           │
├─────────────────────────────────────────────────────────────┤
│  ElaCalendarPicker    [2026-02-04 ▼]                        │
├─────────────────────────────────────────────────────────────┤
│  ┌────────────────────────────────────────────────────┐     │
│  │              ElaCalendar（完整日历）                 │     │
│  └────────────────────────────────────────────────────┘     │
├─────────────────────────────────────────────────────────────┤
│  ElaKeyBinder         [Ctrl+Shift+A]                        │
├─────────────────────────────────────────────────────────────┤
│  ▼ ElaDrawer                                    关 [OFF]    │
├─────────────────────────────────────────────────────────────┤
│  ElaRoller  [滚轮]    ElaRollerPicker  [14:30 PM]           │
│                                        [14:30]              │
└─────────────────────────────────────────────────────────────┘
```

**七种弹出/交互组件对比：**

| 组件 | 类型 | 交互方式 | 适用场景 |
|------|------|----------|----------|
| **ElaToolButton** | 按钮+菜单 | 点击弹出下拉菜单 | 工具栏更多操作 |
| **ElaColorDialog** | 模态对话框 | 弹出颜色选择器 | 调色、主题设置 |
| **ElaCalendar** | 嵌入式面板 | 直接操作日历 | 日程管理面板 |
| **ElaCalendarPicker** | 输入框+弹出 | 点击弹出日历 | 表单日期字段 |
| **ElaKeyBinder** | 输入框 | 点击后捕获按键 | 快捷键设置 |
| **ElaRoller** | 嵌入式滚轮 | 上下滚动选择 | 数字/选项选择 |
| **ElaRollerPicker** | 嵌入式多列滚轮 | 多列滚动选择 | 时间/日期选择 |
| **ElaDrawerArea** | 折叠面板 | 展开/折叠动画 | 设置分组、FAQ |

---

## 九、屏幕捕获组件（仅 Windows）

### 3.56 ElaDxgiManager（DXGI 屏幕捕获管理器）

**头文件：** `#include "ElaDxgiManager.h"`

**平台限制：** 仅支持 Windows（需要 `#ifdef Q_OS_WIN`）

DXGI（DirectX Graphics Infrastructure）屏幕捕获管理器，用于高效捕获屏幕内容。采用单例模式，全局唯一实例。

**基本用法：**

```cpp
#ifdef Q_OS_WIN
// 获取 DXGI 管理器单例
ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();

// 设置捕获区域大小
dxgiManager->setGrabArea(1920, 1080);

// 开始/停止捕获
dxgiManager->startGrabScreen();
dxgiManager->stopGrabScreen();
#endif
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `getInstance()` | 获取单例实例 |
| `setGrabArea(int width, int height)` | 设置捕获区域大小 |
| `getDxDeviceList()` | 获取所有显卡列表（QStringList） |
| `getDxDeviceID()` | 获取当前使用的显卡 ID |
| `setDxDeviceID(int id)` | 设置使用的显卡 |
| `getOutputDeviceList()` | 获取所有显示器列表（QStringList） |
| `getOutputDeviceID()` | 获取当前捕获的显示器 ID |
| `setOutputDeviceID(int id)` | 设置要捕获的显示器 |
| `startGrabScreen()` | 开始屏幕捕获 |
| `stopGrabScreen()` | 停止屏幕捕获 |

**多显卡/多显示器场景：**

```cpp
// 获取显卡列表（如：集成显卡、独立显卡）
QStringList gpuList = dxgiManager->getDxDeviceList();
// 结果示例：["Intel UHD Graphics 630", "NVIDIA GeForce RTX 3080"]

// 获取显示器列表
QStringList monitorList = dxgiManager->getOutputDeviceList();
// 结果示例：["显示器 1 (主)", "显示器 2"]

// 切换到独立显卡
dxgiManager->setDxDeviceID(1);

// 切换到第二个显示器
dxgiManager->setOutputDeviceID(1);
```

**使用场景：** 录屏软件、直播推流、屏幕共享、远程桌面

---

### 3.57 ElaDxgiScreen（DXGI 屏幕显示组件）

**头文件：** `#include "ElaDxgiScreen.h"`（通常包含在 `ElaDxgiManager.h` 或 `T_ElaScreen.h` 中）

**平台限制：** 仅支持 Windows

用于显示 `ElaDxgiManager` 捕获到的屏幕画面的组件。

**基本用法：**

```cpp
#ifdef Q_OS_WIN
// 创建屏幕显示组件
ElaDxgiScreen* dxgiScreen = new ElaDxgiScreen(this);
dxgiScreen->setFixedSize(1200, 678);  // 设置预览窗口大小

// 添加到布局
layout->addWidget(dxgiScreen);

// 刷新显示（在切换设备或停止捕获后调用）
dxgiScreen->update();
#endif
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setFixedSize(int w, int h)` | 设置预览窗口大小 |
| `update()` | 刷新显示画面 |

---

### 3.58 ElaToggleButton（切换按钮）

**头文件：** `#include "ElaToggleButton.h"`

类似开关的按钮，有"按下"和"弹起"两种状态，适合控制开始/停止类操作。

**基本用法：**

```cpp
// 创建切换按钮
ElaToggleButton* toggleBtn = new ElaToggleButton("捕获", this);

// 监听状态变化
connect(toggleBtn, &ElaToggleButton::toggled, this, [=](bool isToggled) {
    if (isToggled) {
        // 按钮被按下（激活状态）
        startSomething();
    } else {
        // 按钮弹起（非激活状态）
        stopSomething();
    }
});
```

**与 ElaToggleSwitch 的区别：**

| 组件 | 外观 | 适用场景 |
|------|------|----------|
| `ElaToggleSwitch` | 滑动开关样式 | 设置项开关（如"启用通知"） |
| `ElaToggleButton` | 按钮样式 | 操作控制（如"开始/停止录制"） |

```
ElaToggleSwitch:          ElaToggleButton:
┌──────────────┐          ┌──────────────┐
│      ○──────│          │    捕获      │  ← 未激活
└──────────────┘          └──────────────┘

┌──────────────┐          ┌██████████████┐
│──────○      │          │    捕获      │  ← 激活
└──────────────┘          └██████████████┘
```

**使用场景：** 录制开始/停止、播放/暂停、连接/断开

---

### 3.59 T_ElaScreen - 屏幕捕获示例

T_ElaScreen 页面展示了 DXGI 屏幕捕获组件的完整使用方法。

**头文件引用：**

```cpp
#ifdef Q_OS_WIN  // 仅 Windows 平台
#include "ElaComboBox.h"
#include "ElaDxgiManager.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaToggleButton.h"
#include <QVBoxLayout>
```

**完整构造流程：**

```cpp
T_ElaScreen::T_ElaScreen(QWidget* parent)
    : T_BasePage(parent)
{
    setWindowTitle("ElaScreen");
    createCustomWidget("DXGI录制组件被放置于此，可在此界面预览录制效果");

    // ========== 1. 初始化 DXGI 管理器 ==========
    ElaDxgiManager* dxgiManager = ElaDxgiManager::getInstance();
    dxgiManager->setGrabArea(1920, 1080);  // 设置捕获分辨率

    // ========== 2. 创建屏幕预览组件 ==========
    ElaScrollPageArea* dxgiScreenArea = new ElaScrollPageArea(this);
    dxgiScreenArea->setFixedHeight(700);
    QHBoxLayout* dxgiScreenLayout = new QHBoxLayout(dxgiScreenArea);

    _dxgiScreen = new ElaDxgiScreen(this);
    _dxgiScreen->setFixedSize(1200, 678);
    dxgiScreenLayout->addWidget(_dxgiScreen);

    // ========== 3. 显卡选择下拉框 ==========
    ElaText* dxText = new ElaText("显卡选择", this);
    dxText->setTextPixelSize(15);

    _dxComboBox = new ElaComboBox(this);
    _dxComboBox->addItems(dxgiManager->getDxDeviceList());
    _dxComboBox->setCurrentIndex(dxgiManager->getDxDeviceID());

    // ========== 4. 屏幕选择下拉框 ==========
    ElaText* outputText = new ElaText("屏幕选择", this);
    outputText->setTextPixelSize(15);

    _outputComboBox = new ElaComboBox(this);
    _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
    _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());

    // ========== 5. 显卡切换信号处理 ==========
    connect(_dxComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged),
            this, [=](int index) {
        dxgiManager->setDxDeviceID(index);

        // 切换显卡后刷新屏幕列表（阻塞信号防止多余回调）
        _outputComboBox->blockSignals(true);
        _outputComboBox->clear();
        _outputComboBox->addItems(dxgiManager->getOutputDeviceList());
        _outputComboBox->setCurrentIndex(dxgiManager->getOutputDeviceID());
        _outputComboBox->blockSignals(false);

        _dxgiScreen->update();
    });

    // ========== 6. 屏幕切换信号处理 ==========
    connect(_outputComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged),
            this, [=](int index) {
        dxgiManager->setOutputDeviceID(index);
        _dxgiScreen->update();
    });

    // ========== 7. 开始/停止捕获按钮 ==========
    ElaToggleButton* startButton = new ElaToggleButton("捕获", this);
    connect(startButton, &ElaToggleButton::toggled, this, [=](bool isToggled) {
        if (isToggled) {
            dxgiManager->startGrabScreen();
        } else {
            dxgiManager->stopGrabScreen();
            _dxgiScreen->update();
        }
    });

    // ========== 8. 整体布局 ==========
    QHBoxLayout* comboBoxLayout = new QHBoxLayout();
    comboBoxLayout->addWidget(dxText);
    comboBoxLayout->addWidget(_dxComboBox);
    comboBoxLayout->addSpacing(10);
    comboBoxLayout->addWidget(outputText);
    comboBoxLayout->addWidget(_outputComboBox);
    comboBoxLayout->addWidget(startButton);
    comboBoxLayout->addStretch();

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addLayout(comboBoxLayout);
    centerLayout->addWidget(dxgiScreenArea);
    addCentralWidget(centralWidget, false, true);
}
#endif
```

**`blockSignals` 技巧说明：**

```cpp
// 问题：切换显卡时需要刷新屏幕列表
// clear() 和 addItems() 会触发 currentIndexChanged 信号，导致不必要的回调

// 解决方案：临时阻塞信号
_outputComboBox->blockSignals(true);   // 阻塞信号
_outputComboBox->clear();               // 不触发信号
_outputComboBox->addItems(...);         // 不触发信号
_outputComboBox->setCurrentIndex(...);  // 不触发信号
_outputComboBox->blockSignals(false);  // 恢复信号
```

**整体布局结构：**

```
┌─────────────────────────────────────────────────────────────┐
│ 显卡选择 [▼ NVIDIA RTX]  屏幕选择 [▼ 显示器1]  [捕获]       │
├─────────────────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────────────────┐ │
│ │                                                         │ │
│ │                                                         │ │
│ │              ElaDxgiScreen（屏幕预览区域）               │ │
│ │                    1200 x 678 像素                      │ │
│ │                                                         │ │
│ │                                                         │ │
│ └─────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

**屏幕捕获组件对比：**

| 组件 | 类型 | 作用 | 平台 |
|------|------|------|------|
| **ElaDxgiManager** | 单例管理器 | 控制捕获（设备选择、开始/停止） | Windows |
| **ElaDxgiScreen** | 显示组件 | 实时显示捕获画面 | Windows |
| **ElaToggleButton** | 切换按钮 | 控制开始/停止 | 全平台 |

**条件编译注意事项：**

```cpp
// 整个屏幕捕获功能需要包裹在条件编译中
#ifdef Q_OS_WIN
    // Windows 专属代码
#endif

// 或者在 .pro 文件中配置
win32 {
    SOURCES += T_ElaScreen.cpp
    HEADERS += T_ElaScreen.h
}

---

## 十、全局设置与管理器

### 3.60 ElaTheme（主题管理器）

**头文件：** `#include "ElaTheme.h"`

全局主题管理器，用于控制应用程序的深色/浅色主题切换。通过全局宏 `eTheme` 访问。

**基本用法：**

```cpp
#include "ElaTheme.h"

// 切换到浅色模式
eTheme->setThemeMode(ElaThemeType::Light);

// 切换到深色模式
eTheme->setThemeMode(ElaThemeType::Dark);

// 获取当前主题模式
ElaThemeType::ThemeMode currentMode = eTheme->getThemeMode();
```

**监听主题变化：**

```cpp
// 当主题改变时执行回调
connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode themeMode) {
    if (themeMode == ElaThemeType::Light) {
        qDebug() << "切换到浅色模式";
    } else {
        qDebug() << "切换到深色模式";
    }
});
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setThemeMode(ElaThemeType::ThemeMode)` | 设置主题模式 |
| `getThemeMode()` | 获取当前主题模式 |

**信号：**

| 信号 | 参数 | 触发时机 |
|------|------|----------|
| `themeModeChanged` | `ElaThemeType::ThemeMode` | 主题模式改变时 |

**主题枚举值：**

```cpp
ElaThemeType::Light   // 浅色模式（日间）
ElaThemeType::Dark    // 深色模式（夜间）
```

---

### 3.61 ElaApplication（应用程序管理器）

**头文件：** `#include "ElaApplication.h"`

应用程序全局管理器，用于控制窗口显示效果（云母、亚克力等）。通过全局宏 `eApp` 访问。

**基本用法：**

```cpp
#include "ElaApplication.h"

// 设置窗口显示模式
eApp->setWindowDisplayMode(ElaApplicationType::Normal);     // 普通
eApp->setWindowDisplayMode(ElaApplicationType::ElaMica);    // Ela 云母效果

// Windows 专属效果
#ifdef Q_OS_WIN
eApp->setWindowDisplayMode(ElaApplicationType::Mica);       // Win11 云母
eApp->setWindowDisplayMode(ElaApplicationType::MicaAlt);    // Win11 云母替代
eApp->setWindowDisplayMode(ElaApplicationType::Acrylic);    // 亚克力毛玻璃
eApp->setWindowDisplayMode(ElaApplicationType::DwmBlur);    // DWM 模糊
#endif

// 获取当前显示模式
int mode = eApp->getWindowDisplayMode();
```

**监听显示模式变化：**

```cpp
connect(eApp, &ElaApplication::pWindowDisplayModeChanged, this, [=]() {
    qDebug() << "窗口显示模式已改变:" << eApp->getWindowDisplayMode();
});
```

**窗口显示模式说明：**

| 模式 | ID | 平台 | 效果描述 |
|------|----|----- |----------|
| `Normal` | 0 | 全平台 | 无特效，普通背景 |
| `ElaMica` | 1 | 全平台 | Ela 自定义云母效果 |
| `Mica` | 2 | Win11 | 系统云母（透过壁纸） |
| `MicaAlt` | 3 | Win11 | 云母替代效果 |
| `Acrylic` | 4 | Win10/11 | 亚克力毛玻璃 |
| `DwmBlur` | 5 | Win7+ | DWM 模糊效果 |

---

### 3.62 ElaLog（日志管理器）

**头文件：** `#include "ElaLog.h"`

日志管理器单例，用于启用/禁用应用程序日志记录功能。

**基本用法：**

```cpp
#include "ElaLog.h"

// 获取日志管理器单例
ElaLog* logger = ElaLog::getInstance();

// 启用日志
logger->initMessageLog(true);
qDebug() << "这条消息会被记录到日志文件";

// 禁用日志
logger->initMessageLog(false);
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `getInstance()` | 获取单例实例 |
| `initMessageLog(bool enable)` | 启用/禁用日志记录 |

**使用场景：** 调试模式、错误追踪、用户行为记录

---

### 3.63 ElaWindow 设置方法

**头文件：** `#include "ElaWindow.h"`

ElaWindow 提供了多种窗口和导航栏的配置方法。

**窗口绘制模式：**

```cpp
ElaWindow* window = ...;

// 设置窗口背景绘制模式
window->setWindowPaintMode(ElaWindowType::Normal);   // 纯色背景
window->setWindowPaintMode(ElaWindowType::Pixmap);   // 静态图片背景
window->setWindowPaintMode(ElaWindowType::Movie);    // GIF 动画背景

// 监听绘制模式变化
connect(window, &ElaWindow::pWindowPaintModeChanged, this, [=]() {
    qDebug() << "绘制模式:" << window->getWindowPaintMode();
});
```

**导航栏显示模式：**

```cpp
// 设置导航栏显示模式
window->setNavigationBarDisplayMode(ElaNavigationType::Auto);      // 自动（根据窗口宽度）
window->setNavigationBarDisplayMode(ElaNavigationType::Minimum);   // 最小化（仅图标）
window->setNavigationBarDisplayMode(ElaNavigationType::Compact);   // 紧凑（图标+短文字）
window->setNavigationBarDisplayMode(ElaNavigationType::Maximum);   // 最大化（完全展开）
```

**导航栏模式效果：**

```
Auto（自动）：根据窗口宽度自适应
Minimum：    ┌──┐
             │🏠│  仅显示图标
             │📊│
             └──┘

Compact：    ┌──────┐
             │🏠 首页│  图标 + 短文字
             │📊 数据│
             └──────┘

Maximum：    ┌──────────────┐
             │ 🏠 首页      │  完全展开
             │ 📊 数据分析  │
             └──────────────┘
```

**页面切换动画模式：**

```cpp
// 设置堆栈切换动画
window->setStackSwitchMode(ElaWindowType::None);    // 无动画
window->setStackSwitchMode(ElaWindowType::Popup);   // 弹出动画（默认）
window->setStackSwitchMode(ElaWindowType::Scale);   // 缩放动画
window->setStackSwitchMode(ElaWindowType::Flip);    // 翻转动画
window->setStackSwitchMode(ElaWindowType::Blur);    // 模糊过渡

// 监听切换模式变化
connect(window, &ElaWindow::pStackSwitchModeChanged, this, [=]() {
    qDebug() << "切换模式:" << window->getStackSwitchMode();
});
```

**用户信息卡片显示：**

```cpp
// 显示/隐藏左上角的用户信息卡片
window->setUserInfoCardVisible(true);   // 显示
window->setUserInfoCardVisible(false);  // 隐藏
```

**ElaWindow 常用设置方法汇总：**

| 方法 | 作用 |
|------|------|
| `setWindowPaintMode()` | 设置窗口背景绘制模式 |
| `getWindowPaintMode()` | 获取当前绘制模式 |
| `setNavigationBarDisplayMode()` | 设置导航栏显示模式 |
| `setStackSwitchMode()` | 设置页面切换动画 |
| `getStackSwitchMode()` | 获取当前切换动画模式 |
| `setUserInfoCardVisible()` | 显示/隐藏用户卡片 |

---

### 3.64 QButtonGroup（单选按钮组）

**头文件：** `#include <QButtonGroup>`

Qt 原生类，用于管理一组互斥的单选按钮。在 ElaWidgetTools 中常与 `ElaRadioButton` 配合使用。

**基本用法：**

```cpp
#include <QButtonGroup>
#include "ElaRadioButton.h"

// 创建单选按钮
ElaRadioButton* btn1 = new ElaRadioButton("选项1", this);
ElaRadioButton* btn2 = new ElaRadioButton("选项2", this);
ElaRadioButton* btn3 = new ElaRadioButton("选项3", this);
btn1->setChecked(true);  // 默认选中第一个

// 创建按钮组实现互斥
QButtonGroup* group = new QButtonGroup(this);
group->addButton(btn1, 0);  // ID = 0
group->addButton(btn2, 1);  // ID = 1
group->addButton(btn3, 2);  // ID = 2

// 监听选中变化
connect(group, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
        this, [=](QAbstractButton* button, bool isToggled) {
    if (isToggled) {
        int id = group->id(button);  // 获取按钮 ID
        qDebug() << "选中了 ID:" << id;

        // 根据 ID 执行操作（常用于枚举转换）
        MyEnum value = static_cast<MyEnum>(id);
    }
});
```

**将 ID 转换为枚举的技巧：**

```cpp
// 定义按钮 ID 与枚举值对应
enum class Mode { Normal = 0, Advanced = 1, Expert = 2 };

group->addButton(normalBtn, static_cast<int>(Mode::Normal));
group->addButton(advancedBtn, static_cast<int>(Mode::Advanced));
group->addButton(expertBtn, static_cast<int>(Mode::Expert));

// 获取选中的枚举值
connect(group, ..., [=](QAbstractButton* button, bool isToggled) {
    if (isToggled) {
        Mode mode = static_cast<Mode>(group->id(button));
    }
});
```

**从外部同步按钮状态：**

```cpp
// 当设置从外部改变时，同步单选按钮状态
connect(someObject, &SomeClass::modeChanged, this, [=]() {
    int currentMode = someObject->getMode();
    QAbstractButton* button = group->button(currentMode);
    if (button) {
        ElaRadioButton* radioBtn = dynamic_cast<ElaRadioButton*>(button);
        if (radioBtn) {
            radioBtn->setChecked(true);
        }
    }
});
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `addButton(QAbstractButton*, int id)` | 添加按钮并指定 ID |
| `button(int id)` | 根据 ID 获取按钮 |
| `id(QAbstractButton*)` | 获取按钮的 ID |
| `checkedButton()` | 获取当前选中的按钮 |
| `checkedId()` | 获取当前选中按钮的 ID |

---

### 3.65 T_Setting - 设置页面示例

T_Setting 页面展示了如何使用全局管理器配置应用程序的各种设置。

**头文件引用：**

```cpp
#include "ElaApplication.h"      // 应用程序管理器
#include "ElaComboBox.h"
#include "ElaLog.h"              // 日志管理器
#include "ElaRadioButton.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaTheme.h"            // 主题管理器
#include "ElaToggleSwitch.h"
#include "ElaWindow.h"
#include <QButtonGroup>
```

**完整构造流程：**

```cpp
T_Setting::T_Setting(QWidget* parent)
    : T_BasePage(parent)
{
    // 获取父窗口引用
    ElaWindow* window = dynamic_cast<ElaWindow*>(parent);
    setWindowTitle("Setting");

    // ========== 1. 主题切换 ==========
    ElaText* themeText = new ElaText("主题设置", this);
    themeText->setTextPixelSize(18);

    _themeComboBox = new ElaComboBox(this);
    _themeComboBox->addItem("日间模式");
    _themeComboBox->addItem("夜间模式");

    // 下拉框 → 切换主题
    connect(_themeComboBox, QOverload<int>::of(&ElaComboBox::currentIndexChanged),
            this, [=](int index) {
        eTheme->setThemeMode(index == 0 ? ElaThemeType::Light : ElaThemeType::Dark);
    });

    // 主题变化 → 同步下拉框（双向绑定）
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=](ElaThemeType::ThemeMode mode) {
        _themeComboBox->blockSignals(true);
        _themeComboBox->setCurrentIndex(mode == ElaThemeType::Light ? 0 : 1);
        _themeComboBox->blockSignals(false);
    });

    // ========== 2. 窗口绘制模式 ==========
    _windowNormalButton = new ElaRadioButton("Normal", this);
    _windowNormalButton->setChecked(true);
    _windowPixmapButton = new ElaRadioButton("Pixmap", this);
    _windowMovieButton = new ElaRadioButton("Movie", this);

    QButtonGroup* windowPaintGroup = new QButtonGroup(this);
    windowPaintGroup->addButton(_windowNormalButton, 0);
    windowPaintGroup->addButton(_windowPixmapButton, 1);
    windowPaintGroup->addButton(_windowMovieButton, 2);

    connect(windowPaintGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled),
            this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled) {
            window->setWindowPaintMode((ElaWindowType::PaintMode)windowPaintGroup->id(button));
        }
    });

    // ========== 3. 窗口特效（跨平台 + Windows 专属） ==========
    _normalButton = new ElaRadioButton("Normal", this);
    _elaMicaButton = new ElaRadioButton("ElaMica", this);
#ifdef Q_OS_WIN
    _micaButton = new ElaRadioButton("Mica", this);
    _micaAltButton = new ElaRadioButton("Mica-Alt", this);
    _acrylicButton = new ElaRadioButton("Acrylic", this);
    _dwmBlurnormalButton = new ElaRadioButton("Dwm-Blur", this);
#endif
    _normalButton->setChecked(true);

    QButtonGroup* displayGroup = new QButtonGroup(this);
    displayGroup->addButton(_normalButton, 0);
    displayGroup->addButton(_elaMicaButton, 1);
#ifdef Q_OS_WIN
    displayGroup->addButton(_micaButton, 2);
    displayGroup->addButton(_micaAltButton, 3);
    displayGroup->addButton(_acrylicButton, 4);
    displayGroup->addButton(_dwmBlurnormalButton, 5);
#endif

    connect(displayGroup, ..., [=](...) {
        eApp->setWindowDisplayMode((ElaApplicationType::WindowDisplayMode)displayGroup->id(button));
    });

    // ========== 4. 日志开关 ==========
    _logSwitchButton = new ElaToggleSwitch(this);
    connect(_logSwitchButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        ElaLog::getInstance()->initMessageLog(checked);
    });

    // ========== 5. 用户卡片开关 ==========
    _userCardSwitchButton = new ElaToggleSwitch(this);
    connect(_userCardSwitchButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {
        window->setUserInfoCardVisible(!checked);
    });

    // ========== 6. 导航栏模式 ==========
    _autoButton = new ElaRadioButton("Auto", this);
    _minimumButton = new ElaRadioButton("Minimum", this);
    _compactButton = new ElaRadioButton("Compact", this);
    _maximumButton = new ElaRadioButton("Maximum", this);
    _autoButton->setChecked(true);

    QButtonGroup* navGroup = new QButtonGroup(this);
    navGroup->addButton(_autoButton, 0);
    navGroup->addButton(_minimumButton, 1);
    navGroup->addButton(_compactButton, 2);
    navGroup->addButton(_maximumButton, 3);

    connect(navGroup, ..., [=](...) {
        window->setNavigationBarDisplayMode((ElaNavigationType::NavigationDisplayMode)navGroup->id(button));
    });

    // ========== 7. 页面切换动画 ==========
    _noneButton = new ElaRadioButton("None", this);
    _popupButton = new ElaRadioButton("Popup", this);
    _scaleButton = new ElaRadioButton("Scale", this);
    _flipButton = new ElaRadioButton("Flip", this);
    _blurButton = new ElaRadioButton("Blur", this);
    _popupButton->setChecked(true);

    QButtonGroup* stackGroup = new QButtonGroup(this);
    stackGroup->addButton(_noneButton, 0);
    stackGroup->addButton(_popupButton, 1);
    stackGroup->addButton(_scaleButton, 2);
    stackGroup->addButton(_flipButton, 3);
    stackGroup->addButton(_blurButton, 4);

    connect(stackGroup, ..., [=](...) {
        window->setStackSwitchMode((ElaWindowType::StackSwitchMode)stackGroup->id(button));
    });

    // ========== 整体布局 ==========
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addWidget(themeText);
    centerLayout->addWidget(themeSwitchArea);
    centerLayout->addWidget(helperText);
    centerLayout->addWidget(logSwitchArea);
    centerLayout->addWidget(userCardSwitchArea);
    centerLayout->addWidget(windowPaintModeArea);
    centerLayout->addWidget(micaSwitchArea);
    centerLayout->addWidget(displayModeArea);
    centerLayout->addWidget(stackSwitchModeArea);
    centerLayout->addStretch();
    addCentralWidget(centralWidget, true, true, 0);
}
```

**整体布局结构：**

```
┌─────────────────────────────────────────────────────────────┐
│  主题设置                                                    │
├─────────────────────────────────────────────────────────────┤
│  主题切换                              [▼ 日间模式]          │
├─────────────────────────────────────────────────────────────┤
│  应用程序设置                                                │
├─────────────────────────────────────────────────────────────┤
│  启用日志功能                                    [OFF]       │
├─────────────────────────────────────────────────────────────┤
│  隐藏用户卡片                                    [OFF]       │
├─────────────────────────────────────────────────────────────┤
│  主窗口绘制设置        ●Normal  ○Pixmap  ○Movie             │
├─────────────────────────────────────────────────────────────┤
│  窗口效果   ●Normal ○ElaMica ○Mica ○Mica-Alt ○Acrylic ○Dwm │
├─────────────────────────────────────────────────────────────┤
│  导航栏模式选择        ○Minimum ○Compact ○Maximum ●Auto     │
├─────────────────────────────────────────────────────────────┤
│  堆栈切换模式选择      ○None ●Popup ○Scale ○Flip ○Blur     │
└─────────────────────────────────────────────────────────────┘
```

**全局管理器对比：**

| 管理器 | 全局宏 | 作用 | 主要方法 |
|--------|--------|------|----------|
| **ElaTheme** | `eTheme` | 主题切换 | `setThemeMode()` |
| **ElaApplication** | `eApp` | 窗口特效 | `setWindowDisplayMode()` |
| **ElaLog** | `getInstance()` | 日志管理 | `initMessageLog()` |
| **ElaWindow** | 实例方法 | 窗口配置 | 多个 set 方法 |

**双向绑定模式总结：**

```cpp
// 1. UI → 数据
connect(comboBox, &ElaComboBox::currentIndexChanged, [=](int index) {
    eTheme->setThemeMode(...);
});

// 2. 数据 → UI（使用 blockSignals 防止循环）
connect(eTheme, &ElaTheme::themeModeChanged, [=](...) {
    comboBox->blockSignals(true);
    comboBox->setCurrentIndex(...);
    comboBox->blockSignals(false);
});
```

---

## 十一、日志系统

### 3.66 T_LogModel（日志数据模型）

**头文件：** `#include "T_LogModel.h"`

自定义日志数据模型，继承自 `QAbstractListModel`，用于存储和管理日志列表数据。

**头文件定义：**

```cpp
#ifndef T_LOGMODEL_H
#define T_LOGMODEL_H

#include <QAbstractListModel>

class T_LogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit T_LogModel(QObject* parent = nullptr);
    ~T_LogModel();

    void setLogList(QStringList list);      // 设置完整日志列表
    void appendLogList(QString log);        // 追加单条日志
    QStringList getLogList() const;         // 获取日志列表

protected:
    // QAbstractListModel 必须实现的虚函数
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _logList;   // 存储日志的字符串列表
};

#endif // T_LOGMODEL_H
```

**实现文件：**

```cpp
#include "T_LogModel.h"

T_LogModel::T_LogModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

T_LogModel::~T_LogModel()
{
}

// 返回数据行数（必须实现）
int T_LogModel::rowCount(const QModelIndex& parent) const
{
    return this->_logList.count();
}

// 返回指定索引的数据（必须实现）
QVariant T_LogModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)        // 只响应显示角色
    {
        return _logList[index.row()];   // 返回对应行的日志字符串
    }
    return QVariant();                  // 其他角色返回空值
}

// 设置完整日志列表
void T_LogModel::setLogList(QStringList list)
{
    beginResetModel();          // 通知视图：模型即将重置
    this->_logList = list;
    endResetModel();            // 通知视图：模型重置完成，请刷新
}

// 追加单条日志（实时更新）
void T_LogModel::appendLogList(QString log)
{
    beginResetModel();
    this->_logList.append(log);
    endResetModel();
}

// 获取日志列表
QStringList T_LogModel::getLogList() const
{
    return this->_logList;
}
```

**`beginResetModel()` / `endResetModel()` 机制：**

```cpp
// 这对函数必须成对调用，用于通知视图数据变化
beginResetModel();   // 告诉视图："我要修改数据了，先别读取"
// ... 修改 _logList 数据 ...
endResetModel();     // 告诉视图："数据改完了，你可以刷新了"
```

**常用方法：**

| 方法 | 作用 |
|------|------|
| `setLogList(QStringList)` | 设置完整日志列表（替换） |
| `appendLogList(QString)` | 追加单条日志（实时更新） |
| `getLogList()` | 获取当前日志列表 |
| `rowCount()` | 返回数据行数（视图调用） |
| `data()` | 返回指定行的数据（视图调用） |

---

### 3.67 T_LogWidget（日志显示组件）

**头文件：** `#include "T_LogWidget.h"`

日志显示组件，使用 MVC 架构展示应用程序的日志信息。继承自 `QWidget`（独立可复用组件）。

**基本用法：**

```cpp
#include "T_LogWidget.h"

// 创建日志组件
T_LogWidget* logWidget = new T_LogWidget(this);

// 添加到布局
layout->addWidget(logWidget);
```

**完整实现：**

```cpp
#include "T_LogWidget.h"

#include <ElaListView.h>    // Ela 列表视图组件
#include <QVBoxLayout>

#include "ElaLog.h"         // Ela 日志管理器（单例）
#include "T_LogModel.h"     // 自定义日志数据模型

T_LogWidget::T_LogWidget(QWidget* parent)
    : QWidget{parent}   // 继承自 QWidget（独立可复用组件）
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 5, 5, 0);

    // 创建 Ela 列表视图
    ElaListView* logView = new ElaListView(this);
    logView->setIsTransparent(true);    // 设置背景透明

    // 创建日志数据模型
    _logModel = new T_LogModel(this);

    // 将模型绑定到视图（MVC 核心）
    logView->setModel(_logModel);
    mainLayout->addWidget(logView);

    // 监听 ElaLog 的日志消息信号（核心连接）
    connect(ElaLog::getInstance(), &ElaLog::logMessage, this, [=](QString log) {
        _logModel->appendLogList(log);  // 将新日志追加到模型
    });

    // 添加测试数据
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
}

T_LogWidget::~T_LogWidget()
{
}
```

**MVC 架构说明：**

| 组件 | 角色 | 作用 |
|------|------|------|
| `ElaListView` | **View（视图）** | 负责显示日志列表 |
| `T_LogModel` | **Model（模型）** | 存储日志数据 |
| `ElaLog` | **数据源** | 产生日志消息 |

---

### 3.68 ElaLog 日志信号机制

**头文件：** `#include "ElaLog.h"`

ElaLog 日志管理器除了 `initMessageLog()` 方法外，还提供 `logMessage` 信号用于实时分发日志消息。

**信号：**

| 信号 | 参数 | 触发时机 |
|------|------|----------|
| `logMessage` | `QString log` | 当有新的日志消息产生时 |

**监听日志消息：**

```cpp
// 连接日志信号
connect(ElaLog::getInstance(), &ElaLog::logMessage, this, [=](QString log) {
    // 处理日志消息
    qDebug() << "收到日志:" << log;
    // 可以追加到列表、写入文件等
});
```

**完整日志流程：**

```
应用程序代码                          ElaLog                         T_LogWidget
    │                                  │                                │
    │  qDebug() << "消息";            │                                │
    │─────────────────────────────────>│                                │
    │                                  │ 捕获日志消息                    │
    │                                  │                                │
    │                                  │  emit logMessage(log)          │
    │                                  │────────────────────────────────>│
    │                                  │                                │ appendLogList()
    │                                  │                                │
    │                                  │                                │ ElaListView 刷新
```

---

### 3.69 日志系统完整架构

**架构图：**

```
┌──────────────────────────────────────────────────────────────┐
│                      应用程序代码                             │
│   qDebug() << "日志消息";                                    │
│   qWarning() << "警告消息";                                  │
│   qInfo() << "信息消息";                                     │
└──────────────────────────┬───────────────────────────────────┘
                           ↓
┌──────────────────────────────────────────────────────────────┐
│                   ElaLog（日志管理器单例）                     │
│   - initMessageLog(true) 启用日志捕获                         │
│   - 捕获 Qt 日志消息                                          │
│   - 发出 logMessage(QString) 信号                            │
└──────────────────────────┬───────────────────────────────────┘
                           ↓ 信号连接
┌──────────────────────────────────────────────────────────────┐
│                    T_LogWidget（日志组件）                     │
│   connect(ElaLog::getInstance(), &ElaLog::logMessage, ...)  │
└──────────────────────────┬───────────────────────────────────┘
                           ↓ appendLogList()
┌──────────────────────────────────────────────────────────────┐
│                    T_LogModel（数据模型）                      │
│   - _logList: QStringList 存储日志                           │
│   - beginResetModel() / endResetModel() 通知视图             │
│   - rowCount() / data() 提供数据给视图                        │
└──────────────────────────┬───────────────────────────────────┘
                           ↓ setModel()
┌──────────────────────────────────────────────────────────────┐
│                    ElaListView（列表视图）                     │
│   - 自动调用 model->rowCount() 获取行数                       │
│   - 自动调用 model->data() 获取每行内容                       │
│   - 显示日志列表                                              │
└──────────────────────────────────────────────────────────────┘
```

**显示效果：**

```
┌─────────────────────────────────────────────────────────────┐
│ 测试条例11223344556677889900                                 │
│ 测试条例11223344556677889900                                 │
│ [2026-02-05 10:30:15] 用户登录成功                           │
│ [2026-02-05 10:30:20] 加载数据完成                           │
│ [2026-02-05 10:30:25] 警告：内存使用率较高                    │
│                       ↓ 新日志自动追加                        │
└─────────────────────────────────────────────────────────────┘
```

**日志系统组件对比：**

| 组件 | 类型 | 作用 |
|------|------|------|
| **ElaLog** | 单例管理器 | 捕获并分发日志消息 |
| **T_LogModel** | 数据模型 | 存储日志列表，提供数据接口 |
| **T_LogWidget** | 视图容器 | 组合 Model + View，监听日志信号 |
| **ElaListView** | 列表视图 | 显示日志内容 |

**使用场景：** 应用调试面板、错误日志查看、操作记录、系统监控

**创建自定义日志组件的步骤：**

```cpp
// 1. 创建数据模型（继承 QAbstractListModel）
class MyLogModel : public QAbstractListModel {
    QStringList _logs;
    int rowCount(...) const override { return _logs.count(); }
    QVariant data(...) const override { return _logs[index.row()]; }
    void append(QString log) {
        beginResetModel();
        _logs.append(log);
        endResetModel();
    }
};

// 2. 创建视图组件
ElaListView* view = new ElaListView(this);
MyLogModel* model = new MyLogModel(this);
view->setModel(model);

// 3. 连接日志信号
connect(ElaLog::getInstance(), &ElaLog::logMessage, [=](QString log) {
    model->append(log);
});

// 4. 启用日志捕获
ElaLog::getInstance()->initMessageLog(true);
```
