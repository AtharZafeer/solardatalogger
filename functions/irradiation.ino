
unsigned long int utc = 1566559089;
char name[15];
void names(unsigned long int);
void setup() {

  //snprintf(name, sizeof(name), "%lu.csv",utc++);
  Serial.begin(9600);

}
void loop() {
  delay(1000);
  names(utc);
  utc++;
}

void names(unsigned long int utc) {
  snprintf(name, sizeof(name), "%lu.csv",utc++);
  Serial.printf("%s\n",name);
}
