import 'package:flutter/material.dart';
import 'package:iotsmart/login.dart';
import 'package:iotsmart/sigpup.dart';

class HomePage extends StatelessWidget {
@override
Widget build(BuildContext context) {
return Scaffold(
backgroundColor: Colors.white,
appBar: AppBar(
backgroundColor: Colors.white,
//title: Text('Welcome to your smart home application',style: TextStyle(
//color: Colors.white , fontWeight: FontWeight.bold,letterSpacing: 1

//),), centerTitle:true,

),
body:SafeArea(

child: Container(
//we will givemedia query height
//doube.infinity make it big as my perant allows
//while mediaquery make it big as per the screen
width: double.infinity,
height: MediaQuery.of(context).size.height,
padding: EdgeInsets.symmetric(horizontal: 30,vertical: 50),

child: Column(
//even space distr
mainAxisAlignment: MainAxisAlignment.spaceBetween,
crossAxisAlignment: CrossAxisAlignment.center,
children:<Widget> [
Column(
children: <Widget>[
Text('🤗 Welcome 🤗 ',
style: TextStyle(
fontWeight: FontWeight.bold,
color: Colors.deepOrange,
fontSize: 30,

),
),
SizedBox(
height: 20,
),
Text('The app about Smart Home',textAlign: TextAlign.center,
style: TextStyle(
color: Colors.blueGrey,
fontSize: 20,

),)
],
),
Container(
padding: EdgeInsets.only(top: 100),
height: 200,
decoration: BoxDecoration(
image: DecorationImage(
image: NetworkImage('https://icon-library.com/images/smart-home-icon/smart-home-icon-1.jpg'),
),
),
),
Column(
children: <Widget>[
//the login button
MaterialButton(
color: Colors.brown.shade300,
minWidth: double.infinity,
height: 60,
onPressed: (){
Navigator.push(context, MaterialPageRoute(builder: (context)=> LoginPage()));
},
//defining the shape
shape: RoundedRectangleBorder(
side: BorderSide(
color: Colors.brown.shade300
),
borderRadius: BorderRadius.circular(50)
),
child: Text("Login",
style:TextStyle(
color: Colors.black,
fontWeight: FontWeight.w600,
fontSize: 20
) ,),
)
//creating the signup button
,SizedBox(height: 20),
MaterialButton(minWidth: double.infinity,
height: 60,
onPressed: (){
Navigator.push(context, MaterialPageRoute(builder: (context)=>SignUpPage()));
},
color: Color(0xff573b2a),
shape: RoundedRectangleBorder(
borderRadius: BorderRadius.circular(50)
),
child: Text(
"sign up",
style: TextStyle(
color: Colors.white,
fontWeight: FontWeight.w600,
fontSize: 20
),
),
)


],
)
],
),
),

),
);
}
}