// https://github.com/TheBanditOfRed
// https://github.com/Rafaela-Fernandes-Groberio

/ Define pins
const int LED = 4;
const int BUZZER = 9;
const int BUTTON = 12;
const int SWITCH = 13;

// Define input/output pins
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(SWITCH, INPUT);
}

void loop()
{   
  int button_value = digitalRead(BUTTON);				//Check button value
  int switch_value = digitalRead(SWITCH);				//Check switch value
  
  if (switch_value == LOW){					//Check if switch is on
    if (button_value == HIGH){					//Check if button is pressed
      for (int buzzer_loop = 0; buzzer_loop <= 3; buzzer_loop++){	//Loop 4x
        tone(BUZZER, 3000);						//Play 3kHz on buzzer
        delay(1000);							//Wait 1 sec
        tone(BUZZER, 6000);						//Play 6kHz on buzzer
        delay(1000);							//Wait 1 sec
      }
      noTone(BUZZER);						//Turn buzzer off
    } else if (button_value == LOW){					//Check if button is not pressed
      for (int led_loop = 0; led_loop <= 3; led_loop++){			//Loop 4x
        digitalWrite(LED, HIGH);					//Turn LED on
        delay(500);							//Wait 0.5sec
        digitalWrite(LED, LOW);					//Turn LED off
        delay(500);							//Wait 0.5sec
      }
      delay(2000);							//Wait 2sec
    }
  }
}

//