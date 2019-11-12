import React, {Component} from 'react';
import Api from '../Logic/api'
import './MainPanel.css';
import swal from 'sweetalert';
import $ from 'jquery';
import {Bootstrap, Alert, Row, Col, Button, BDiv, Form, ListGroup, BA, BSpan, BTd, BTr, Modal} from 'bootstrap-4-react';
import {FontAwesomeIcon} from '@fortawesome/react-fontawesome';
import {faFile, faFolder, faFileDownload, faFileUpload} from '@fortawesome/free-solid-svg-icons';
import QRCode from 'qrcode.react';

class MainPanel extends Component {
    constructor(props) {
        super(props);
        this.state = {
            folders: [],
            files: [],

            showAddFolderDialog: false,
            addFolderError: false,
            showUploadFileDialog: false,
            uploadFileError: false,

            showShareDialog: false,
            selectedShareObj: '',


            curFolder: null,
            selectedFolderId: 0,    // 选中目录ID
            rootFolders: [],        // 地址列表
        };
        this.addFolder = this.addFolder.bind(this);
        this.uploadFile = this.uploadFile.bind(this);

        this.rootPath = window.location.protocol + '//' + window.location.host;
    }

    refreshFolderAndFileList() {
        console.log("refresh");
        Api.getFolder(this.state.selectedFolderId).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    this.setState({
                        curFolder: responseJson.data,
                        folders: responseJson.subfolders,
                        files: responseJson.files,
                    });
                    if (this.state.rootFolders.length === 0)
                        this.setState({
                            rootFolders: [responseJson.data]
                        });
                });
            }
        });
    }

    addFolder(folderName = null) {
        if (folderName == null)
            folderName = this.newFolderName;
        Api.addFolder(folderName, this.state.curFolder.id).then(response => {
            this.setState({
                addFolderError: !response.ok,
            });
            if (response.ok) {
                this.setState({
                    showAddFolderDialog: false
                });
                this.refreshFolderAndFileList();
            }
        });
    }

    deleteFolder(folderName, folderId) {
        swal({
            title: "确定删除文件夹 " + folderName + " 吗？",
            text: "删除后文件夹及其内容不可恢复！",
            icon: "warning",
            buttons: {
                cancel: {
                    text: "取消",
                    value: false,
                    visible: true,
                    className: "",
                    closeModal: true,
                },
                confirm: {
                    text: "确定",
                    value: true,
                    visible: true,
                    className: "",
                    closeModal: true
                }
            },
            dangerMode: true,
        }).then((willDelete) => {
            if (willDelete) {
                Api.deleteFolder(folderId).then(response => {
                    if (response.ok) {
                        swal("文件夹删除成功.", {
                            icon: "success",
                        });
                        this.refreshFolderAndFileList();
                    }
                })
            }
        })
    }

    changeToChildSelectedFolder(folder) {
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

    changeToParentSelectedFolder(index) {
        if (this.state.selectedFolderId === this.state.rootFolders[index].id) {
            return;
        }
        while (this.state.rootFolders.length - 1 > index)
            this.state.rootFolders.pop();
        this.setState({
            selectedFolderId: this.state.rootFolders[index].id
        }, () => {
            this.refreshFolderAndFileList();
        });
    }

    uploadFile() {
        Api.uploadFile(this.state.curFolder.id, this.newUploadFile).then(response => {
            this.setState({
                uploadFileError: !response.ok
            });
            if (response.ok) {
                this.setState({
                    showUploadFileDialog: false
                });
                this.refreshFolderAndFileList();
            }
        });
    }

    deleteFile(filename, fileid) {
        swal({
            title: "确定删除文件 " + filename + " 吗？",
            text: "删除后文件不可恢复！",
            icon: "warning",
            buttons: {
                cancel: {
                    text: "取消",
                    value: false,
                    visible: true,
                    className: "",
                    closeModal: true,
                },
                confirm: {
                    text: "确定",
                    value: true,
                    visible: true,
                    className: "",
                    closeModal: true
                }
            },
            dangerMode: true,
        }).then(willDelete => {
            if (willDelete) {
                Api.deleteFile(fileid).then(response => {
                    if (response.ok) {
                        swal("文件删除成功.", {
                            icon: "success",
                        });
                        this.refreshFolderAndFileList();
                    }
                });
            }
        });
    }

    downloadFile(fileid) {
        window.open(Api.downloadFile(fileid), "_blank");
    }


    componentDidMount() {
        this.refreshFolderAndFileList();
    }


    render() {
        console.log("update");
        let folderInfo;
        let rootFolderList = this.state.rootFolders.map((folder, index) => {
            return (
                <a href="#" className="folderItem ml-1" key={folder.id} onClick={() => {
                    this.changeToParentSelectedFolder(index);
                }}>
                    {folder.name}
                </a>
            )
        });
        if (this.state.curFolder != null) {
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
        } else {
            folderInfo = <BSpan></BSpan>
        }
        let folderList = this.state.folders.map(folder => {
            return (
                <ListGroup.Item display="flex" justifyContent="between" alignItems="center"
                                key={folder.id}>

                    <BA href="#" className="folderItem" display="flex" float="left" alignItems="center" onClick={() => {
                        this.changeToChildSelectedFolder(folder)
                    }}>
                        <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                        <BSpan ml="1" mr="1">{folder.name}</BSpan>
                    </BA>
                    <BDiv float="right">
                        <Button danger mr="1" ml="1" aria-label="Close"
                                onClick={() => this.deleteFolder(folder.name, folder.id)}>
                            <BSpan aria-hidden="true">删 除</BSpan>
                        </Button>
                        <Button primary mr="1" ml="1" data-toggle="modal" data-target="#shareModal"
                                onClick={() => {
                                    this.setState({showShareDialog: true, selectedShareObj: folder});
                                    this.setShared = folder.isShared;
                                    this.setSharePassword = folder.isShareEncryped;
                                    $('#sharedModalSwitch1').prop("checked", folder.isShared);
                                    $('#sharedModalSwitch2').prop("checked", folder.isShareEncryped);
                                }}>
                            分 享
                        </Button>
                    </BDiv>
                </ListGroup.Item>
            );
        });

        let addFolderError;
        if (this.state.addFolderError) {
            addFolderError = (
                <Alert danger className="alert-dismissible fade show" role="alert">
                    <strong>添加文件夹失败！</strong>请确认名称是否无重复。
                </Alert>
            );
        } else {
            addFolderError = <BSpan/>;
        }

        let fileList = this.state.files.map(file => {
            return (
                <ListGroup.Item display="flex" justifyContent="between" alignItems="center"
                                key={file.id}>
                    <BA href="#" display="flex" alignItems="center" float="left" className="fileItem">
                        <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFile}/>
                        {file.name}
                    </BA>
                    <BDiv float="right">

                        <Button danger mr="1" ml="1"
                                onClick={() => this.deleteFile(file.name, file.id)}>
                            <BSpan aria-hidden="true">删 除</BSpan>
                        </Button>
                        <Button primary mr="1" ml="1"
                                onClick={() => this.downloadFile(file.id)}>
                            <BSpan aria-hidden="true">下 载</BSpan>
                        </Button>
                        <Button primary mr="1" ml="1" data-toggle="modal" data-target="#shareModal"
                                onClick={() => {
                                    this.setState({showShareDialog: true, selectedShareObj: file});
                                    this.setShared = file.isShared;
                                    this.setSharePassword = file.isShareEncryped;
                                    let sharedModalSwitch2 = $('#sharedModalSwitch2');
                                    $('#sharedModalSwitch1').prop("checked", file.isShared);
                                    sharedModalSwitch2.prop("checked", file.isShareEncryped);
                                    if (this.setShared)
                                        sharedModalSwitch2.removeAttr("disabled");
                                    else
                                        sharedModalSwitch2.attr("disabled", "disabled");
                                }}>
                            <BSpan aria-hidden="true">分 享</BSpan>
                        </Button>
                    </BDiv>
                </ListGroup.Item>
            );
        });

        let uploadFileError;
        if (this.state.uploadFileError) {
            uploadFileError = (
                <Alert danger className="alert-dismissible fade show" role="alert">
                    <strong>上传文件失败！</strong>请确认是否文件名重复，或服务器运行和网络连接是否正常。
                </Alert>
            );
        } else {
            uploadFileError = <BSpan/>;
        }

        let shareInfo = <BSpan/>;
        if (this.state.selectedShareObj !== null && this.state.selectedShareObj.isShared === true) {
            if (this.state.selectedShareObj.isShareEncryped === true) {
                shareInfo = (
                    <Row w="100" pl="4" pr="4" display="flext" justifyContent="between" alignItems="center">
                        <table className="table table-borderless d-flex justify-content-between align-items-center">
                            <tbody>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">链接：</td>
                                <td colSpan={"3"} className="text-break p-1">
                                    <BA href={this.rootPath + "/s/" + this.state.selectedShareObj.shareUrl}>
                                        {this.rootPath + "/s/" + this.state.selectedShareObj.shareUrl}
                                    </BA>
                                </td>
                            </tr>
                            <tr display={"none"}>
                                <td className="text-nowrap p-1 align-middle">密码：</td>
                                <td colSpan={"3"} className="text-nowrap p-1">
                                    <Form.Input type="text" value={this.state.selectedShareObj.sharePassword}
                                                onChange={evt => {
                                                    this.sharedPassword = evt.target.value;
                                                }}/>
                                </td>
                            </tr>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">二维码：</td>
                                <td className="text-nowrap p-1" colSpan={"3"}>
                                    <QRCode
                                        value={window.location + "s/" + this.state.selectedShareObj.shareUrl}
                                        size={64} bgColor={'#ffffff'}
                                        fgColor={'#000000'}
                                        level={'L'}/>
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </Row>
                );
            } else {
                shareInfo = (
                    <Row w="100" pl="4" pr="4" display="flex" justifyContent="between" alignItems="center">
                        <table className="table table-borderless d-flex justify-content-between align-items-center">
                            <tbody>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">链接：</td>
                                <td colSpan={"3"} className="text-break p-1">
                                    <BA href={this.rootPath + "/s/" + this.state.selectedShareObj.shareUrl}>
                                        {this.rootPath + "/s/" + this.state.selectedShareObj.shareUrl}
                                    </BA>
                                </td>
                            </tr>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">密码：</td>
                                <td className="text-nowrap p-1"
                                    colSpan={"3"}>无
                                </td>

                            </tr>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">二维码：</td>
                                <td className="text-nowrap p-1" colSpan={"3"}>
                                    <QRCode
                                        value={window.location + "s/" + this.state.selectedShareObj.shareUrl}
                                        size={64} bgColor={'#ffffff'}
                                        fgColor={'#000000'}
                                        level={'L'}/>
                                </td>
                            </tr>
                            </tbody>
                        </table>
                    </Row>
                );
            }
        }

        // if (this.state.showAddFolderDialog)
        //     $('#addFolderModal').modal('show');
        // else
        //     $('#addFolderModal').modal('hide');
        // if (this.state.showUploadFileDialog)
        //     $('#addFileModal').modal('show');
        // else
        //     $('#addFileModal').modal('hide');
        // if (this.state.showShareDialog)
        //     $('#shareModal').modal('show');
        // else
        //     $('#shareModal').modal('hide');

        return (
            <div className="container-fluid w-100 p-4 mt-3">
                <Row p="2" justifyContent="end">
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <Col col="2">
                        <Button primary w="100" p="2" display="flex" alignItems="center"
                                justifyContent="around" data-toggle="modal" data-target="#addFolderModal"
                                onClick={() => this.setState({showAddFolderDialog: true})}>
                            <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                            添加文件夹
                        </Button>
                    </Col>
                    {/*</div>*/}
                    <Col col="2">
                        <Button primary w="100" p="2" display="flex" alignItems="center"
                                justifyContent="around" data-toggle="modal" data-target="#addFileModal"
                                onClick={() => this.setState({showUploadFileDialog: true})}>
                            <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                            上传文件
                        </Button>
                    </Col>
                </Row>
                <hr/>
                {folderInfo}
                <Row p="2">
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <Col col="4">
                        <ListGroup>
                            {folderList}
                        </ListGroup>
                        {/*</div>*/}
                    </Col>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <Col col="7" offset="1">
                        <ListGroup>
                            {fileList}
                        </ListGroup>
                    </Col>
                    {/*</div>*/}
                </Row>

                <div>
                    <Modal id="addFolderModal" fade>
                        <Modal.Dialog centered>
                            <Modal.Content>
                                <Modal.Header>
                                    <Modal.Title>添加文件夹</Modal.Title>
                                    <Modal.Close onClick={() => {
                                        this.setState({showAddFolderDialog: false})
                                    }}>
                                        <BSpan aria-hidden="true">&times;</BSpan>
                                    </Modal.Close>
                                </Modal.Header>
                                <Modal.Body>
                                    {addFolderError}
                                    <Row w="100" p="3" alignItems="center">
                                        <Col col="4" h="100" alignSelf="center">
                                            文件夹名称：
                                        </Col>
                                        <Col col="8">
                                            <Form.Input type="text" className="form-control" id="inputFolderName"
                                                        placeholder="请输入文件夹名称"
                                                        onChange={evt => this.newFolderName = evt.target.value}/>
                                        </Col>
                                    </Row>
                                </Modal.Body>
                                <Modal.Footer>
                                    <Button type="submit" primary onClick={() => this.addFolder()}>创
                                        建
                                    </Button>
                                    <Button primary data-toggle="modal" data-target="#addFolderModal"
                                            onClick={() => {
                                                this.setState({showAddFolderDialog: false})
                                            }}>返 回
                                    </Button>
                                </Modal.Footer>
                            </Modal.Content>
                        </Modal.Dialog>
                    </Modal>

                    <Modal id="addFileModal" fade>
                        <Modal.Dialog centered>
                            <Modal.Content>
                                <Modal.Header>
                                    <Modal.Title>上传文件</Modal.Title>
                                    <Modal.Close onClick={() => {
                                        this.setState({showUploadFileDialog: false})
                                    }}>
                                        <BSpan aria-hidden="true">&times;</BSpan>
                                    </Modal.Close>
                                </Modal.Header>
                                <Modal.Body>
                                    {uploadFileError}
                                    <Row w="100" p="3" alignItems="center">
                                        <Col col="4" h="100" alignSelf="center">
                                            文件夹名称：
                                        </Col>
                                        <Col col="8">
                                            <Form.Input type="file" className="form-control" id="inputFileName"
                                                        style={{height: "auto"}}
                                                        placeholder="上传文件"
                                                        onChange={evt => this.newUploadFile = evt.target.files[0]}/>
                                        </Col>
                                    </Row>
                                </Modal.Body>
                                <Modal.Footer>
                                    <Button type="submit" primary onClick={() => this.uploadFile()}>
                                        上 传
                                    </Button>
                                    <Button primary data-toggle="modal" data-target="#addFileModal"
                                            onClick={() => {
                                                this.setState({showUploadFileDialog: false})
                                            }}>返 回
                                    </Button>
                                </Modal.Footer>
                            </Modal.Content>
                        </Modal.Dialog>
                    </Modal>

                    <Modal id="shareModal" fade>
                        <Modal.Dialog centered>
                            <Modal.Content>
                                <Modal.Header>
                                    <Modal.Title>{this.state.selectedShareObj.path ? "文件夹分享" : "文件分享"}</Modal.Title>
                                    <Modal.Close onClick={() => {
                                        this.setState({showShareDialog: false})
                                    }}>
                                        <BSpan aria-hidden="true">&times;</BSpan>
                                    </Modal.Close>
                                </Modal.Header>
                                <Modal.Body>
                                    {shareInfo}
                                    <Row w="100" pl="4" pr="4">
                                        <Form display="flex" justifyContent="around" w="100">
                                            <Form.Check className="custom-control custom-switch ">
                                                <Form.Checkbox type="checkbox" className="custom-control-input"
                                                               id="sharedModalSwitch1"
                                                               onChange={evt => {
                                                                   this.setShared = evt.target.checked;
                                                                   let sharedModalSwitch2 = $('#sharedModalSwitch2');
                                                                   if (evt.target.checked)
                                                                       sharedModalSwitch2.removeAttr("disabled");
                                                                   else {
                                                                       sharedModalSwitch2.attr("disabled", "disabled");
                                                                       sharedModalSwitch2.prop("checked", false);
                                                                   }
                                                               }}/>
                                                <Form.CheckLabel className="custom-control-label"
                                                                 htmlFor="sharedModalSwitch1">分
                                                    享</Form.CheckLabel>
                                            </Form.Check>
                                            <Form.Check className="custom-control custom-switch ">
                                                <Form.Checkbox type="checkbox" className="custom-control-input"
                                                               id="sharedModalSwitch2"
                                                               onChange={evt => {
                                                                   this.setSharePassword = evt.target.checked;
                                                               }}/>
                                                <Form.CheckLabel className="custom-control-label"
                                                                 htmlFor="sharedModalSwitch2">加
                                                    密</Form.CheckLabel>
                                            </Form.Check>
                                        </Form>
                                    </Row>
                                </Modal.Body>
                                <Modal.Footer>
                                    <Button type="submit" primary
                                            onClick={() => {
                                                Api.setObjShared(this.state.selectedShareObj, this.setShared, this.setSharePassword).then(response => {
                                                    if (response.ok) {
                                                        response.json().then(responseJson => {
                                                            let shareObj = this.state.selectedShareObj;
                                                            shareObj.isShared = responseJson.data.isShared;
                                                            shareObj.isShareEncryped = responseJson.data.isShareEncryped;
                                                            shareObj.sharePassword = responseJson.data.sharePassword;
                                                            shareObj.shareUrl = responseJson.data.shareUrl;
                                                            this.setState({selectedShareObj: shareObj});
                                                        });

                                                    }
                                                })
                                            }}>
                                        保 存
                                    </Button>
                                    <Button primary data-toggle="modal" data-target="#shareModal"
                                            onClick={() => {
                                                this.setState({showShareDialog: false});
                                            }}>返 回
                                    </Button>
                                </Modal.Footer>
                            </Modal.Content>
                        </Modal.Dialog>
                    </Modal>
                </div>
            </div>
        );
    }
}

export default MainPanel;
