/*
 * buttons.h
 *
 *  Created on: 23.10.2024
 *      Author: A.Huth
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

extern bool button1event;
extern bool button2event;
extern bool button3event;

void initButtons(void);
void buttonStatus (void);

bool getButton1 (void);
bool getButton2 (void);
bool getButton3 (void);




#endif /* BUTTONS_H_ */