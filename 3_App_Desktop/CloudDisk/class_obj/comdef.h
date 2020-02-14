#ifndef COMDEF_H
#define COMDEF_H

enum class requestType {GET_FOLDERS,
                        GET_FOLDER, GET_FOLDER_REFRESH, GET_FOLDER_LAST,
                        ADD_FOLDER,
                        DELETE_FOLDER,
                        UPLOAD_FILE, GET_FILE, DELETE_FILE, DOWNLOAD_FILE,
                        GET_SHARE_OBJ_INFO, GET_SHARE_FOLDER, SET_OBJ_SHARED, SET_SHARED_OBJ_PASSWORD, DOWNLOAD_SHAREOBJ
                       };

#endif // COMDEF_H
