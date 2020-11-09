//This is code to read from all three sensors (Electrical Conductivity, pH, and Dissolved Oxygen) at the same time
//To send a command, first type what sensor it is intended for (EC: for EC sensor, PH: for pH sensor, DO: for DO sensor, AS: for all sensors) 
//and then type the command (C,0 to turn off; R to read one reading; C,1 to get a reading every second; ect....)
//Code has been taken from the sample code from Atlas Scientific for each sensor
//MC Lusardi - 10/27/2020

String inputstring = "";                              //a string to hold incoming data from the Python Script
boolean input_string_complete = false;                //have we received all the data from the Python Script
String sensorType = "";
String command = "";

String DOsensorstring = "";                           //a string to hold the data from the DO sensor
boolean DOsensor_string_complete = false;             //have we received all the data from the DO sensor
String DO = "";                                       //used to hold a floating point number that is the DO
boolean DOvalue_string_complete = false;

String ECsensorstring = "";                           //a string to hold the data from the EC sensor
boolean ECsensor_string_complete = false;             //have we received all the data from the EC sensor
String EC = "";                                       //used to hold a floating point number that is the pH
boolean ECvalue_string_complete = false;

String pHsensorstring = "";                           //a string to hold the data from the pH sensor
boolean pHsensor_string_complete = false;             //have we received all the data from the pH sensor
String pH = "";                                        //used to hold a floating point number that is the pH
boolean pHvalue_string_complete = false;

void setup() {
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial1.begin(9600);                                //set baud rate for software serial port_1 (pins 18 and 19) to 9600 (DO sensor)
  Serial2.begin(9600);                                //set baud rate for software serial port_2 (pins 16 and 17) to 9600 (EC sensor)
  Serial3.begin(9600);                                //set baud rate for software serial port_3 (pins 15 and 14) to 9600 (pH sensor)
  
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  DOsensorstring.reserve(30);                         //set aside some bytes for receiving data from DO sensor
  ECsensorstring.reserve(30);                         //set aside some bytes for receiving data from EC sensor
  pHsensorstring.reserve(30);                         //set aside some bytes for receiving data from pH sensor
}

//Function for reading input from serial (Python Script)
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the Python Script
}

//Function for reading input from serial1 (DO Sensor)
void serialEvent1() {                                 //if the hardware serial port_1 receives a char
  DOsensorstring = Serial1.readStringUntil(13);       //read the string until we see a <CR>
  DOsensor_string_complete = true;                    //set the flag used to tell if we have received a completed string from the PC
}

//Function for reading input from serial2 (EC Sensor)
void serialEvent2() {                                 //if the hardware serial port_2 receives a char
  ECsensorstring = Serial2.readStringUntil(13);       //read the string until we see a <CR>
  ECsensor_string_complete = true;                    //set the flag used to tell if we have received a completed string from the EC sensor
}

//Function for reading input from serial3 (pH Sensor)
void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  pHsensorstring = Serial3.readStringUntil(13);       //read the string until we see a <CR>
  pHsensor_string_complete = true;                    //set the flag used to tell if we have received a completed string from the pH sensor
}

void loop() {
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    sensorType = parseSensorTypeInput(inputstring);
    command = parseCommandInput(inputstring);
    if(sensorType.compareTo("DO:") == 0) {            //will see if input string begins with "DO:"
      //Serial.print("Sending command to DO: ");
      //Serial.println(command);
      Serial1.print(command);                         //send that string to the DO sensor
      Serial1.print('\r');                            //add a <CR> to the end of the string
    }
    else if(sensorType.compareTo("EC:") == 0) {       //will see if input string begins with "EC:"
      //Serial.print("Sending command to EC: ");
      //Serial.println(command);
      
      Serial2.print(command);                         //send that string to the EC sensor
      Serial2.print('\r');                            //add a <CR> to the end of the string
    }
    else if(sensorType.compareTo("PH:") == 0){        //will see if input string begins with "PH:"
      //Serial.print("Sending command to PH: ");
      //Serial.println(command);
      
      Serial3.print(command);                         //send that string to the pH sensor
      Serial3.print('\r');                            //add a <CR> to the end of the string
    }
    else if(sensorType.compareTo("AS:") == 0){        //will see if input string begins with "PH:"
      //Serial.print("Sending command to AS: ");
      //Serial.println(command);
      
      Serial1.print(command);                         //send that string to the DO sensor
      Serial1.print('\r');                            //add a <CR> to the end of the string
      
      Serial2.print(command);                         //send that string to the EC sensor
      Serial2.print('\r');                            //add a <CR> to the end of the string
      
      Serial3.print(command);                         //send that string to the pH sensor
      Serial3.print('\r');                            //add a <CR> to the end of the string
    }
    else {
      Serial.println("incorrect format for command");
    }
    inputstring = "";                                 //clear the string
    sensorType = "";
    command = "";
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the Python Script 
  }

  //What to do if input comes from the DO sensor
  if (DOsensor_string_complete == true) {               //if a string from the DO sensor has been received in its entirety
    if (isdigit(DOsensorstring[0]) == false) {
      Serial.print("DO: ");
      Serial.println(DOsensorstring);                     //send that string to the PC's serial monitor
    }
    else {
      DO = DOsensorstring;
      DOvalue_string_complete = true;
      //Serial.print("DO: ");
      //Serial.println(DO);
    }
    DOsensorstring = "";
    DOsensor_string_complete = false;
  }

  //What to do if input comes from the EC sensor
  if (ECsensor_string_complete == true) {             //if a string from the EC sensor has been received in its entirety
    if (isdigit(ECsensorstring[0]) == false) {        //if the first character in the string is not a digit
      Serial.print("EC: ");
      Serial.println(ECsensorstring);                 //send that string to the PC's serial monitor
    }
    else {                                            //if the first character in the string is a digit
      get_EC_data();                                  //then call this function
      //Serial.print("EC: ");
      //Serial.println(EC);
    }
    ECsensorstring = "";
    ECsensor_string_complete = false;
  }

  //What to do if input comes from the pH sensor
  if (pHsensor_string_complete == true) {             //if a string from the pH sensor has been received in its entirety
    if (isdigit(pHsensorstring[0]) == false) {
      Serial.print("PH: ");
      Serial.println(pHsensorstring);                 //send that string to the PC's serial monitor
    }
    else {
      pH = pHsensorstring;
      pHvalue_string_complete = true;
      //Serial.print("PH: ");
      //Serial.println(pH);
    }
    pHsensorstring = "";
    pHsensor_string_complete = false;
  }

  //print data if all three sensors have values available
  if(ECvalue_string_complete == true  && pHvalue_string_complete == true && DOvalue_string_complete == true) {
    //Serial.println("Values filled");
    printASData(DO, EC, pH);
    DOvalue_string_complete = false;                                          //Resetting values
    ECvalue_string_complete = false;
    pHvalue_string_complete = false;
  }
  else if (ECvalue_string_complete == true && pHvalue_string_complete == true) {
    printECPHData(EC, pH);
    ECvalue_string_complete = false;
    pHvalue_string_complete = false;
  }
}

//this function will return a string of the first three characters of the input, which will indicates the sensor it's intended for
String parseSensorTypeInput(String input){
  String result = input.substring(0,3);
  //Serial.print("First three char are: ");
  //Serial.println(result);
  return result;
}

//this function will return a string of the characters after the first three in the input, which indicates the command
String parseCommandInput(String input) {
  String result = input.substring(3);
  //Serial.print("Last char are: ");
  //Serial.println(result);
  return result;
}

//EC needs a separate function to print data because it reads in additional values besides the EC value
void get_EC_data(void) {
  char ECsensorstring_array[30];                          //we make a char array
  
  ECsensorstring.toCharArray(ECsensorstring_array, 30);   //convert the string to a char array
  EC = strtok(ECsensorstring_array, ",");       //let's pars the array at each comma, (in C++, const char* == string)
  ECvalue_string_complete = true;
  //Serial.print("EC:");                                  //we now print each value we parsed separately
  //Serial.println(EC);                                   //this is the EC value
}

//Prints data from all sensors on one line
void printASData(String doValue, String ecValue, String phValue) {
  /*char resultBuffer[50];
  sprintf(resultBuffer, "DO: %s, EC: %s, PH: %s", doValue.c_str(), ecValue.c_str(), phValue.c_str());
  Serial.println(resultBuffer);*/

  Serial.print("DO: ");
  Serial.print(doValue);
  Serial.print(", EC: ");
  Serial.print(ecValue);
  Serial.print(", PH: ");
  Serial.println(phValue);
}

//Prints data from just pH and EC sensors on one line
void printECPHData(String ecValue, String phValue) {
  Serial.print("EC: ");
  Serial.print(ecValue);
  Serial.print(", PH: ");
  Serial.println(phValue);
}
