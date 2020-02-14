import React, {Component} from 'react';
import Api from '../Logic/api';

import {Alert, Row, Col, Button, BDiv, Form} from 'bootstrap-4-react';

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
                <Alert danger className="alert-dismissible fade show" role="alert">
                    <strong>账号密码错误！</strong> 请检查你的账号和密码是否正确。
                    <Button className="close" data-dismiss="alert" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </Button>
                </Alert>
            );
        } else {
            alert = <span></span>;
        }
        return (
            <Row className="row " style={{height: "800px"}}>
                <BDiv display="flex" p="3" w="100" alignItems="center">
                    <Col col="md-4" offset="md-4">
                        <Row display="flex" alignItems="center" justifyContent="center" p="3" mb="2" bg="primary"
                             text="white">
                            <h3>用户登录</h3>
                        </Row>
                        <Row bg="light">
                            <Form className="w-100 p-3" onSubmit={this.handleSubmit}>
                                <Form.Group>
                                    <label htmlFor="inputLoginEmail">邮箱：</label>
                                    <Form.Input type="email" id="inputLoginEmail"
                                                placeholder="请输入邮箱"
                                                onChange={evt => this.email = evt.target.value}/>
                                    <Form.Text text="muted">我们不会向任何人展示你的邮箱地址！</Form.Text>
                                </Form.Group>
                                <Form.Group>
                                    <label htmlFor="inputLoginPassword">密码：</label>
                                    <Form.Input type="password" className="form-control" id="inputLoginPassword"
                                                placeholder="请输入密码"
                                                onChange={evt => this.password = evt.target.value}/>
                                </Form.Group>
                                <Button primary type="submit">登录</Button>
                            </Form>
                        </Row>
                        <Row>
                            {alert}
                        </Row>
                    </Col>
                </BDiv>
            </Row>
        );
    }
}

export default LoginForm;