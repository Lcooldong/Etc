import React from 'react';
// import logo from './logo.svg';
// import './App.css';
import {Button, Progress, Input} from 'semantic-ui-react';

function Login(){
  return(
    <div>
      <Input/>
      <Button primary onClick = {() => alert("Pushed")}>로그인</Button>
  </div>

  )

}

class Password extends React.Component{
  constructor(){
    super()
    this.state = {pw : "입력해주세요!"}
  }


  render(){
    return(
      <div>
      <Input onChange = {(e)=> this.setState({pw : e.target.value})}/>
      <Button value = {this.state.pw} onClick = {(event) => {
        alert(this.state.pw)//   event.target.value
      }}> Password </Button>
      </div>
    )
  }
}


function App() {
  return (
    <div>
      <p>안녕하세요!</p>
      <Button primary> 버튼 </Button>
      <Progress percent = {25} indicating/>
      <Login/>
      <Password/>
</div>
  );
}

export default App;
