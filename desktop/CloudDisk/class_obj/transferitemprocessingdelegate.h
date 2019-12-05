#ifndef TRANSFERITEMPROCESSINGDELEGATE_H
#define TRANSFERITEMPROCESSINGDELEGATE_H

#include <QAbstractItemDelegate>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QProgressBar>
#include <QToolTip>

#include "transferitem.h"

class ItemButton
{
public:
    static const int posx = 15;
    static const int poxy = 5;
    static const int width = 20;
    static const int height = width;
    enum BUTTON_STATUS
    {
        NONE, HOVER, PRESSED, RELEASED
    };

private:
    QString buttonIconName;
    QScopedPointer<QPushButton> buttonWidget;
    BUTTON_STATUS buttonStatus = NONE;

public:
    ItemButton(QString iconName);
    void paintButton(QPainter* painter, const QStyleOptionViewItem& option, const QPoint& pos );

    void inline setStatusNone();
    void inline setStatusHover();
    void inline setStatusPressed();
    void inline setStatusReleased();

    static bool withinButtonRegion(const QPoint, const QStyleOptionViewItem& option);

};
class ItemProgressBar
{
public:
    static const int left = 2;
    static const int right = left;
    static const int top = 6;
    static const int bottom = top;

private:
    QScopedPointer<QProgressBar> progressBarWidget;

public:
    ItemProgressBar();
    void paintQProgressBar(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index );

};

class TransferItemProcessingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TransferItemProcessingDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
    QScopedPointer<ItemButton> itemButton1;
    QScopedPointer<ItemButton> itemButton2;
    QScopedPointer<ItemButton> itemButton3;
    QScopedPointer<ItemProgressBar> itemProgressBar;
    QPoint mousePos;

};

#endif // TRANSFERITEMPROCESSINGDELEGATE_H
