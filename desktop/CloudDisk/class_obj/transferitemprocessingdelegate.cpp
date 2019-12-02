#include "transferitemprocessingdelegate.h"


TransferItemProcessingDelegate::TransferItemProcessingDelegate(QObject* parent): QStyledItemDelegate(parent)
{

}

void TransferItemProcessingDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    switch(index.column())
    {
        case TransferItem::NameRole:      // 0 名称
            {
//                QStyledItemDelegate::paint(painter, viewOption, index);
            }
            break;
        case TransferItem::InfoRole_1:    // 1 [已下载大小/]总大小
            {
//                QStyledItemDelegate::paint(painter, viewOption, index);
            }
            break;
        case TransferItem::InfoRole_2:    // 2 [下载速度]
            {
//                QStyledItemDelegate::paint(painter, viewOption, index);
            }
            break;
        case TransferItem::InfoRole_3:    // 3  下载进度
            {
                double progress = index.data().toDouble();

                QStyleOptionProgressBar progressBarOption;
                progressBarOption.rect = option.rect;
                progressBarOption.minimum = 0;
                progressBarOption.maximum = 100;
                progressBarOption.progress = progress;
                progressBarOption.text = QString::number(progress) + "%";
                progressBarOption.textVisible = true;

                QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
            }
            break;
        case TransferItem::OperatorRole_1:// 4
            {
                QStyleOptionButton buttonOption;
                buttonOption.state = QStyle::State_Enabled | QStyle::State_Raised;
                buttonOption.rect = option.rect;
//                buttonOption.text = index.data().toString();
                buttonOption.iconSize = QSize(20, 20);
                buttonOption.icon = QIcon(":/panel/icon/panel/download (1).png");
                QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, &buttonOption, painter);
            }
            break;
        case TransferItem::OperatorRole_2:// 5
            {
                QStyleOptionButton buttonOption;
                buttonOption.state = QStyle::State_Enabled | QStyle::State_Raised;
                buttonOption.rect = option.rect;
//                buttonOption.text = index.data().toString();
                buttonOption.iconSize = QSize(20, 20);
                buttonOption.icon = QIcon(":/panel/icon/panel/download (2).png");
                QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, &buttonOption, painter);
            }
            break;
        case TransferItem::OperatorRole_3:// 6
            {
                QPushButton btn;
                QStyleOptionButton buttonOption;
                buttonOption.state = QStyle::State_Enabled | QStyle::State_Raised;

                buttonOption.rect = option.rect;
//                buttonOption.text = index.data().toString();
                buttonOption.iconSize = QSize(20, 20);
                buttonOption.icon = QIcon(":/panel/icon/panel/download (3).png");
                QApplication::style()->drawControl(QStyle::CE_PushButtonLabel, &buttonOption, painter);
            }
            break;
    }
}

//QSize TransferItemProcessingDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
//{
//    if(!index.isValid())
//        return QSize();
//    switch(index.column())
//    {
//        case TransferItem::NameRole:      // 0 名称
//        case TransferItem::InfoRole_1:    // 1 [(]已下载大小/]总大小
//        case TransferItem::InfoRole_2:    // 2 [下载速度]
//            {
//                return QSize(100, 30);
//            }
//            break;
//        case TransferItem::InfoRole_3:    // 3  下载进度
//            {
//                return QSize(200, 30);
//            }
//            break;
//        case TransferItem::OperatorRole_1:// 4
//        case TransferItem::OperatorRole_2:// 5
//        case TransferItem::OperatorRole_3:// 6
//            {
//                return QSize(50, 30);
//            }
//            break;
//    }

//}
