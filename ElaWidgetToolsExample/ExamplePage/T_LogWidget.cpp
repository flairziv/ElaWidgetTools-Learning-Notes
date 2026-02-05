#include "T_LogWidget.h"

#include <ElaListView.h>    // Ela 列表视图组件

#include <QVBoxLayout>

#include "ElaLog.h"         // Ela 日志管理器（单例）
#include "T_LogModel.h"     // 自定义日志数据模型

// T_LogWidget 继承自 QWidget 而不是 T_BasePage，是一个独立的可复用组件
T_LogWidget::T_LogWidget(QWidget* parent)
    : QWidget{parent}
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

    // 监听 ElaLog 的日志消息信号
    connect(ElaLog::getInstance(), &ElaLog::logMessage, this, [=](QString log) {
        _logModel->appendLogList(log);
    });
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
}

T_LogWidget::~T_LogWidget()
{
}
