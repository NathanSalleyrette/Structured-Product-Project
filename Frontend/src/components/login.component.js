import React, { useEffect, useState } from "react";
import Row from 'react-bootstrap/Row'
import Col from 'react-bootstrap/Col'
const BACKEND = "http://localhost:5000/api";

//export default class Login extends Component {
const Login = props => {
    //render() {
    const [login, setLogin] = useState('admin');
    const [passwd, setPasswd] = useState('admin');
    const [text, setText] = useState("");
    //const [hasconnected, setHasConnected] = useState(false);
    const [loginValidate, setLoginValidate] = useState(true);
    const [tryToConnect, setTryToConnect] = useState(false);

    const [infoUser, setInfoUser] = useState({
        token: '',
        admin: false,
        isAuthUser: false,
    })



    async function connect(username, password) {
        // console.log("login", username, password);
        await fetch(`${BACKEND}/login`, {
          method: "POST",
          headers: { "Content-Type": "application/x-www-form-urlencoded" },
          body: `data=${JSON.stringify({ username, password })}`,
          //credentials: 'include',
        })
          .then((response) => response.json())
          .then((data) => {
            setTryToConnect(true)
            if (data.message === 'login success') {
              //setToken(data.token);
              //setIsAuthUser(true);

              //setIsAdminUser(data.admin);
            //   console.log(data.admin);

              setLoginValidate(true)
              setInfoUser({
                  token: data.token,
                  admin: data.admin,
                  isAuthUser: true
              })
            } else {
              setLoginValidate(false);
              setInfoUser({
                token: '',
                admin: false,
                isAuthUser: false
            })
            };
          })
    
          .catch((error) => {
            console.log(error)
            setLoginValidate(false)
            setInfoUser({
                token: '',
                admin: false,
                isAuthUser: false
            })
          });
      }

    useEffect(()=>{
        if (tryToConnect) {
        if (loginValidate) {
            setText("")
            props.onConnect(infoUser)
        }
        else setText("Login or password incorrect")
    }
      }
,[loginValidate, tryToConnect, infoUser]);

    const onChangeLogin = (event) => {
        setLogin(event.target.value);
        setLoginValidate(true);

    };

    const onChangePassword = (event) => {
        setPasswd(event.target.value);
        setLoginValidate(true)
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

                    <div className="form-group username">
                        <label>Username</label>
                        <input type="text" value={login} onChange={onChangeLogin} className="form-control" placeholder="Enter username" />
                    </div>

                    <div className="form-group password">
                        <label>Password</label>
                        <input type="password" value={passwd} onChange={onChangePassword} className="form-control" placeholder="Enter password" />
                    </div>
                    <div className="text-danger">{text}</div>

                    <button
                        type="submit"
                        onClick={() => {
                            //setHasConnected(true)
                            connect(login, passwd)}}
                        className="btn btn-primary btn-block">Submit</button>
                </form>
            </div>
        </div>
        </Col></Row>
    );

}

export default Login;