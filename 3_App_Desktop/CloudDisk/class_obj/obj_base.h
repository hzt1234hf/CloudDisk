#ifndef OBJ_BASE_H
#define OBJ_BASE_H

#include <QDate>
#include <QWidget>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

class Obj_Base : public QWidget
{
    Q_OBJECT
public:
    /* 对象类型*/
    bool isFile;

    /* 对象具有的公共属相 */
    long id;
    QString name;
    long parentid;
    bool isShared;
    bool isShareEncryped;
    QString sharePassword;
    QString shareUrl;
    QDate sharePeriod;

public:

private:
    void paintEvent(QPaintEvent*);

public:
    explicit Obj_Base(QWidget* parent = nullptr);
    explicit Obj_Base(QWidget* parent, long id, QString name, long parentid, QDate sharePeriod, bool isFile,
                      bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
    void setProperty(long id, QString name, long parentid, QDate sharePeriod, bool isFile,
                     bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
signals:

public slots:
};

#endif // OBJ_BASE_H
