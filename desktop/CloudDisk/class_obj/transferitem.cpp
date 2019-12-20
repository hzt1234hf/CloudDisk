#include "transferitem.h"

TransferItem::TransferItem(QObject* parent): m_datum(self_datum)
{

}

TransferItem::TransferItem(QList<Obj_Transfer*>& d, QObject* parent): m_datum(d)
    , QAbstractTableModel(parent)
{

}

QVariant TransferItem::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role == Qt::SizeHintRole)
        return QSize(0, 0);
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
    if (!index.isValid() || index.row() > m_datum.count())
    {
        return QVariant();
    }
    int nColumn = index.column();
    switch(role)
    {
        case Qt::FontRole:          // 6
            return QVariant();
        case Qt::TextAlignmentRole: // 7
//            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
            return QVariant();
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
                            if(m_datum[index.row()]->objIsFinished())
                                return tr("总大小");
                            else if(m_datum[index.row()]->objIsDownload())
                                return tr("已下载大小/总大小");
                            else
                                return tr("已上传大小/总大小");
                        }
                        break;
                    case InfoRole_2:    // 2 [下载速度]
                        {
                            if(m_datum[index.row()]->objIsFinished())
                                return "";
                            else
                                return tr("下载速度");
                        }
                        break;
                    case InfoRole_3:    // 3  下载进度
                        {
                            if(m_datum[index.row()]->objIsFinished())
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
                            if(m_datum[index.row()]->objIsFinished())
                                return tr("打开文件");
                            else if(m_datum[index.row()]->objIsTransmitting())
                                return tr("暂停");
                            else
                                return tr("开始");
                        }
                        break;
                    case OperatorRole_2:// 5
                        {
                            if(m_datum[index.row()]->objIsFinished())
                                return tr("打开所在文件夹");
                            else
                                return tr("删除");
                        }
                        break;
                    case OperatorRole_3:// 6
                        {
                            if(m_datum[index.row()]->objIsFinished())
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
                            if(m_datum[index.row()]->objIsFinished())
                                return m_datum[index.row()]->objTotalSizeStr();
                            else
                                return m_datum[index.row()]->objSizeScale();
                        }
                        break;
                    case InfoRole_2:    // 2 [下载速度]
                        {
                            if(m_datum[index.row()]->objIsFinished() == false)
                                return m_datum[index.row()]->objTransferSpeed();
                        }
                        break;
                    case InfoRole_3:    // 3 下载进度
                        {
                            QList<QVariant> result;
                            result.push_back(QVariant(m_datum[index.row()]->objIsFinished()));
                            if(m_datum[index.row()]->objIsFinished() == false)
                                result.push_back(QVariant(m_datum[index.row()]->objTransferRate()));
                            else
                            {
                                if(m_datum[index.row()]->objIsDownload())
                                    result.push_back("下载完成");
                                else
                                    result.push_back("上传完成");
                            }
                            return result;
                        }
                        break;
                    case OperatorRole_1:// 4
                        {
                            QList<QVariant> res;
                            res.push_back(m_datum[index.row()]->objIsTransmitting());
                            res.push_back(m_datum[index.row()]->objIsFinished());
                            return res;
                        }
                        break;
                    case OperatorRole_2:// 5
                        {
                            return m_datum[index.row()]->objIsFinished();
                        }
                        break;
                    case OperatorRole_3:// 6
                        {
                            return m_datum[index.row()]->objIsFinished();
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



Qt::ItemFlags TransferItem::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QList<Obj_Transfer*>& TransferItem::getSelfDatum()
{
    return self_datum;
}

void TransferItem::addData(Obj_Transfer* dataObj)
{
    qDebug() << "add Data";
    beginResetModel();
    m_datum.push_back(dataObj);
    endResetModel();
}

void TransferItem::update()
{
    beginResetModel();
    endResetModel();
}
