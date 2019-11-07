import React, {Component} from 'react';
import Api from '../Logic/api'
import './MainPanel.css';
import swal from 'sweetalert';
import $ from 'jquery'
import Octicon, {File, FileDirectory, Fold, FoldUp, X} from '@primer/octicons-react'
import {FontAwesomeIcon} from '@fortawesome/react-fontawesome'
import {faFile, faFolder, faFileDownload, faFileUpload} from '@fortawesome/free-solid-svg-icons'
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
            selectedFolderId: 0,
            rootFolders: [],
        };
        this.addFolder = this.addFolder.bind(this);
        this.uploadFile = this.uploadFile.bind(this);
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

    componentDidUpdate(prevProps, prevState, snapshot) {
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
                <div className="row  align-items-center p-2">
                    <div className="col-4 d-flex align-items-center">
                        <div className="">
                            <span className="">当前目录：</span>
                            {rootFolderList}
                        </div>
                    </div>
                </div>
            );
        } else {
            folderInfo = <span></span>
        }
        let folderList = this.state.folders.map(folder => {
            return (
                <li className="list-group-item  d-flex justify-content-between align-items-center"
                    key={folder.id}>

                    <a href="#" className="d-flex align-items-center folderItem float-left" onClick={() => {
                        this.changeToChildSelectedFolder(folder)
                    }}>
                        <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                        <span className="ml-1 mr-1">{folder.name}</span>
                    </a>
                    <div className="float-right ">
                        <button type="button" className="btn btn-danger mr-1 ml-1" aria-label="Close"
                                onClick={() => this.deleteFolder(folder.name, folder.id)}>
                            <span aria-hidden="true">删 除</span>
                        </button>
                        <button type="button" className="btn btn-primary mr-1 ml-1" aria-label="Close"
                                onClick={() => this.setState({showShareDialog: true, selectedShareObj: folder})}>
                            分 享
                        </button>
                    </div>
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

        let fileList = this.state.files.map(file => {
            return (
                <li className="list-group-item d-flex justify-content-between align-items-center"
                    key={file.id}>
                    <a href="#" className="d-flex align-items-center fileItem float-left">
                        <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFile}/>
                        {file.name}
                    </a>
                    <div className="float-right ">

                        <button type="button" className="btn btn-danger mr-1 ml-1"
                                onClick={() => this.deleteFile(file.name, file.id)}>
                            <span aria-hidden="true">删 除</span>
                        </button>
                        <button type="button" className="btn btn-primary mr-1 ml-1"
                                onClick={() => this.downloadFile(file.id)}>
                            <span aria-hidden="true">下 载</span>
                        </button>
                        <button type="button" className="btn btn-primary mr-1 ml-1"
                                onClick={() => this.setState({showShareDialog: true, selectedShareObj: file})}>
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
                    <strong>上传文件失败！</strong>请确认是否文件名重复，或服务器运行和网络连接是否正常。
                </div>
            );
        } else {
            uploadFileError = <span></span>;
        }

        let shareInfo = <span></span>;
        console.log(this.state.selectedShareObj);
        if (this.state.selectedShareObj !== null && this.state.selectedShareObj.isShared === true) {
            if (this.state.selectedShareObj.isShareEncryped === true) {
                shareInfo = (
                    <div className="row w-100 pl-4 pr-4 d-flex justify-content-between align-items-center">
                        <table className="table table-borderless d-flex justify-content-between align-items-center">
                            <tbody>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">链接：</td>
                                <td colSpan={"3"} className="text-break p-1">
                                    <a href={window.location + "s/" + this.state.selectedShareObj.shareUrl}>
                                        {window.location + "s/" + this.state.selectedShareObj.shareUrl}
                                    </a>
                                </td>
                            </tr>
                            <tr display={"none"}>
                                <td className="text-nowrap p-1 align-middle">密码：</td>
                                <td colSpan={"3"} className="text-nowrap p-1">
                                    <input type="text"
                                           defaultValue={this.state.selectedShareObj.sharePassword}
                                           onChange={evt => {
                                               this.sharedPassword = evt.target.value;
                                               console.log(this.sharedPassword)
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
                    </div>
                );
            } else {
                shareInfo = (
                    <div className="row w-100 pl-4 pr-4 d-flex justify-content-between align-items-center">
                        <table className="table table-borderless d-flex justify-content-between align-items-center">
                            <tbody>
                            <tr>
                                <td className="text-nowrap p-1 align-middle">链接：</td>
                                <td colSpan={"3"} className="text-break p-1">
                                    <a href={window.location + "s/" + this.state.selectedShareObj.shareUrl}>
                                        {window.location + "s/" + this.state.selectedShareObj.shareUrl}
                                    </a>
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
                    </div>
                );
            }

        }

        if (this.state.showAddFolderDialog)
            $('#addFolderModal').modal('show');
        else
            $('#addFolderModal').modal('hide');
        if (this.state.showUploadFileDialog)
            $('#addFileModal').modal('show');
        else
            $('#addFileModal').modal('hide');
        if (this.state.showShareDialog)
            $('#shareModal').modal('show');
        else
            $('#shareModal').modal('hide');
        return (
            <div className="container-fluid w-100 p-4 mt-3">
                <div className='row p-2 justify-content-end'>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-2 ">
                        <button type="button"
                                className="btn btn-primary w-100 p-2 d-flex align-items-center justify-content-around"
                                onClick={() => this.setState({showAddFolderDialog: true})}>
                            <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                            添加文件夹
                        </button>
                    </div>
                    {/*</div>*/}
                    <div className="col-2 ">
                        <button type="button"
                                className="btn btn-primary w-100 p-2 d-flex align-items-center justify-content-around "
                                onClick={() => this.setState({showUploadFileDialog: true})}>
                            <FontAwesomeIcon className="float-left ml-1 mr-1" icon={faFolder}/>
                            上传文件
                        </button>
                    </div>
                </div>
                <hr/>
                {folderInfo}
                <div className="row p-2">
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-4">
                        <ul className="list-group">
                            {folderList}
                        </ul>
                        {/*</div>*/}
                    </div>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-7 offset-1">
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
                                    <button type="submit" className="btn btn-primary" onClick={() => this.addFolder()}>创
                                        建
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
                                            <input type="file" className="form-control" id="inputFileName"
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

                    <div className="modal fade " id="shareModal" tabIndex="-1" role="dialog"
                         aria-labelledby="addFilerErrorModelLabel" aria-hidden="true">
                        <div className="modal-dialog modal-dialog-centered" role="document">
                            <div className="modal-content">
                                <div className="modal-header">
                                    <h5 className="modal-title" id="addFileErrorModelLabel">文件分享</h5>
                                    <button type="button" className="close" data-dismiss="modal" aria-label="Close"
                                            onClick={() => {
                                                this.setState({showShareDialog: false})
                                            }}>
                                        <span aria-hidden="true">&times;</span>
                                    </button>
                                </div>
                                <div className="modal-body ">
                                    {shareInfo}
                                    <div className="row w-100  pl-4 pr-4">
                                        <form>
                                            <div className="custom-control custom-switch float-left">
                                                <input type="checkbox" className="custom-control-input"
                                                       id="customSwitch1" defaultChecked={this.state.selectedShareObj.isShared} onChange={evt =>{
                                                    this.setShared = evt.target.checked;
                                                    console.log(this.setShared);
                                                }}/>
                                                    <label className="custom-control-label" htmlFor="customSwitch1" >分 享</label>
                                            </div>
                                            <div className="custom-control custom-switch float-left">
                                                <input type="checkbox" className="custom-control-input"
                                                       id="customSwitch2" defaultChecked={this.state.selectedShareObj.isShareEncryped} onChange={evt =>{
                                                    this.setPassword = evt.target.checked;
                                                    console.log(this.setPassword);
                                                }}/>
                                                <label className="custom-control-label" htmlFor="customSwitch2">加 密</label>
                                            </div>
                                            <div className="form-check  form-check-inline">
                                                <input className="form-check-input" type="radio" name="fileShareRadio"
                                                       id="exampleRadios1" value="option1"
                                                       defaultChecked={this.state.selectedShareObj.isShared}
                                                       onClick={() => this.setShared = true}/>
                                                <label className="form-check-label">
                                                    分享
                                                </label>
                                            </div>
                                            <div className="form-check  form-check-inline">
                                                <input className="form-check-input" type="radio" name="fileShareRadio"
                                                       id="exampleRadios2" value="option2"
                                                       defaultChecked={this.state.selectedShareObj.isShareEncryped}
                                                       onClick={() => this.setPassword = true}/>
                                                <label className="form-check-label">
                                                    密码
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