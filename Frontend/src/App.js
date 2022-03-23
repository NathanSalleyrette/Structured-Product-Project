import React, { useEffect, useState } from "react";
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
import ProductManager from "./components/productManager";

import Container from "react-bootstrap/Container";
import Row from "react-bootstrap/Row";
import Col from "react-bootstrap/Col";

//import style from "./index.css"

const BACKEND = "http://localhost:5000/api";
//const BACKEND = "https://peps-group1.herokuapp.com/api";

function App() {

  const [token, setToken] = useState("");
  const [isAuthUser, setIsAuthUser] = useState(false);
  const [isAdmin, setIsAdminUser] = useState(false);

  useEffect(() => {
    fetch(`${BACKEND}/verify`, {
    method: "GET",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
  })
    .then((response) => response.json())
    .then((data) => {
      // console.log("Verify Here")
      // console.log(data)
      setIsAdminUser(data.admin)
      setIsAuthUser(data.auth)
    })

    .catch((error) => console.log(error));
}, [])




  function RequireAdmin() {
    if (!isAdmin) {
      return <Navigate to="/" />;
    }

    return <Outlet />;
  }

  async function logout() {
    await fetch(`${BACKEND}/logout`, {
      method: "GET",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
    })
      .then((response) => response.json())
      .catch((error) => console.log(error));


      await fetch(`${BACKEND}/verify`, {
        method: "GET",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
      })
        .then((response) => response.json())
        .then((data) => { 
          // console.log(data)
          setIsAdminUser(data.admin)
          setIsAuthUser(data.auth)
        })
    
        .catch((error) => console.log(error));

    setToken("");
    setIsAuthUser(false);
    setIsAdminUser(false);
  }


  function setInfo(info) {
    console.log(isAuthUser)
    if (!isAuthUser) {
    console.log(info)
    setToken(info.token);
    setIsAuthUser(info.isAuthUser);
    setIsAdminUser(info.admin);
    }
  }



 

  return (
    <div className="App">
      <NavBar isAuthUser={isAuthUser} isAdmin={isAdmin} onLogout={logout} />
      <div className="m-4">
        <Routes>
            {!isAuthUser &&
            (<Route>
            <Route exact path="/" element={<Login onConnect={(info) => setInfo(info)}/>} />

            <Route path="/sign-in" element={<Login onConnect={(info) => setInfo(info)}/>} />
            
              <Route
                path="/sign-up"
                element={<SignUp />}
              />
            </Route>
            )}

          {isAuthUser && (
          <Route>
            <Route
              path="/home"
              element={<h3>Congrats, you made it !!! {token}</h3>}
            />
            <Route path="/graph" element={<ChartStock />} />

            <Route element={<RequireAdmin />}>
              <Route path="/admin" element={<AdminController />} />

              <Route path="/productManager" element={<ProductManager />} />
              
            </Route>
            </Route>)}

          <Route path="*" element={<Navigate to={isAuthUser ? "/graph" : "/"} />} />

            
        </Routes>
      </div>
    </div>
  );
}

export default App;
