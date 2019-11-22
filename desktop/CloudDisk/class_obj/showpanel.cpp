#include "showpanel.h"


ShowPanel::ShowPanel(QWidget* parent) : QWidget(parent)
{
    this->setStyleSheet("QWidget#show_panel{\
                           background-color: rgb(255, 255, 255);\
                            }");

    connect(ServerConnect::getInstance().getNetwordAccessManager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(requestCallback(QNetworkReply*)));
    GetFolder(2);
}

ShowPanel::~ShowPanel()
{
    while(files.size())
    {
        delete(files.last());
        files.pop_back();
    }
    while(folders.size())
    {
        delete(folders.last());
        folders.pop_back();
    }
    while(unusedFiles.empty() == false)
    {
        delete(unusedFiles.front());
        unusedFiles.pop_front();
    }
    while(unusedFolders.empty() == false)
    {
        delete(unusedFolders.front());
        unusedFolders.pop_front();
    }
}

void ShowPanel::resetFileVector(int fileCnt)
{
    if(files.size() < fileCnt)
    {
        while(files.size() < fileCnt && unusedFiles.empty() == false)
        {
            auto file  = unusedFiles.front();
            files.push_back(file);
            unusedFiles.pop_front();
            file->show();
        }
        while(files.size() < fileCnt)
        {
            auto file = new obj_frame(this);
            files.push_back(file);
            file->show();
        }
    }
    else
    {
        while(files.size() > fileCnt)
        {
            auto file = files.back();
            file->hide();
            unusedFiles.push_front(file);
            files.pop_back();

        }
    }
}

void ShowPanel::resetFolderVector(int folderCnt)
{
    if(folders.size() < folderCnt)
    {
        while(folders.size() < folderCnt && unusedFolders.empty() == false)
        {
            auto folder = unusedFolders.front();
            folders.push_back(folder);
            folders.pop_front();
            folder->show();
        }
        while(folders.size() < folderCnt)
        {
            auto folder = new obj_frame(this);
            folders.push_back(folder);
            folder->show();
        }
    }
    else
    {
        while(folders.size() > folderCnt)
        {
            auto folder = folders.back();
            folder->hide();
            unusedFolders.push_front(folder);
            folders.pop_back();
        }
    }
}

void ShowPanel::resetObjVector(int fileCnt, int folderCnt)
{
    resetFileVector(fileCnt);
    resetFolderVector(folderCnt);
}

void ShowPanel::setFileInfo(const QJsonArray& jsonData)
{
    add();
    add();
    add();
    resetFileVector(jsonData.size());

    for(int i = 0; i < jsonData.size(); ++i)
    {
        qDebug() << jsonData[0]["id"];
        auto tmpData = jsonData[0].toObject();
        files[i]->resetInfo(true, tmpData["id"].toDouble(), tmpData["name"].toString(), tmpData["parentid"].toDouble(), QDate::fromString(tmpData["sharePeriod"].toString()),
                            tmpData["path"].toString(), tmpData["isShared"].toBool(), tmpData["isShareEncryped"].toBool(), tmpData["shareUrl"].toString() );
        files[i]->show();
    }
}

void ShowPanel::setFolderInfo(const QJsonArray& jsonData)
{
    resetFolderVector(jsonData.size());
}

void ShowPanel::rearrange()
{
    int row = 0, col = 0;
    for(auto folder : folders)
    {
        folder->setPosition(gap + col * (gap + obj_frame::fwidth), gap + row * (gap + obj_frame::fheight));
        ++col;
        if(col >= curCount_MaxRowObj)
        {
            col = 0;
            ++row;
        }
    }
    for(auto file : files)
    {
        file->setPosition(gap + col * (gap + obj_frame::fwidth), gap + row * (gap + obj_frame::fheight));
        ++col;
        if(col >= curCount_MaxRowObj)
        {
            col = 0;
            ++row;
        }
    }
    this->setMinimumHeight((row + 1) * (gap + obj_frame::fheight) + gap);
}

void ShowPanel::GetFolders()
{
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/folders");
    replyMap.insert(reply, requestType::GET_FOLDERS);
}

void ShowPanel::GetFolder(int id)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/folders/" + QString::number(id));
    replyMap.insert(reply, requestType::GET_FOLDER);
}

void ShowPanel::AddFolder(QString name, int parentid)
{
    QJsonObject data;
    data.insert("name", name);
    data.insert("parentid", parentid);
    QNetworkReply* reply = ServerConnect::getInstance().http_post("/folders", QJsonDocument(data));
    replyMap.insert(reply, requestType::ADD_FOLDER);
}
void ShowPanel::DeleteFolder(int id)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_post("/folders/" + QString::number(id));
    replyMap.insert(reply, requestType::DELETE_FOLDER);
}
void ShowPanel::UploadFile()
{
}
void ShowPanel::DeleteFile(int fileid)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_delete("/files/" + QString::number(fileid));
    replyMap.insert(reply, requestType::DELETE_FILE);
}
void ShowPanel::DownloadFile(int fileid)
{
}
void ShowPanel::SetObjShared(bool isFile, int objId, bool isShared, bool isShareEncryped)
{
    QJsonObject data;
    data.insert("isShare", isShared);
    data.insert("isShareEncryped", isShareEncryped);
    data.insert("sharePeriod", 7);
    QNetworkReply* reply = nullptr;
    if(isFile)
    {
        reply = ServerConnect::getInstance().http_patch("/files/" + QString::number(objId), QJsonDocument(data));
    }
    else
    {
        reply = ServerConnect::getInstance().http_patch("/folders/" + QString::number(objId), QJsonDocument(data));
    }
    replyMap.insert(reply, requestType::SET_OBJ_SHARED);
}
void ShowPanel::GetShareObjInfo(QString path)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/share/" + path);
    replyMap.insert(reply, requestType::GET_SHARE_OBJ_INFO);
}
void ShowPanel::DownloadShareObj()
{

}
void ShowPanel::SetShareObjPasswd(QString path, QString password)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/share/" + path + "?password=" + password);
    replyMap.insert(reply, requestType::SET_SHARED_OBJ_PASSWORD);
}
void ShowPanel::GetShareFolder(QString path, int folderid)
{

}
void ShowPanel::add()
{
    obj_frame* obj;
    if(rand() % 2)
    {
        obj = new obj_frame(this, true, 1, QString::number(rand() % 100) + ".txt", 2, QDate(2019, 11, 21));
        files.push_back(obj);
    }
    else
    {
        obj = new obj_frame(this, false, 1, QString::number(rand() % 100), 2, QDate(2019, 11, 21), "/ac/");
        folders.push_back(obj);
    }
    obj->show();
    rearrange();
}

void ShowPanel::requestCallback(QNetworkReply* reply)
{
    if(replyMap.contains(reply))
    {
        switch(replyMap[reply])
        {
            case requestType::GET_FOLDERS:
                {
                    auto data = reply->readAll();
                    auto jsonData = QJsonDocument::fromJson(data);
                    qDebug() << jsonData;
                }
                break;
            case requestType::GET_FOLDER:
                {
                    auto data = reply->readAll();
                    auto jsonData = QJsonDocument::fromJson(data);
                    setFileInfo(jsonData["files"].toArray());
//                    qDebug() << jsonData["subfolders"].toArray().size();
//                    qDebug() << jsonData["files"].toArray().size();
////                    resetObjVector(jsonData["files"].toArray().size(), jsonData["subfolders"].toArray().size());

//                    qDebug() << jsonData["subfolders"] << endl << endl;
//                    qDebug() << jsonData["subfolders"][0] << endl << endl;
//                    qDebug() << jsonData["subfolders"][0]["id"] << endl << endl;
//                    qDebug() << jsonData["files"];
                    rearrange();
                }
                break;
            case requestType::ADD_FOLDER:
                {

                } break;
            case requestType::DELETE_FOLDER:
                {

                } break;
            case requestType::UPLOAD_FILE:
                {

                } break;
            case requestType::DELETE_FILE:
                {

                } break;
            case requestType::DOWNLOAD_FILE:
                {

                } break;
            case requestType::GET_SHARE_OBJ_INFO:
                {

                } break;
            case requestType::GET_SHARE_FOLDER:
                {

                } break;
            case requestType::SET_OBJ_SHARED:
                {

                } break;
            case requestType::SET_SHARED_OBJ_PASSWORD:
                {

                } break;
            case requestType:: DOWNLOAD_SHAREOBJ:
                {

                } break;
        }
    }
}

void ShowPanel::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    if(lastWidth != this->geometry().width() || lastHeight != this->geometry().height())
    {
        lastWidth = this->geometry().width();
        lastHeight = this->geometry().height();
        curCount_MaxRowObj = (lastWidth - gap) / (obj_frame::fwidth + gap);
        rearrange();
    }
}
