/*Deze schets is getest en werkt zoals het hoort.
  + De deur gaat open/dicht en automatisch.
  + Het slot gaat niet als de deur open staat.
  + Als de deur aan het dicht gaan is kan die tijdens de beweging terug opengezet worden.
  + Er staat een vertraging tussen het ontvangen van het EW signaal en het uitsturen van het slot signaal (De 2 samen zou voor problemen kunnen zorgen?)
  + Meer vertraging tussen het openen van de deur en het aansturen van het slot. Daardoor heeft het slot de tijd om te hercalibreren moest het eventueel in storing zijn.
    Zonder de vertraging begint de deur al te trekken en kan de calibratie niet goed verlopen.
  - te doen; proberen ervoor zorgen dat het systeem pas reageert als de ew signalen weggevallen zijn. Dus als iemand zijn knop blijft indrukken gebeurd er niets,
      als men loslaat dan schiet de boel in gang.
  -
*/


int DoorControlOut = 4; 
int LockControlOpenOut = 5; 
int LockControlCloseOut = 6; 

int DoorInputAuto = 7;  //=EWMini 2
int DoorInputOC = 8;    //=EWMini 1
int DoorState = 9;      //Read if the door is open or closed

int DoorInputAutoRead;
int DoorInputOCRead;
int DoorStateRead;
int DoorControlOutRead;
int LockControlCloseOutRead;
int LockControlOpenOutRead;
int WhileClosing;


int AutoState=0;
int AutoStateNew;
int AutoStateOld = 1;
int OCState=0;
int OCStateNew;
int OCStateOld = 1;

int Delay1 = 2000;
int Delay2 = 10000;
int Delay3 = 1000;



void setup() {
  
  Serial.begin(9600);

  pinMode(DoorControlOut, OUTPUT);
  pinMode(LockControlOpenOut, OUTPUT);
  pinMode(LockControlCloseOut, OUTPUT);

  pinMode(DoorInputAuto, INPUT_PULLUP);
  pinMode(DoorInputOC, INPUT_PULLUP);
  pinMode(DoorState, INPUT_PULLUP);


}

void readInput()
{
  DoorInputAutoRead = digitalRead(DoorInputAuto);
  DoorInputOCRead = digitalRead(DoorInputOC);
  DoorStateRead = digitalRead(DoorState);
}

void printDebug()
{
  Serial.print(" DoorInputAutoRead= ");
  Serial.println(DoorInputAutoRead);
  Serial.print(" DoorInputOCRead= ");
  Serial.println(DoorInputOCRead);
  Serial.print(" DoorStateRead= ");
  Serial.println(DoorStateRead);
  Serial.print(" WhileClosing= ");
  Serial.println(WhileClosing);

  Serial.println();
}
void OpenLock()
{
  delay(Delay3);
  digitalWrite(LockControlOpenOut, HIGH);
  delay(Delay1);
  digitalWrite(LockControlOpenOut, LOW);
  delay(Delay2);
}

void CloseLock()
{
  digitalWrite(LockControlCloseOut, HIGH);
  delay(Delay1);
  digitalWrite(LockControlCloseOut, LOW);
  delay(Delay2);
}

void OpenDoor()
{
  digitalWrite(DoorControlOut, HIGH);
  delay(Delay1);
  WhileClosing = 0;

}

void CloseDoor()
{
  digitalWrite(DoorControlOut, LOW);
  delay(Delay1);
  WhileClosing = 1;
}

void loop()
{
  readInput();
  printDebug();

  delay(100);

  if (DoorInputAutoRead == 0 && DoorInputOCRead == 0 && DoorStateRead == 0)   //The Lock Closes
  {
    CloseLock();
  }
  if (DoorInputAutoRead == 1 && DoorInputOCRead == 0 && DoorStateRead == 0)   // The Door stays open
  {
    OpenLock();
    OpenDoor();
  }

  if (DoorInputAutoRead == 0 && DoorInputOCRead == 1 && DoorStateRead == 0)     //The Door Opens briefly
  {
    OpenLock();
    OpenDoor();
    CloseDoor();
  }
  if (DoorInputAutoRead == 0 && DoorInputOCRead == 1 && DoorStateRead == 1)
  {
    OpenDoor();
    CloseDoor();
  }
  if (DoorInputAutoRead == 1 && DoorInputOCRead == 0 && DoorStateRead == 1) // && WhileClosing == 1
  {
    if (WhileClosing == 0)
    {
      CloseDoor();
    }
    else if (WhileClosing == 1)
    {
      OpenDoor();
    }
  }

}
