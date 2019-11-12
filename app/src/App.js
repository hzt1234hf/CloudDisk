import React, {Component} from 'react';
import Cookies from 'js-cookie';
import Api from './Logic/api';

import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap/dist/js/bootstrap.min';
import {Alert, Container, Row, Navbar, Button, Form, BDiv, BSpan} from 'bootstrap-4-react';
import 'jquery';
import 'react-popper';
import {BrowserRouter as Router, Route} from "react-router-dom";
import {FontAwesomeIcon} from '@fortawesome/react-fontawesome';
import {faFile, faFolder} from '@fortawesome/free-solid-svg-icons';

import LoginForm from './component/LoginForm';
import MainPanel from './component/MainPanel';

class MainPage extends Component {
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
        Cookies.set('token', data.token, {expires: 7});
        this.setState({
            login: true
        })
    }

    clearCookie() {
        let token = Cookies.get('token');
        if (token) {
            Cookies.set('token', null);
            console.log("清除成功！");
            window.location.reload();
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
            <Container>
                <Navbar expand="lg" light bg="light">
                    <Navbar.Brand href="#">
                        CloudDisk - 私有云硬盘
                    </Navbar.Brand>
                    <Button primary onClick={this.clearCookie}>清除Cookie</Button>
                </Navbar>
                {content}
            </Container>
        );
    }
}

class SharePage extends Component {
    constructor(props) {
        super(props);
        this.state = {
            objPath: "",
            objName: "",
            objIsShared: false,
            objIsFile: false,
            objNeedPassword: false,
        };
        this.password = null;
    }

    getObjInfo() {
        const path = this.props.match.params.path;
        Api.getShareObjInfo(path).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    this.setState({
                        objPath: responseJson.data.path,
                        objName: responseJson.data.name,
                        objIsShared: responseJson.data.isShared,
                        objIsFile: responseJson.data.isFile,
                        objNeedPassword: responseJson.data.needPassword,
                    });
                })
            }
        });
    }

    setOjbPassword(path, password) {
        Api.setShareObjPasswd(path, password).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    Cookies.set('shareToken', responseJson.data.token, {expires: 1});
                    this.getObjInfo();
                });
            }
        });
    }

    clearCookie() {
        Cookies.set('shareToken', null, {expires: 0});
        window.location.reload();
    }

    componentDidMount() {
        this.getObjInfo();
    }

    render() {
        console.log("update");
        let content = (
            <BDiv h="75" w="100" display="flex" justifyContent="center" alignItems="center">
                <BDiv float="left" m="3">
                    <FontAwesomeIcon icon={faFile} size={"6x"}/>
                </BDiv>
                <BDiv float="right" m="3">
                    <BDiv style={{wdith: "300px"}} className="text-wrap" m="1">
                        <p>{this.state.objIsFile ? "文件名： " : "文件夹名： "} {this.state.objName}</p>
                    </BDiv>
                    <BDiv m="1">
                        <Button primary onClick={() => {
                            window.open(Api.downloadShareObj(this.props.match.params.path), '__blank');
                        }}> 下 载
                        </Button>
                        <Button m="1" primary onClick={this.clearCookie}>清除Cookie</Button>
                    </BDiv>
                </BDiv>
            </BDiv>
        );
        if (this.state.objIsShared === false)
            content = (
                <BDiv h="75" w="100" display="flex" justifyContent="center" alignItems="center">
                    <BDiv>
                        <BSpan>地址访问错误！</BSpan>
                    </BDiv>
                </BDiv>
            );
        if (this.state.objNeedPassword === true) {
            content = (
                <BDiv h="75" w="100" display="flex" justifyContent="center" alignItems="center">
                    <BDiv>
                        <Row>
                            <p>{this.state.objIsFile ? "文件名： " : "文件夹名： "} {this.state.objName}</p>
                        </Row>
                        <Row>
                            <BDiv display="flex">
                                {/*<label htmlFor="exampleInputPassword1">密码：</label>*/}
                                <Form.Input m="1" type="password" id="inputPassword" placeholder="请输入分享密码"
                                            onChange={evt => this.password = evt.target.value}/>
                                <Button m="1" primary onClick={() =>
                                    this.setOjbPassword(this.state.objPath, this.password)
                                }>提 交</Button>
                                <Button m="1" primary onClick={this.clearCookie}>清除Cookie</Button>
                            </BDiv>
                        </Row>
                    </BDiv>
                </BDiv>
            );
        }

        return (
            <Container style={{position: "fixed", top: "0", left: "0", right: "0", bottom: "0"}}>
                {content}
            </Container>
        );
    }
}

const App = () => (
    <Router>
        <Route path="/" exact component={MainPage}/>
        <Route path="/s/:path" component={SharePage}/>
    </Router>
);

export default App;