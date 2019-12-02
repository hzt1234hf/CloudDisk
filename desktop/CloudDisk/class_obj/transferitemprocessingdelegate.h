#ifndef TRANSFERITEMPROCESSINGDELEGATE_H
#define TRANSFERITEMPROCESSINGDELEGATE_H

#include <QAbstractItemDelegate>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QApplication>
#include <QPainter>
#include <QPushButton>

#include "transferitem.h"

class TransferItemProcessingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TransferItemProcessingDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

//    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // TRANSFERITEMPROCESSINGDELEGATE_H
