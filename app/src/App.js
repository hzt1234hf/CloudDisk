import React, {Component} from 'react';
import Cookies from 'js-cookie';
import Api from './Logic/api';

import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap/dist/js/bootstrap.min';
import 'jquery';
import 'react-popper';
import swal from 'sweetalert';
import {BrowserRouter as Router, Route} from "react-router-dom";

import {FontAwesomeIcon} from '@fortawesome/react-fontawesome'
import {faFile} from '@fortawesome/free-solid-svg-icons'

import LoginForm from './component/LoginForm';
import MainPanel from './component/MainPanel';


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
        Cookies.set('token', data.token, {expires: 7});
        this.setState({
            login: true
        })
    }

    clearCookie() {
        let token = Cookies.get('token');
        if (token) {
            Cookies.set('token', null);
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
        Api.getFileInfoWithShareUrl(path).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    this.setState({
                        file: responseJson.data
                    });
                    if (responseJson.data.open_private_share) {
                        swal({
                            text: "请输入密码",
                            content: {
                                element: "input",
                                attributes: {
                                    placeholder: "输入密码",
                                    type: "password",
                                },
                            },
                            button: {
                                text: "确认",
                                closeModal: false,
                                closeOnEsc: false,
                            },
                            closeOnClickOutside: false,
                        }).then(password => {
                            Api.getFileInfoWithShareUrl(path, password).then(response => {
                                response.json().then(responseJson => {
                                    if (responseJson.data.token) {
                                        Cookies.set('token', responseJson.data.token, {expires: 1});
                                        swal.close();
                                    } else {
                                        swal("密码错误！");
                                    }
                                });
                            });
                        })
                    }
                    Cookies.set('token', responseJson.data.token, {expires: 1});
                });
            }
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
            <div className="d-flex justify-content-center align-content-center">
                <tbody>
                <tr>
                    <td rowspan="2"><FontAwesomeIcon icon={faFile} size={"4x"}/></td>
                    <td>{this.state.file.filename}</td>
                </tr>
                <tr>
                    <td className="d-flex justify-content-center align-content-center">
                        <button className="btn btn-primary w-75" onClick={() => {
                            // window.open(Api.generateShareFileDownloadUrl(this.props.match.params.path), "_blank");
                            window.open(Api.generateShareFileDownloadUrl(this.props.match.params.path), '_blank');
                        }}>
                            下 载
                        </button>
                    </td>
                </tr>
                </tbody>
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
                    window.location.href = "/";
                }
            }}>手动清除Cookie
                {/*（分享和登录的cookie）*/}
            </button>
        </nav>
        <Router>
            <Route path="/" exact component={MainPage}/>
            <Route path="/s/:path" component={SharePage}/>
        </Router>
    </div>
);
export default App;