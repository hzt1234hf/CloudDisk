import React, {Component} from 'react';
import Cookies from 'js-cookie';
import Api from './Logic/api';

import './App.css';
import 'bootstrap/dist/css/bootstrap.min.css';
import 'bootstrap/dist/js/bootstrap.min';
import {Alert, Container, Row, Navbar, Button, Form, BDiv, BSpan, Col, ListGroup, BA} from 'bootstrap-4-react';
import 'jquery';
import 'react-popper';
import {BrowserRouter as Router, Route} from "react-router-dom";
import {FontAwesomeIcon} from '@fortawesome/react-fontawesome';
import {faFile, faFolder} from '@fortawesome/free-solid-svg-icons';

import LoginForm from './component/LoginForm';
import MainPanel from './component/MainPanel';
import $ from "jquery";

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
            objName: "",
            objIsShared: false,
            objIsFile: false,
            objNeedPassword: false,
            objPassword: null,

            folders: [],
            files: [],

            curFolderId: 0,
            selectedFolderId: 1,    // 选中目录ID
            rootFolders: [{'id': 1, 'name': '全部文件'}],        // 地址列表
        };
        this.password = null;
        this.rootPath = this.props.match.params.path;
    }

    getObjInfo() {
        Api.getShareObjInfo(this.rootPath).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    console.log(responseJson);
                    this.setState({
                        objName: responseJson.data.name,
                        objIsShared: responseJson.data.isShared,
                        objIsFile: responseJson.data.isFile,
                        objNeedPassword: responseJson.data.needPassword,
                        objPassword: responseJson.data.password
                    });
                    if (responseJson.data.needPassword === false && responseJson.data.isFile === false) {
                        this.refreshFolderAndFileList();
                    }
                })
            }
        });
    }

    setOjbPassword(path, password) {
        Api.setShareObjPasswd(path, password).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    Cookies.set('shareToken', responseJson.token, {expires: 1});
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


    refreshFolderAndFileList() {
        console.log("refresh");
        Api.getShareFolder(this.rootPath, this.state.selectedFolderId).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    console.log(responseJson);
                    this.setState({
                        curFolderId: this.state.selectedFolderId,
                        folders: responseJson.folders,
                        files: responseJson.files,
                    });
                });
            }
        });
    }

    downloadFile(fileid) {
        window.open(Api.downloadFile(fileid), "_blank");
    }

    changeToParentSelectedFolder(index) {
        if (this.state.selectedFolderId === this.state.rootFolders[index].id) {
            return;
        }
        while (this.state.rootFolders.length - 1 > index)
            this.state.rootFolders.pop();
        console.log(this.state.rootFolders);
        console.log(index);
        this.setState({
            selectedFolderId: this.state.rootFolders[index].id
        }, () => {
            this.refreshFolderAndFileList();
        });
    }

    changeToChildSelectedFolder(folder) {
        console.log(this.state.selectedFolderId);
        console.log(folder.id);
        if (this.state.selectedFolderId === folder.id) {
            return;
        }
        this.state.rootFolders.push(folder);
        this.setState({
            selectedFolderId: folder.id
        }, () => {
            this.refreshFolderAndFileList();
        });
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
                            window.open(Api.downloadShareObj(this.props.match.params.path, this.state.objPassword), '__blank');
                        }}> 下 载
                        </Button>
                        <Button m="1" primary onClick={this.clearCookie}>清除Cookie</Button>
                    </BDiv>
                </BDiv>
            </BDiv>
        );
        if (this.state.objIsFile === false) {

            let folderInfo;
            let rootFolderList = this.state.rootFolders.map((folder, index) => {
                return (
                    <span key={folder.id}>
                        {index === 0 ? '' : '>'}
                        <a href="#" className="folderItem ml-1" onClick={() => {
                            this.changeToParentSelectedFolder(index);
                        }}>
                            {folder.name}
                        </a>
                    </span>
                );
            });

            folderInfo = (
                <Row alignSelf="center" p="2">
                    <Col col="4" display="flex" alignSelf="center">
                        <div className="">
                            <BSpan className="">当前目录：</BSpan>
                            {rootFolderList}
                        </div>
                    </Col>
                </Row>
            );

            let folderList = this.state.folders.map(folder => {
                return (
                    <ListGroup.Item display="flex" justifyContent="between" alignItems="center"
                                    key={folder.id}>
                        <BA href="#" className="folderItem" display="flex" float="left" alignItems="center"
                            onClick={() => {
                                this.changeToChildSelectedFolder(folder)
                            }}>
                            <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                            <BSpan ml="1" mr="1">{folder.name}</BSpan>
                        </BA>
                    </ListGroup.Item>
                );
            });
            let fileList = this.state.files.map(file => {
                return (
                    <ListGroup.Item display="flex" justifyContent="between" alignItems="center"
                                    key={file.id}>
                        <BA href="#" display="flex" alignItems="center" float="left" className="fileItem">
                            <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFile}/>
                            {file.name}
                        </BA>
                        <BDiv float="right">
                            <Button primary mr="1" ml="1"
                                    onClick={() => this.downloadFile(file.id)}>
                                <BSpan aria-hidden="true">下 载</BSpan>
                            </Button>
                        </BDiv>
                    </ListGroup.Item>
                );
            });
            content = (
                <div className="container-fluid w-100 p-4 mt-3">
                    <Row>
                        {/*<BDiv h="75" w="100" display="flex" justifyContent="center" alignItems="center">*/}
                        <BDiv m="3">
                            <FontAwesomeIcon icon={faFolder} size={"6x"}/>
                        </BDiv>
                        <BDiv m="3">
                            <BDiv style={{wdith: "300px"}} className="text-wrap" m="1">
                                <p>{this.state.objIsFile ? "文件名： " : "文件夹名： "} {this.state.objName}</p>
                            </BDiv>
                            <BDiv m="1">
                                <span>请使用下载工具下载</span>
                                <Button m="1" primary onClick={this.clearCookie}>清除Cookie</Button>
                            </BDiv>
                        </BDiv>
                        {/*</BDiv>*/}
                    </Row>
                    <hr/>
                    {folderInfo}
                    <hr/>
                    <Row p="2">
                        <Col col="4">
                            <ListGroup>
                                <ListGroup.Link display="flex" justifyContent="center" alignItems="center">
                                    <BSpan className="font-set">文件夹</BSpan>
                                </ListGroup.Link>
                                {folderList}
                            </ListGroup>
                        </Col>
                        <Col col="7" offset="1">
                            <ListGroup>
                                <ListGroup.Link display="flex" justifyContent="center" alignItems="center">
                                    <BSpan className="font-set">文件</BSpan>
                                </ListGroup.Link>
                                {fileList}
                            </ListGroup>
                        </Col>
                    </Row>
                </div>
            );
        }
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
                                    this.setOjbPassword(this.rootPath, this.password)
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