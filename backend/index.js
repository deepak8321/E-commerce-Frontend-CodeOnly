const express = require('express');
const cors = require('cors')
require('./db/config') // data base ka url hai
const User = require('./db/User') // model
const Product = require("./db/Product")
const app = express();

const Jwt = require('jsonwebtoken')
const jwtkey='e-comm' // isko secure rakhna hai nahi koi use token banna lega 

// postman se data aayega midileware ki help se:- 
app.use(express.json())
app.use(cors())

// Make Route:- kis link ko hit kare gaye Api ko call karne ke liya
app.post('/register', async (req,resp)=>{
    // resp.send("api in Progress..") // yaha se data postman par ja raha
    // jo data json format me aa raha postman se node  me vahi data vapas ja raha postman me(middileware ki help se)
    // resp.send(req.body)

    // jo data aa raha postman se vo database me store kar rahe hai by using of model
    let user = new User(req.body);
    let result = await user.save() // time taking process use async and await
    
    // postman me password bhi show ho raha jo sahi nahi hai
    // login vale API ki tarah password remove nahi kar sakte
    // jab user save karte tab [.select("-password")] use nahi kar sakte
    // password delete by below method
    result = result.toObject(); // convert into an object
    delete result.password
    Jwt.sign({result},jwtkey, {expiresIn: "2h"},(err,token)=>{
        resp.send({result,auth: token});
        if(err){
            resp.send({result:'something went wrrong, Please try after some time'});
        }
    }) 
    // resp.send(result); // jo database me insert ho raha vo postman parsend kar rahe hai
})
// http://localhost:5000/register
// for check:- pass:- post->body->raw->JSON
// {
//     "name": "kalu",
//     "email":"kalu@gmail.com",
//     "password":"kalu@9236"
// }


app.post('/login',async (req,resp)=>{
    // resp.send(req.body)
    // console.log(resp.body);

    // dono sath me check ho email aur password koi bhi galat ho to login na ho
    if (req.body.password && req.body.email) {
        // password hata raha hai aur check kar raha hai email id aur password database me hai ki nahi 
        let user = await User.findOne(req.body).select("-password");
        if (user) {
            // agar user mil gaya to tocken pass ho jayega
            Jwt.sign({user},jwtkey, {expiresIn: "2h"},(err,token)=>{
                resp.send({user,auth: token});
                if(err){
                    resp.send({result:'something went wrrong, Please try after some time'});
                }
            })
            // resp.send(user);
        }else{
            resp.send({result:'No User Found'});
        }
    }else{
        resp.send({result:'No User Found'});
    }
})
// http://localhost:5000/login
// for check:- pass:- post->body->raw->JSON
// {
//     "email":"pak8465@gmail.com",
//     "password":"deepak4651"
// }

// Token varification: search vali API par apply kiya hai- 
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

app.post("/add-product",verifyToken,async (req,resp)=>{
    let product = new Product(req.body);
    let result = await product.save();
    resp.send(result);
})

// for check:- pass:- post->body->raw->JSON
// {
//     "name": "M14",
//     "price": "400",
//     "category": "mobaile",
//     "userId": "6491ea1f2be864365520c573", //id user me se liya hai abhi ke liya
//     "company": "samsung",
//     "_id": "6496baa9e8bad9dcac920010", 
//     "__v": 0
// }

app.get('/products',verifyToken,async (req,resp)=>{
    let products = await Product.find();
    // check product hoga to respone me send kar do
    if (products.length>0) {
        resp.send(products)
    }else{
        resp.send({result:"No Products Found"})
    }
})
// For check:- Get
// http://localhost:5000/products


// API For Delete Product from Product list:-
app.delete("/product/:id",verifyToken,async (req,resp)=>{
    // resp.send("working...")
    // postman/frontend se product ki id aa rahi ussi ko vapas postman ko send kar rahe hai 
    // resp.send(req.params.id)
    const result = await Product.deleteOne({_id:req.params.id})
    resp.send(result);

})
// For check:-Delete method:-
// http://localhost:5000/product/past_id from UpdateProduct

// 1:- API for Single Product:- then single product detail use in Prefilling data:- Product.js
// For Check:-
// http://localhost:5000/product/6496baa9e8bad9dcac920010
app.get('/product/:id',verifyToken,async (req,resp)=>{
    let result = await Product.findOne({_id:req.params.id});
    if (result) {
        resp.send(result)
    }else{
        resp.send({result:"No Record Found"})
    }
})

// 3:- Update API
app.put("/product/:id",verifyToken, async (req,resp)=>{
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

// API For Search Product:-
app.get("/search/:key",verifyToken,async (req,resp)=>{
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


app.listen(5000);