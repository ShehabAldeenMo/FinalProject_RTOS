/*
 * LCD_Private.h
 *
 *  Created on: Aug 25, 2023
 *      Author: HP
 */

#ifndef LCD_PRIVATE_H_
#define LCD_PRIVATE_H_

/* LCD defines */
/*** to enable 8-bit mode ***/
#ifdef EIGHT_BIT
#define FUNC_SET (0x003C)
#endif

/*** to enable 4-bit mode ***/
#ifndef EIGHT_BIT
#define FUNC_SET (0x002C)
#endif

#define DISPLAY_ON_CURSOR_ON_BLINKING     ((0x000F))
#define DISPLAY_CLEAR                     ((0x0001))
#define HOME_RETURN                       ((0x0002))
#define DISPLAY_ON_CURSOR_ON              ((0x000E))
#define DISPLAY_ON_CURSOR_OFF             ((0x000C))
#define DISPLAY_OFF_CURSOR_OFF            ((0x0008))
#define DISPLAY_SHIFT_RIGHT               ((0x001C))
#define DISPLAY_SHIT_LEFT                 ((0x0018))
#define CURSOR_SHIFT_RIGHT                ((0x0014))
#define CURSOR_SHIFT_LEFT                 ((0x0010))
#define LINE_2_START                      ((0x00C0))
#define ENTERY_MODE_RIGHT                 ((0x0006))


#define HIGH 1
#define LOW  0

#endif /* LCD_PRIVATE_H_ */
