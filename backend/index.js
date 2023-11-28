const express = require("express");
const app = express();

app.get("/", function (req, res) {
  res.send("안녕하세요!");
});
app.listen(3002, () => console.log("3002포트에서 대기 중..."));