// حساس المسافة
const int trigPin = 9;
const int echoPin = 8;

// الليدات الأساسية والبازر
const int greenLed = 11; // ليد أخضر تدريجي (PWM)
const int redLed = 7; // ليد أحمر عند 10 سم أو أقل
const int buzzer = 6; // بازر عند 5 سم أو أقل

// ليدات الـ Bar Graph (5 ليدات من الأبعد للأقرب)
const int barPins[] = {2, 3, 4, 5, 10};
const int barCount = 5;

void setup() {
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

pinMode(greenLed, OUTPUT);
pinMode(redLed, OUTPUT);
pinMode(buzzer, OUTPUT);

for (int i = 0; i < barCount; i++) {
pinMode(barPins[i], OUTPUT);
}

Serial.begin(9600);
}

void setBarGraph(int activeLeds) {
// activeLeds من 0 إلى 5: ينور أول activeLeds ويطفي الباقي
for (int i = 0; i < barCount; i++) {
if (i < activeLeds) {
digitalWrite(barPins[i], HIGH);
} else {
digitalWrite(barPins[i], LOW);
}
}
}

void loop() {
// 1. قياس المسافة
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

long duration = pulseIn(echoPin, HIGH);
int distance = duration * 0.034 / 2;

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");

if (distance <= 0) {
delay(50);
return;
}

// 2. التحكم في الـ Bar Graph والليدات والبازر
if (distance >= 50) {
// بعيد: Bar Graph كامل (5 ليدات) + الأخضر بأقصى إضاءة
setBarGraph(5);
analogWrite(greenLed, 255);
digitalWrite(redLed, LOW);
noTone(buzzer);
} 
else if (distance > 10 && distance < 70) {
// بين 50 و 10 سم: عدد ليدات الـ Bar Graph يقل تدريجياً + الأخضر يخفت
int ledsToLight = map(distance, 10, 50, 1, 5);
ledsToLight = constrain(ledsToLight, 1, 5);
setBarGraph(ledsToLight);

int brightness = map(distance, 10, 50, 0, 255);
brightness = constrain(brightness, 0, 255);
analogWrite(greenLed, brightness);

digitalWrite(redLed, LOW);
noTone(buzzer);
} 
else if (distance > 10 && distance <= 30) {
// عند 10 سم: يطفي Bar Graph والأخضر وينور الأحمر الرئيسي
setBarGraph(0);
analogWrite(greenLed, 0);
digitalWrite(redLed, HIGH);
noTone(buzzer);
} 
else {
// عند 5 سم أو أقل: الأحمر الرئيسي + تشغيل البازر
setBarGraph(0);
analogWrite(greenLed, 0);
digitalWrite(redLed, HIGH);
tone(buzzer, 1200);
}

delay(100);
}