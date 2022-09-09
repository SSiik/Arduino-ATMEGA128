#include <SimpleTimer.h> 
SimpleTimer timer; 
const int trigPin = 38;
const int echoPin = 37;
void setup() { 
   Serial.begin(115200);   Serial1.begin(115200); 
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   timer.setInterval(100, Uart1); 
   timer.setInterval(100, getDistance);
   pinMode(4, OUTPUT); pinMode(5, OUTPUT);
   pinMode(28, OUTPUT); pinMode(29, OUTPUT);
   pinMode(30, OUTPUT); pinMode(31, OUTPUT);

} 

int duration;
float distance;
unsigned char Uart0ProtocolPointer = 0; 
unsigned char Uart0ReciveCheckEnd = 0;
unsigned char Data[12] = {0,}; 
unsigned int X_center =0;
unsigned int Y_center =0;
unsigned int SigNum = 1;
unsigned int area = 0;
unsigned int x_loc = 0; 


void UartRxProtocol(){ 
   unsigned char Uart0_Data; 
   if(Serial.available()){ 
      Uart0_Data = Serial.read(); 

      switch(Uart0ProtocolPointer) {
         case 0: if(0xaa == Uart0_Data ) { Uart0ProtocolPointer = 1; } 
                          else { Uart0ProtocolPointer = 0; }  break; 
         case 1: if(0x55 == Uart0_Data ){ Uart0ProtocolPointer = 10;     } 
                          else {  Uart0ProtocolPointer = 0; }  break; 
        case 10: Data[0] = Uart0_Data; Uart0ProtocolPointer = 11; break;
        case 11: Data[1] = Uart0_Data; Uart0ProtocolPointer = 12; break;
        case 12: Data[2] = Uart0_Data;Uart0ProtocolPointer = 13; break;
        case 13: Data[3] = Uart0_Data; Uart0ProtocolPointer = 14; break; //signature number
        case 14: Data[4] = Uart0_Data; Uart0ProtocolPointer = 15; break; //6
        case 15: Data[5] = Uart0_Data; Uart0ProtocolPointer = 16; break; //X좌표
        case 16: Data[6] = Uart0_Data; Uart0ProtocolPointer = 17; break; //8
        case 17: Data[7] = Uart0_Data; Uart0ProtocolPointer = 18; break; //Y좌표
        case 18: Data[8] = Uart0_Data; Uart0ProtocolPointer = 19; break; //10
        case 19: Data[9] = Uart0_Data; Uart0ProtocolPointer = 20; break; //11
        case 20: Data[10] = Uart0_Data; Uart0ProtocolPointer = 21; break; //12
        case 21: Data[11] = Uart0_Data; Uart0ProtocolPointer = 100; break; //13
    
      case 100: if(0x00 == Uart0_Data ) { Uart0ProtocolPointer = 101; } 
                         else { Uart0ProtocolPointer = 0; } 
      break; 
      case 101: if(0x00 == Uart0_Data ) { Uart0ProtocolPointer = 102; } 
                         else { Uart0ProtocolPointer = 0; } 
      break; 
      case 102: if(0x00 == Uart0_Data ) { Uart0ProtocolPointer = 103; } 
                         else {Uart0ProtocolPointer = 0;} 
      break; 
      case 103: if(0x55 == Uart0_Data) { 
                        Uart0ProtocolPointer = 0; Uart0ReciveCheckEnd = 1; } 
                        else { Uart0ProtocolPointer = 0; }
      break; 

      default: Uart0ProtocolPointer = 0; 
      break;
      } 
   } 
} 

void Serial_Main0(void) { 
   if(Uart0ReciveCheckEnd==1) { 
      Uart0ReciveCheckEnd = 0; 
      unsigned int curLoc = X_center;
      X_center = Data[5] ;
      Y_center = Data[7] ; 
      SigNum = Data[3];
      area = Data[9]*Data[11];
      if(curLoc > 230 && X_center <= 57 || x_loc > 222 && X_center <= 57){ 
        x_loc = (255 + X_center); 
      }
      else
      {
        x_loc = X_center; 
      }
      
     if(SigNum == 1 && distance >= 5.5)
      {
        if (x_loc <= 90 && x_loc != 0) {  //좌회전
          //analogWrite(4, 35); digitalWrite(28, HIGH); digitalWrite(29, HIGH);
          //analogWrite(5, 25); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
          analogWrite(4, 48); digitalWrite(28, LOW); digitalWrite(29, HIGH);
          analogWrite(5, 55); digitalWrite(30, LOW); digitalWrite(31, HIGH);
        }
        else if (x_loc > 90 && x_loc < 222 && x_loc != 0) { //전방 질주
          analogWrite(4, 48); digitalWrite(28, LOW); digitalWrite(29, HIGH);
          analogWrite(5, 48); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
        }
        else if (x_loc >= 222) { //우회전
          //analogWrite(4, 25); digitalWrite(28, LOW); digitalWrite(29, HIGH);
          //analogWrite(5, 35); digitalWrite(30, LOW); digitalWrite(31, HIGH);
          analogWrite(4, 55); digitalWrite(28, HIGH); digitalWrite(29, HIGH);
          analogWrite(5, 48); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
        }
        else if(x_loc == 0)
        {
          analogWrite(4, 48); digitalWrite(28, HIGH); digitalWrite(29, HIGH);
          analogWrite(5, 48); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
        }
      }
      else if(distance < 5.5)//거리 가까울 시 후진
      {
        if(SigNum == 2 || SigNum == 3 || SigNum == 4)
        {
             //28 LOW 전방, 30 HIGH 전방
          if (x_loc <= 90) {  //좌회전
            //analogWrite(4, 35); digitalWrite(28, HIGH); digitalWrite(29, HIGH);
            //analogWrite(5, 25); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
            analogWrite(4, 50); digitalWrite(28, LOW); digitalWrite(29, HIGH);
            analogWrite(5, 50); digitalWrite(30, LOW); digitalWrite(31, HIGH);
          }
          else if (x_loc > 90 && x_loc < 222) { //전방 질주
            analogWrite(4, 50); digitalWrite(28, LOW); digitalWrite(29, HIGH);
            analogWrite(5, 50); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
          }
          else if (x_loc >= 222) { //우회전
            //analogWrite(4, 25); digitalWrite(28, LOW); digitalWrite(29, HIGH);
            //analogWrite(5, 35); digitalWrite(30, LOW); digitalWrite(31, HIGH);
            analogWrite(4, 50); digitalWrite(28, HIGH); digitalWrite(29, HIGH);
            analogWrite(5, 50); digitalWrite(30, HIGH); digitalWrite(31, HIGH);
          }
        } else //탐색 대상 없음
        { //한바퀴 돌며 탐색
           analogWrite(4, 25); digitalWrite(28, LOW); digitalWrite(29, HIGH);
           analogWrite(5, 25); digitalWrite(30, LOW); digitalWrite(31, HIGH);
        }
           
      }
     
   } 
}


void Uart1(){ 
   Serial1.print("X_center : ");
   Serial1.print(X_center); 
   Serial1.print(' '); 
   Serial1.print("Y_center : ");
   Serial1.print(Y_center); 
   Serial1.print(' '); 
   Serial1.print("Area : ");
   Serial1.print(area);
   Serial1.println(' ');
   Serial1.print("X_loc : ");
   Serial1.println(x_loc);
   Serial1.print("Signum : ");
   Serial1.println(SigNum);
} 
void getDistance(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034/2;
    Serial1.println(duration);
    Serial1.print(" distance : ");
    Serial1.println(distance);
}
void loop() { 
   timer.run(); 
   UartRxProtocol(); 
   Serial_Main0();
  
}
