#include "transferitemprocessingdelegate.h"


TransferItemProcessingDelegate::TransferItemProcessingDelegate(QObject* parent): QStyledItemDelegate(parent)
    , itemButton1(new ItemButton("play"))
    , itemButton2(new ItemButton("cancle"))
    , itemButton3(new ItemButton("folder"))
    , itemProgressBar(new ItemProgressBar())
{

}

void TransferItemProcessingDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

//    painter->save();
//    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->setPen(Qt::NoPen);
//    if (option.state & QStyle::State_Selected)
//        painter->setBrush(QBrush());
//    else
//        painter->setBrush(option.palette.text());

    switch(index.column())
    {
        case TransferItem::NameRole:      // 0 名称
            {

                QStyledItemDelegate::paint(painter, viewOption, index);
            }
            break;
        case TransferItem::InfoRole_1:    // 1 [已下载大小/]总大小
            {
                viewOption.displayAlignment = Qt::AlignCenter;
                QStyledItemDelegate::paint(painter, viewOption, index);
            }
            break;
        case TransferItem::InfoRole_2:    // 2 [下载速度]
            {
                viewOption.displayAlignment = Qt::AlignCenter;
                QStyledItemDelegate::paint(painter, viewOption, index);
            }
            break;
        case TransferItem::InfoRole_3:    // 3  下载进度
            {
                if (option.state & QStyle::State_Selected)
                    painter->fillRect(option.rect, option.palette.highlight());

                itemProgressBar->paintQProgressBar(painter, option, index);

            }
            break;
        case TransferItem::OperatorRole_1:// 4
            {
//                QStyledItemDelegate::paint(painter, viewOption, index);

                if (option.state & QStyle::State_Selected)
                    painter->fillRect(option.rect, option.palette.highlight());

                itemButton1->paintButton(painter, option, mousePos);
            }
            break;
        case TransferItem::OperatorRole_2:// 5
            {
                if (option.state & QStyle::State_Selected)
                    painter->fillRect(option.rect, option.palette.highlight());

                itemButton2->paintButton(painter, option, mousePos);
            }
            break;
        case TransferItem::OperatorRole_3:// 6
            {
                if (option.state & QStyle::State_Selected)
                    painter->fillRect(option.rect, option.palette.highlight());

                itemButton3->paintButton(painter, option, mousePos);
            }
            break;
        default:
            {
                QStyledItemDelegate::paint(painter, viewOption, index);
            }
    }
//    painter->restore();
}

QSize TransferItemProcessingDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    switch(index.column())
    {
        case TransferItem::NameRole:      // 0 名称
        case TransferItem::InfoRole_1:    // 1 [(]已下载大小/]总大小
        case TransferItem::InfoRole_2:    // 2 [下载速度]
            {
                return QSize(100, 30);
            }
            break;
        case TransferItem::InfoRole_3:    // 3  下载进度
            {
                return QSize(200, 30);
            }
            break;
        case TransferItem::OperatorRole_1:// 4
        case TransferItem::OperatorRole_2:// 5
        case TransferItem::OperatorRole_3:// 6
            {
                return QSize(50, 30);
            }
            break;
    }
    return QStyledItemDelegate::sizeHint(option, index);
}

bool TransferItemProcessingDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    QMouseEvent* pEvent = static_cast<QMouseEvent*> (event);
    mousePos = pEvent->pos();

    if (ItemButton::withinButtonRegion(mousePos, option))
    {
        switch (event->type())
        {
            case QEvent::MouseMove:
                {
                    switch(index.column())
                    {
                        case TransferItem::OperatorRole_1:// 4
                            {
                                itemButton1->setStatusHover();
                            }
                            break;
                        case TransferItem::OperatorRole_2:// 5
                            {
                                itemButton2->setStatusHover();
                            }
                            break;
                        case TransferItem::OperatorRole_3:// 6
                            {
                                itemButton3->setStatusHover();
                            }
                            break;
                    }
                    return true;
                }
                break;
            case QEvent::MouseButtonPress:
                {
                    switch(index.column())
                    {
                        case TransferItem::OperatorRole_1:// 4
                            {
                                itemButton1->setStatusPressed();
                            }
                            break;
                        case TransferItem::OperatorRole_2:// 5
                            {
                                itemButton2->setStatusPressed();
                            }
                            break;
                        case TransferItem::OperatorRole_3:// 6
                            {
                                itemButton3->setStatusPressed();
                            }
                            break;
                    }
                    return false;
                }
                break;
            case QEvent::MouseButtonRelease:
                {
                    switch(index.column())
                    {
                        case TransferItem::OperatorRole_1:// 4
                            {
                                itemButton1->setStatusReleased();
                            }
                            break;
                        case TransferItem::OperatorRole_2:// 5
                            {
                                itemButton2->setStatusReleased();
                            }
                            break;
                        case TransferItem::OperatorRole_3:// 6
                            {
                                itemButton3->setStatusReleased();
                            }
                            break;
                    }
                    return true;
                }
                break;
            default:
                {
                    switch(index.column())
                    {
                        case TransferItem::OperatorRole_1:// 4
                            {
                                itemButton1->setStatusNone();
                            }
                            break;
                        case TransferItem::OperatorRole_2:// 5
                            {
                                itemButton2->setStatusNone();
                            }
                            break;
                        case TransferItem::OperatorRole_3:// 6
                            {
                                itemButton3->setStatusNone();
                            }
                            break;
                    }
                    QStyledItemDelegate::editorEvent(event, model, option, index);
                    return false;
                }
                break;
        }
    }
    else
    {
        switch(index.column())
        {
            case TransferItem::OperatorRole_1:// 4
                {
                    itemButton1->setStatusNone();
                }
                break;
            case TransferItem::OperatorRole_2:// 5
                {
                    itemButton2->setStatusNone();
                }
                break;
            case TransferItem::OperatorRole_3:// 6
                {
                    itemButton3->setStatusNone();
                }
                break;
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

    return true;
}

ItemButton::ItemButton(QString iconName): buttonWidget(new QPushButton())
{
    buttonIconName = iconName;
    buttonWidget->setStyleSheet("\
                            QPushButton{\
                                border: none;\
                                background-color: transparent;\
                                image: url(:/control/icon/control/" + iconName + ".png);\
                            }\
                            QPushButton:hover {\
                                image: url(:/control/icon/control/" + iconName + "-hover.png);\
                            }\
                            QPushButton:pressed {\
                                image: url(:/control/icon/control/" + iconName + "-pressed.png);\
                            }\
                          ");
}

void ItemButton::paintButton(QPainter* painter, const QStyleOptionViewItem& option, const QPoint& pos)
{
    QStyleOptionButton buttonOption;
    buttonOption.rect = QRect(option.rect.x() + posx, option.rect.y() + poxy, width, height);
    buttonOption.state = QStyle::State_Enabled;
    if(buttonOption.rect.contains(pos))
    {
        if(buttonStatus == BUTTON_STATUS::HOVER)
        {
            buttonOption.state |= QStyle::State_MouseOver;
            QToolTip::showText(pos, "kkkk");
        }
        else if(buttonStatus == BUTTON_STATUS::PRESSED)
            buttonOption.state |= QStyle::State_Sunken;
    }
//    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, button1.data());
    option.widget->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, buttonWidget.data());
//    button1.data()->style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter, button1.data());
}

void ItemButton::setStatusNone()
{
    buttonStatus = NONE;
}

void ItemButton::setStatusHover()
{
    buttonStatus = HOVER;
}

void ItemButton::setStatusPressed()
{
    buttonStatus = PRESSED;
}

void ItemButton::setStatusReleased()
{
    buttonStatus = RELEASED;
}

bool ItemButton::withinButtonRegion(const QPoint pos, const QStyleOptionViewItem& option)
{
    QStyleOptionButton buttonOption;
    buttonOption.rect = QRect(option.rect.x() + posx, option.rect.y() + poxy, width, height);
    return buttonOption.rect.contains(pos);
}

ItemProgressBar::ItemProgressBar(): progressBarWidget(new QProgressBar())
{
    progressBarWidget->setStyleSheet("\
                                        QProgressBar{\
                                            border: 2px solid #347;\
                                            border-radius:4px;\
                                        }\
                                        QProgressBar::chunk {\
                                            background-color: #77AAEE;\
                                        }\
                                     ");
}

void ItemProgressBar::paintQProgressBar(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)
{

    double progress = index.data().toInt();

    QStyleOptionProgressBar progressBarOption;
    progressBarOption.initFrom(option.widget);
    progressBarOption.rect = QRect(option.rect.x() + left, option.rect.y() + top, option.rect.width() - left - right, option.rect.height() - top - bottom);
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.progress = progress;
    progressBarOption.text = QString::number(progress) + "%";
    progressBarOption.textVisible = true;
    progressBarOption.textAlignment = Qt::AlignHCenter | Qt::AlignVCenter;

//    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, progressBarWidget.data());
    option.widget->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, progressBarWidget.data());
//    progressBarWidget.data()->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, progressBarWidget.data());

}
