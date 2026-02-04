#include "T_Card.h"

#include <QVBoxLayout>

#include "ElaLCDNumber.h"       // LCD 数字显示（支持自动时钟）
#include "ElaPromotionCard.h"   // 促销/推广卡片（带图片和文字）
#include "ElaPromotionView.h"   // 轮播视图（自动滚动多个卡片）
T_Card::T_Card(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaCard");

    // 顶部元素
    createCustomWidget("一些常用的卡片组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    // ElaLCDNumber（LCD 数字时钟）
    _lcdNumber = new ElaLCDNumber(this);
    _lcdNumber->setIsUseAutoClock(true);    // 开启自动时钟模式
    _lcdNumber->setIsTransparent(false);    // 非透明背景（有背景色）
    //    _lcdNumber->setAutoClockFormat("hh:mm:ss");   // 可选：自定义时间格式
    _lcdNumber->setFixedHeight(100);        // 固定高度 100px

    // ElaPromotionCard（单个促销卡片）
    _promotionCard = new ElaPromotionCard(this);
    _promotionCard->setFixedSize(600, 300); // 固定尺寸 600×300
    _promotionCard->setCardPixmap(QPixmap(":/Resource/Image/Card/miku.png"));   // 设置图片
    _promotionCard->setCardTitle("MiKu");           // 卡片标题（图片上方小字）
    _promotionCard->setPromotionTitle("SONG~");     // 促销标题（醒目大字）
    _promotionCard->setTitle("STYX HELIX");         // 主标题
    _promotionCard->setSubTitle("Never close your eyes, Searching for a true fate");    // 副标题

    // ElaPromotionView（轮播视图）
    _promotionView = new ElaPromotionView(this);

    // 创建第 1 个轮播卡片
    ElaPromotionCard* exampleCard1 = new ElaPromotionCard(this);
    exampleCard1->setCardPixmap(QPixmap(":/Resource/Image/Card/miku.png"));
    exampleCard1->setCardTitle("MiKu");
    exampleCard1->setPromotionTitle("SONG~");
    exampleCard1->setTitle("STYX HELIX");
    exampleCard1->setSubTitle("Never close your eyes, Searching for a true fate");

    // 创建第 2 个轮播卡片
    ElaPromotionCard* exampleCard2 = new ElaPromotionCard(this);
    exampleCard2->setCardPixmap(QPixmap(":/Resource/Image/Card/beach.png"));
    exampleCard2->setCardTitle("Beach");
    exampleCard2->setPromotionTitle("SONG~");
    exampleCard2->setTitle("STYX HELIX");
    exampleCard2->setSubTitle("Never close your eyes, Searching for a true fate");

    // 创建第 3 个轮播卡片
    ElaPromotionCard* exampleCard3 = new ElaPromotionCard(this);
    exampleCard3->setCardPixmap(QPixmap(":/Resource/Image/Card/dream.png"));
    exampleCard3->setCardTitle("Dream");
    exampleCard3->setPromotionTitle("SONG~");
    exampleCard3->setTitle("STYX HELIX");
    exampleCard3->setSubTitle("Never close your eyes, Searching for a true fate");

    // 创建第 4 个轮播卡片
    ElaPromotionCard* exampleCard4 = new ElaPromotionCard(this);
    exampleCard4->setCardPixmap(QPixmap(":/Resource/Image/Card/classroom.png"));
    exampleCard4->setCardTitle("Classroom");
    exampleCard4->setPromotionTitle("SONG~");
    exampleCard4->setTitle("STYX HELIX");
    exampleCard4->setSubTitle("Never close your eyes, Searching for a true fate");

    // 将卡片添加到轮播视图
    _promotionView->appendPromotionCard(exampleCard1);
    _promotionView->appendPromotionCard(exampleCard2);
    _promotionView->appendPromotionCard(exampleCard3);
    _promotionView->appendPromotionCard(exampleCard4);
    _promotionView->setIsAutoScroll(true);  // 开启自动滚动

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaCard");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(_lcdNumber);
    centerLayout->addSpacing(20);
    centerLayout->addWidget(_promotionCard);
    centerLayout->addSpacing(20);
    centerLayout->addWidget(_promotionView);
    centerLayout->addSpacing(100);
    centerLayout->addStretch();
    addCentralWidget(centralWidget, true, true, 0);
}

T_Card::~T_Card()
{
}
