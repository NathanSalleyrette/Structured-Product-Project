import React, { useState } from "react";
import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'

//export default class Login extends Component {
const Login = props => {
    //render() {
    const [login, setLogin] = useState('nicolas');
    const [passwd, setPasswd] = useState('nicolas');

    const onChangeLogin = (event) => {
        setLogin(event.target.value);
    };

    const onChangePassword = (event) => {
        setPasswd(event.target.value);
    };

    const onSubmit = (event) => {
        event.preventDefault();
    }


    return (
        <Row><Col>
        <div className="auth-wrapper">
            <div className="auth-inner">
                <form onSubmit={onSubmit}>
                    <h3>Sign In</h3>

                    <div className="form-group">
                        <label>Username</label>
                        <input type="text" value={login} onChange={onChangeLogin} className="form-control" placeholder="Enter username" />
                    </div>

                    <div className="form-group">
                        <label>Password</label>
                        <input type="password" value={passwd} onChange={onChangePassword} className="form-control" placeholder="Enter password" />
                    </div>


                    <button
                        type="submit"
                        onClick={() => props.onConnect(login, passwd)}
                        className="btn btn-primary btn-block">Submit</button>
                </form>
            </div>
        </div>
        </Col></Row>
    );

}

export default Login;