# FireDetect

## 앱 다운로드 (안드로이드)

1. Expo dev
https://expo.dev//accounts/ciker/projects/FireDetect/builds/28d66e71-519d-4203-aff6-67d2181bd9cf

2. Google Drive
https://drive.google.com/file/d/1t8_V3WLzOsnDpPyPPkQDLc5SkuuRTsln/view?usp=sharing


앱 화면 
=======
<img src="/images/firedetectapp.jpeg" width="50%" height="50%" title="앱 화면" alt="MainPage"></img>



DB 구성
=======
## 센서마다 하나의 Table을 생성함.

### 1. 조도센서
| Id | value | creat_time | status |
|---|---|---|---|
|1|443|2023-12-13 02:18:33|Normal|

### 2. 적외선 센서
| Id | value | creat_time | status |
|---|---|---|---|
|1|0|2023-12-13 02:18:33|Not Exist|

### 3. 온도센서
| Id | value | creat_time | status |
|---|---|---|---|
|1|22|2023-12-13 02:18:33|Normal|

### 4. 부저모듈
| Id | value | creat_time | status |
|---|---|---|---|
|1|Off|2023-12-13 02:18:33|Null|

### 5. 화재감지여부 테이블
| Id | value | creat_time | status |
|---|---|---|---|
|1|Safe|2023-12-13 02:18:33|Null|



