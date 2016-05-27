/*
    This file is part of Shipfix
 
    Shipfix is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef SONIDO_H
#define SONIDO_H

#include <stdlib.h>

// Puertos y valores de Turbosound
#define PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO 65533
#define VALOR_CHIP_DEFECTO 255
#define VALOR_CHIP_SEGUNDO 254

#define PUERTO_SONIDO_DATO 49149

// Registros de un chip de sonido
// 00..ff
#define REGISTRO_SONIDO_A_FINE_PITCH 0
// 00..0f
#define REGISTRO_SONIDO_A_COARSE_PITCH 1
// 00..ff
#define REGISTRO_SONIDO_B_FINE_PITCH 2
// 00..0f
#define REGISTRO_SONIDO_B_COARSE_PITCH 3
// 00..ff
#define REGISTRO_SONIDO_C_FINE_PITCH 4
// 00..0f
#define REGISTRO_SONIDO_C_COARSE_PITCH 5
// 00..1f
#define REGISTRO_SONIDO_NOISE_PITCH 6
// 0  0 NC NB NA TC TB TA
#define REGISTRO_SONIDO_MIXER 7
// 00..0f, 10
#define REGISTRO_SONIDO_A_VOLUME 8
// 00..0f, 10
#define REGISTRO_SONIDO_B_VOLUME 9
// 00..0f, 10
#define REGISTRO_SONIDO_C_VOLUME 10
// 00..ff
#define REGISTRO_SONIDO_ENVELOPE_FINE_DURATION 11
// 00..ff
#define REGISTRO_SONIDO_ENVELOPE_COARSE_DURATION 12
// ver tabla en datasheet
#define REGISTRO_SONIDO_ENVELOPE_SHAPE 13

#define NUM_REGISTROS_SONIDO 14

#define VOLUMEN_SONIDO_JETPAC 0x06

// Indices de sonidos
#define SONIDO_JETPAC 0
#define SONIDO_COGER_OBJETO 1
#define SONIDO_SOLTAR_OBJETO 2
#define SONIDO_DEJAR_OBJETO_NAVE 3
#define SONIDO_GOLPE 4
#define SONIDO_BERBIQUI 5
#define SONIDO_MARTILLAZO 6
#define SONIDO_FINAL_NAVE 7

extern void inicializarSonido();

extern void setVolumenSonidoJetpac( unsigned char volumen );

extern void reproducirIndiceSonido( unsigned char indiceSonido, unsigned char chip );
extern void reproducirSonido( unsigned char *sonido, unsigned char chip );
extern void apagarSonido( unsigned char chip );

extern unsigned char *getSonido( unsigned char indiceSonido );

#endif /* SONIDO_H */

