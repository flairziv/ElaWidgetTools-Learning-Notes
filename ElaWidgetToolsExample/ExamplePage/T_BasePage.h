#ifndef T_BASEPAGE_H
#define T_BASEPAGE_H

#include <ElaScrollPage.h>

class QVBoxLayout;
class T_BasePage : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_BasePage(QWidget* parent = nullptr);
    ~T_BasePage() override;

protected:
    // 创建所有页面顶部的统一标题区域：
    void createCustomWidget(QString desText);
};

#endif // T_BASEPAGE_H
