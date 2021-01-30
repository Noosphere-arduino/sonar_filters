#define TRIG_PIN 9
#define ECHO_PIN 10

float dist_3[3] = {0.0, 0.0, 0.0}; // массив для хранения трех последних измерений
byte i;

float dist_prev, dist_filtered, k;
unsigned long sensTimer;

void setup() {
  
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  k = 0.5;

}

void loop() {

  if (millis() - sensTimer > 50) {    // измерение и вывод каждые 50 мс
  
  //объявлеям переменные для хранения длительности импульса и расстояния для объекта
  int duration;
  float distance;
  float distance_mean;
  float distance_median;

  // счетчик от 0 до 2
  if (i > 1){
    i = 0;
  }
  else i++;
  
  //выставляем значение LOW на пине TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);

  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // измерим длительность высокого сигнала на пине ECHO_PIN
  duration = pulseIn(ECHO_PIN, HIGH);

  distance = duration / 58.0;

  if (distance > 400) distance = 400;

  dist_3[i] = distance;

  if (millis() < 200) {
    distance_mean = distance;
    distance_median = distance;
  }
  else {
    distance_mean = mean_of_3(dist_3[0], dist_3[1], dist_3[2]);
    distance_median = middle_of_3(dist_3[0], dist_3[1], dist_3[2]); 
  }

  // фильтр Брауна
  dist_filtered = distance_median * k + dist_prev * (1-k);

  //Serial.print("distance = ");
  Serial.print(distance);
  Serial.print(" ");
  //Serial.print(" distance_mean = ");
  //Serial.print(distance_mean);
  //Serial.print(" distance_median = ");
  //Serial.print(distance_median);
  //Serial.print(" dist_filtered = ");
  Serial.println(dist_filtered);

  dist_prev = dist_filtered;
  
  // обновляем значение переменной sensTimer
  sensTimer = millis(); 
  
  }  

}

// эта функция вычислеяет среднее арифметическое из трех значений
float mean_of_3(float a, float b, float c){
  float mean = (a + b + c) / 3.0;
  return mean;
}

// медианный фильтр из трех значений
float middle_of_3(float a, float b, float c){
  float middle;
  if ((a <= b) && (a <= c)) {
    //middle = (b <= c) ? b : c;
    if ( b <= c ) middle = b; 
    else middle = c;
  }
  else {
    if (( b <= a) && (b <= c)){
      middle = (a <= c) ? a : c;
    }
    else {
      middle = (a <= b) ? a : b;
    }
  }
  return middle;
}
