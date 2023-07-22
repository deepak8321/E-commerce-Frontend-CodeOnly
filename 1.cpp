backend :- 



Route:- jo humara complete page banta ek url par
Link:- jo humara link banta like 'href'
Nav.js par sabhi Link place kiya hai :- Sabhi Link ke Routes Banne gaye
Sabhi Routes ko App .js par Place karo

Agar Routing Lage ga to BrowserRouting ke andar rahe ga

// mongoose me schema tabhi use aati jab insert, update, Delete Perform karte hai
// Data Get karne ke liya Schema Define karne ke jarurat nahi

Model Schema ko check karta hai DataBase ke andar kon sa data nikalna hai ya nahi
example:- 
koi Schema me 3 Field Define hai To model 3 field hi nikal kar layega koi aur nahi
Model Schema data find karne me help karta hai database se

    const express = require('express');
    const mongoose = require('mongoose')
    const app = express();
    const connectDB = async ()=>{
        mongoose.connect('mongodb://localhost:27017/e-comm');
        const productSchema = new mongoose.Schema({})
        const product = mongoose.model('product',productSchema)
        const data = await product.find();
        console.log(data);
    }
    connectDB();
    app.listen(5000);


Signup API:-
Make Inside campass:- Database:- e-commerce and collection:- users


Fix Cors issue in Node js:-
jab bhi ya issue aata hai to yasha lage ga Frontend ki issue hai but vo backend ki issue hoti
because vo postman par sahi run karta but browser par ya issue dekhne ko milata
Cors issue:- request block ho jata hai browser se API hit karte tab kuch security issue ki vajah se.


    // collectData data ko store karke ab API me send karna hai
    // iske andar hi data API me ja raha hai
    // axios module bhi use kar sakte at place of fetch
    // fetch javascript ka part hai and axios ek new module hai 
    const collectData = async () => {
        console.warn(name, email, password);
        let result = await fetch('http://localhost:5000/register', {
            // POST:- Data save karna hai and body ke andar data hota vo jata
            // agar kuch alag se header send karna hoga to backend devloper bol dega(nahi to 'Content-Type': 'application/json')
            method: 'post',
            body: JSON.stringify({ name, email, password }),
            headers: {
                'Content-Type': 'application/json'
            },
        });
        result = await result.json();
        console.warn(result);
    }

keep data in LocalStorage:-
refresh hone ke baad bhi data LocalStorage me save rahta hai(kafi dino tak data store rahta)

Private Component:- jab hum components hide show karna hota koi condition par
Private components me codition check hoti tab components show hoti

check Fatch API in Chrome:- Inspect->Network->Fatch/XHR
check localStorage in Chrome:- Inspect->Application->localStorage

req.params.id:- postman/frontend se jis product ki id ko send  
// resp.send(req.params.id):- postman/frontend se product ki id aa rahi ussi ko vapas postman ko send kar rahe hai 

*****************************************************

// new notes:-
2 api ka same url ho sakta but method dusari honi chahiya.
Backend:-
// 1:- API for Single Product:- then single product detail use in Prefilling data:- Product.js
// For Check:-
// http://localhost:5000/product/6496baa9e8bad9dcac920010
app.get('/product/:id',async (req,resp)=>{
    let result = await Product.findOne({_id:req.params.id});
    if (result) {
        resp.send(result)
    }else{
        resp.send({result:"No Record Found"})
    }
})

// 3:- Update API
app.put("/product/:id", async (req,resp)=>{
    let result = await Product.updateOne(
        {_id:req.params.id},
        {
            $set:req.body
        }
    )
    resp.send(result);
})
// for check:-
// http://localhost:5000/product/6496baa9e8bad9dcac920010
// Postman->Body->raw->JSON
// {
//     "price":900,
//      "company": "microsoft"
// }
//  id update nahi hoti but custum Id daal sakte

// Frontend:-
// 1:- Integrate single product API
// 2:- Prefill Update Product from
// 3:- Integrate update product API
    
    import React, { useEffect, useState } from 'react'
    import { useNavigate, useParams } from 'react-router-dom';

    const UpdateProduct = () => {
        // collect all value input by clicking button
        const [name, setName] = useState('')
        const [price, setPrice] = useState('')
        const [category, setCategory] = useState('')
        const [company, setCompany] = useState('')
        const [error, setError] = useState(false)
        const params = useParams();
        const navigate = useNavigate();

        useEffect(()=>{
        // console.warn(params); // params aa raha hai
        getProductDetails();
        },[])
        
        const getProductDetails = async ()=>{
        // console.warn(params);
        //1:-  Integrate Single Product API Data.
        let result = await fetch(`http://localhost:5000/product/${params.id}`) // promise ruturn karega
        result = await result.json(); // data aa raha hai prefill kqarne ke liya 
        // console.warn(result);

        //2:- Prefilling data:-
        setName(result.name);
        setPrice(result.price);
        setCategory(result.category);
        setCompany(result.company)
        }
        
        // 3:- Integrate Update API:-
        const updateProduct = async ()=> {
            // console.warn(name,price,category,company);
            let result = await fetch(`http://localhost:5000/product/${params.id}`,{
            method:'Put',  
            body: JSON.stringify({name,price,category,company}),
            headers:{
            'Content-Type':"application/json"
            }
        });
        result = await result.json()
        console.warn(result);
        navigate('/');
        }

    return (
        <div className='product'>
        <h1>Update Product</h1>
        <input type="text" placeholder='Enter Product Name' className='inputBox'
        value={name} onChange={(e)=>{setName(e.target.value)}}
        />
        
        <input type="text" placeholder='Enter Product Price' className='inputBox'
        value={price} onChange={(e)=>{setPrice(e.target.value)}}
        />

        <input type="text" placeholder='Enter Product category' className='inputBox'
        value={category} onChange={(e)=>{setCategory(e.target.value)}}
        />

        <input type="text" placeholder='Enter Product company' className='inputBox'
        value={company} onChange={(e)=>{setCompany(e.target.value)}}
        />

        <button onClick={updateProduct} className='appButton'>Update Product</button>
        </div>
    )
    }
    export default UpdateProduct;
    // import in App.js

// Search API For Product:-
// Backend:-
app.get("/search/:key",async (req,resp)=>{
    let result = await Product.find({
        // ek se jada field me search karte to use:- "$or"
        "$or":[ // array
            {name:{$regex: req.params.key}}, // name field seach kar sakte 
            {company:{$regex: req.params.key}},
            {category:{$regex: req.params.key}}
        ]
    });
    resp.send(result)
})
// For check
// http://localhost:5000/search/M14 
// http://localhost:5000/search/v
// v:- jaha jaha hoga sab display ho jayega(name,company,category)

// Frontend:-
// ProductList:- add this line
<input type='text' className='search-product-box' placeholder='Search Product' 
    onChange={searchHandle} />
// Dekho:- SearchHandle Function kaise kaam kar raha dekho.
// Esko dekho kaise kaam kar raha hai:- agar product nahi hoga search karne par to:- record not found

// Product List:-
    import React, { useEffect, useState } from 'react'
    import { Link } from 'react-router-dom'

    const ProductList = () => {
        const [products, setProducts] = useState([])
        // useEffect method se getProducts function call kar rahe hai  
        useEffect(() => {
            getProducts();
        }, [])

        const getProducts=async()=>{
            // single Product Data come
            let result = await fetch('http://localhost:5000/products');
            result = await result.json();
            setProducts(result);
        }
        // console.warn('products', products);

        const deleteProduct= async (id)=>{
            // console.warn(id);
            // Delete Api call:-
            let result = await fetch(`http://localhost:5000/product/${id}`,{
                method: "Delete"
            });
            result = await result.json();
            if (result) {
                // alert("record is deleted")
                // yaha par getProducts call karne se screen par turant show hoga after deleting product
                getProducts();
            }
        }
        const searchHandle = async (event)=>{ 
            console.warn(event.target.value); // sabhi element ko print kar deta
            let key = event.target.value;
            // agar key me kuch na ho getProduct vala function re-call ho jaye:- Product dubara list ho jayega
            if (key) {
                let result = await fetch(`http://localhost:5000/search/${key}`)
                result = await result.json();
                if (result) {
                    setProducts(result)
                }
            }else{
                getProducts()
            }
        }
        
    return (
        <div className='product-list'>
            <h3>Product List</h3>
            <input type='text' className='search-product-box' placeholder='Search Product' 
                onChange={searchHandle} />
            <ul>
                <li>S.No</li>
                <li>Name</li>
                <li>Price</li>
                <li>Category</li>
                <li>Operation</li>
            </ul>
            {
                // agar product nahi hoga search karne par to:- record not found
                products.length>0 ? products.map((item,index)=>
                <ul key={item._id}>
                <li>{index+1}</li>
                <li>{item.name}</li>
                <li>{item.price}</li>
                <li>{item.category}</li>
                <li>
                    <button onClick={()=>deleteProduct(item._id)}>Delete</button>
                    <Link to={'/update/'+item._id}>Update</Link>
                </li>
                </ul>
                )
                : <h1>No Record Found</h1>
            }
        </div>
    )
    }
    export default ProductList
    // import in app.js


// JWT Authentication:-
// (JSON Web Token)
// jab bhi humko API ko private karna hai to hum JWT token ka use karte hai(taki koi aur user humare API ko use na kar sake)
// jab bhi koi user login karta hai tab usko ek token milata hai jisse hum use user ko backend database use karne deta hai
// data secure karne ke liya JSON Web Token use hota hai

// ques:-
// front-ques:- how to manage jwt-token in frontend when come from API
// backend:- api me jwt-Token kaise send kare gaye 

// backend:-
// npm i jsonwebtoken

// 1:- JWT Token baana raha hai
// index.js:- backend
// const Jwt = require('jsonwebtoken')
// const jwtkey='e-comm'

// har bar tocken alag alag genrate hota hai and 2hour baad expire bhi ho jaye ga
// add in Login API and Registation API:-
// Jwt.sign({user},jwtkey, {expiresIn: "2h"},(err,token)=>{
//     resp.send({user,auth: token});
//     if(err){
//         resp.send({result:'something went wrrong, Please try after some time'});
//     }
// })

// 2:- Handle kar rahe hai Token ko frontend par and localstorage me send kar rahe hai:-
// localeStorage me store karo token ko:-
// Login.js:-
// if (result.auth) {
//     localStorage.setItem('user', JSON.stringify(result.user)) // user : is key
//     localStorage.setItem('token', JSON.stringify(result.auth)) // tocken ko localstorage me store kar rahe hai.
//     navigate('/') // home(product) page par redirect kar diya
// }

// check All API:-login any user:- --> Network--> login--> (login and Product API call)
// check All API:-login any user:- --> Application--> go localstorage url-> (see user and tocken is set)

// Same with SignUp.js:- add this line 
// localStorage.setItem('user', JSON.stringify(result.result)) // user : is key
// localStorage.setItem('token', JSON.stringify(result.auth))

// 3:- dusare API me tockn send kar rahe hai
// localstorage se token ko liya hai:- kyuki backend se token product API me nahi aa rah hai
// ProductList.js
// let result = await fetch('http://localhost:5000/products',{
//     headers:{
//         authorization: JSON.parse(localStorage.getItem('token'))
//     }
// });
// token localstoreage se aa raha product API me
// For check:- postman-> Get->(http://localhost:5000/products)->Headers->Authorization->send

// 4:-Token varify kar rahe hai:- token sahi hai ya nahi.
// index.js:- backend
function verifyToken(req,resp,next) {
    let token = req.headers['authorization'];
    if (token) {
        token = token.split(' ')[1]; // array me aa jayega 
        // console.warn("middleware called if", token);
        Jwt.verify(token,jwtkey, (err,valid)=>{
            if (err) {
                resp.status(401).send({result: "Please provide valid token"})
            }else{
                next();
            }
        })
    }
    else{
         resp.status(403).send({result: "Please add token with header"})
    }
    // console.warn("middleware called", token);
    // next()
}
// For check:- postman-> Get->(http://localhost:5000/search/0)->Headers->Authorization-> key:- bearer (paste token)->send-> vs code terminal  

// backend par sabhi API par verifyToken function call karo
// frontend par sabhi API par headers add karo:-
headers:{
    authorization: `bearer ${JSON.parse(localStorage.getItem('token'))}`
}