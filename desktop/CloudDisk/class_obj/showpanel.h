#ifndef SHOWPANEL_H
#define SHOWPANEL_H

#include <QWidget>
#include <QQueue>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "obj_frame.h"
#include "../serverconnect.h"

#include <QKeyEvent>
#include <QMenu>


class ShowPanel : public QWidget
{
    Q_OBJECT

public:
    static constexpr int gap = 10;
    QMenu* objToolPalette;
    QMenu* panelToolPalette;

private:
    QVector<obj_frame*> files;
    QVector<obj_frame*> folders;
    QQueue<obj_frame*> unusedFiles;
    QQueue<obj_frame*> unusedFolders;

    int lastWidth = 0, lastHeight = 0;
    int curCount_MaxRowObj = 0;


    enum class requestType {GET_FOLDERS,
                            GET_FOLDER, GET_FOLDER_REFRESH, GET_FOLDER_LAST,
                            ADD_FOLDER,
                            DELETE_FOLDER,
                            UPLOAD_FILE, GET_FILE, DELETE_FILE, DOWNLOAD_FILE,
                            GET_SHARE_OBJ_INFO, GET_SHARE_FOLDER, SET_OBJ_SHARED, SET_SHARED_OBJ_PASSWORD, DOWNLOAD_SHAREOBJ
                           };
    QMap<QNetworkReply*, requestType> replyMap; // 请求映射数组

    const long initFolderId = 2;        // 初始目录ID
    QVector<long> lastFolderId;         // 上一次目录ID记录
    long upperFolderId = initFolderId;  // 上一层目录ID
    long curFolderId = initFolderId;    // 当前目录ID

    bool singleSelected = true;     // 单选模式
    QList<obj_frame*> selectedObj;  // 选中对象数组



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

    /** 服务器通信API */
    void GetFolders();
    QNetworkReply* GetFolder(long id);
    void AddFolder(QString name, long parentid);
    void DeleteFolder(long id);
    void UploadFile();  //  未编写
    QNetworkReply* GetFile(long id);
    void DeleteFile(long fileid);
    void DownloadFile(long fileid);// 未编写
    void SetObjShared(bool isFile, long objId, bool isShared, bool isShareEncryped);
    void GetShareObjInfo(QString path);
    void DownloadShareObj();// 未编写
    void SetShareObjPasswd(QString path, QString password);
    void GetShareFolder(QString path, long folderid);// 未编写

signals:
    void enableBackbtn(bool);
    void enableUpperbtn(bool);
    void enableObjbtn(bool);

public slots:
    void add();
    void requestCallback(QNetworkReply*);
    void setSelected(Qt::MouseButton, obj_frame*);
    void resetSelected();
    void createObjToolPalette(const QPoint&);
    void createPanelToolPalette(const QPoint&);

    /** 服务器通信应用 */
    void refresh();
    void getLastFolderInfo();
    void getUpperFolderInfo();
    void getFolderInfo(int id);
    void getFileInfo(int id);

protected:
    void mousePressEvent(QMouseEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void paintEvent(QPaintEvent* event) override;
};

#endif // SHOWPANEL_H
