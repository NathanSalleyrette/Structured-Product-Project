import React, { useState } from "react";
import "../node_modules/bootstrap/dist/css/bootstrap.min.css";
import "./App.css";
import {
  Outlet,
  Navigate,
  Router,
  Routes,
  Route,
  Link,
} from "react-router-dom";

import Login from "./components/login.component";
import SignUp from "./components/signup.component";
import NavBar from "./components/navbar";
import ChartStock from "./components/chart";
import AdminController from "./components/adminController";

import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";

//import style from "./index.css"

const BACKEND = "http://localhost:5000/api";
//const BACKEND = "https://backend-peps.herokuapp.com";

function App() {
  async function connect(username, password) {
    console.log("login", username, password);

    await fetch(`${BACKEND}/login`, {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: `data=${JSON.stringify({ username, password })}`,
      //credentials: 'include',
    })
      .then((response) => response.json())
      .then((data) => {
        if (data.message === 'login success') {
          setToken(data.token);
          setIsAuthUser(true);
          // let myStorage = window.localStorage;
          // myStorage.setItem('')
          setIsAdminUser(data.admin);
          console.log(data.admin);
        } else {
          alert("Wrong username or password")
        };
      })

      .catch((error) => console.log(error));
  }


  async function getUsers() {
    await fetch(`${BACKEND}/users`, {
      method: "GET",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
    })
      .then((response) => response.json())
      .then((data) => {
      })

      .catch((error) => console.log(error));
  }

  async function createUser(username, password) {
    console.log("Nouvel Utilisateur", username, password);

    await fetch(`${BACKEND}/users`, {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: `data=${JSON.stringify({ username, password })}`,
    })
      .then((response) => response.json())
      .then((data) => {
        data.status ? setUserCreated(true) : console.log("ONO");
      })
      .catch((error) => console.log("MIAMIAM"));
  }

  function UserHasBeenCreated() {
    if (userCreated) {
      // Problème lors de l'ajout de cette ligne
      //setUserCreated(false);
      return <Navigate to="/" />;
    }
    return <Outlet />;
  }

  function NotRequireAuth() {
    if (isAuthUser) {
      return <Navigate to="/home" />;
    }

    return <Outlet />;
  }

  function RequireAuth() {
    if (!isAuthUser) {
      return <Navigate to="/" />;
    }

    return <Outlet />;
  }

  function RequireAdmin() {
    if (!isAdmin) {
      return <Navigate to="/" />;
    }

    return <Outlet />;
  }

  function logout() {
    setToken("");
    setIsAuthUser(false);
    setIsAdminUser(false);
  }

  const [token, setToken] = useState("");
  const [isAuthUser, setIsAuthUser] = useState(false);
  const [isAdmin, setIsAdminUser] = useState(false);
  const [userCreated, setUserCreated] = useState(false);

  return (
    <div className="App">
      <NavBar isAuthUser={isAuthUser} isAdmin={isAdmin} onLogout={logout} />
      <Container fluid className="m-4">
        <Routes>
          <Route element={<NotRequireAuth />}>
            <Route exact path="/" element={<Login onConnect={connect} />} />
            <Route path="/sign-in" element={<Login onConnect={connect} />} />
            <Route element={<UserHasBeenCreated />}>
              <Route
                path="/sign-up"
                element={<SignUp onCreateUser={createUser} />}
              />
            </Route>
          </Route>

          <Route element={<RequireAuth />}>
            <Route
              path="/home"
              element={<h3>Congrats, you made it !!! {token}</h3>}
            />
            <Route path="/graph" element={<ChartStock />} />

            <Route element={<RequireAdmin />}>
              <Route path="/admin" element={<AdminController />} />

            </Route>

          </Route>


        </Routes>
      </Container>
    </div>
  );
}

export default App;
