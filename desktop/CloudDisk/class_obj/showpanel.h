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

class ShowPanel : public QWidget
{
    Q_OBJECT

public:
    static constexpr int gap = 10;

private:
    QVector<obj_frame*> files;
    QVector<obj_frame*> folders;
    QQueue<obj_frame*> unusedFiles;
    QQueue<obj_frame*> unusedFolders;

    int lastWidth = 0, lastHeight = 0;
    int curCount_MaxRowObj = 0;


    enum class requestType {GET_FOLDERS, GET_FOLDER, ADD_FOLDER, DELETE_FOLDER,
                            UPLOAD_FILE, DELETE_FILE, DOWNLOAD_FILE,
                            GET_SHARE_OBJ_INFO, GET_SHARE_FOLDER, SET_OBJ_SHARED, SET_SHARED_OBJ_PASSWORD, DOWNLOAD_SHAREOBJ
                           };
    QMap<QNetworkReply*, requestType> replyMap;

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

    void GetFolders();
    void GetFolder(int id);
    void AddFolder(QString name, int parentid);
    void DeleteFolder(int id);
    void UploadFile();  //  未编写
    void DeleteFile(int fileid);
    void DownloadFile(int fileid);// 未编写
    void SetObjShared(bool isFile, int objId, bool isShared, bool isShareEncryped);
    void GetShareObjInfo(QString path);
    void DownloadShareObj();// 未编写
    void SetShareObjPasswd(QString path, QString password);
    void GetShareFolder(QString path, int folderid);// 未编写

signals:

public slots:
    void add();
    void requestCallback(QNetworkReply*);

protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // SHOWPANEL_H
