// ain_sample_on_timer.ino
//
// Reads from analog inputs at fixed frequency using a hardware timer.
//


// Analog input pins
const uint8_t NUM_AIN = 14; 
const uint8_t AIN_PIN[NUM_AIN] = {36,37,38,39,32,33,34,35,4,0,2,15,13,12};

// Hardware timer
hw_timer_t *timer = nullptr;
portMUX_TYPE timer_mux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR on_timer_callback();

// Toggle pin - to check timing on an oscilloscope
const uint8_t toggle_pin = 18;
bool toggle_state = false;
volatile bool on_timer_flag = false;


void setup()
{
    Serial.begin(115200);

    // Start timer. The the base frequensy is 80MHz so with of prescalar 80 so we get 1Mhz 
    // clock ticks.
    timer = timerBegin(1, 80, true);      
    timerAttachInterrupt(timer, &on_timer_callback, true);

    // Set timer count at which on_timer_callback will be called. A count of 1000 gives 1kHz.
    timerAlarmWrite(timer, 1000, true);  
    timerAlarmEnable(timer);

    pinMode(toggle_pin, OUTPUT);
}

void loop()
{
    static int cnt = 0;

    // Read copy of timer flag 
    portENTER_CRITICAL(&timer_mux);
    bool on_timer_flag_copy = on_timer_flag;
    on_timer_flag = false;
    portEXIT_CRITICAL(&timer_mux);

    // If timer flag is true read analog inputs 
    uint16_t values[NUM_AIN];
    if (on_timer_flag_copy)
    {
        for (uint8_t i=0; i<NUM_AIN; i++)
        { 
            values[i] = analogRead(AIN_PIN[i]);
        }

        // Toggle pin to check timing  - pulse frequency should 1/2 of sample frequency
        toggle_state = !toggle_state;
        digitalWrite(toggle_pin,toggle_state);
        Serial.println(toggle_state);
    }

}

void IRAM_ATTR on_timer_callback()
{
    // Set timer flag to indicate that it is time to read analog inputs.
    portENTER_CRITICAL_ISR(&timer_mux);
    on_timer_flag = true;
    portEXIT_CRITICAL_ISR(&timer_mux);
}
