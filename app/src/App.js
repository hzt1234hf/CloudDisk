import React, {Component} from 'react';
import Cookies from 'js-cookie';
import Api from './Logic/api';

import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap/dist/js/bootstrap.min';
import 'jquery';
import 'react-popper'
import {BrowserRouter as Router, Route} from "react-router-dom";

import LoginForm from './component/LoginForm';
import MainPanel from './component/MainPanel';
import Octicon, {File} from "@primer/octicons-react";

class MainPage extends Component {
    constructor(props) {
        super(props);
        this.state = {
            login: false
        };
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

    clearCookie() {
        let token = Cookies.get('token');
        if (token) {
            Cookies.set('token', null);
            console.log("清除成功！");
            window.location.href = "/";
        }
    }

    render() {
        if (this.state.login) {
            return <MainPanel title="主页面"/>;
        } else {
            return <LoginForm onLogin={this.onLogin}/>
        }
    }
}

class SharePage extends Component {
    constructor(props) {
        super(props);
        this.state = {
            file: null
        };
    }

    componentDidMount() {
        const path = this.props.match.params.path;
        console.log(this.props.match);
        console.log(this.props.match.params);
        console.log(this.props.match.params.path);
        Api.getFileInfoWithShareUrl(path).then(response => {
            response.json().then(responseJson => {
                this.setState({
                    file: responseJson.data
                });
                let token = null;
                token = Cookies.get('token');
                // if ("null" !== token) {
                //     return;
                // }
                Cookies.set('token', responseJson.data.token, {expires: 1});
                console.log(Cookies.get('token'));
            });
        });
    }

    render() {
        if (!this.state.file) {
            return (
                <div className="row">
                    <div className="col-4 offset-4">
                        <span>文件不存在！</span>
                    </div>
                </div>
            );
        }
        return (
            <div className="row">
                <div className="col-2 offset-5">
                    <table className="table table-borderless w-100">
                        <tbody>
                        <tr>
                            <td rowspan="2"><Octicon className="ml-1 mr-1" icon={File}/></td>
                            <td>{this.state.file.filename}</td>
                        </tr>
                        <tr>
                            <td>
                                <button className="btn btn-primary" onClick={() => {
                                    // window.open(Api.generateShareFileDownloadUrl(this.props.match.params.path), "_blank");
                                    window.open(Api.generateShareFileDownloadUrl(this.props.match.params.path), '_blank');
                                }}>
                                    下 载
                                </button>
                            </td>
                        </tr>
                        </tbody>
                    </table>
                </div>
            </div>
        )
    }
}

const App = () => (
    <div className="container">
        <nav className="nav p-3 mb-2 bg-light text-dark">
            <a className="nav-link active" href="/">CloudDisk - 私有云硬盘</a>
            <button type="button" className="btn btn-primary" onClick={() => {
                let token = Cookies.get('token');
                if (token) {
                    Cookies.set('token', null);
                    console.log("清除成功！");
                    window.location.href = "/";
                }
            }}>清除Cookie
            </button>
        </nav>
        <Router>
            <Route path="/" exact component={MainPage}/>
            <Route path="/s/:path" component={SharePage}/>
        </Router>
    </div>
);
export default App;