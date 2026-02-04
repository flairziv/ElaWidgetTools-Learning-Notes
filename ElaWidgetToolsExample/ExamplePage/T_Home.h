#ifndef T_HOME_H
#define T_HOME_H

#include "T_BasePage.h"
class ElaMenu;
class T_Home : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Home(QWidget* parent = nullptr); // Q_INVOKABLE 允许从 QML 调用
    ~T_Home();
Q_SIGNALS:
    Q_SIGNAL void elaScreenNavigation();        // 导航到屏幕捕获页
    Q_SIGNAL void elaBaseComponentNavigation(); // 导航到基础组件页
    Q_SIGNAL void elaSceneNavigation();         // 导航到图形场景页
    Q_SIGNAL void elaCardNavigation();          // 导航到卡片页
    Q_SIGNAL void elaIconNavigation();          // 导航到图标页

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaMenu* _homeMenu{nullptr};    // 右键菜单，C++11 成员初始化语法
};

#endif // T_HOME_H
