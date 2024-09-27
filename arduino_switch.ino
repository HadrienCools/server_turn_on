const int yellowPin = 2;
const int bluePin = 3;
const int redPin = 6;
const int greenPin = 9;
const int buttonPin = 12;  // Pin du bouton poussoir

String comdata;           // Variable pour stocker la commande reçue

// finite state machine 
// etats - > eteint -> en train d'allumer -> allumé 
String etat = "STOPPED";  // État initial du serveur
String etatSanitaire = "SAIN"; // État sanitaire
String changeState = "TRUE";

unsigned long buttonPressStartTime = 0; // Temps de début d'appui sur le bouton
bool buttonPressed = false; // État du bouton

void setup() {

  pinMode(yellowPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Activer la résistance de pull-up interne

  // S'assurer que toutes les LEDs sont éteintes au départ
  digitalWrite(yellowPin, LOW);
  digitalWrite(bluePin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);

  Serial.begin(115200);
  Serial.println("Enter Value");
  Serial.print("Input:");
  delay(100);

}

void loop() {

    if (Serial.available() > 0) {
      comdata  = Serial.readStringUntil(0x0A);
      delay(10);
      comdata.toUpperCase();
      Serial.println(comdata);
    }

    if (comdata == "TURN ON BLUE LED") {
      Serial.println("bluePin LED is ON");
      digitalWrite(bluePin, HIGH);
      comdata = "";
    }
    if (comdata == "TURN OFF BLUE LED") {
      Serial.println("bluePin LED is OFF");
      digitalWrite (bluePin, LOW);
      comdata = "";
    }
    if (comdata == "TURN ON RED LED") {
      Serial.println("redPin LED is ON");
      digitalWrite(redPin, HIGH);
      comdata = "";
    }
    if (comdata == "TURN OFF RED LED") {
      Serial.println("redPin LED is OFF");
      digitalWrite (redPin, LOW);
      comdata = "";
    }
    if (comdata == "TURN ON GREEN LED") {
      Serial.println("greenPin LED is ON");
      digitalWrite(greenPin, HIGH);
      comdata = "";
    }
    if (comdata == "TURN OFF GREEN LED") {
      Serial.println("greenPin LED is OFF");
      digitalWrite (greenPin, LOW);
      comdata = "";
    }

    if (comdata == "GET STATE") {
      Serial.println("État: " + etat);
      comdata = "";
    }

    if (etat == "STOPPED" && changeState == "TRUE"){
      Serial.println("redPin LED is ON");
      digitalWrite(redPin, HIGH);
      digitalWrite (greenPin, LOW);
      changeState = "FALSE";
    }

   if (etat == "STARTED" && changeState == "TRUE" ){
      Serial.println("redPin LED is ON");
      digitalWrite(redPin, LOW );
      digitalWrite (greenPin,HIGH );
      changeState = "FALSE";
    }
    //
    // Vérifier si le bouton est pressé
  if (digitalRead(buttonPin) == LOW) {
    if (!buttonPressed) {
      buttonPressed = true;
      buttonPressStartTime = millis(); // Enregistrer le temps de début d'appui
    }
  } else {
    if (buttonPressed) {
      buttonPressed = false; // Réinitialiser l'état du bouton
      unsigned long pressDuration = millis() - buttonPressStartTime; // Durée d'appui
      
      // Vérifier si l'état est STARTED et si l'appui dure plus de 3000 ms
      if (etat == "STARTED" && pressDuration > 3000) {
        etat = "STOPPED";
        Serial.println("État: " + etat); // Afficher l'état courant
      }
      
      // Vérifier si l'état est STOPPED et si l'appui dure plus de 500 ms
      else if (etat == "STOPPED" && pressDuration > 500) {
        etat = "STARTED";
        Serial.println("État: " + etat); // Afficher l'état courant
      }
    }
  }


    // Vérifier si la commande est "ALLUMER SERVEUR"
    if (comdata == "ALLUMER SERVEUR") {
      // Passer l'état à STARTING
      etat = "STARTING";
      Serial.println("État: " + etat); // Afficher l'état courant
      // Allumer la LED pendant 1 seconde
      digitalWrite(yellowPin, HIGH);
      delay(1000); // Attendre 1 seconde
      digitalWrite(yellowPin, LOW);
      
      // Envoyer une confirmation au port série
      Serial.println("LED allumée pendant 1 seconde");
      comdata = "";
      etat = "STARTED";
      changeState = "TRUE";
      Serial.println("État: " + etat); // Afficher l'état courant
    }
    if (comdata == "ETEINDRE SERVEUR") {
      // Passer l'état à STOPPING
      etat = "STOPPING";
      Serial.println("État: " + etat); // Afficher l'état courant
      // Allumer la LED pendant 3 secondes
      digitalWrite(yellowPin, HIGH);
      delay(5000); // Attendre 3 secondes
      digitalWrite(yellowPin, LOW);
      
      // Envoyer une confirmation au port série
      Serial.println("LED allumée pendant 3 secondes");

      etat = "STOPPED";
      changeState = "TRUE";
      Serial.println("État: " + etat); // Afficher l'état courant
      comdata = "";
    }
    if (comdata == "DETECTER ERREUR") {
      // Passer l'état sanitaire à DEGRADÉ
      etatSanitaire = "DEGRADÉ";
      Serial.println("État Sanitaire: " + etatSanitaire); // Afficher l'état sanitaire
      
      // Allumer la LED pour signaler l'erreur
      digitalWrite(bluePin, HIGH);
      //delay(10000);
      //digitalWrite(bluePin, LOW); // Éteindre la LED après 1 seconde
      comdata = "";
    }

}