/*
    This file is part of Shipfix
 
    Shipfix  is free software: you can redistribute it and/or modify
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


#include "mapa.h"
#include "alturas.h"

extern unsigned char *getMapaAlturas() {
    return alturas;
}

extern unsigned char getNumPantallasX() {
    return NUM_PANTALLAS_X;
}

extern void pintarMapaColorPlano( unsigned char *pantalla, int posX2, unsigned char color ) {
    
    int x2 = posX2;
    
    int maxX2 = posX2 + PANTALLA_X2;
    unsigned char maxi = PANTALLA_X2;
    unsigned char i, j;
    unsigned char *p;

    if ( maxX2 >= NUM_ALTURAS ) {
        maxi = ( unsigned char ) NUM_ALTURAS - posX2;
    }

    pantalla += PANTALLA_X2 * ( PANTALLA_Y - 1 );
    for ( i = 0; i < maxi; i++ ) {
        unsigned char altura = alturas[ x2++ ];
        p = pantalla;
        // Pinta terreno de abajo a arriba
        for ( j = 0; j <= altura; j++ ) {
            *p = color;
            p -= PANTALLA_X2;
        }
        
        // Borra el resto
        for ( ; j < ALTURA_TERRENO; j++ ) {
            *p = 0x00;
            p -= PANTALLA_X2;
        }
        

        pantalla ++;
    }
}

extern void borrarMapa( unsigned char *pantalla ) {

    int i;

    // Avanza lineas hasta altura inicio terreno
    pantalla += OFFSET_ALTURA_TERRENO;

    for ( i = OFFSET_ALTURA_TERRENO; i < NUM_BYTES_PANTALLA; i++ ) {
        *pantalla++ = 0;
    }

}

