/*
 * magnet.h
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#ifndef INC_MAGNET_H_
#define INC_MAGNET_H_

#pragma once
#include "main.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} Magnet_t;

void Magnet_On(const Magnet_t *m);
void Magnet_Off(const Magnet_t *m);


#endif /* INC_MAGNET_H_ */
