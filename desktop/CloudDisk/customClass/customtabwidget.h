#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QStylePainter>
#include <QStyleOption>


class CustomTabBar: public QTabBar
{
public:
    explicit CustomTabBar(QWidget* parent = nullptr): QTabBar(parent)
    {

    }
    QSize tabSizeHint(int index) const
    {
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return s;
    }
protected:
    void paintEvent(QPaintEvent* /*event*/)
    {
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for(int i = 0; i < count(); i++)
        {
            initStyleOption(&opt, i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
            painter.restore();
        }
    }
};

class CustomTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    CustomTabWidget(QWidget* parent = 0): QTabWidget(parent)
    {
        setTabBar(new CustomTabBar);
        setTabPosition(QTabWidget::West);
    }
};

#endif // CUSTOMTABWIDGET_H
