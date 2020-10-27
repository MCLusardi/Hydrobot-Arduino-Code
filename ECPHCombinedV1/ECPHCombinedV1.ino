//This is test code to read in from two sensors (Electrical Conductivity and pH) at the same time
//Code has been taken from the sample code from Atlas Scientific for each sensor
//MC Lusardi - 10/23/2020

String inputstring = "";                              //a string to hold incoming data from the Python Script
boolean input_string_complete = false;                //have we received all the data from the Python Script
String sensorType = "";
String command = "";

String ECsensorstring = "";                           //a string to hold the data from the EC sensor
boolean ECsensor_string_complete = false;             //have we received all the data from the EC sensor
float EC;

String pHsensorstring = "";                           //a string to hold the data from the pH sensor
boolean pHsensor_string_complete = false;             //have we received all the data from the pH sensor
float pH;                                             //used to hold a floating point number that is the pH


void setup() {
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial2.begin(9600);                                //set baud rate for software serial port_2 (pins 16 and 17) to 9600
  Serial3.begin(9600);                                //set baud rate for software serial port_3 to 9600
  
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  ECsensorstring.reserve(30);                         //set aside some bytes for receiving data from EC sensor
  pHsensorstring.reserve(30);                         //set aside some bytes for receiving data from pH sensor
}

//Function for reading input from serial (Python Script)
void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the Python Script
}

//Function for reading input from serial2 (EC Sensor)
void serialEvent2() {                                 //if the hardware serial port_2 receives a char
  ECsensorstring = Serial2.readStringUntil(13);         //read the string until we see a <CR>
  ECsensor_string_complete = true;                    //set the flag used to tell if we have received a completed string from the EC sensor
}

//Function for reading input from serial3 (pH Sensor)
void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  pHsensorstring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  pHsensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the pH sensor
}

void loop() {
  //To handle commands to two different sensors, format must begin with EC: or PH: indicating EC or PH sensors respectively
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    sensorType = parseSensorTypeInput(inputstring);
    command = parseCommandInput(inputstring);
    if(sensorType.compareTo("EC:") == 0) {              //will see if input string begins with "EC:"
      //Serial.print("Sending command to EC: ");
      //Serial.println(command);
      Serial2.print(command);                           //send that string to the EC sensor
      Serial2.print('\r');                              //add a <CR> to the end of the string
    }
    else if(sensorType.compareTo("PH:") == 0){          //will see if input string begins with "PH:"
      //Serial.print("Sending command to PH: ");
      //Serial.println(command);
      Serial3.print(command);                       //send that string to the pH sensor
      Serial3.print('\r');                              //add a <CR> to the end of the string
    }
    else {
      Serial.println("incorrect format for command");
    }
    inputstring = "";                                 //clear the string
    sensorType = "";
    command = "";
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the Python Script 
  }

  //What to do if input comes from the EC sensor
  if (ECsensor_string_complete == true) {             //if a string from the EC sensor has been received in its entirety
    if (isdigit(ECsensorstring[0]) == false) {          //if the first character in the string is not a digit
      Serial.print("EC: ");
      Serial.println(ECsensorstring);                   //send that string to the PC's serial monitor
    }
    else {                                             //if the first character in the string is a digit
      print_EC_data();                                 //then call this function
    }
    ECsensorstring = "";                               //clear the string
    ECsensor_string_complete = false;                  //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

  //What to do if input comes from the pH sensor
  if (pHsensor_string_complete == true) {               //if a string from the pH sensor has been received in its entirety
    Serial.print("PH: ");
    Serial.println(pHsensorstring);                     //send that string to the PC's serial monitor
    
    pHsensorstring = "";
    pHsensor_string_complete = false;
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
void print_EC_data(void) {
  char ECsensorstring_array[30];                      //we make a char array
  char *EC;                                           //char pointer used in string parsing
  
  ECsensorstring.toCharArray(ECsensorstring_array, 30);   //convert the string to a char array
  EC = strtok(ECsensorstring_array, ",");                 //let's pars the array at each comma

  Serial.print("EC:");                                //we now print each value we parsed separately
  Serial.println(EC);                                 //this is the EC value
}
