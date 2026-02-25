/*
 * magnet.c
 *
 *  Created on: Jan 19, 2026
 *      Author: moham
 */

#include "magnet.h"

void Magnet_On(const Magnet_t *m)  { if (m) HAL_GPIO_WritePin(m->port, m->pin, GPIO_PIN_SET); }
void Magnet_Off(const Magnet_t *m) { if (m) HAL_GPIO_WritePin(m->port, m->pin, GPIO_PIN_RESET); }



