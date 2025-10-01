/* 
 * File:   PWM.h
 * Author: E306-PC3
 *
 * Created on 1 octobre 2025, 08:52
 */

#ifndef PWM_H
#define	PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1


void InitPWM(void);
void PWMSetSpeedConsigne(float,int);

#endif	/* PWM_H */

