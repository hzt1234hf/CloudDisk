#ifndef SHOWPANEL_H
#define SHOWPANEL_H

#include <QWidget>
#include <QMenu>

//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QHttpPart>

#include <QThread>
#include <QQueue>
#include <QSaveFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QElapsedTimer>

#include <QKeyEvent>

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "obj_frame.h"
#include "comdef.h"
#include "../serverconnect.h"
#include "obj_transfer.h"


class DownloadThreadWorker: public QObject
{
    Q_OBJECT

public:

private:
    bool isRunningWork = false;

    QNetworkReply* DownloadFile(long fileid);

public slots:
    void readDownloadData();

    void stopWork();
    void startWork();

    void createDownloadTask(obj_frame*);

signals:
    void addDownloadItem(Obj_Transfer*);
};

class ShowPanel : public QWidget
{
    Q_OBJECT

public:
    static constexpr int gap = 10;
    QMenu* objToolPalette;
    QMenu* panelToolPalette;
    DownloadThreadWorker* downloadThreadWorker;// 线程

private:
    QVector<obj_frame*> files;
    QVector<obj_frame*> folders;
    QQueue<obj_frame*> unusedFiles;
    QQueue<obj_frame*> unusedFolders;

    int lastWidth = 0, lastHeight = 0;
    int curCount_MaxRowObj = 0;

    QThread downloadThread;



    QMap<QNetworkReply*, requestType> replyMap; // 请求映射数组

    const long initFolderId = 2;        // 初始目录ID
    QVector<long> lastFolderId;         // 上一次目录ID记录
    long upperFolderId = initFolderId;  // 上一层目录ID
    long curFolderId = initFolderId;    // 当前目录ID

    bool singleSelected = true;     // 单选模式
    QList<obj_frame*> selectedObj;  // 选中对象数组

    QTime timer;                    // 定时器
    int lastTime = 0;               // 上一次时间
    const int TimeInterval = 1000;   // 时间间隔

    QList<Obj_Transfer*> file_download;
    QList<Obj_Transfer*> file_upload;
    QList<Obj_Transfer*> file_finished;



public:
    explicit ShowPanel(QWidget* parent = nullptr);
    ~ShowPanel() override;


private:

    void resetFileVector(int fileCnt);
    void resetFolderVector(int folderCnt);
    void resetObjVector(int fileCnt, int folderCnt);
    void setFileInfo(const QJsonArray& jsonData);
    void setFolderInfo(const QJsonArray& jsonData);

    void rearrange();

    void addLastFolder(long );
    long getLastFolder();
    void resetSelected();
    obj_frame* getSelectedObj();

    /** 服务器通信API */
    void GetFolders();
    QNetworkReply* GetFolder(long id);
    void AddFolder(QString name, long parentid);
    void DeleteFolder(long id);
    void UploadFile(QString);
    QNetworkReply* GetFile(long id);
    void DeleteFile(long fileid);
    QNetworkReply* DownloadFile(long fileid);   // 未编写-over
    void SetObjShared(bool isFile, long objId, bool isShared, bool isShareEncryped);
    void GetShareObjInfo(QString path);
    void DownloadShareObj();    // 未编写
    void SetShareObjPasswd(QString path, QString password);
    void GetShareFolder(QString path, long folderid);   // 未编写

signals:
    void enableBackbtn(bool);
    void enableUpperbtn(bool);
    void enableObjbtn(bool);
    void addDownloadFile(Obj_Transfer*);
    void createDownloadTask(obj_frame*);

    void updateView();

public slots:
    void add();
    void requestCallback(QNetworkReply*);
    void setSelected(Qt::MouseButton, obj_frame*);

    void createObjToolPalette(const QPoint&);
    void createPanelToolPalette(const QPoint&);

    /** 服务器通信应用 */
    void refreshCurFolderInfo();
    void getLastFolderInfo();
    void getUpperFolderInfo();
    void getFolderInfo(int id);
    void getFileInfo(int id);
    void uploadLocalFile();
    void downloadFile();
    void addNewFolder();
    void deleteObj();

    void requestUpdateView();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;



};

#endif // SHOWPANEL_H
