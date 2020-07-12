  float t1 = 0, t2 = 0;
  int Uout = 0,Uoutt = 0, Uinn = 0;

void setup() {
  pinMode(3, INPUT_PULLUP);
  Serial.begin(9600);
}
  void loop() {
    Uoutt = 0;
    for(int i = 0; i<50; i++){
      t1 = analogRead(A0);
      delay(50); 
      t2 = analogRead(A1);
      delay(50); 
      Uoutt = Uoutt+((t1+t2) / 2);
    }
    Uout = Uoutt/50;
    Serial.print(1024);
    Serial.print("   ");
    Serial.println(Uout);
    delay(5000);
  }
