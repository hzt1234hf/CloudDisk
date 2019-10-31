import React, {Component} from 'react';
import Cookies from 'js-cookie';
import Api from './Logic/api';

import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap/dist/js/bootstrap.min';
import 'jquery';
import 'react-popper'

import LoginForm from './component/LoginForm';
import MainPanel from './component/MainPanel';

class App extends Component {
    constructor(props) {
        super(props);
        this.state = {
            login: false
        }
        this.onLogin = this.onLogin.bind(this);
    }

    componentDidMount() {
        let token = Cookies.get('token');
        if (token) {
            Api.auth(token).then(response => {
                if (response.ok) {
                    this.setState({
                        login: true
                    })
                }
            })
        }
    }

    onLogin(data) {
        console.log(data);
        Cookies.set('token', data.token, {expires: 7});
        console.log(data.token);
        this.setState({
            login: true
        })
    }
    clearCookie(){
        let token = Cookies.get('token');
        if(token)
        {
            Cookies.set('token',null);
            console.log("清除成功！");
            window.location.href = "/";
        }
    }

    render() {
        var content;
        if (this.state.login) {
            content = <MainPanel title="主页面"/>;
        } else {
            content = <LoginForm onLogin={this.onLogin}/>
        }
        return (
            <div className="container">
                <nav className="nav p-3 mb-2 bg-light text-dark">
                    <a className="nav-link active" href="/">CloudDisk - 私有云硬盘</a>
                    <button type="button" className="btn btn-primary" onClick={this.clearCookie}>清除Cookie</button>
                </nav>
                {/*<div className="row justify-content-center bg-light" style={{height: "500px"}}>*/}
                {/*    <div className="align-self-center">*/}
                {/*        水平垂直居中*/}
                {/*    </div>*/}
                {/*</div>*/}
                {content}
            </div>
        );
    }
}

export default App;