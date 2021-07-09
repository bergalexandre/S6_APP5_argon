/*
 * Project app5
 * Description:
 * Author:
 * Date:
 */

int compteur = 0;
int sortie_A0 = 0;

void callback_lecture_A0(const char *event, const char *data)
{
  analogWrite(A0, atoi(data));
  Serial.printlnf("Event: %s data=%i", event, atoi(data));
}

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Start");
  WiFi.on();
  // C'est marqué de les mettres au début pour le mode automatic
  Particle.variable("compteur", compteur);
  // Particle.variable("output/A0", sortie_A0);
  Particle.subscribe("output/AO", callback_lecture_A0);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // Connection au cloud de particle (C'est sensé se faire tout seul)
  //On est 3!!

  Serial.println("Publish le compteur");

  bool success = Particle.publish("compteur", NULL, WITH_ACK);
  if (!success) {
    Serial.println("You Failed");
  }
  compteur++;
  delay(1000);
  
}