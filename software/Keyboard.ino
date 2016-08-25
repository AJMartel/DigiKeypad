#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "DigiKeyboard.h"

const PROGMEM char numpad_layout[] =  {
 0x2e, 0x2d, 0x2d, 0x2d, 0x2e, 0x2d, 0x2d, 0x2d, 0x2e, 0x2d, 0x2d, 0x2d, 0x2e, 0x2d, 0x2d, 0x2d, 0x2e, 0x0d, 0x0a,
 0x7c, 0x20, 0x46, 0x20, 0x7c, 0x20, 0x2f, 0x20, 0x7c, 0x20, 0x2a, 0x20, 0x7c, 0x20, 0x3c, 0x20, 0x7c, 0x0d, 0x0a,
 0x3a, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x3a, 0x0d, 0x0a,
 0x7c, 0x20, 0x37, 0x20, 0x7c, 0x20, 0x38, 0x20, 0x7c, 0x20, 0x39, 0x20, 0x7c, 0x20, 0x2d, 0x20, 0x7c, 0x0d, 0x0a,
 0x3a, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x3a, 0x0d, 0x0a,
 0x7c, 0x20, 0x34, 0x20, 0x7c, 0x20, 0x35, 0x20, 0x7c, 0x20, 0x36, 0x20, 0x7c, 0x20, 0x2b, 0x20, 0x7c, 0x0d, 0x0a,
 0x3a, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x3a, 0x0d, 0x0a,
 0x7c, 0x20, 0x31, 0x20, 0x7c, 0x20, 0x32, 0x20, 0x7c, 0x20, 0x33, 0x20, 0x7c, 0x20, 0x45, 0x20, 0x7c, 0x0d, 0x0a,
 0x3a, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x2d, 0x2d, 0x2d, 0x2b, 0x20, 0x6e, 0x20, 0x3a, 0x0d, 0x0a,
 0x7c, 0x20, 0x20, 0x20, 0x30, 0x20, 0x20, 0x20, 0x7c, 0x20, 0x2e, 0x20, 0x7c, 0x20, 0x74, 0x20, 0x7c, 0x0d, 0x0a,
 0x27, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x27, 0x2d, 0x2d, 0x2d, 0x27, 0x2d, 0x2d, 0x2d, 0x27, 0x0d, 0x0a, 0x00 
 };
const char key_0[] 	PROGMEM = "[Num Lock]";
const char key_1[] 	PROGMEM = "[/]";
const char key_2[] 	PROGMEM = "[*]";
const char key_3[] 	PROGMEM = "[Backspace]";
const char key_4[]	PROGMEM = "[7]";
const char key_5[]	PROGMEM = "[8]";
const char key_6[] 	PROGMEM = "[9]";
const char key_7[] 	PROGMEM = "[-]";
const char key_8[]	PROGMEM = "[4]";
const char key_9[] 	PROGMEM = "[5]";
const char key_10[] PROGMEM = "[6]";
const char key_11[] PROGMEM = "[+]";
const char key_12[] PROGMEM = "[1]";
const char key_13[] PROGMEM = "[2]";
const char key_14[] PROGMEM = "[3]";
const char key_15[] PROGMEM = "[0]";
const char key_16[] PROGMEM = "[.]";
const char key_17[] PROGMEM = "[Enter]";

const char* const numpad_keyname[] PROGMEM = {
	key_0, key_1, key_2, key_3,
	key_4, key_5, key_6, key_7,
	key_8, key_9, key_10, key_11,
	key_12, key_13, key_14,
	key_15, 	key_16, 	key_17
	};
	
const PROGMEM byte numpad_keymap[18] = {
	0x53, 0x54, 0x55, 0x2A,
	0x5F, 0x60, 0x61, 0x56,
	0x5C, 0x5D, 0x5E, 0x57,
	0x59, 0x5A, 0x5B,
	0x62,	0x63,	0x58
	};
	
void print_highlowVal(uint16_t low_Val, uint16_t highVal){
	DigiKeyboard.print(low_Val);
	DigiKeyboard.print(" ... ");
	DigiKeyboard.println(highVal);
}

uint16_t AnalogKey = 0;

void key_calibrate(){
	byte i, capture_times, capture_timeout;
	uint16_t addr, key_aVal_Range[18][2];
	char buffer[200];
	
	strcpy_P(buffer, numpad_layout);
	DigiKeyboard.println(buffer);
	
	for( i = 0; i < 18; i++){
		DigiKeyboard.print(F("\nPlease press "));
		strcpy_P(buffer, (char*)pgm_read_word(&(numpad_keyname[i])));
		DigiKeyboard.print(buffer);
		DigiKeyboard.println(F(" for me to calibrate the analog value..."));
		DigiKeyboard.delay(500);
		
		uint16_t highVal = 0;
		uint16_t low_Val = 0;
		capture_times = 0;
		capture_timeout = 0;

		DigiKeyboard.println(F("LOW vs HIGH"));
		
		while(capture_times < 20){
			AnalogKey = analogRead(1) >> 1;
			if(AnalogKey > 100){
				if(!(highVal & low_Val)){
					highVal = AnalogKey;
					low_Val = AnalogKey;
				}
				else {
					if(AnalogKey > highVal){
						highVal = AnalogKey;
						capture_times++;
						capture_timeout = 0;
						print_highlowVal(low_Val, highVal);
					}
					else if(AnalogKey < low_Val){
						low_Val = AnalogKey;
						capture_times++;
						capture_timeout = 0;
						print_highlowVal(low_Val, highVal);
					}
				}
			}
			else if(AnalogKey == 0){
				if(capture_timeout++ > 10) break;			//stay calm for 5 second to confirm
			}
			DigiKeyboard.delay(200);
		}
		
		key_aVal_Range[i][0] = low_Val;
		key_aVal_Range[i][1] = highVal;
		
		DigiKeyboard.print(F("\nThe final range is: "));
		print_highlowVal(low_Val, highVal);
	}
	
	DigiKeyboard.println(F("\nPress [+] to save all the calibration data..."));
	DigiKeyboard.println(F("Unplug to abort"));
	while((analogRead(1) >> 1) != 511);
	
	DigiKeyboard.print(F("clear EEPROM..."));
	for (addr = 0 ; addr < EEPROM.length() ; addr++){
		if(!(addr%51)){
			int percentage = (addr*100)/(EEPROM.length());
			DigiKeyboard.print(percentage);
			DigiKeyboard.println(F("%"));
		}
		EEPROM.write(addr, 0);
	}
	DigiKeyboard.println(F("done!"));
	
	DigiKeyboard.print(F("writing to EEPROM..."));
	for(i = 0; i < 18; i++){
		byte keycode = pgm_read_byte_near(numpad_keymap + i);
		while(key_aVal_Range[i][0] <= key_aVal_Range[i][1]){
			EEPROM.write(key_aVal_Range[i][0], keycode);
			key_aVal_Range[i][0]++;
		}
	}
	DigiKeyboard.println(F("done!"));
}
	
void setup() {
	// don't need to set anything up to use DigiKeyboard 
	pinMode(1, OUTPUT);
  
	DigiKeyboard.sendKeyStroke(0);
	// this is generally not necessary but with some older systems it seems to
	// prevent missing the first character after a delay:
	DigiKeyboard.delay(500);
}

short int calibration_trigger = 0;
void loop() {
	
	// Type out this string letter by letter on the computer (assumes US-style
	// keyboard)
	//DigiKeyboard.println("Hello Digispark!");
	AnalogKey = analogRead(1) >> 1;
	
	if(AnalogKey == 322)calibration_trigger++;
	else if (!AnalogKey)calibration_trigger = 0;
	if (calibration_trigger > 10)key_calibrate();
	
	DigiKeyboard.sendKeyStroke(EEPROM.read(AnalogKey));

	// It's better to use DigiKeyboard.delay() over the regular Arduino delay()
	// if doing keyboard stuff because it keeps talking to the computer to make
	// sure the computer knows the keyboard is alive and connected
  
  DigiKeyboard.delay(100);
}
#define NUM_LOCK 1
#define CAPS_LOCK 2
#define SCROLL_LOCK 4
volatile static uchar LED_state = 0xff; // received from PC
usbMsgLen_t usbFunctionWrite(uint8_t * data, uchar len) {
    if (data[0] == LED_state)
        return 1;
    else
        LED_state = data[0];
        
    // LED state changed
    if(LED_state & CAPS_LOCK)
        digitalWrite(1, HIGH);  // LED on
    else
        digitalWrite(1, LOW);  // LED off
        
    return 1; // Data read, not expecting more
}
