#ifndef OBJ_FRAME_H
#define OBJ_FRAME_H

#include <QWidget>
#include <QLabel>
#include "obj_file.h"
#include "obj_folder.h"


class obj_frame : public QWidget
{
    Q_OBJECT
public:
    static constexpr  int fwidth = 120;
    static constexpr  int fheight = fwidth;

private:
    static constexpr  int fpadding = 5;

    static constexpr  int lwidth = fwidth - 2 * fpadding;
    static constexpr int lheight = 16;
    static constexpr int opadding = 5;
    static constexpr int oheight = fheight - 2 * (fpadding + opadding) - lheight;
    static constexpr int owidth = oheight;

    static constexpr int objx = (fwidth - owidth) / 2;
    static constexpr int objy = fpadding;
    static constexpr int labelx = fpadding;
    static constexpr int labely = fpadding + oheight;



    bool isSelected = false;



public:
    Obj_Base* obj = nullptr;
    QLabel* label = nullptr;
    explicit obj_frame(QWidget* parent = nullptr);
    explicit obj_frame(QWidget* parent,
                       bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                       QString path = "", bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
    ~obj_frame();
    void setPosition(int x, int y);
    void setInfo(bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                 QString path = "", bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");
    void resetInfo(bool isFile, long id, QString name, long parentid, QDate sharePeriod,
                   QString path = "", bool isShared = false, bool isShareEncryped = false, QString sharePassword = "", QString shareUrl = "");

protected:
    void paintEvent(QPaintEvent* painter);

signals:

public slots:
};

#endif // OBJ_FRAME_H
