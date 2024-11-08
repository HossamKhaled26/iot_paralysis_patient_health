
void init_wifi()
{
  lcd.clear();
  lcd.print(F("Connecting Wifi"));

  while (notConected)
  {
     Serial.println("1");
    if (connectWiFi()) //connect to wifi with given SSID and password
    {
       Serial.println("2");
      while (!login)
      {
        if (GeckoVerify(id, pass)) //login to iotgecko.com with given ID and password
        {
          
          lcd.clear();
         //digitalWrite(led,HIGH);
          lcd.print(F("Connected"));
          login = true;
          notConected = false;
        }
      }
    }
    delay(1000);
  }
}



bool connectWiFi()
{
  sendCommand(F("AT+RST\r\n"), F("ready"), 2000, 1); // reset module
  Serial.println("3");
  esp8266.flush();
  if (sendCommand(ATE, ok, 1000, 1))
    if (sendCommand(F("AT+CIPMUX=1\r\n"), ok, 2000, 1))
      if (sendCommand(F("AT+CWMODE=3\r\n"), ok, 2000, 1)) // configure as access point
        if (sendCommand(CWJAP, ok, 20000, 1))
          return true;
        else
          return false;
      else
        return false;
    else
      return false;
  else
    return false;
}



bool GeckoVerify(String id, String password)
{
  if (sendCommand(CIPSTART, ok, 5000, 1))
  {
    linkTosendData = "GET /IOTHit.aspx?Id=" + id + "&Pass=" + password + "&Data=";
    linkTogetsiteResponse =  linkTosendData + "0" + Host;
    lenghtOflinstr = String(linkTogetsiteResponse.length()) + "\r\n" ;
    return true;
  }
  else
  {
    return false;
  }
}


bool sendCommand(String atcmd, String response, int timeout, int debug)
{
  esp8266.print(atcmd);
  String espResponse = "";
  int first = 0;
  if (debug == 1)
  {
    long current_time = millis();
    while (1)
    {
      espResponse = "";
      while (esp8266.available() > 0)
      {
        char  inChar = (char)esp8266.read();
        //        Serial.print(inChar );
        if ((inChar != '\n') || (inChar != '\r'))
        {
          espResponse += inChar;
        }
        if ((inChar == '\n') || (inChar == '\r'))
        {
          break;
        }
        if (first == 1)
        {
          data += inChar;
        }
      }
      espResponse.trim();
      if (((espResponse.indexOf(pinstr) >= 0) || (espResponse.indexOf(F("Label1")) >= 0) || (espResponse.indexOf(F("<span")) >= 0)) && (first == 0))
      {
        first++;
        data = espResponse;
        espResponse = "";
      }
      if (espResponse.indexOf(F("<tr>")) >= 0)
      {
        first++;
      }
      else if ((espResponse.endsWith(F("ERROR)")) || (espResponse.endsWith(F("FAIL")))))
      {
        return false;
      }
      if (espResponse.startsWith(F("busy")))
      {
        current_time = millis();
      }
      if ((current_time + timeout) < millis())
      {
        return false;
      }
      if (espResponse.indexOf(response) >= 0)
      {
        esp8266.flush();
        return true;
      }
    }
  }
  return 0;
}

int hit_link()
{
  int iot_status = SendandGetGParams(iot_data, no_of_senddata);/*, motor_status , no_of_getdata);*/ //send and get data on iotgecko.com
  if (iot_status == VALID)
  {
    return 1;
  }
  else
  {

    // lcd.print(iot_status);

    if ( connectWiFi())
      if (sendCommand(CIPSTART, ok, 5000, 1))
        while (!Reconnect()) //reconnect to the iotgecko.com
        {
        lcd.clear();
         lcd.print(F("reconnecting"));
          delay(1000);
          //Serial.println(F("conncetion failed.....reconnecting"));

        }
    lcd.clear();
   lcd.print(F("Connected"));
    delay(1000);

    return 0;  //Serial.println(F("connected succesfully"));
  }
}



int SendandGetGParams(String *dataTosend, int sizeOFarray) //, int*data_buff, int totalData)
{


  finallinktoSenddata = linkTosendData + dataTosend[0];
    Serial.println(finallinktoSenddata);
//  for (int loopForarry = 1 ; loopForarry < sizeOFarray ; loopForarry++)
//  {
//    finallinktoSenddata += '*';
//    finallinktoSenddata += dataTosend[loopForarry];
//  }
  sendCommand(CIPSEND, ">", 100, 0);
  finallinktoSenddata += Host;
  lenghtOflinstr = String(finallinktoSenddata.length()) + "\r\n" ;
  if (sendCommand(lenghtOflinstr, ">", 5000, 1))
  {
        Serial.println(finallinktoSenddata);

    if (sendCommand(finallinktoSenddata, F("</html>"), 10000, 1))
    {
//      for (int looptofilldata = 0 ; looptofilldata < totalData ; looptofilldata++)
//      {
//        data_buff[looptofilldata] = -1;
//      }
      if ((data.indexOf(err) >= 0)) // || (data.indexOf("No.")>=0) ||(data.indexOf("Invalid") >= 0))
      {
        err_index = data.indexOf(err);
        data = data.substring(err_index + 9, err_index + 13);
        data.trim();
        if (data.indexOf(error18) >= 0)
        {
          return InvalidUserIdOrPassword;
        }
        else if (data.indexOf(error45) >= 0)
        {
          return InvalidData;
        }
        else if (data.indexOf(error104) >= 0)
        {
          return HitLimitExceed;
        }
      }
      else
      {
        //        lcd.clear();
        //        lcd.print(data);
        data = data.substring(data.indexOf(pinstr) + 4);
        data = data.substring(0, data.indexOf('<') + 1);
        data.toCharArray(buff, data.length() + 1);
        //        lcd.setCursor(0, 1);
        //        lcd.print(data);
        //        delay(2000);
        //sscanf(buff, "%d*%d", & motor_status[0], & motor_status[1]);
        Serial.println(buff);
        
        if (sscanf(buff, "%c*%c", & motor_status/*[0]*/, & motor_status1/*[1]*/) == 2)
        {
//          lcd.clear();
//          lcd.print(motor_status[0]);
//          lcd.setCursor(0, 1);
//          lcd.print(motor_status[1]);
//motor_status[0]=char(motor_status[0]);
Serial.print("motor{0}:");
Serial.println(motor_status);
Serial.print("motor1:");
Serial.println(motor_status1);
        }

        else
    { //lcd.clear();
//          lcd.print("connecting.."); 
Serial.print("else");
Serial.print("motor{0}:");
Serial.println(motor_status);
Serial.print("motor1:");
Serial.println(motor_status1);
        }
        //        delay(3000);
        return VALID;
      }
    }
    else
    {
      return 1;
    }
  }
  else
  {
    return 1;
  }
}

bool Reconnect()
{
  sendCommand(F("AT+CIPCLOSE=4\r\n\r\n"), ok, 5000, 1);
  if (connectWiFi())
    if (GeckoVerify(id,  pass))
      return true;
    else
      return false;
  else
    return false;
}
