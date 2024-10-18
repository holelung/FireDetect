
# FireDetect

## 앱 다운로드 (안드로이드)

1. Expo dev  
   https://expo.dev//accounts/ciker/projects/FireDetect/builds/28d66e71-519d-4203-aff6-67d2181bd9cf

2. Google Drive  
   https://drive.google.com/file/d/1t8_V3WLzOsnDpPyPPkQDLc5SkuuRTsln/view?usp=sharing

---

## 프로젝트 소개
FireDetect는 화재를 실시간으로 감지하고 사용자에게 경고를 보내는 시스템입니다. 여러 센서를 통해 데이터를 수집하고 화재 발생 여부를 판단합니다.
### 앱화면
<img src="/images/firedetectapp.jpeg" width="50%" height="50%" title="앱 화면" alt="MainPage"></img>

## 설치 및 실행 방법

### Backend 설치
1. Node.js를 설치합니다.
2. 백엔드 폴더에서 필요한 종속성을 설치합니다:
   ```bash
   npm install
   ```
3. 백엔드 서버를 실행합니다:
   ```bash
   npm start
   ```

### Frontend 설치
1. Expo CLI를 설치합니다:
   ```bash
   npm install -g expo-cli
   ```
2. 프론트엔드 폴더에서 앱을 실행합니다:
   ```bash
   expo start
   ```

### Database 설정
1. `firedetect.sql` 파일을 사용하여 MySQL 데이터베이스를 설정합니다.

---

## 기술 스택
- **Backend**: Node.js, Express
- **Frontend**: React Native, Expo
- **Database**: MySQL
- **Hardware**: 다양한 센서 (조도, 적외선, 온도), 부저모듈

---

## 주요 기능 설명
- 실시간 화재 감지 및 알림
- 센서를 통해 수집된 데이터의 시각화
- 화재 발생 시 알림 시스템
- 데이터베이스에 센서 값 기록 및 분석

---

## DB 구성
센서마다 하나의 Table을 생성하여 데이터를 관리합니다.

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

---

# 통신 구성
<img width="592" alt="image" src="https://github.com/holelung/FireDetect/assets/66117028/3f3f6cb0-7520-4957-ac23-b58fd7674cec">
<img width="526" alt="image" src="https://github.com/holelung/FireDetect/assets/66117028/baab4d73-aead-475c-a0fa-e40c16616539">
