import React, { useState } from "react";

const SignUp = props => {

    const [username, setUsername] = useState('nicolas');
    const [passwd, setPasswd] = useState('nicolas');

    const onChangeUsername = (event) => {
        setUsername(event.target.value);
    };

    const onChangePassword = (event) => {
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


                    <div className="form-group">
                        <label>Password</label>
                        <input type="password" value={passwd} onChange={onChangePassword} className="form-control" placeholder="Enter password" />
                    </div>

                    <button type="submit"
                        onClick={() => props.onCreateUser(username, passwd)}
                        className="btn btn-primary btn-block">Sign Up</button>
                </form>
            </div>
        </div>
    );
}

export default SignUp;