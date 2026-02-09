#include <QApplication>
#include <QScreen>

#include "ElaApplication.h"
#include "ElaLog.h"
#include "mainwindow.h"
#include <QMenu>
#include <QIcon>
#include <QPixmap>
#include <QSplashScreen>
#include <QSystemTrayIcon>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif
int main(int argc, char* argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else
    //根据实际屏幕缩放比例更改
    qputenv("QT_SCALE_FACTOR", "1.5");
#endif
#endif
    QApplication a(argc, argv);
    eApp->init();
    QPixmap pixmap(":/Resource/Image/Cirno.jpg");
    QSplashScreen spalsh(pixmap);
    // 启动此显示图片
    spalsh.show();
    // 使程序在显示启动画面的同时仍能响应鼠标等其他事件
    a.processEvents();
    //ElaLog::getInstance()->initMessageLog(true);
    MainWindow w;

    QSystemTrayIcon sysTray(QIcon(":/Resource/Image/Cirno.jpg"), &w);

    QMenu menu;
    auto showAct = new QAction("show", &sysTray);
    auto exitAct = new QAction("exit", &sysTray);

    QObject::connect(showAct, &QAction::triggered, [&](){
        w.setVisible(true);
    });
    QObject::connect(exitAct, &QAction::triggered, [&](){
        QApplication::quit();
    });

    menu.addAction(showAct);
    menu.addAction(exitAct);

    sysTray.setContextMenu(&menu);

    // 点击托盘图标也显示/隐藏窗口
    QObject::connect(&sysTray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason){
        if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick) {
            if (!w.isVisible()) {
                w.show();
                w.raise();
                w.activateWindow();
            } else {
                // 切换隐藏
                w.hide();
            }
        }
    });

    sysTray.show();
    w.show();
#ifdef Q_OS_WIN
//    HWND handle = FindWindowA(NULL, "ElaWidgetTool");
//    if (handle != NULL)
//    {
//        SetWindowDisplayAffinity(handle, WDA_EXCLUDEFROMCAPTURE);
//    }
#endif
    return a.exec();
}
