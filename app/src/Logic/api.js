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
    }

}

export default Api;