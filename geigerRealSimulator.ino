#define _DEBUG

#define CLIK 8
#define ECHO 2
#define TRIG 3

#define TAU 2.2414 // the decay time in minutes
#define c 340.e-6  // the speed of sound

float tau = TAU*60.;
unsigned long t0;

void setup() {
  pinMode(CLIK, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW);
  digitalWrite(CLIK, LOW);
  t0 = millis();
#ifdef _DEBUG
  Serial.begin(9600);
  Serial.print("============ tau = ");
  Serial.print(tau);
  Serial.println(" s");
#endif
}

void trigger() {
  /* trigger the sensor */
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
}

float measure() {
  /* measure the distance between the sensor and the obstacle */
  float d = 0.;
  for (int i = 0; i < 15; i++) {
    trigger();
    unsigned long T = pulseIn(ECHO, HIGH);
    d += c*T/2.;
  }
  return d;
}

int status = HIGH; // the current status of the relay

void loop() {
  /* measure distance and time */
  float d = measure();
  float t = (millis() - t0)*1.e-3;
  /* compute the probability of a decay */
  float Pdecay = exp(-t/tau);
  float f = (float)random(1000)/1000.;
  /* if an atom decay... */
  if (f < Pdecay) {
    /* ...detect it with a probability that depens on d */
    unsigned long trigger = 10000./(d*d); 
    unsigned long r = random(10000);
    if (r < trigger) {
      digitalWrite(CLIK, status);
#ifdef _DEBUG
      Serial.println(t);
#endif
      if (status == HIGH) {
        status = LOW;
      } else {
        status = HIGH;
      }
    }
  }
}
