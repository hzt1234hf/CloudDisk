const Api = {
    BASE_API: 'http://localhost:5000',

    login(email, password) {
        return fetch(this.BASE_API + "/login", {
            method: 'POST',
            body: JSON.stringify({email: email, password: password}),
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
        });
    },

    // 由于跨域的问题，我们暂时不能使用 Cookie 来认证，这里改用 Header 进行认证
    auth(token) {
        return fetch(this.BASE_API + "/auth", {
            method: 'GET',
            headers: {
                'Authorization': token,
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
        });
    },

    getFolders() {
        return fetch(this.BASE_API + "/folders", {
            method: 'GET',
            headers: {
                'Accept': 'application/json'
            }
        });
    },
    getFolder(name) {
        return fetch(this.BASE_API + '/folders/' + name, {
            method: 'GET',
            headers: {
                'Accept': 'application/json'
            }
        });
    },
    addFolder(name) {
        return fetch(this.BASE_API + "/folders", {
            method: 'POST',
            body: JSON.stringify({name: name}),
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            }
        });
    },
    deleteFolder(name) {
        return fetch(this.BASE_API + '/folders/' + name, {
            method: 'DELETE',
            headers: {
                'Accept': 'application/json'
            }
        });
    },
    uploadFile(folder, file) {
        let data = new FormData();
        data.append('file', file);
        return fetch(this.BASE_API + '/folders/' + folder, {
            method: 'POST',
            body: data,
            headers: {
                'Accept': 'application/json'
            }
        });
    },
    deleteFile(folder, file) {
        return fetch(this.BASE_API + '/folders/' + folder + '/' + file, {
            method: 'DELETE',
            headers: {
                'Accept': 'application/json',
            }
        });
    },
    downloadFile(folder, file) {
        return this.BASE_API + '/folders/' + folder + '/' + file;
    },
    getFileInfo(folder, file) {
        return fetch(this.BASE_API + '/folders/' + folder + '/' + file + "?query=info", {
            method: 'GET',
            headers: {
                'Accept': 'application/json',
            }
        });
    },
    generateShareFileDownloadUrl(url) {
        return this.BASE_API + "/share/" + url + "?download=true";
    },

    updateFileShareType(folder, filename, shareType) {
        return fetch(this.BASE_API + "/folders/" + folder + "/" + filename + "?shareType=" + shareType, {
            method: 'PATCH',
            headers: {
                'Accept': 'application/json',
            },
        });
    },

    getFileInfoWithShareUrl(url, password = null) {
        let addr = this.BASE_API + "/share/" + url;
        if (password != null)
            addr += "?password=" + password;
        return fetch(addr, {
            method: 'GET',
            headers: {
                'Accept': 'application/json',
            }
        });
    }
}

export default Api;