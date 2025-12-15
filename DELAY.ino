#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 10
#define ECHO_PIN 11
#define SERVO_PIN 12

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int angle = 0;
int direction = 1;
int distance = -1;
int lastAngle = -1;

unsigned long lastDistanceTime = 0;
const unsigned long distanceInterval = 1500;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  myServo.attach(SERVO_PIN);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Radar Ready");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle:");
  lcd.setCursor(0, 1);
  lcd.print("Dist :");
}

void loop() {
  if (millis() - lastDistanceTime >= distanceInterval) {
    distance = getDistance();
    lastDistanceTime = millis();
  }

  angle += direction * 5;

  if (angle >= 180) {
    angle = 180;
    direction = -1;
  } else if (angle <= 0) {
    angle = 0;
    direction = 1;
  }

  myServo.write(angle);

  if (angle != lastAngle) {
    lcd.setCursor(6, 0);
    lcd.print("    ");
    lcd.setCursor(6, 0);
    lcd.print(angle);
    lastAngle = angle;
  }

  lcd.setCursor(6, 1);
  lcd.print("             ");
  lcd.setCursor(6, 1);

  if (distance <= 0 || distance > 500) {
    lcd.print("000 cm");
  } else {
    if (distance < 10) lcd.print("00");
    else if (distance < 100) lcd.print("0");
    lcd.print(distance);
    lcd.print(" cm");
  }

  Serial.print(angle);
  Serial.print(",");
  Serial.println(distance);

  delay(200);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 35000);

  if (duration == 0) return -1;

  int d = duration * 0.034 / 2;

  if (d <= 0 || d > 500) return 0;

  return d;
}
