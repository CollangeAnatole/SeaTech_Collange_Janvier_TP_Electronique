/* 
 * File:   main.c
 * Author: E306-PC3
 *
 * Created on September 29, 2025, 2:32 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "ADC.h"
#include "main.h"


unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE_DOUX:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_DOUX_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE_DOUX:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_DOUX_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //ÈDtermination de la position des obstacles en fonction des ÈÈËtlmtres
    
    if (robotState.distanceTelemetreCentre < DistMil){
        if (robotState.distanceTelemetreDroit < Dist && robotState.distanceTelemetreGauche < Dist){
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        }
        else if (robotState.distanceTelemetreDroit < Dist ){
            nextStateRobot = STATE_TOURNE_GAUCHE;
        }
        else if (robotState.distanceTelemetreGauche < Dist ){
            nextStateRobot = STATE_TOURNE_DROITE;
        }
        else{
            if (robotState.distanceTelemetreExtremeDroit < DistExt && robotState.distanceTelemetreExtremeGauche < DistExt){
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            }
            else if (robotState.distanceTelemetreExtremeDroit < DistExt ){
                nextStateRobot = STATE_TOURNE_GAUCHE;
            }
            else if (robotState.distanceTelemetreExtremeGauche < DistExt ){
                nextStateRobot = STATE_TOURNE_DROITE;
            }
            else{
                nextStateRobot = STATE_TOURNE_GAUCHE;
            }
        }
    }
    else{
        if (robotState.distanceTelemetreExtremeDroit < DistExt && robotState.distanceTelemetreExtremeGauche < DistExt){
            if (robotState.distanceTelemetreDroit < Dist && robotState.distanceTelemetreGauche < Dist){
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            }
            else if (robotState.distanceTelemetreDroit < Dist ){
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            }
            else if (robotState.distanceTelemetreGauche < Dist ){
                nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            }
            else{
                nextStateRobot = STATE_AVANCE;
            }
        }
        else if (robotState.distanceTelemetreExtremeDroit < DistExt ){
            if (robotState.distanceTelemetreDroit < Dist ){
                nextStateRobot = STATE_TOURNE_GAUCHE;
            }
            else{
                nextStateRobot = STATE_TOURNE_GAUCHE_DOUX;
            }
        }
        else if (robotState.distanceTelemetreExtremeGauche < DistExt ){
            if (robotState.distanceTelemetreGauche < Dist ){
                nextStateRobot = STATE_TOURNE_DROITE;
            }
            else{
                nextStateRobot = STATE_TOURNE_DROITE_DOUX;
            }
        }
        else{
            if (robotState.distanceTelemetreDroit < Dist && robotState.distanceTelemetreGauche < Dist){
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            }
            else if (robotState.distanceTelemetreDroit < Dist ){
                nextStateRobot = STATE_TOURNE_GAUCHE;
            }
            else if (robotState.distanceTelemetreGauche < Dist ){
                nextStateRobot = STATE_TOURNE_DROITE;
            }
            else{
                nextStateRobot = STATE_AVANCE;
            }            
        }
        
    }
    /*
    if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreExtremeDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30 &&
            robotState.distanceTelemetreExtremeGauche < 20) //Obstacle ‡ extreme gauche
        positionObstacle = OBSTACLE_A_EXTREME_GAUCHE;
    else if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreExtremeDroit < 20 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30 &&
            robotState.distanceTelemetreExtremeGauche > 30) //Obstacle ‡ extreme droit
        positionObstacle = OBSTACLE_A_EXTREME_DROIT;
    else if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreExtremeDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30 &&
            robotState.distanceTelemetreExtremeGauche > 30) //Obstacle ‡droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreExtremeDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30 &&
            robotState.distanceTelemetreExtremeGauche < 30) //Obstacle ‡gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreExtremeDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30 &&
            robotState.distanceTelemetreExtremeGauche < 30) //Obstacle ‡ extreme gauche
        positionObstacle = OBSTACLE_A_EXTREME_GAUCHE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreExtremeDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30 &&
            robotState.distanceTelemetreExtremeGauche < 20) //Obstacle ‡ extreme droit
        positionObstacle = OBSTACLE_A_EXTREME_DROIT;
    //ÈDtermination de lÈ?tat ‡venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXTREME_DROIT)
        nextStateRobot = STATE_TOURNE_GAUCHE_DOUX;
    else if (positionObstacle == OBSTACLE_A_EXTREME_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE_DOUX;
    */
    //Si l?on n?est pas dans la transition de lÈ?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}

    
void Marche(){
    EN_PWM=1;
    robotState.compteur=0;
    robotState.EnMarche=1;
}

void UpdateCompteur (){
    if (robotState.EnMarche==1){
        if (robotState.compteur<MaxCompt){
            robotState.compteur+=1;
        }
        else{
            EN_PWM=0;
            robotState.EnMarche=0;
            robotState.compteur=0;
        }
    }
}



int main(void) {
    /***********************************************************************************************/
    //Initialisation oscillateur
    /***********************************************************************************************/
    InitOscillator();

    /***********************************************************************************************/
    // Configuration des input et output (IO)
    /***********************************************************************************************/
    InitIO();
    LED_BLANCHE_1 = 0;
    LED_BLEUE_1 = 0;
    LED_ORANGE_1 = 0;
    LED_ROUGE_1 = 0;
    LED_VERTE_1 = 0;
    LED_VERTE_2 = 0;
    LED_BLANCHE_2 = 0;
    LED_BLEUE_2 = 0;
    LED_ORANGE_2 = 0;
    LED_ROUGE_2 = 0;
    InitTimer23();
    InitTimer1();
    InitTimer4();

    InitPWM();
    InitADC1();
    SetFreqTimer1(50);
    SetFreqTimer4(1000);
    //PWMSetSpeedConsigne(20,MOTEUR_DROIT);
    //PWMSetSpeedConsigne(20,MOTEUR_GAUCHE);

    

    /***********************************************************************************************/
    // Boucle Principale
    /***********************************************************************************************/
    while (1) {



        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;
        }


        int valeur_seuil = 30;

        if (robotState.distanceTelemetreGauche < valeur_seuil) {
            LED_BLEUE_1 = 1;
        } else {
            LED_BLEUE_1 = 0;
        }

        if (robotState.distanceTelemetreCentre < valeur_seuil) {
            LED_ORANGE_1 = 1;
        } else {
            LED_ORANGE_1 = 0;
        }

        if (robotState.distanceTelemetreDroit < valeur_seuil) {
            LED_ROUGE_1 = 1;
        } else {
            LED_ROUGE_1 = 0;
        }
        
        if (robotState.distanceTelemetreExtremeDroit < valeur_seuil) {
            LED_VERTE_1 = 1;
        } else {
            LED_VERTE_1 = 0;
        }
        
        if (robotState.distanceTelemetreExtremeGauche < valeur_seuil) {
            LED_BLANCHE_1 = 1;
        } else {
            LED_BLANCHE_1 = 0;
        }
        
        
        if(_RH0==1 || _RH1==1 || _RH2==1){
           LED_BLEUE_2 = 1;
           LED_ORANGE_2 =1;
           LED_ROUGE_2 = 1;
           LED_VERTE_2 = 1;
           LED_BLANCHE_2 = 1;
           Marche();
        }
        else{
           LED_BLEUE_2 = 0;
           LED_ORANGE_2 =0;
           LED_ROUGE_2 = 0;
           LED_VERTE_2 = 0;
           LED_BLANCHE_2 = 0;
        }

    } // fin main
}

