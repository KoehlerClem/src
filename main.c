
#include "../h/main.h"


/// DO NOT DELETE THIS METHOD
/// It is called every 1ms and e.g. can be used for implementing a
/// real time counter / clock.
void user_1ms_isr_type2(void) {
}

void ecrobot_device_initialize(void) {
}

void ecrobot_device_terminate(void) {
}

void hello_world() {
    ecrobot_status_monitor("I AM ULTRON");

}

int a = 750;
void move(int v){   // bewege dich vorwärts mit der geschwindigkeit v

    nxt_motor_set_speed(NXT_PORT_C,v,0);
    nxt_motor_set_speed(NXT_PORT_A,v,0);

}

void left() {			// drehe dich nach links ( mit v = 65) bis ein schwarzer strich kommt
	do{
	nxt_motor_set_speed(NXT_PORT_A, -65,0);
	nxt_motor_set_speed(NXT_PORT_C, 65,0);
	}
	while(ecrobot_get_light_sensor(NXT_PORT_S2) <= a);
}

void right() {		// drehe dich nach rechts (  mit v = 65) bis ein schwarzer strich kommt
	do{
	nxt_motor_set_speed(NXT_PORT_A, 65,0);
	nxt_motor_set_speed(NXT_PORT_C, -65,0);
	}
	while(ecrobot_get_light_sensor(NXT_PORT_S2) <= a);
}



void left2(int g) {				// drehe dich nach links( v = 65) bis schwarzer strich kommt
								//und die umdrehungszahl erreicht ist
        nxt_motor_set_count(NXT_PORT_C,0);

        do{
            nxt_motor_set_speed(NXT_PORT_A,-65,0);
            nxt_motor_set_speed(NXT_PORT_C,65,0);



        }while((ecrobot_get_light_sensor(NXT_PORT_S2) <= a) && ( nxt_motor_get_count(NXT_PORT_C) <= g));


}

void right2(int g) {				// drehe dich nach recht ( v = 65) bis schwarzer strich kommt
									//und umdrehungszahl erreicht ist
        nxt_motor_set_count(NXT_PORT_A,0);

        do{
            nxt_motor_set_speed(NXT_PORT_A,65,0);
            nxt_motor_set_speed(NXT_PORT_C,-65,0);


        }while(( ecrobot_get_light_sensor(NXT_PORT_S2) <= a) && (nxt_motor_get_count(NXT_PORT_A) <= g));
}

void left3(int g){					// drehe dich nach links ( v= 65) bis umdrehungszahl(g) erreicht ist( schwarz/weiss egal)
	 nxt_motor_set_count(NXT_PORT_C,0);

	        do{
	            nxt_motor_set_speed(NXT_PORT_A,-65,0);
	            nxt_motor_set_speed(NXT_PORT_C,65,0);
	          }
	        while(nxt_motor_get_count(NXT_PORT_C) <= g);
}

void right3(int g){					// drehe dich nach recht ( v = 65) bis umdrehungszahl(g) erreicht ist ( schwarz/weiss egal)
	 nxt_motor_set_count(NXT_PORT_A,0);

	        do{
	            nxt_motor_set_speed(NXT_PORT_A,65,0);
	            nxt_motor_set_speed(NXT_PORT_C,-65,0);
	          }
	        while(nxt_motor_get_count(NXT_PORT_A) <= g);
}






void token(){ 					// erkennt gegenstand, gibt ton von sich, fährt ein stück zurück( g= -100), wartet 10 sek

    ecrobot_sound_tone(500, 500, 50);
    ecrobot_get_RCX_touch_sensor(NXT_PORT_S1);
    nxt_motor_set_count(NXT_PORT_A,0);
    do{
        move(-65);
    }while( nxt_motor_get_count(NXT_PORT_A) >= -100);
     systick_wait_ms(10000);
}

void cross(){								// kreuzung, fährt nach links( g = 100) sodass er gerade steht

	int lineV;
    int lineL;
    int lineR;

    left2(100); 											// grade auf der Linie

    nxt_motor_set_count(NXT_PORT_A,0);

    do{
        move(70);
           }
    while(nxt_motor_get_count(NXT_PORT_A) <= 160);  		//bewegt sich nach vorn ( g = 160) über die kreuzung

            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){	//testet ob grade voraus eine linie ist
                        left2(100);
                        right2(200);
            }
            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){	//keine Linie
                lineV = 0;
                ecrobot_sound_tone(1000, 500, 30);

            }
            else{
                lineV = 1;
                ecrobot_sound_tone(100, 500, 30);         	//Linie

            }
      //links
            left3(100);										// dreht sich nach links( g = 100)
            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){  //keine linie --> g = 300 nach links
                            left2(300);

                }
                if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){//Linie nicht gefunden
                    lineL = 0;
                    ecrobot_sound_tone(1000, 500, 30);
                }
                else{
                    lineL = 1;
                    ecrobot_sound_tone(100, 500, 30);		//Linie
                   	 }
        //rechts
                left3(100);								// untersucht ob es eine linie rechts gibt
                left2(300);
                left3(100);
                left2(300);
                if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){
                      lineR = 0;						// keine Linie
                      ecrobot_sound_tone(1000, 500, 30);

                }else{
                      lineR = 1;						// Linie
                      ecrobot_sound_tone(100, 500, 30);

                      }


                int richtung;
                           //Hilfsfunktion
                        if( lineL == 0){
                       	 if(lineR == 0){
                       		if(lineV == 0){			//keine Linie
                       			richtung = 3;
                       		}else{					// nur geradeaus
                       			richtung = 2;
                       		}
                       	 }else{						//geradeaus und links-> links
                       		 richtung = 0;
                       	 }
                        }else{						//alle->rechts
                       	richtung = 1;
                        }


                           //switch

                switch(richtung){
                           case 0: 					//rechts abbiegen
                           		break;
                           	case 1: 				//links abbiegen
                           		if(lineV==0){
                           			left3(50);
                           			ecrobot_sound_tone(1000, 500, 30);
                           			left2(500);
                           		}else{
                           			left3(50);
                           			left2(300);
                           			ecrobot_sound_tone(100, 500, 30);
                           			left3(50);
                           			left2(350);
                           		}
                               	break;

                           	case 2:					//geradeaus fahren
                           		left3(30);
                           		left();

                         	  	  break;

                         	  case 3:				//zurück fahren
                         		  	 right3(30);
                         		  	 right();
               						 break;

                         	  default:
                         	  	  	  ecrobot_sound_tone(1000, 400, 50);
                         	  	  	  ecrobot_sound_tone(100, 400, 50);
                         	  	  	  ecrobot_sound_tone(1000, 400, 50);


                }


}

void sensor(){
    ecrobot_set_light_sensor_active(NXT_PORT_S2);  // sensor aktiviert

    if(ecrobot_get_touch_sensor(NXT_PORT_S3) == 1 || ecrobot_get_touch_sensor(NXT_PORT_S4) == 1){ // touchsensor wird gedrückt
        token(); // gehe in void funktion
    }

    move(70); // bewege dich mit v = 70 vor

    if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){ // wenn lichtsensor weißen bereich entddeckt, soll er erst
    												//erkunden ob er nicht von der Linie abgekommen ist, durch
    												//links/rechts Bewegung
        //do{
            left2(100);
            right2(200);
        //} while(ecrobot_get_light_sensor(NXT_PORT_S2) <= 650);
            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){ //wenn immernoch weiß und keine schwarze linie entdeckt--> kreuzung
                cross();
            }
    }

}

   TASK(OSEK_Main_Task) { // main funktion
        while (1) {
            hello_world();
            sensor();		// void funktion wird ausgeführt

        }
    }
