
long lastPWMChanged = 0;
bool PWMHigh = true;
int PWMPin = 11;

int dutyTime = 1000;
int dutyPeriod = 300;
int dutyMin = 10;
int dutyMax = 900;

int rpmSensorPin = 2;
int targetRPM = 600;
long lastRPMTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(rpmSensorPin, INPUT);
  pinMode(PWMPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(rpmSensorPin), revolutionIsMade, CHANGE);
}

void loop() {
  long currentTime = millis();
  
  if((PWMHigh && currentTime - lastPWMChanged > dutyPeriod) || (!PWMHigh && currentTime - lastPWMChanged > dutyTime - dutyPeriod)){
    lastPWMChanged = millis();
    PWMHigh = !PWMHigh;
  }

  digitalWrite(PWMPin, PWMHigh);
}

void revolutionIsMade() {
    if(lastRPMTime == 0) {
    lastRPMTime = millis();
    return;
  }

  float currentRPM = 60.0 / ((millis() - lastRPMTime) / 1000.0);
  float scaleFactor = targetRPM / currentRPM; 

  dutyPeriod = dutyPeriod * scaleFactor;
  dutyPeriod = min(dutyPeriod, dutyMax);
  dutyPeriod = max(dutyPeriod, dutyMin);

  lastRPMTime = millis();
}
