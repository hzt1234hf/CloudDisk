import React, {Component} from 'react';
import Api from '../Logic/api';

import './LoginForm.css'

class LoginForm extends Component {
    constructor(props) {
        super(props);
        this.state = {
            error: false
        };
        this.handleSubmit = this.handleSubmit.bind(this);
    }

    handleSubmit(e) {
        e.preventDefault();
        const email = this.email;
        const password = this.password;
        Api.login(email, password).then(response => {
            if (!response.ok) {
                this.setState({error: true});
                return;
            }
            response.json().then(data => this.props.onLogin(data));
        })
    }

    render() {
        let alert;
        if (this.state.error) {
            alert = (
                <div className="alert alert-danger alert-dismissible fade show" role="alert">
                    <strong>账号密码错误！</strong> 请检查你的账号和密码是否正确。
                    <button type="button" className="close" data-dismiss="alert" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </button>
                </div>
            );
        } else {
            alert = <span></span>;
        }
        return (
            <div className="row " style={{height: "800px"}}>
                <div className="align-self-center w-100 p-3">
                    <div className="col-4 offset-4 ">
                        <div className="row justify-content-center p-3 mb-2 bg-primary text-white">
                            <h3>用户登录</h3>
                        </div>
                        <div className="row bg-light">
                            <form className="w-100 p-3" onSubmit={this.handleSubmit}>
                                <div className="form-group">
                                    <label htmlFor="exampleInputEmail1">邮箱：</label>
                                    <input type="email" className="form-control" id="exampleInputEmail1"
                                           aria-describedby="emailHelp"
                                           placeholder="请输入邮箱"
                                           onChange={evt => this.email = evt.target.value}/>
                                    <small id="emailHelp" className="form-text text-muted">我们不会向任何人展示你的邮箱地址！</small>
                                </div>
                                <div className="form-group">
                                    <label htmlFor="exampleInputPassword1">密码：</label>
                                    <input type="password" className="form-control" id="exampleInputPassword1"
                                           placeholder="请输入密码"
                                           onChange={evt => this.password = evt.target.value}/>
                                </div>
                                <button type="submit" className="btn btn-primary">登录</button>
                            </form>
                        </div>
                        <div className="row">
                            {alert}
                        </div>
                    </div>
                </div>
            </div>
            // <Col md={4} mdOffset={4}>
            //     <h3>Login</h3>
            //         <form id='loginForm' onSubmit={this.handleSubmit}>
            //             <FormGroup>
            //                 <FormControl type='text' placeholder='Email' />
            //             </FormGroup>
            //             <FormGroup>
            //                 <FormControl type='password' placeholder='Password'/>
            //             </FormGroup>
            //             <FormGroup>
            //                 <FormControl id='submitButton' type='submit' value='登录'/>
            //             </FormGroup>
            //         </form>
            // </Col>
        );
    }
}

export default LoginForm;