int b = 0;
int a = 0;
int data = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  long x = Serial.read();
    if(x == 90)
    {
      noTone(3);
      delay(b);
      tone(3, 1479);
      delay(a);
    }
  if(x == 89)
    {
      noTone(3);
      delay(b);
      tone(3, 1396);
      delay(a);
    }
  if(x == 88)
  {
      noTone(3);
      delay(b);
      tone(3, 1318);
      delay(a);
  }
  if(x == 87)
  {
      noTone(3);
      delay(b);
      tone(3, 1244);
      delay(a);
  }
    if(x == 86)
    {
      noTone(3);
      delay(b);
      tone(3, 1174);
      delay(a);
    }
  if(x == 85)
    {
      noTone(3);
      delay(b);
      tone(3, 1108);
      delay(a);
    }
  if(x == 84)
  {
      noTone(3);
      delay(b);
      tone(3, 1046);
      delay(a);
  }
  if(x == 83)
  {
      noTone(3);
      delay(b);
      tone(3, 987);
      delay(a);
  }
    if(x == 82)
    {
      noTone(3);
      delay(b);
      tone(3, 932);
      delay(a);
    }
  if(x == 81)
    { 
      noTone(3);
      delay(b);
      tone(3, 880);
      delay(a);
    }
  if(x == 80)
  { 
      noTone(3);
      delay(b);
      tone(3, 830);
      delay(a);
  }
  if(x == 79)
  {
      noTone(3);
      delay(b);
      tone(3, 783);
      delay(a);
  }
    if(x == 78)
    {
      noTone(3);
      delay(b);
      tone(3, 739);
      delay(a);
    }
  if(x == 77)
    {
      noTone(3);
      delay(b);
      tone(3, 698);
      delay(a);
    }
  if(x == 76)
  {
      noTone(3);
      delay(b);
      tone(3, 659);
      delay(a);
  }
  if(x == 75)
  { 
      noTone(3);
      delay(b);
      tone(3, 622);
      delay(a);
  }
  if(x == 74)
    {
      noTone(3);
      delay(b);
      tone(3, 587);
      delay(a);
    }
  if(x == 73)
    { 
      noTone(3);
      delay(b);
      tone(3, 554);
      delay(a);
    }
  if(x == 72)
  {
      noTone(3);
      delay(b);
      tone(3, 523);
      delay(a);
  }
  if(x == 71)
  {
      noTone(3);
      delay(b);
      tone(3, 493);
      delay(a);
  }
    if(x == 70)
    {
      noTone(3);
      delay(b);
      tone(3, 466);
      delay(a);
    }
  if(x == 69)
    {
      noTone(3);
      delay(b);
      tone(3, 440);
      delay(a);
    }
  if(x == 68)
  { 
      noTone(3);
      delay(b);
      tone(3, 415);
      delay(a);
  }
  if(x == 67)
  {
      noTone(3);
      delay(b);
      tone(3, 392);
      delay(a);
  }
    if(x == 66)
    {
      noTone(3);
      delay(b);
      tone(3, 369);
      delay(a);
    }
  if(x == 65)
    {
      noTone(3);
      delay(b);
      tone(3, 349);
      delay(a);
    }
  if(x == 64)
  {
      noTone(3);
      delay(b);
      tone(3, 329);
      delay(a);
  }
  if(x == 63)
  { 
      noTone(3);
      delay(b);
      tone(3, 311);
      delay(a);
  }
    if(x == 62)
    {
      noTone(3);
      delay(b);
      tone(3, 293);
      delay(a);
    }
  if(x == 61)
    {
      noTone(3);
      delay(b);
      tone(3, 277);
      delay(a);
    }
  if(x == 60)
  {
      noTone(3);
      delay(b);
      tone(3, 261);
      delay(a);
  }
  if(x == 59)
  {
      noTone(3);
      delay(b);
      tone(3, 246);
      delay(a);
  }
    if(x == 58)
    {
      noTone(3);
      delay(b);
      tone(3, 233);
      delay(a);
    }
  if(x == 57)
    {
      noTone(3);
      delay(b);
      tone(3, 220);
      delay(a);
    }
  if(x == 56)
  { 
      noTone(3);
      delay(b);
      tone(3, 207);
      delay(a);
  }
  if(x == 55)
  {
      noTone(3);
      delay(b);
      tone(3, 196);
      delay(a);
  }
    if(x == 54)
    {
      noTone(3);
      delay(b);
      tone(3, 185);
      delay(a);
    }
  if(x == 53)
    {
      noTone(3);
      delay(b);
      tone(3, 174);
      delay(a);
    }
  if(x == 52)
  {
      noTone(3);
      delay(b);
      tone(3, 164);
      delay(a);
  }
  if(x == 51)
  {
      noTone(3);
      delay(b);
      tone(3, 155);
      delay(a);
  }
    if(x == 50)
    {
      noTone(3);
      delay(b);
      tone(3, 146);
      delay(a);
    }
  if(x == 49)
    {
      noTone(3);
      delay(b);
      tone(3, 138);
      delay(a);
    }
  if(x == 48)
  {
      noTone(3);
      delay(b);
      tone(3, 130);
      delay(a);
  }
  if(x == 47)
  {
      noTone(3);
      delay(b);
      tone(3, 123);
      delay(a);
  }
    if(x == 46)
    {
      noTone(3);
      delay(b);
      tone(3, 116);
      delay(a);
    }
  if(x == 45)
    {
      noTone(3);
      delay(b);
      tone(3, 110);
      delay(a);
    }
  if(x == 44)
  { 
      noTone(3);
      delay(b);
      tone(3, 103);
      delay(a);
  }
  if(x == 43)
  {
      noTone(3);
      delay(b);
      tone(3, 98);
      delay(a);
  }
    if(x == 42)
  { 
      noTone(3);
      delay(b);
      tone(3, 96);
      delay(a);
  }
  if(x == 41)
  {
      noTone(3);
      delay(b);
      tone(3, 87);
      delay(a);
  }
    if(x == 40)
    {
      noTone(3);
      delay(b);
      tone(3, 82);
      delay(a);
    }
  if(x == 39)
    {
      noTone(3);
      delay(b);
      tone(3, 78);
      delay(a);
    }
  if(x == 38)
  {
      noTone(3);
      delay(b);
      tone(3, 73);
      delay(a);
  }
  if(x == 37)
  {
      noTone(3);
      delay(b);
      tone(3, 69);
      delay(a);
  }
    if(x == 36)
    {
      noTone(3);
      delay(b);
      tone(3, 65);
      delay(a);
    }
  if(x == 35)
    {
      noTone(3);
      delay(b);
      tone(3, 62);
      delay(a);
    }
  if(x == 34)
  {
      noTone(3);
      delay(b);
      tone(3, 58);
      delay(a);
  }
  if(x == 33)
  {
      noTone(3);
      delay(b);
      tone(3, 55);
      delay(a);
  }
    if(x == 32)
    {
      noTone(3);
      delay(b);
      tone(3, 52);
      delay(a);
    }
  if(x == 31)
    {
      noTone(3);
      delay(b);
      tone(3, 49);
      delay(a);
    }
  if(x == 30)
  { 
      noTone(3);
      delay(b);
      tone(3, 46);
      delay(a);
  }
  if(x == 29)
  {
      noTone(3);
      delay(b);
      tone(3, 44);
      delay(a);
  }
}
