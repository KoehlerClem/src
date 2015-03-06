
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
    ecrobot_status_monitor("");

}

int a = 750;
void move(int v){

    nxt_motor_set_speed(NXT_PORT_C,v,0);
    nxt_motor_set_speed(NXT_PORT_A,v,0);

}

void left() {
	do{
	nxt_motor_set_speed(NXT_PORT_A, -65,0);
	nxt_motor_set_speed(NXT_PORT_C, 65,0);
	}
	while(ecrobot_get_light_sensor(NXT_PORT_S2) <= a);
}

void right() {
	do{
	nxt_motor_set_speed(NXT_PORT_A, 65,0);
	nxt_motor_set_speed(NXT_PORT_C, -65,0);
	}
	while(ecrobot_get_light_sensor(NXT_PORT_S2) <= a);
}



void left2(int g) {
        nxt_motor_set_count(NXT_PORT_C,0);

        do{
            nxt_motor_set_speed(NXT_PORT_A,-65,0);
            nxt_motor_set_speed(NXT_PORT_C,65,0);


            ecrobot_status_monitor("wert");
        }while((ecrobot_get_light_sensor(NXT_PORT_S2) <= a) && ( nxt_motor_get_count(NXT_PORT_C) <= g));


}

void right2(int g) {
        nxt_motor_set_count(NXT_PORT_A,0);

        do{
            nxt_motor_set_speed(NXT_PORT_A,65,0);
            nxt_motor_set_speed(NXT_PORT_C,-65,0);


        }while(( ecrobot_get_light_sensor(NXT_PORT_S2) <= a) && (nxt_motor_get_count(NXT_PORT_A) <= g));
}

void left3(int g){
	 nxt_motor_set_count(NXT_PORT_C,0);

	        do{
	            nxt_motor_set_speed(NXT_PORT_A,-65,0);
	            nxt_motor_set_speed(NXT_PORT_C,65,0);
	          }
	        while(nxt_motor_get_count(NXT_PORT_C) <= g);
}
void right3(int g){
	 nxt_motor_set_count(NXT_PORT_A,0);

	        do{
	            nxt_motor_set_speed(NXT_PORT_A,65,0);
	            nxt_motor_set_speed(NXT_PORT_C,-65,0);
	          }
	        while(nxt_motor_get_count(NXT_PORT_A) <= g);
}



void left3(int g){
	 nxt_motor_set_count(NXT_PORT_A,0);

	        do{
	            nxt_motor_set_speed(NXT_PORT_C,-65,0);
	            nxt_motor_set_speed(NXT_PORT_A,65,0);
	          }
	        while(nxt_motor_get_count(NXT_PORT_A) <= g);
}

void stopp(){
    nxt_motor_set_speed(NXT_PORT_C,0,0);
    nxt_motor_set_speed(NXT_PORT_A,0,0);
}

void token(){

    ecrobot_sound_tone(500, 500, 50);
    ecrobot_get_RCX_touch_sensor(NXT_PORT_S1);
    nxt_motor_set_count(NXT_PORT_A,0);
    do{
        move(-65);
    }while( nxt_motor_get_count(NXT_PORT_A) >= -100);
    stopp();
    systick_wait_ms(10000);
}

void cross(){

	int lineV;
    int lineL;
    int lineR;

    left2(100); 											// grade auf der Linie

    nxt_motor_set_count(NXT_PORT_A,0);

    do{
        move(70);
           }
    while(nxt_motor_get_count(NXT_PORT_A) <= 160);  		//bewegt sich nach vorn

            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){	//keine Linie
                        left2(100);
                        right2(200);
            }
            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){	//Linie nicht gefunden
                lineV = 0;
                ecrobot_sound_tone(1000, 500, 30);

            }
            else{
                lineV = 1;
                ecrobot_sound_tone(100, 500, 30);         	//Linie gefunden

            }
      //links
            left3(100);
            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){//keine Linie
                            left2(300);

                }
                if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){//Linie nicht gefunden
                    lineL = 0;
                    ecrobot_sound_tone(1000, 500, 30);
                }
                else{
                    lineL = 1;
                    ecrobot_sound_tone(100, 500, 30);		//sonst null
                   	 }
        //rechts
                left3(100);
                left2(300);
                left3(100);
                left2(300);
                if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){
                      lineR = 0;
                      ecrobot_sound_tone(1000, 500, 30);

                }else{
                      lineR = 1;
                      ecrobot_sound_tone(100, 500, 30);
                      stopp();
                      }


                int richtung;
                           //Hilfsfunktion
                        if( lineL == 0){
                       	 if(lineR == 0){
                       		if(lineV == 0){//keine Linie
                       			richtung = 3;
                       		}else{// nur geradeaus
                       			richtung = 2;
                       		}
                       	 }else{//geradeaus und links-> links
                       		 richtung = 0;
                       	 }
                        }else{//alle->rechts
                       	richtung = 1;
                        }


                           //switch

                switch(richtung){
                           case 0: //rechts
                           		break;
                           	case 1: //links
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

                           	case 2:	//geradeaus
                           		left3(30);
                           		left();

                         	  	  break;

                         	  case 3://zurück
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
    ecrobot_set_light_sensor_active(NXT_PORT_S2);

    if(ecrobot_get_touch_sensor(NXT_PORT_S3) == 1 || ecrobot_get_touch_sensor(NXT_PORT_S4) == 1){
        token();
    }

    move(70);

    if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){
        //do{
            left2(100);
            right2(200);
        //} while(ecrobot_get_light_sensor(NXT_PORT_S2) <= 650);
            if(ecrobot_get_light_sensor(NXT_PORT_S2) <= a){
                cross();
            }
    }

}

   TASK(OSEK_Main_Task) {
        while (1) {
            hello_world();
            sensor();

        }
    }
