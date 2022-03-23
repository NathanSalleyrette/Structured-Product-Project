import React, { useState, useEffect } from "react";
const BACKEND = "http://localhost:5000/api";


const SignUp = props => {


    async function createUser(username, password) {
        // console.log("Nouvel Utilisateur", username, password);
    
        await fetch(`${BACKEND}/users`, {
          method: "POST",
          headers: { "Content-Type": "application/x-www-form-urlencoded" },
          body: `data=${JSON.stringify({ username, password })}`,
        })
          .then((response) => response.json())
          .then((data) => {
            if (data.status) {
                setUserCreated(true);
                setLoginValidate(true);
            } else {
                setUserCreated(false);
                setLoginValidate(false);
            }
          })
          .catch((error) => console.log("Error"));
      }

    const [username, setUsername] = useState('nicolas');
    const [passwd, setPasswd] = useState('nicolas');
    const [textIncorrect, setTextIncorrect] = useState("");
    const [textSuccess, setTextSuccess] = useState("");
    const [userCreated, setUserCreated] = useState(false);
    const [loginValidate, setLoginValidate] = useState(true);

    useEffect(()=>{
        if (loginValidate) {
            setTextIncorrect("")
        }
        else setTextIncorrect("Username not available")
      },[loginValidate]);

    useEffect(()=>{
    if (userCreated) {
        setTextSuccess("Successfully created, please sign-in to connect")
    }
    else setTextSuccess("")
    },[userCreated]);


    const onChangeUsername = (event) => {
        setLoginValidate(true)
        setUserCreated(false)
        setUsername(event.target.value);
    };

    const onChangePassword = (event) => {
        setLoginValidate(true)
        setUserCreated(false)
        setPasswd(event.target.value);
    };

    const onSubmit = (event) => {
        event.preventDefault();
    }


    return (
        <div className="auth-wrapper">
            <div className="auth-inner">
                <form onSubmit={onSubmit}>
                    <h3>Sign Up</h3>

                    <div className="form-group">
                        <label>Username</label>
                        <input type="text" value={username} onChange={onChangeUsername} className="form-control" placeholder="Enter username" />
                    </div>
                    <div className="text-danger">{textIncorrect}</div>

                    <div className="form-group">
                        <label>Password</label>
                        <input type="password" value={passwd} onChange={onChangePassword} className="form-control" placeholder="Enter password" />
                    </div>
                    <div className="text-success">{textSuccess}</div>

                    <button type="submit"
                        onClick={() => createUser(username, passwd)}
                        className="btn btn-primary btn-block">Sign Up</button>
                </form>
            </div>
        </div>
    );
}

export default SignUp;