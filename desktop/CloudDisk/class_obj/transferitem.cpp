#include "transferitem.h"

TransferItem::TransferItem(QObject* parent)
    : QAbstractTableModel(parent)
{
}

QVariant TransferItem::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role == Qt::SizeHintRole)
        return QSize(1, 1);
    return QVariant();
}

int TransferItem::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_datum.count();
    // FIXME: Implement me!
}

int TransferItem::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return 7;
    // FIXME: Implement me!
}

QVariant TransferItem::data(const QModelIndex& index, int role) const
{
    qDebug() << role << "   " << index;
    if (!index.isValid() || index.row() > m_datum.count())
    {
        qDebug() << "???" ;
        return QVariant();
    }
    int nColumn = index.column();
    switch(role)
    {
        case Qt::FontRole:          // 6
            return QVariant();
        case Qt::TextAlignmentRole: // 7
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        case Qt::TextColorRole:     // 9
            return QColor(Qt::black);
        case Qt::CheckStateRole:    // 10
            return QVariant();
        case Qt::DecorationRole:    // 1
            return QVariant();
        case Qt::ToolTipRole:
            {
                switch(nColumn)
                {
                    case NameRole:      // 0 名称
                        {
                            return tr("文件名称");
                        }
                        break;
                    case InfoRole_1:    // 1 [已下载大小/]总大小
                        {
                            if(m_datum[index.row()]->isFinished)
                                return tr("总大小");
                            else if(m_datum[index.row()]->objIsDownload())
                                return tr("已下载大小/总大小");
                            else
                                return tr("已上传大小/总大小");
                        }
                        break;
                    case InfoRole_2:    // 2 [下载速度]
                        {
                            if(m_datum[index.row()]->isFinished)
                                return "";
                            else
                                return tr("下载速度");
                        }
                        break;
                    case InfoRole_3:    // 3  下载进度
                        {
                            if(m_datum[index.row()]->isFinished)
                                if(m_datum[index.row()]->objIsDownload())
                                    return tr("下载完成");
                                else
                                    return tr("上传完成");
                            else
                                return tr("下载进度");
                        }
                        break;
                    case OperatorRole_1:// 4
                        {
                            if(m_datum[index.row()]->isFinished)
                                return tr("打开文件");
                            else if(m_datum[index.row()]->isTransmitting)
                                return tr("暂停");
                            else
                                return tr("开始");
                        }
                        break;
                    case OperatorRole_2:// 5
                        {
                            if(m_datum[index.row()]->isFinished)
                                return tr("打开所在文件夹");
                            else
                                return tr("删除");
                        }
                        break;
                    case OperatorRole_3:// 6
                        {
                            if(m_datum[index.row()]->isFinished)
                                return tr("删除");
                            else
                                return tr("打开所在文件夹");
                        }
                        break;
                }
            }
            break;
        case Qt::DisplayRole:       // 0
            {
                switch(nColumn)
                {
                    case NameRole:      // 0 名称
                        {
                            return m_datum[index.row()]->objName();
                        }
                        break;
                    case InfoRole_1:    // 1 [(]已下载大小/]总大小
                        {
                            if(m_datum[index.row()]->isFinished)
                                return m_datum[index.row()]->objSize();
                            else
                                return m_datum[index.row()]->objProcSize();
                        }
                        break;
                    case InfoRole_2:    // 2 [下载速度]
                        {
                            if(m_datum[index.row()]->isFinished == false)
                                return m_datum[index.row()]->objTransSpeed();
                        }
                        break;
                    case InfoRole_3:    // 3 下载进度
                        {
                            if(m_datum[index.row()]->isFinished == false)
                                return m_datum[index.row()]->objTransScale();
                        }
                        break;
                    case OperatorRole_1:// 4
                        {

                        }
                        break;
                    case OperatorRole_2:// 5
                        {

                        }
                        break;
                    case OperatorRole_3:// 6
                        {

                        }
                        break;
                }
            }
        case Qt::BackgroundColorRole:   // 8
            return QVariant();
    }
    // FIXME: Implement me!
    return QVariant();
}

void TransferItem::addData(Obj_Transfer* dataObj)
{
    beginResetModel();
    m_datum.push_back(dataObj);
    endResetModel();
}

Qt::ItemFlags TransferItem::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
