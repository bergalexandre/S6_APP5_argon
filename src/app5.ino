/*
 * Project app5
 * Description:
 * Author:
 * Date:
 */

String UUID;
int compteur = 0;
int sortie_A0 = 0;

void scanBluethoot(const BleScanResult *scanResult, void *context)
{
  Serial.println("Scan");
  int taille = scanResult->advertisingData().length();
  Serial.printlnf("taille = %i", taille);

  if(taille == 27)
  {
    UUID = "";
    unsigned char* array = new unsigned char[taille+1];
    memset(array, 0, taille+1);
    scanResult->advertisingData().get(array, taille);

    // 0X02 et 0x15 == iBeacon Advertiser
    if(array[4] == 0x02 && array[5] == 0x15)
    {
      for(int i = 6; i<22; i++)
      {
        UUID += String(array[i], HEX);
        Serial.printf("%02x", array[i]);
      }
      bool success = Particle.publish("uuid", NULL, WITH_ACK);
      if (!success) {
        
        Serial.println("erreur de transmission du uuid");
      }
    }
  }
}

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

  Particle.variable("uuid", UUID);

  // Appel du callback quand un scan
  BLE.on();
  BLE.begin();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // Connection au cloud de particle (C'est sensé se faire tout seul)
  //On est 3!!

  BLE.scan(scanBluethoot, NULL);
  
  Serial.println("Publish le compteur");

  bool success = Particle.publish("compteur", NULL, WITH_ACK);
  if (!success) {
    Serial.println("You Failed");
  }
  compteur++;
  
}