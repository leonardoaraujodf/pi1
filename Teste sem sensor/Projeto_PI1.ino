unsigned char id[11] = {1,2,3,4,5};
unsigned int i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(i == 5){
    i = 0;
  }
  Serial.write(id[i]);
  Serial1.write(id[i]);
  i ++;
  delay(3000);
}
