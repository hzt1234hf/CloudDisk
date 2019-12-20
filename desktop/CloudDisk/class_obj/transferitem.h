#ifndef TRANSFERITEM_H
#define TRANSFERITEM_H

#include <QAbstractTableModel>
#include <QSize>
#include <QDebug>
#include <QColor>
#include "obj_transfer.h"

class TransferItem : public QAbstractTableModel
{
    Q_OBJECT

private:

public:
    explicit TransferItem(QObject* parent = nullptr);
    explicit TransferItem(QList<Obj_Transfer*>& m_datum, QObject* parent = nullptr);


    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QList<Obj_Transfer*>& getSelfDatum();


private:
    QList<Obj_Transfer*>& m_datum;
    QList<Obj_Transfer*> self_datum;

public:
    enum
    {
        NameRole = 0,
        InfoRole_1,
        InfoRole_2,
        InfoRole_3,
        OperatorRole_1,
        OperatorRole_2,
        OperatorRole_3,
    };

signals:

public slots:
    void addData(Obj_Transfer*);
    void update();

};

#endif // TRANSFERITEM_H
