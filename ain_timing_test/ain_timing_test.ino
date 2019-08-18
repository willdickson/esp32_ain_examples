// ain_timing_test.ino
//
// Reads from analog inputs and measure how long it takes
// 

// Analog input pins
const uint8_t NUM_AIN = 14; 
const uint8_t AIN_PIN[NUM_AIN] = {36,37,38,39,32,33,34,35,4,0,2,15,13,12};

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // Read analog inputs and time how long it takes
    uint32_t t0 = micros();  
    for (uint8_t i=0; i<NUM_AIN; i++)
    {
        uint16_t value = analogRead(AIN_PIN[i]);
    }
    uint32_t t1 = micros();
    uint32_t dt = t1 - t0;

    // Print time required to read samples
    Serial.print("dt(us):  ");
    Serial.println(dt);
    delay(100);
}
