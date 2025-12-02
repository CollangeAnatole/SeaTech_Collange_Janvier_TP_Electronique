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
    SetFreqTimer1(5);
    SetFreqTimer4(5);
    //PWMSetSpeed(20,MOTEUR_DROIT);
    //PWMSetSpeed(20,MOTEUR_GAUCHE);
    /***********************************************************************************************/
    // Boucle Principale
    /***********************************************************************************************/
    while (1) {

        unsigned int ADCValue0;
        unsigned int ADCValue1;
        unsigned int ADCValue2;
        
        if (ADCIsConversionFinished() == 1)
        {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
           }

        
        int valeur_seuil = 30;
        
        if (robotState.distanceTelemetreGauche<valeur_seuil){
            LED_BLEUE_1 = 1;
        }
        else{
            LED_BLEUE_1 = 0;
        }
        
        if (robotState.distanceTelemetreCentre<valeur_seuil){
            LED_ORANGE_1 = 1;
        }
        else{
            LED_ORANGE_1 = 0;
        }
        
        if (robotState.distanceTelemetreDroit<valeur_seuil){
            LED_ROUGE_1 = 1;
        }
        else{
            LED_ROUGE_1 = 0;
        }

    } // fin main
}

