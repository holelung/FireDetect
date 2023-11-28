import * as Location from 'expo-location';
import React, { useEffect, useState } from "react";
import { View, Text, TouchableOpacity, StyleSheet, Dimensions, ActivityIndicator } from 'react-native';
import { StatusBar } from 'expo-status-bar';
import { Fontisto } from "@expo/vector-icons";
import moment from 'moment';

const API_KEY = "b0f19f92cddff3a3762ecc76d0a4917c";
const { width : SCREEN_WIDTH} = Dimensions.get('window');


const icons = {
  Clouds: "cloudy",
  Clear: "day-sunny",
  Atmosphere: "cloudy-gusts",
  Snow: "snow",
  Rain: "rains",
  Drizzle: "rain",
  Thunderstorm: "lightning",
} 

const AppScreen = () => {

  const [city, setCity] = useState("Loading...");
  const [days, setDays] = useState([]);

  var date = moment().utcOffset('+09:00').format('YYYY-MM-DD hh:mm:ss');

  const getWeather = async() => {
    const {granted} = await Location.requestForegroundPermissionsAsync(); 
    if(!granted){
      setOk(false);
    }
    const {coords:{latitude, longitude}} = await Location.getCurrentPositionAsync({accuracy:5});
    const location = await Location.reverseGeocodeAsync({latitude, longitude}, {useGoogleMaps:false});
    setCity(location[0].city);
    const response = await  fetch(`https://api.openweathermap.org/data/2.5/weather?lat=${latitude}&lon=${longitude}&appid=${API_KEY}&units=metric`);
    const json = await response.json();
    setDays([json]);
    
  };
  useEffect(() => {
    getWeather(); 
  }, [])


  return (
    <View style={styles.container}>
       <StatusBar style="dark" />
      <View style={styles.weatherBox}>
        <Text style={styles.weatherText}>{city}</Text>
        <View styles={styles.weather}>
          {days.length === 0 ?(
            <View style={styles.day}>
              <ActivityIndicator 
                color="white" 
                style={{ marginTop:10 }}
                size="large"
              />
            </View>
          ):(
            days.map((day, index) => 
              <View key={index} style={styles.day}>
                <Text style={styles.today}>{(date).slice(5, 7)}월 {(date).slice(8, 10)}일 {(date).slice(11, 13)}시</Text>
                <View style={styles.tempView}>
                  <Text style={styles.temp}>{parseFloat(day.main.temp).toFixed(1)}</Text>
                  <Fontisto name={icons[day.weather[0].main] } size={58 } color="white" />
                </View>
                <Text style={styles.description}>{day.weather[0].main}</Text>
                <Text style={styles.tinyText}>{day.weather[0].description}</Text>
              </View>
            )
          )} 
        </View>
        <TouchableOpacity ><Text style={styles.smallButtonText}>위치설정</Text></TouchableOpacity>
      </View>
      <View style={styles.largeButton2}>
        <View style={styles.largeButton2Header}>
          <Text style={styles.largeButton2Text}>화재감지 여부</Text>
          <TouchableOpacity>
            <Text style={styles.smallButton2Text}>작동 on/off</Text>
          </TouchableOpacity>
        </View>
        <View style={styles.isFire}>
            <Text style={styles.isFireText}>화재감지 여부: 화재아님</Text>
        </View>
      </View>      
      

      <View style={styles.grid}>
        <TouchableOpacity style={styles.gridButton}>
          <Text style={styles.gridButtonText}>온도</Text>
        </TouchableOpacity>
        <TouchableOpacity style={styles.gridButton}>
          <Text style={styles.gridButtonText}>가스</Text>
        </TouchableOpacity>
        <TouchableOpacity style={styles.gridButton}>
          <Text style={styles.gridButtonText}>조도</Text>
        </TouchableOpacity>
        <TouchableOpacity style={styles.gridButton}>
          <Text style={styles.gridButtonText}>적외선</Text>
        </TouchableOpacity>
      </View>

      <View style={styles.footerButtons}>
        <TouchableOpacity style={[styles.footerButton, styles.leftButton]}>
          <Text style={styles.footerButtonText}>부재 on/off</Text>
        </TouchableOpacity>
        <TouchableOpacity style={[styles.footerButton, styles.rightButton]}>
          <Text style={styles.footerButtonText}>화재 신고</Text>
        </TouchableOpacity>
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    margin:10,
    padding: 10,
    backgroundColor: 'white', // Replace with the actual background color of your screen
  },
  weatherBox: {
    flex: 2,
    color: 'white',
    backgroundColor: 'blue', // Replace with your actual button color
    padding: 20,
    borderRadius: 10,
    marginTop:40,
    marginBottom: 20,
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  weather: {
    color: 'white',
  },
  weatherText: {
    color: 'white',
  },

   largeButton2: {
    flex: 2,
    backgroundColor: 'blue', // Replace with your actual button color
    padding: 20,
    borderRadius: 10,
    marginBottom: 20,
  },
  largeButton2Header: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  largeButtonText: {
    color: 'white',
    fontSize: 18,
  },
  smallButtonText: {
    color: 'white',
    fontSize: 14,
  },
  largeButton2Text: {
    color: 'white',
    fontSize: 18,
    justifyContent: "flex-start"
  },
  smallButton2Text: {
    color: 'white',
    fontSize: 14,
  },
  locationButtonText: {
    color: 'white',
    fontSize: 14,
  },
  isFire: {
    marginTop: 30,
    alignItems: 'center',
    justifyContent:"center",
  },
  isFireText: {
    color: 'white',
    fontSize: 28,
  },

  grid: {
    flex: 5,
    flexDirection: 'row',
    flexWrap: 'wrap',
    justifyContent: 'space-between',
    marginTop: 20,
    marginBottom: 10,
  },
  gridButton: {
    backgroundColor: '#f0f0f0', // Replace with your actual button color
    width: '48%', // Approximately 2 columns
    padding: 50,
    borderRadius: 10,
    marginBottom: 10,
    alignItems: 'center',
  },
  gridButtonText: {
    fontSize: 16,
  },
  footerButtons: {
    flex: 1,
    marginTop: -10,
    marginBottom: 10,
    flexDirection: 'row',
    justifyContent: 'space-between',
  },
  footerButton: {
    flex: 1,
    padding: 20,
    borderRadius: 10,
  },
  leftButton: {
    backgroundColor: 'darkblue', // Replace with your actual button color
    marginRight: 5,
  },
  rightButton: {
    backgroundColor: 'darkred', // Replace with your actual button color
    marginLeft: 5,
  },
  footerButtonText: {
    color: 'white',
    textAlign: 'center',
    fontSize: 16,
  },
  today:{
    color: 'white',
  },
  tempView: {
    
  },
  temp: {
    color: 'white',
  },
  description: {
    color: 'white',
  },
  tinyText: {
    color: 'white',
  },

});

export default AppScreen;
