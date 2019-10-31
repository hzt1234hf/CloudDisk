import React, {Component} from 'react';
import Api from '../Logic/api'
import './MainPanel.css';
import $ from 'jquery'
import swal from 'sweetalert';
import Octicon from '@primer/octicons-react'

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

            selectedFolder: ''
        };
        this.addFolder = this.addFolder.bind(this);
        this.uploadFile = this.uploadFile.bind(this);
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
                    console.log(responseJson);
                    this.setState({
                        files: responseJson.data.files,
                    });
                })
            }
        });
    }

    addFolder(folderName = null) {
        if (folderName == null) folderName = this.newFolderName;
        console.log(folderName);
        Api.addFolder(folderName).then(response => {
            this.setState({
                addFolderError: !response.ok,
            });
            if (response.ok) {
                this.setState({
                    showAddFolderDialog: false
                });
                this.refreshFolderList();
            }
        });
    }

    deleteFolder(folderName) {
        console.log("???");
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
        console.log(Api.downloadFile(this.state.selectedFolder.name));
        window.open(Api.downloadFile(this.state.selectedFolder.name, filename), "_blank");
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
                <li className="list-group-item w-100 align-middle" style={{textAlignlign: 'center'}}
                    key={folder.id}>
                    <a><span>{folder.name}</span></a>
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
                <li className="list-group-item w-100 align-middle" style={{textAlignlign: 'center'}}
                    key={file.id}>
                    <a><span>{file.filename}</span></a>
                    <button type="button" className="float-right btn btn-danger"
                            onClick={() => this.deleteFile(file.filename)}>
                        <span aria-hidden="true">删 除</span>
                    </button>
                    <button type="button" className="float-right btn btn-primary"
                            onClick={() => this.downloadFile(file.filename)}>
                        <span aria-hidden="true">下 载</span>
                    </button>
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
            <div className="w-100 p-3">
                <div className='row'>
                    {/*<div className="align-self-center w-100 p-3">*/}
                    <div className="col-2 ">
                        <button type="button" className="btn btn-primary w-100 p-2" data-toggle="modal"
                                data-target="#addFolderModal"
                                onClick={() => this.setState({showAddFolderDialog: true})}>
                            添加文件夹
                        </button>
                    </div>
                    {/*</div>*/}
                    <div className="col-2">
                        <button type="button" className="btn btn-primary w-100 p-2" data-toggle="modal"
                                data-target="#addFileModal"
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
                                            <input type="file" className="form-control" id="inputFolderName"
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