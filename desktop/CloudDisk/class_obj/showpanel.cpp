#include "showpanel.h"


ShowPanel::ShowPanel(QWidget* parent) : QWidget(parent)
{
    objToolPalette = new QMenu(this);
    panelToolPalette = new QMenu(this);
//    objToolPalette->
    this->setStyleSheet("QWidget#show_panel{\
                           background-color: rgb(255, 255, 255);\
                            }");

    connect(ServerConnect::getInstance().getNetwordAccessManager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(requestCallback(QNetworkReply*)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createPanelToolPalette(const QPoint&)));
    refresh();
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
    while(unusedFiles.isEmpty() == false)
    {
        delete(unusedFiles.front());
        unusedFiles.pop_front();
    }
    while(unusedFolders.isEmpty() == false)
    {
        delete(unusedFolders.front());
        unusedFolders.pop_front();
    }
}

void ShowPanel::resetFileVector(int fileCnt)
{
    if(files.size() < fileCnt)
    {
        while(files.size() < fileCnt && unusedFiles.isEmpty() == false)
        {
            auto file  = unusedFiles.front();
            files.push_back(file);
            unusedFiles.pop_front();
        }
        while(files.size() < fileCnt)
        {
            auto file = new obj_frame(this);
            file->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(file, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createObjToolPalette(const QPoint&)));
            connect(file, SIGNAL(selected(Qt::MouseButton, obj_frame*)), this, SLOT(setSelected(Qt::MouseButton, obj_frame*)));
            connect(file, SIGNAL(open_obj(int)), this, SLOT(getFileInfo(int)));
            files.push_back(file);
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
        while(folders.size() < folderCnt && unusedFolders.isEmpty() == false)
        {
            auto folder = unusedFolders.front();
            folders.push_back(folder);
            unusedFolders.pop_front();
        }
        while(folders.size() < folderCnt)
        {
            auto folder = new obj_frame(this);
            folder->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(folder, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(createObjToolPalette(const QPoint&)));
            connect(folder, SIGNAL(selected(Qt::MouseButton, obj_frame*)), this, SLOT(setSelected(Qt::MouseButton, obj_frame*)));
            connect(folder, SIGNAL(open_obj(int)), this, SLOT(getFolderInfo(int)));
            folders.push_back(folder);
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
    resetFileVector(jsonData.size());
    for(int i = 0; i < jsonData.size(); ++i)
    {
        auto tmpData = jsonData[i].toObject();
        files[i]->resetInfo(true, tmpData["id"].toDouble(), tmpData["name"].toString(), tmpData["parentid"].toDouble(), QDate::fromString(tmpData["sharePeriod"].toString()),
                            tmpData["path"].toString(), tmpData["isShared"].toBool(), tmpData["isShareEncryped"].toBool(), tmpData["shareUrl"].toString() );
    }

}

void ShowPanel::setFolderInfo(const QJsonArray& jsonData)
{
    resetFolderVector(jsonData.size());
    for(int i = 0; i < jsonData.size(); ++i)
    {
        auto tmpData = jsonData[i].toObject();
        folders[i]->resetInfo(false, tmpData["id"].toDouble(), tmpData["name"].toString(), tmpData["parentid"].toDouble(), QDate::fromString(tmpData["sharePeriod"].toString()),
                              tmpData["path"].toString(), tmpData["isShared"].toBool(), tmpData["isShareEncryped"].toBool(), tmpData["shareUrl"].toString() );
    }
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

void ShowPanel::addLastFolder(long id)
{
    if(lastFolderId.isEmpty())
        emit enableBackbtn(true);
    lastFolderId.push_back(id);
}

long ShowPanel::getLastFolder()
{

    auto val = lastFolderId.back();
    lastFolderId.pop_back();
    if(lastFolderId.isEmpty())
        emit enableBackbtn(false);
    return val;
}

void ShowPanel::GetFolders()
{
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/folders");
    replyMap.insert(reply, requestType::GET_FOLDERS);
}

QNetworkReply* ShowPanel::GetFolder(long id)
{
    resetSelected();
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/folders/" + QString::number(id));
    return reply;
}

void ShowPanel::AddFolder(QString name, long parentid)
{
    QJsonObject data;
    data.insert("name", name);
    data.insert("parentid", (double)parentid);
    QNetworkReply* reply = ServerConnect::getInstance().http_post("/folders", QJsonDocument(data));
    replyMap.insert(reply, requestType::ADD_FOLDER);
}
void ShowPanel::DeleteFolder(long id)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_post("/folders/" + QString::number(id));
    replyMap.insert(reply, requestType::DELETE_FOLDER);
}
void ShowPanel::UploadFile()
{
}

QNetworkReply* ShowPanel::GetFile(long id)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_get("/files/" + QString::number(id) + "?query=info");
    return reply;
}

void ShowPanel::DeleteFile(long fileid)
{
    QNetworkReply* reply = ServerConnect::getInstance().http_delete("/files/" + QString::number(fileid));
    replyMap.insert(reply, requestType::DELETE_FILE);
}
void ShowPanel::DownloadFile(long fileid)
{
}
void ShowPanel::SetObjShared(bool isFile, long objId, bool isShared, bool isShareEncryped)
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
void ShowPanel::GetShareFolder(QString path, long folderid)
{

}
void ShowPanel::add()
{
    obj_frame* obj;
    if(rand() % 2)
    {
//        obj = new obj_frame(this, true, 1, QString::number(rand() % 100) + ".txt", 2, QDate(2019, 11, 21));
        obj  = new obj_frame(this);
        obj->setInfo(true, 1, QString::number(rand() % 100) + ".txt", 2, QDate(2019, 11, 21));
        connect(obj, SIGNAL(selected(obj_frame*)), this, SLOT(setSelected(obj_frame*)));
        files.push_back(obj);
    }
    else
    {
        obj = new obj_frame(this, false, 1, QString::number(rand() % 100), 2, QDate(2019, 11, 21), "/ac/");
        connect(obj, SIGNAL(selected(obj_frame*)), this, SLOT(setSelected(obj_frame*)));
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
                    setFolderInfo(jsonData["subfolders"].toArray());
//                    qDebug() << jsonData["subfolders"] << endl << endl;
//                    qDebug() << jsonData["files"];
                    rearrange();

                    auto curFolder = jsonData["data"].toObject();
                    addLastFolder(curFolderId);
                    curFolderId = curFolder["id"].toDouble();
                    upperFolderId = curFolder["parentid"].toDouble();
                    if(upperFolderId >= initFolderId)
                        emit enableUpperbtn(true);
                    else
                        emit enableUpperbtn(false);
                }
                break;
            case requestType::GET_FOLDER_REFRESH:
                {
                    auto data = reply->readAll();
                    auto jsonData = QJsonDocument::fromJson(data);
                    setFileInfo(jsonData["files"].toArray());
                    setFolderInfo(jsonData["subfolders"].toArray());
                    rearrange();
                }
                break;
            case requestType::GET_FOLDER_LAST:
                {

                    auto data = reply->readAll();
                    auto jsonData = QJsonDocument::fromJson(data);
                    setFileInfo(jsonData["files"].toArray());
                    setFolderInfo(jsonData["subfolders"].toArray());

                    rearrange();

                    auto curFolder = jsonData["data"].toObject();
                    curFolderId = curFolder["id"].toDouble();
                    upperFolderId = curFolder["parentid"].toDouble();
                    if(upperFolderId >= initFolderId)
                        emit enableUpperbtn(true);
                    else
                        emit enableUpperbtn(false);
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
            case requestType::GET_FILE:
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
            default:
                {
                    qDebug() << "???";
                }
        }
    }
}

void ShowPanel::setSelected(Qt::MouseButton info, obj_frame* of)
{
    if(info == Qt::LeftButton)
    {
        if( singleSelected )
        {
            resetSelected();
        }
        else
        {
            if(selectedObj.removeOne(of))
            {
                of->setUnselected();
                return;
            }
        }
        selectedObj.push_back(of);
        of->setSelected();
    }
    else if(info == Qt::RightButton)
    {
        if(selectedObj.contains(of) == false)
        {
            resetSelected();
            selectedObj.push_back(of);
            of->setSelected();
        }
    }
    emit enableObjbtn(true);
}

void ShowPanel::resetSelected()
{
    while(selectedObj.isEmpty() == false)
    {
        selectedObj.front()->setUnselected();
        selectedObj.pop_front();
    }
    emit enableObjbtn(false);
}


void ShowPanel::createObjToolPalette(const QPoint&)
{
    qDebug() << "obj";
    objToolPalette->exec(QCursor::pos());
}

void ShowPanel::createPanelToolPalette(const QPoint&)
{
    qDebug() << "panel";
    panelToolPalette->exec(QCursor::pos());
}

void ShowPanel::refresh()
{
    replyMap.insert(GetFolder(curFolderId), requestType::GET_FOLDER_REFRESH);
}

void ShowPanel::getLastFolderInfo()
{
    replyMap.insert(GetFolder(getLastFolder()), requestType::GET_FOLDER_LAST);
}

void ShowPanel::getUpperFolderInfo()
{
    replyMap.insert(GetFolder(upperFolderId), requestType::GET_FOLDER);
}

void ShowPanel::getFolderInfo(int id)
{
    if(id == curFolderId)
        return;
    replyMap.insert(GetFolder(id), requestType::GET_FOLDER);
}

void ShowPanel::getFileInfo(int id)
{
    replyMap.insert(GetFile(id), requestType::GET_FILE);
}


void ShowPanel::mousePressEvent(QMouseEvent* event)
{
//    if(event->button() == Qt::LeftButton)
    this->resetSelected();
}

void ShowPanel::keyReleaseEvent(QKeyEvent* event)
{
    qDebug() << "release";
    qDebug() << (Qt::Key)event->key();
    if(event->key() == Qt::Key_Control)
        singleSelected = true;
}

void ShowPanel::keyPressEvent(QKeyEvent* event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        qDebug() << "set false";
        singleSelected = false;
    }
    else
    {

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
