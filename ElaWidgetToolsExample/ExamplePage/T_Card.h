#ifndef T_CARD_H
#define T_CARD_H

#include "T_BasePage.h"

class ElaLCDNumber;
class ElaPromotionCard;
class ElaPromotionView;
class T_Card : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Card(QWidget* parent = nullptr);
    ~T_Card();

private:
    ElaLCDNumber* _lcdNumber{nullptr};          // LCD 数字显示
    ElaPromotionCard* _promotionCard{nullptr};  // 单个促销卡片
    ElaPromotionView* _promotionView{nullptr};  // 轮播视图
};

#endif // T_CARD_H
