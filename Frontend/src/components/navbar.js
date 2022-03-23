import React from "react";
import Navbar from 'react-bootstrap/Navbar';
import Container from 'react-bootstrap/Container';

//import "../../node_modules/bootstrap/dist/css/bootstrap.min.css";
//import "../App.css";
import {
  Link
} from "react-router-dom";



const NavBar = (props) => {
      let AdminButton = null;
      let ProductManagerButton = null;
      if (props.isAdmin) {
        AdminButton = <li className="nav-item adminbtn">
        <Link className="nav-link" to={"/admin"}>
          DB Controler
        </Link>
      </li>;
      ProductManagerButton = <li className="nav-item adminbtn">
      <Link className="nav-link" to={"/productManager"}>
        Product Manager
      </Link>
    </li>;
      

      }
      // console.log(AdminButton);



    //console.log(props.isAuthUser);
  return ( props.isAuthUser === false ?
    <Navbar expand="sm">
    <Container fluid>
    {/* <Navbar.Brand href="/sign-in">React-Bootstrap</Navbar.Brand> */}
      <Link className="navbar-brand" to={"/sign-in"}>
        BANQUE
      </Link>
      <Navbar.Toggle aria-controls="basic-navbar-nav" />
      <Navbar.Collapse id="basic-navbar-nav">
          <ul className="navbar-nav ml-auto">
            <li className="nav-item">
              <Link className="nav-link" to={"/sign-in"}>
                Login
              </Link>
            </li>
            <li className="nav-item">
              <Link className="nav-link" to={"/sign-up"}>
                Sign up
              </Link>
            </li>
          </ul>
        </Navbar.Collapse>
      </Container>
    </Navbar> :
    <Navbar  expand="sm">
    <Container fluid>
    {/* <Navbar.Brand href="/sign-in">React-Bootstrap</Navbar.Brand> */}
      <Link className="navbar-brand" to={"/sign-in"}>
         BANQUE ENSIMAG
      </Link>
      <Navbar.Toggle aria-controls="basic-navbar-nav" />
      <Navbar.Collapse id="basic-navbar-nav">
        <ul className="navbar-nav ml-auto">
          <li className="nav-item">
          <button type="button" className="btn btn-light logoutbtn" onClick={() => props.onLogout()}>
              Logout
          </button>
          </li>
          <li className="nav-item">
              <Link className="nav-link" to={"/graph"}>
                Dashboard
              </Link>
            </li>
            {AdminButton}
            {ProductManagerButton}
            {/* <li className="nav-item">
          <a className="nav-link" href="#">Link</a>
        </li> */}
        
        
        </ul>
        {/* <form className="d-flex">
        <input className="form-control me-2" type="search" placeholder="Search" aria-label="Search"></input>
        <button className="btn btn-outline-success" type="submit">Search</button>
      </form> */}
      </Navbar.Collapse>
    </Container>
  </Navbar>
  );
};

export default NavBar;
