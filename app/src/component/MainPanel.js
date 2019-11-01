import React, {Component} from 'react';
import Api from '../Logic/api'
import './MainPanel.css';
import swal from 'sweetalert';
import Octicon, {File, FileDirectory, Fold, FoldUp, X} from '@primer/octicons-react'

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

            curFolder: null,
            selectedFolderId: 0
        };
        this.addFolder = this.addFolder.bind(this);
        this.uploadFile = this.uploadFile.bind(this);
    }

    refreshFolderAndFileList() {
        Api.getFolder(this.state.selectedFolderId).then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    this.setState({
                        curFolder: responseJson.data,
                        folders: responseJson.subfolders,
                        files: responseJson.files,
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

    changeSelectedFolder(id) {
        if (this.state.selectedFolderId === id) {
            return;
        }
        this.setState({
            selectedFolderId: id
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
        // if (this.state.showAddFolderDialog) {
        //     $('#addFolderModal').modal('show');
        // } else {
        //     $('#addFolderModal').modal('hide');
        // }
    }

    render() {
        let folderInfo;
        if (this.state.curFolder != null) {
            folderInfo = (
                <div className="row  align-items-center p-2">
                    <div className="col-4 d-flex  align-items-center">
                        <div className="">
                            <span className="">当前目录：</span>
                            <a href="#" className="folderItem " onClick={() => {
                                this.changeSelectedFolder(this.state.curFolder.id)
                            }}>
                                {this.state.curFolder.name}
                            </a>
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

                    <a href="#" className="folderItem float-left" onClick={() => {
                        this.changeSelectedFolder(folder.id)
                    }}>
                        <Octicon className="ml-1 mr-1" icon={FileDirectory}/>
                        <span className="ml-1 mr-1">{folder.name}</span>
                    </a>
                    <button type="button" className="float-right btn btn-danger mr-1 ml-1" aria-label="Close"
                            onClick={() => this.deleteFolder(folder.name, folder.id)}>
                        <span aria-hidden="true"><Octicon icon={X}/></span>
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

        let fileList = this.state.files.map(file => {
            return (
                <li className="list-group-item d-flex justify-content-between align-items-center"
                    key={file.id}>
                    <a href="#" className="fileItem float-left">
                        <Octicon className="ml-1 mr-1" icon={File}/>
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
                    </div>

                </li>
            );
        });

        let uploadFileError;
        if (this.state.uploadFileError) {
            uploadFileError = (
                <div className="alert alert-danger alert-dismissible fade show" role="alert">
                    <strong>上传文件失败！</strong>请确认服务器或网络连接是否运行正常。
                </div>
            );
        } else {
            uploadFileError = <span></span>;
        }
        return (
            <div className="container-fluid w-100 p-4 mt-3">
                <div className='row p-2 justify-content-end'>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-2 ">
                        <button type="button" className="btn btn-primary w-100 p-2" data-toggle="modal"
                                data-target="#addFolderModal"
                                onClick={() => this.setState({showAddFolderDialog: true})}>
                            <Octicon className="ml-1 mr-1" icon={FileDirectory}/>
                            添加文件夹
                        </button>
                    </div>
                    {/*</div>*/}
                    <div className="col-2 ">
                        <button type="button" className="btn btn-primary w-100 p-2" data-toggle="modal"
                                data-target="#addFileModal"
                                onClick={() => this.setState({showUploadFileDialog: true})}>
                            <Octicon className="ml-1 mr-1" icon={FileDirectory}/>
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
                </div>
            </div>
        );
    }
}

export default MainPanel;