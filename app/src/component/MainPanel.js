import React, {Component} from 'react';
import Api from '../Logic/api'
import './MainPanel.css';
import $ from 'jquery'
import swal from 'sweetalert';
import QRCode from 'qrcode.react';

import Octicon from 'react-octicon'
import {FontAwesomeIcon} from '@fortawesome/react-fontawesome'
import {faFile, faFolder, faFileDownload, faFileUpload} from '@fortawesome/free-solid-svg-icons'

class MainPanel extends Component {
    constructor(props) {
        super(props);
        this.state = {
            folders: [],
            showAddFolderDialog: false,
            addFolderError: false,

            files: [],
            showUploadFileDialog: false,
            uploadFileError: false,

            showFileShareDialog: false,

            selectedFolder: '',
            selectedFile: '',
            selectedFileId: 0,

        };
        this.addFolder = this.addFolder.bind(this);
        this.uploadFile = this.uploadFile.bind(this);
        this.saveFileShareType = this.saveFileShareType.bind(this);
    }

    refreshFolderList() {
        Api.getFolders().then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    const firstFolder = responseJson.data[0];
                    this.setState({
                        folders: responseJson.data,
                        selectedFolder: firstFolder,
                    }, () => {
                        this.refreshFileList();
                    })
                });
            }
        });
    }

    refreshFileList() {
        if (!this.state.selectedFolder) {
            return;
        }
        Api.getFolder(this.state.selectedFolder.name).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    this.setState({
                        files: responseJson.data.files,
                    });
                })
            }
        });
    }

    addFolder(folderName = null) {
        if (folderName == null) folderName = this.newFolderName;
        Api.addFolder(folderName).then(response => {
            this.setState({
                addFolderError: !response.ok,
            });
            if (response.ok) {
                this.setState({
                    showAddFolderDialog: false
                });
                $('#inputFolderName').val('');
                this.refreshFolderList();
            }
        });
    }

    deleteFolder(folderName) {
        swal({
            title: "确定删除文件夹 " + folderName + " 吗？",
            text: "删除后文件夹内容不可恢复！",
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
                Api.deleteFolder(folderName).then(response => {
                    if (response.ok) {
                        swal("文件夹删除成功.", {
                            icon: "success",
                        });
                        this.refreshFolderList();
                    }
                })
            }
        })
    }

    changeSelectedFolder(id) {
        if (this.state.selectedFolder.id === id) {
            return;
        }
        this.setState({
            selectedFolder: this.state.folders.find(x => x.id == id)
        }, () => {
            this.refreshFileList();
        });
    }

    uploadFile() {

        Api.uploadFile(this.state.selectedFolder.name, this.newUploadFile).then(response => {
            this.setState({
                uploadFileError: !response.ok
            });
            if (response.ok) {
                this.setState({
                    showUploadFileDialog: false
                });
                $('#inputFile').val('');
                this.refreshFileList();
            }
        });
    }

    deleteFile(filename) {
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
                Api.deleteFile(this.state.selectedFolder.name, filename).then(response => {
                    if (response.ok) {
                        swal("文件删除成功.", {
                            icon: "success",
                        });
                        this.refreshFileList();
                    }
                });
            }
        });
    }

    downloadFile(filename) {
        window.open(Api.downloadFile(this.state.selectedFolder.name, filename), "_blank");
    }

    saveFileShareType(isShared) {
        let shareType = "none";
        if (isShared == 1)
            shareType = "private";
        else if (isShared == 2)
            shareType = "public";
        Api.updateFileShareType(this.state.selectedFolder.name, this.state.selectedFile.filename, shareType).then(response => {
            if (response.ok) {
                Api.getFileInfo(this.state.selectedFolder.name, this.state.selectedFile.filename).then(responseData => {
                    if (responseData.ok) {
                        responseData.json().then(responseJson => {
                            this.refreshFileList();
                            this.setState({
                                selectedFile: responseJson.data
                            })
                        });
                    }
                });
            }
        });
    }

    componentDidMount() {
        this.refreshFolderList();

    }

    componentDidUpdate(prevProps, prevState, snapshot) {
        // if (this.state.showAddFolderDialog) {
        //     $('#addFolderModal').modal('show');
        // } else {
        //     $('#addFolderModal').modal('hide');
        // }
    }

    render() {

        let folderList = this.state.folders.map(folder => {
            return (
                <li className="list-group-item d-flex justify-content-between align-items-center"
                    style={{textAlignlign: 'center'}}
                    key={folder.id}>
                    <a href="#" className="d-flex align-items-center folderItem float-left" onClick={() => {
                        this.changeSelectedFolder(folder.id)
                    }}>
                        <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                        <span>{folder.name}</span>
                    </a>
                    <button type="button" className="float-right btn btn-danger" aria-label="Close"
                            onClick={() => this.deleteFolder(folder.name)}>
                        <span aria-hidden="true">&times;</span>
                    </button>
                </li>
            );
        });

        let addFolderError;
        if (this.state.addFolderError) {
            addFolderError = (
                <div className="alert alert-danger alert-dismissible fade show" role="alert">
                    <strong>添加文件夹失败！</strong>请确认名称是否无重复。
                </div>
            );
        } else {
            addFolderError = <span></span>;
        }

        let addFolderModal;
        if (this.state.showAddFolderDialog) {
            addFolderModal = (
                <div className="modal fade" id="addFolderModal" tabIndex="-1" role="dialog"
                     aria-labelledby="addFolderErrorModelLabel" aria-hidden="true">
                    <div className="modal-dialog modal-dialog-centered" role="document">
                        <div className="modal-content">
                            <div className="modal-header">
                                <h5 className="modal-title" id="addFolderErrorModelLabel">添加文件夹</h5>
                                <button type="button" className="close" data-dismiss="modal" aria-label="Close"
                                        onClick={() => {
                                            this.setState({showAddFolderDialog: false})
                                        }}>
                                    <span aria-hidden="true">&times;</span>
                                </button>
                            </div>
                            <div className="modal-body">
                                {addFolderError}
                                <div className="row w-100 p-3  align-items-center">
                                    <div className="col-4 h-100 align-self-center">
                                        文件夹名称：
                                    </div>
                                    <div className="col-8">
                                        <input type="text" className="form-control" id="inputFolderName"
                                               placeholder="请输入文件夹名称"
                                               onChange={evt => this.newFolderName = evt.target.value}/>
                                    </div>
                                </div>
                            </div>
                            <div className="modal-footer">
                                <button type="submit" className="btn btn-primary" onClick={() => this.addFolder()}>创
                                    建
                                </button>
                                <button type="button" className="btn btn-primary" data-dismiss="modal"
                                        aria-label="Close"
                                        onClick={() => {
                                            this.setState({showAddFolderDialog: false})
                                        }}>关 闭
                                </button>
                            </div>
                        </div>
                    </div>
                </div>
            );
        } else {
            addFolderModal = <span></span>;
        }

        let fileList = this.state.files.map(file => {
            return (
                <li className="list-group-item d-flex justify-content-between align-items-center"
                    key={file.id}>
                    <a href="#" className="d-flex align-items-center fileItem float-left">
                        <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFile}/>
                        <span>{file.filename}</span>
                    </a>
                    <div className="float-right">

                        <button type="button" className="btn btn-danger mr-1 ml-1"
                                onClick={() => this.deleteFile(file.filename)}>
                            <span aria-hidden="true">删 除</span>
                        </button>
                        <button type="button" className="btn btn-primary mr-1 ml-1"
                                onClick={() => this.downloadFile(file.filename)}>
                            <span aria-hidden="true">下 载</span>
                        </button>
                        <button type="button" className="btn btn-primary mr-1 ml-1"
                                onClick={() => {
                                    this.setState({
                                        selectedFileId: file.id,
                                        selectedFile: file,
                                        showFileShareDialog: true
                                    });
                                }}>
                            <span aria-hidden="true">分 享</span>
                        </button>
                    </div>
                </li>
            );
        });

        let uploadFileError;
        if (this.state.uploadFileError) {
            uploadFileError = (
                <div className="alert alert-danger alert-dismissible fade show" role="alert">
                    <strong>上传文件失败！</strong>请确认文件是否重复，或者服务器运行和网络连接是否正常。
                </div>
            );
        } else {
            uploadFileError = <span></span>;
        }
        if (this.state.showFileShareDialog === true)
            $('#shareFileModal').modal('show');
        else
            $('#shareFileModal').modal('hide');


        if (this.state.showUploadFileDialog === true)
            $('#addFileModal').modal('show');
        else
            $('#addFileModal').modal('hide');

        if (this.state.showAddFolderDialog === true)
            $('#addFolderModal').modal('show');
        else
            $('#addFolderModal').modal('hide');

        let shareInfo = <span></span>;
        if (this.state.selectedFile.open_private_share === true) {
            shareInfo = (
                <div className="row w-100 pl-4 pr-4 d-flex justify-content-between align-items-center">
                    <div className="float-left">
                        {"私密分享地址：" + this.state.selectedFile.private_share_url +
                        "   密码：" + this.state.selectedFile.private_share_password}
                    </div>
                    <div className="float-right">
                        <QRCode
                            value={window.location + "s/" + this.state.selectedFile.private_share_url}
                            size={64} bgColor={'#ffffff'}
                            fgColor={'#000000'}
                            level={'L'}/>
                    </div>
                </div>);
        } else if (this.state.selectedFile.open_public_share === true) {
            shareInfo = (
                <div className="row w-100 pl-4 pr-4 d-flex justify-content-between align-items-center">
                    <div className="float-left">
                        {"公开分享地址：" + this.state.selectedFile.public_share_url}
                    </div>
                    <div className="float-right">
                        <QRCode
                            value={window.location + "s/" + this.state.selectedFile.public_share_url}
                            size={64} bgColor={'#ffffff'}
                            fgColor={'#000000'}
                            level={'L'}/>
                    </div>
                </div>);
        }
        return (
            <div className="w-100 p-3">
                <div className='row'>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-2 ">
                        <button type="button" className="btn btn-primary w-100 p-2"
                                onClick={() => this.setState({showAddFolderDialog: true})}>
                            添加文件夹
                        </button>
                    </div>
                    {/*</div>*/}
                    <div className="col-2">
                        <button type="button" className="btn btn-primary w-100 p-2"
                                onClick={() => this.setState({showUploadFileDialog: true})}>
                            上传文件
                        </button>
                    </div>
                </div>
                <div className="row">
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-4  w-100 p-3">
                        <ul className="list-group">
                            {folderList}
                        </ul>
                        {/*</div>*/}
                    </div>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-7 offset-1 w-100 p-3">
                        <ul className="list-group">
                            {fileList}
                        </ul>
                    </div>
                    {/*</div>*/}
                </div>
                <div>
                    <div className="modal fade" id="addFolderModal" tabIndex="-1" role="dialog"
                         aria-labelledby="addFolderErrorModelLabel" aria-hidden="true">
                        <div className="modal-dialog modal-dialog-centered" role="document">
                            <div className="modal-content">
                                <div className="modal-header">
                                    <h5 className="modal-title" id="addFolderErrorModelLabel">添加文件夹</h5>
                                    <button type="button" className="close" data-dismiss="modal" aria-label="Close"
                                            onClick={() => {
                                                this.setState({showAddFolderDialog: false})
                                            }}>
                                        <span aria-hidden="true">&times;</span>
                                    </button>
                                </div>
                                <div className="modal-body">
                                    {addFolderError}
                                    <div className="row w-100 p-3  align-items-center">
                                        <div className="col-4 h-100 align-self-center">
                                            文件夹名称：
                                        </div>
                                        <div className="col-8">
                                            <input type="text" className="form-control" id="inputFolderName"
                                                   placeholder="请输入文件夹名称"
                                                   onChange={evt => this.newFolderName = evt.target.value}/>
                                        </div>
                                    </div>
                                </div>
                                <div className="modal-footer">
                                    <button type="submit" className="btn btn-primary" onClick={() => {
                                        this.addFolder();
                                    }}>创 建
                                    </button>
                                    <button type="button" className="btn btn-primary" data-dismiss="modal"
                                            aria-label="Close"
                                            onClick={() => {
                                                this.setState({showAddFolderDialog: false})
                                            }}>返 回
                                    </button>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div className="modal fade" id="addFileModal" tabIndex="-1" role="dialog"
                         aria-labelledby="addFilerErrorModelLabel" aria-hidden="true">
                        <div className="modal-dialog modal-dialog-centered" role="document">
                            <div className="modal-content">
                                <div className="modal-header">
                                    <h5 className="modal-title" id="addFileErrorModelLabel">上传文件</h5>
                                    <button type="button" className="close" data-dismiss="modal" aria-label="Close"
                                            onClick={() => {
                                                this.setState({showUploadFileDialog: false})
                                            }}>
                                        <span aria-hidden="true">&times;</span>
                                    </button>
                                </div>
                                <div className="modal-body">
                                    {uploadFileError}
                                    <div className="row w-100 p-3  align-items-center">
                                        <div className="col-4 h-100 align-self-center">
                                            文件夹名称：
                                        </div>
                                        <div className="col-8">
                                            <input type="file" className="form-control" id="inputFile"
                                                   placeholder="上传文件"
                                                   onChange={evt => this.newUploadFile = evt.target.files[0]}/>
                                        </div>
                                    </div>
                                </div>
                                <div className="modal-footer">
                                    <button type="submit" className="btn btn-primary" onClick={() => this.uploadFile()}>
                                        上 传
                                    </button>
                                    <button type="button" className="btn btn-primary" data-dismiss="modal"
                                            aria-label="Close"
                                            onClick={() => {
                                                this.setState({showUploadFileDialog: false})
                                            }}>返 回
                                    </button>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div className="modal fade" id="shareFileModal" tabIndex="-1" role="dialog"
                         aria-labelledby="addFilerErrorModelLabel" aria-hidden="true">
                        <div className="modal-dialog modal-dialog-centered" role="document">
                            <div className="modal-content">
                                <div className="modal-header">
                                    <h5 className="modal-title" id="addFileErrorModelLabel">文件分享</h5>
                                    <button type="button" className="close" data-dismiss="modal" aria-label="Close"
                                            onClick={() => {
                                                this.setState({showUploadFileDialog: false})
                                            }}>
                                        <span aria-hidden="true">&times;</span>
                                    </button>
                                </div>
                                <div className="modal-body">
                                    {shareInfo}
                                    <div className="row w-100  pl-4 pr-4">
                                        <form>
                                            <div className="form-check  form-check-inline">
                                                <input className="form-check-input" type="radio" name="fileShareRadio"
                                                       id="exampleRadios1" value="option1"
                                                       defaultChecked={this.state.selectedFile.open_public_share}
                                                       onClick={() => this.publicShareValue = 2}/>
                                                <label className="form-check-label">
                                                    公开分享
                                                </label>
                                            </div>
                                            <div className="form-check  form-check-inline">
                                                <input className="form-check-input" type="radio" name="fileShareRadio"
                                                       id="exampleRadios2" value="option2"
                                                       defaultChecked={this.state.selectedFile.open_private_share}
                                                       onClick={() => this.publicShareValue = 1}/>
                                                <label className="form-check-label">
                                                    私密分享
                                                </label>
                                            </div>
                                            <div className="form-check  form-check-inline">
                                                <input className="form-check-input" type="radio" name="fileShareRadio"
                                                       id="exampleRadios3" value="option3"
                                                       defaultChecked={!this.state.selectedFile.open_public_share && !this.state.selectedFile.open_private_share}
                                                       onClick={() => this.publicShareValue = 0}/>
                                                <label className="form-check-label">
                                                    关闭分享
                                                </label>
                                            </div>
                                        </form>
                                    </div>
                                </div>
                                <div className="modal-footer">
                                    <button type="submit" className="btn btn-primary"
                                            onClick={() => this.saveFileShareType(this.publicShareValue)}>
                                        保 存
                                    </button>
                                    <button type="button" className="btn btn-primary" data-dismiss="modal"
                                            aria-label="Close"
                                            onClick={() => {
                                                this.setState({showFileShareDialog: false})
                                            }}>返 回
                                    </button>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
}

export default MainPanel;
