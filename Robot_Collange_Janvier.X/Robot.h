/* 
 * File:   Robot.h
 * Author: E306-PC3
 *
 * Created on 1 octobre 2025, 08:40
 */

#ifndef ROBOT_H
#define ROBOT_H
#define acceleration 5
#define MaxCompt 990

typedef struct robotStateBITS {
    union {
        struct {
            unsigned char taskEnCours;
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            
            float distanceTelemetreDroit;
            float distanceTelemetreCentre;
            float distanceTelemetreGauche;
            float distanceTelemetreExtremeDroit;
            float distanceTelemetreExtremeGauche;
            
            double compteur;
            int EnMarche;
        };
    };
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */


