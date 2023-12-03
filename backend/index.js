const express = require("express");
const mysql = require("mysql");
const cors = require("cors");
const serialPort  = require('serialport');
const axios = require('axios');


const app = express();
const port = 3002;

const db = mysql.createConnection({
    user: "root",
    host: "localhost",
    password: "335276",
    database: "fire_detect",
})
db.connect();

const parsers = serialPort.parsers;


// 시리얼 포트 설정
// COM6 : 아두이노가 연결된 포트

// 아래 ####은 본인 아두이노의 시리얼 포트에 맞게 경로 입력하기
// var com6 = new serialPort('#####',{
//     baudrate : 9600,
//     // defaults for Arduino serial communication
//     dataBits : 8,
//     parity : 'none',
//     stopBits: 1,
//     flowControl: false
// })
// com6.on('open', function () {
//     console.log('open serial communication');
// })

app.get("/", function (req, res) {
    console.log('Arduino로부터 요청 받음');
    res.send("안녕하세요!");
});

//센서값 저장
app.get('/api/update', (req, res) => {
  const sensorValue = req.query.value;

  // DB에 센서 값 저장
  db.query('INSERT INTO sensor VALUES (?)', [sensorValue], (err, result) => {
    if (err) throw err;
    console.log('Data inserted');
    res.send('Data received');
  });
});

// 온도 데이터 가져오기
app.get('/api/getTemperature', (req, res) => {
    db.query('SELECT temperature FROM sensor ORDER BY id DESC LIMIT 1', (error, results) => {
        if (error) {
            return res.status(500).send({ message: "서버 에러 발생" });
        }
        res.status(200).json(results[0]);
    });
});

// 조도 데이터 가져오기
app.get('/api/getBrightness', (req, res) => {
    db.query('SELECT brightness FROM sensor ORDER BY id DESC LIMIT 1', (error, results) => {
        if (error) {
            return res.status(500).send({ message: "서버 에러 발생" });
        }
        res.status(200).json(results[0]);
    });
});

// 가스 데이터 가져오기
app.get('/api/getGas', (req, res) => {
    db.query('SELECT gas FROM sensor ORDER BY id DESC LIMIT 1', (error, results) => {
        if (error) {
            return res.status(500).send({ message: "서버 에러 발생" });
        }
        res.status(200).json(results[0]);
    });
});

// 적외선 데이터 가져오기
app.get('/api/getInfrared', (req, res) => {
    db.query('SELECT infrared FROM sensor ORDER BY id DESC LIMIT 1', (error, results) => {
        if (error) {
            return res.status(500).send({ message: "서버 에러 발생" });
        }
        res.status(200).json(results[0]);
    });
});



app.listen(port, () => console.log("3002포트에서 대기 중..."));