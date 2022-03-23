
import React, { useState, useEffect } from "react";
import {Table, Button, Tab, Tabs} from "react-bootstrap";

//const BACKEND = "https://peps-group1.herokuapp.com/";
const BACKEND = "http://localhost:5000";


const AdminController = props => {

    const [users, setUsers] = useState([]);
    const [assets, setAssets] = useState([]);
    const [userToModifyIndex, setUserToModifyIndex] = useState(-1);
    const [userToModify, setUserToModify] = useState({username:"", password:"", admin:false, id:-1});
    const [assetToModifyIndex, setAssetToModifyIndex] = useState(-1);
    const [assetToModify, setAssetToModify] = useState({assetName:"", yahooCode:"", id:-1});
    const [assetToAdd, setAssetToAdd] = useState({assetName:"", yahooCode:"", id:-1});
    useEffect(()=>{
      // On utilise ici les fonctionnalités de javascript pour les requêtes Ajax
      // Nous n'utiliserons pas jquery au sein de reactnative.
      fetch(`${BACKEND}/api/users`, 
      //{credentials: 'include'}
      ).then(response => response.json())
      .then(data => setUsers(data.data))
      .catch(reason => console.log(reason))

      fetch(`${BACKEND}/api/assets`, 
      //{credentials: 'include'}
      ).then(response => response.json())
      .then(data => setAssets(data.data))
      .catch(reason => console.log(reason))
    },[]);

    useEffect(()=>{
        //console.log(assetToAdd)
        const newAssets = [...assets] //copy the array
        newAssets.push(assetToAdd) //execute the manipulations
        setAssets(newAssets); //set the new state
    },[assetToAdd.id])

    function handleClick(e){
        if (e.target.name === "modifier"){
            setUserToModifyIndex(e.target.value);
            setUserToModify(users[e.target.value]);
        }
        else {
            if (userToModify.admin === "true" || userToModify.admin === "false") {
            fetch(`${BACKEND}/api/users/modify/${userToModify.id}`, {
                method: "PUT",
                headers: { "Content-Type": "application/x-www-form-urlencoded" },
                body: `data=${JSON.stringify({ username: userToModify.username, password: userToModify.password, admin: userToModify.admin })}`,
                //credentials: 'include',
              })
                .then((response) => response.json())
                // .then((data) => {
                //   console.log(data.message);
                // })
                .catch((error) => console.log(error));
                const newUsers = [...users] //copy the array
                newUsers[userToModifyIndex] = userToModify //execute the manipulations
                setUsers(newUsers); //set the new state
                setUserToModifyIndex(-1);
            }
            else {
                alert("isAdmin should be a boolean : true or false")
            }
        }
    }

    async function handleClickAsset(e){
        if (e.target.name === "modifier"){
            setAssetToModifyIndex(e.target.value);
            setAssetToModify(assets[e.target.value]);
        }
        else {
            var requestOptions = {
            method: "GET",
            headers: {
              accept: "application/json",
              "Content-Type": "application/x-www-form-urlencoded",
            },
          };
          const url = new URL(`${BACKEND}/api/yahoo`);
          url.searchParams.set("yahooCode", assetToModify.yahooCode)
          url.searchParams.set("begin_date", Math.floor(new Date().getTime() / 1000)-86400);
          url.searchParams.set("end_date", Math.floor(new Date().getTime() / 1000))
          url.searchParams.set("frequency", "1d")
          await fetch(url, requestOptions)
            .then((response) => response.json())
            .then((body)=> 
            {if (body.status){
                modifyAsset()
            }else {
                alert('yahooCode non valide')
            }
            })
            .catch((e) => console.log(e));
        }
    }

    function modifyAsset() {
        fetch(`${BACKEND}/api/assets/modify/${assetToModify.id}`, {
            method: "PUT",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: `data=${JSON.stringify({ assetName: assetToModify.assetName, yahooCode: assetToModify.yahooCode })}`,
            //credentials: 'include',
          })
            .then((response) => response.json())
            .then((data) => {
                if(data.status){
                    const newAssets = [...assets] //copy the array
                    newAssets[assetToModifyIndex] = assetToModify //execute the manipulations
                    setAssets(newAssets); //set the new state
                    setAssetToModifyIndex(-1);
                }else {
                    alert("yahooCode déjà présent")
                }
            })
            .catch((error) => console.log(error));
            
    }
    function ajoutAsset() {
        fetch(`${BACKEND}/api/assets`, {
            method: "POST",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body: `data=${JSON.stringify({ assetName: assetToAdd.assetName, yahooCode: assetToAdd.yahooCode })}`,
            //credentials: 'include',
          })
            .then((response) => response.json())
            .then((data) => {
                if (data.status) setAssetToAdd({assetName: assetToAdd.assetName, yahooCode:assetToAdd.yahooCode, id:data.id});
                else alert("yahooCode déjà présent")
            })
            .catch((error) => console.log(error))
    }

    async function handleAddAsset() {
        var requestOptions = {
          method: "GET",
          headers: {
            accept: "application/json",
            "Content-Type": "application/x-www-form-urlencoded",
          },
        };
        const url = new URL(`${BACKEND}/api/yahoo`);
        url.searchParams.set("yahooCode", assetToAdd.yahooCode)
        url.searchParams.set("begin_date", Math.floor(new Date().getTime() / 1000)-86400);
        url.searchParams.set("end_date", Math.floor(new Date().getTime() / 1000))
        url.searchParams.set("frequency", "1d")
        await fetch(url, requestOptions)
          .then((response) => response.json())
          .then((body)=> 
          {if (body.status){
              ajoutAsset()
          }else {
              alert('yahooCode non valide')
          }
          })
          .catch((e) => console.log(e));
      }



    function handleDelete(e){
        fetch(`${BACKEND}/api/users/${users[e.target.value].id}`, {
            method: "DELETE",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
          })
            .then((response) => response.json())
            .then((data) => {
                // console.log(data.message);
                const newUsers = [...users]
                newUsers.splice(e.target.value, 1);
                setUsers(newUsers);
                
                setUserToModifyIndex(-1);
            })
            .catch((error) => console.log(error));
            
    }

    function handleDeleteAsset(e){
        fetch(`${BACKEND}/api/assets/${assets[e.target.value].id}`, {
            method: "DELETE",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
          })
            .then((response) => response.json())
            .then((data) => {
                // console.log(data.message);
                const newAssets = [...assets]
                newAssets.splice(e.target.value, 1);
                setAssets(newAssets);
                setAssetToModifyIndex(-1);
            })
            .catch((error) => console.log(error));
            
    }

    const onChangeUsername = (event) => {
        setUserToModify({...userToModify, username:event.target.value});
    };
    const onChangePassword = (event) => {
        setUserToModify({...userToModify, password:event.target.value});
    };
    const onChangeAdmin = (event) => {
        setUserToModify({...userToModify, admin:event.target.value});
    };
    const onChangeAssetName = (event) => {
        setAssetToModify({...assetToModify, assetName:event.target.value});
    };
    const onChangeYahooCode = (event) => {
        setAssetToModify({...assetToModify, yahooCode:event.target.value});
    };
    const onChangeAddAssetName = (event) => {
        setAssetToAdd({...assetToAdd, assetName:event.target.value});
    };
    const onChangeAddYahooCode = (event) => {
        setAssetToAdd({...assetToAdd, yahooCode:event.target.value});
    };



    function renderUser(user, index) {
        if (userToModifyIndex === index) {
            return  (
                <tr key = {index} className = {user.username}>
                    <td>{userToModify.id}</td>
                    <td><input value={userToModify.username} onChange={onChangeUsername}/></td>
                    <td><input value={userToModify.password} onChange={onChangePassword}/></td>
                    <td><input value={userToModify.admin.toString()} onChange={onChangeAdmin}/></td>
                    <td><Button value = {index} name = 'enregistrer'  onClick={handleClick}>Enregistrer</Button></td>
                    <td><Button value = {index} name = 'supprimer'  onClick={handleDelete}>Supprimer</Button></td>
                </tr> )
        } else {
            return  (
                <tr key = {index} className = {user.username}>
                    <td>{user.id}</td>
                    <td>{user.username}</td>
                    <td>{user.password}</td>
                    <td>{user.admin.toString()}</td>
                    <td><Button value = {index} name = 'modifier'  onClick={handleClick}>Modifier</Button></td>
                    <td><Button value = {index} name = 'supprimer'  onClick={handleDelete}>Supprimer</Button></td>
                </tr> )
        }
    }

    function renderAsset(asset, index){
        if (assetToModifyIndex === index) {
            return  (
                <tr key = {index} className = {asset.assetName}>
                    <td>{assetToModify.id}</td>
                    <td><input value={assetToModify.assetName} onChange={onChangeAssetName}/></td>
                    <td><input value={assetToModify.yahooCode} onChange={onChangeYahooCode}/></td>
                    <td><Button value = {index} name = 'enregistrer'  onClick={handleClickAsset}>Enregistrer</Button></td>
                    <td><Button value = {index} name = 'supprimer'  onClick={handleDeleteAsset}>Supprimer</Button></td>
                </tr> )
        } else {
            return (
                <tr key = {index} className = {asset.assetName}>
                    <td>{asset.id}</td>
                    <td>{asset.assetName}</td>
                    <td>{asset.yahooCode}</td>
                    <td><Button value = {index} name = 'modifier'  onClick={handleClickAsset}>Modifier</Button></td>
                    <td><Button value = {index} name = 'supprimer'  onClick={handleDeleteAsset}>Supprimer</Button></td>
                </tr>
            )
        }
        
        
    }

    const [key, setKey] = useState('users');

    return (
        <div className="auth-wrapper">
            <Tabs defaultActiveKey="home" transition={false} className="mb-3" activeKey={key} onSelect={(k) => setKey(k)}>
            <Tab  key = "users" eventKey="users" title="Users">
            <Table>
                <thead>
                    <tr>
                        <td>id</td>
                        <td>username</td>
                        <td>password</td>
                        <td>isAdmin?</td>
                        <td></td>
                        <td></td>
                    </tr>
                </thead>
                <tbody>
                {users.map((user, index) => renderUser(user, index))}
                </tbody>
            </Table>
            </Tab>
            <Tab  key = "assets" eventKey="assets" title="Assets">
            <Table>
                <thead>
                    <tr>
                        <td>id</td>
                        <td>assetName</td>
                        <td>yahooCode</td>
                        <td></td>
                        <td></td>
                    </tr>
                </thead>
                <tbody>
                    {assets.map((asset, index) => renderAsset(asset, index))}
                    <tr key = {assets.length} className = "newAsset">
                        <td></td>
                        <td><input onChange={onChangeAddAssetName} placeholder = "Asset Name"/></td>
                        <td><input onChange={onChangeAddYahooCode} placeholder = "Yahoo Code"/></td>
                        <td><Button value = {assets.length} name = 'ajout'  onClick={handleAddAsset}>Ajouter</Button></td>
                    </tr> 
                </tbody>
            </Table>
            </Tab>
                 
            </Tabs> 
        </div>
    );
}

export default AdminController;