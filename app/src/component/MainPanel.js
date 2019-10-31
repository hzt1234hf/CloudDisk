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
            addFolderError: false
        };
        this.addFolder = this.addFolder.bind(this);
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
    deleteFolder(folderName)
    {
        console.log("???");
        swal({
            title:"确定删除文件夹 " +  folderName + " 吗？",
            text:"删除后文件夹内容不可恢复！",
            icon:"warning",
            buttons:{
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
            dangerMode:true,
        }).then((willDelete) =>{
            if(willDelete)
            {
                Api.deleteFolder(folderName).then(response =>{
                    if(response.ok)
                    {
                        swal("文件夹删除成功.",{
                            icon:"success",
                        });
                        this.refreshFolderList();
                    }
                })
            }
        })
    }

    refreshFolderList() {
        Api.getFolders().then(response => {
            if (response.ok) {
                response.json().then(responseJson => {
                    this.setState({
                        folders: responseJson.data
                    })
                });
            }
        });
    }

    componentDidMount() {
        this.refreshFolderList();

    }

    componentDidUpdate(prevProps, prevState, snapshot) {
        if (this.state.showAddFolderDialog) {
            $('#addFolderModal').modal('show');
        }else{
            $('#addFolderModal').modal('hide');
        }
    }

    render() {
        let folderList = this.state.folders.map(folder => {
            return (
                <li className="list-group-item w-100 align-middle" style={{textAlignlign: 'center'}}
                    key={folder.id}>
                    <a><span>{folder.name}</span></a>
                    <button type="button" className="float-right btn btn-light" aria-label="Close" onClick={()=>this.deleteFolder(folder.name)}>
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

        return (
            <div className="w-100 p-3">
                <div className='row'>
                    <div className="align-self-center w-100 p-3">
                        <div className="col-3 offset-1 ">
                            <button type="button" className="btn btn-primary" data-toggle="modal" data-target="#addFolderModal"
                                    onClick={() => this.setState({showAddFolderDialog: true})}>
                                添加文件夹
                            </button>
                        </div>
                    </div>
                </div>
                <div className="row">
                    <div className="align-self-center w-100 p-3">
                        <div className="col-6 offset-1 ">
                            <ul className="list-group">
                                {folderList}
                            </ul>
                        </div>
                    </div>
                </div>
                <div className='row'>
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
                </div>
            </div>
        );
    }
}

export default MainPanel;