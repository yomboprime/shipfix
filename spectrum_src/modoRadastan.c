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


#include <stdlib.h>
#include <spectrum.h>
#include "modoRadastan.h"

// Pantalla en la que se dibuja. Al inicio es la 1 (la que no se ve))
unsigned char iPantallaActual = 1;

unsigned char *pantallaActual = DIRECCION_PANTALLA1;
unsigned char *pantallaLaOtra = DIRECCION_PANTALLA0;

extern void radasPonerModoRadastan( unsigned char atributos, unsigned char borde, unsigned char *paleta ) {
    
    // Los atributos y el borde se ponen antes de borrar las dos pantallas shadow screen

    zx_colour( atributos );
    zx_border( borde );

    radasClsPantalla0( 0 );
    radasClsPantalla1( 0 );

    radasPonerPaleta( paleta );

    outp( 0xFC3B, 64 );
    outp( 0xFD3B, 3 );

    iPantallaActual = 1;
    pantallaActual = DIRECCION_PANTALLA1;
    pantallaLaOtra = DIRECCION_PANTALLA0;

}

extern void radasPonerModoNormal( unsigned char atributos, unsigned char borde ) {

    radasClsPantalla0( 0 );
    
    if ( ! iPantallaActual ) {
         radasCambiarPantalla();
    }
    
    outp( 0xFC3B, 64 );
    outp( 0xFD3B, 0 );

    zx_colour( atributos );
    zx_border( borde );

}

extern void radasPonerPaleta( unsigned char *paleta ) {
    
    unsigned char i;
    for ( i = 0; i < 16; i++ ) {
        ulaplus_set( i, paleta[ i ] );
    }

}

extern unsigned char *radasObtenerPantallaActual() {

    return pantallaActual;

}

extern unsigned char *radasObtenerLaOtraPantalla() {
    
    return pantallaLaOtra;
    
}

extern unsigned char obtenerIndicePantalla() {
    return iPantallaActual;
}

extern void radasCambiarPantalla() {
    
    if ( iPantallaActual ) {
        outp( 0x00FF, 1 );
        pantallaActual = DIRECCION_PANTALLA0;
        pantallaLaOtra = DIRECCION_PANTALLA1;
        iPantallaActual = 0;
    }
    else {
        outp( 0x00FF, 0 );
        pantallaActual = DIRECCION_PANTALLA1;
        pantallaLaOtra = DIRECCION_PANTALLA0;
        iPantallaActual = 1;
    }

}

extern void radasCambiarVariablesPantalla() {
    
    if ( iPantallaActual ) {
        pantallaActual = DIRECCION_PANTALLA0;
        pantallaLaOtra = DIRECCION_PANTALLA1;
        iPantallaActual = 0;
    }
    else {
        pantallaActual = DIRECCION_PANTALLA1;
        pantallaLaOtra = DIRECCION_PANTALLA0;
        iPantallaActual = 1;
    }

}

extern void radasCls( unsigned char *pantalla, unsigned char valor ) {
    int i;
    for ( i = 0; i < NUM_BYTES_PANTALLA; i++ ) {
        *pantalla++ = valor;
    }
}

extern void radasClsPantalla0( unsigned char valor ) {
    unsigned char *p;
    for ( p = 0x4000U; p < 22528; p++ ) {
        *p = valor;
    }
}

extern void radasClsPantalla1( unsigned char valor ) {
    unsigned char *p;
    for ( p = 0x6000U; p < 30720; p++ ) {
        *p = valor;
    }
}

extern void radasCopiarPantalla( unsigned char *origen, unsigned char *destino ) {
    unsigned int i;
    for ( i = 0; i < NUM_BYTES_PANTALLA; i++ ) {
        *destino++ = *origen++;
    }
}

extern void radasPintarPixel( unsigned char x, unsigned char y, unsigned char color ) {

    // 0 <= x <= 128
    // 0 <= y <= 96
    // De color solo se usa el nibble bajo
    
    unsigned char valorPixeles;
    
    unsigned char *ptrPixel = ( unsigned char * ) ( pantallaActual + ( ( ( unsigned int ) x ) >> 1 ) + ( ( (  unsigned int ) y ) << BITS_ANCHO_PANTALLA ) );

    if ( ! ( x & 1 ) ) {
        color <<= 4;
    }

    valorPixeles = *ptrPixel;

    valorPixeles |= color;

    *ptrPixel = valorPixeles;

}

extern void radasPintar2Pixels( unsigned char x2, unsigned char y, unsigned char color ) {
    
    // x2 es en pares de pixels, o sea x/2
    // 0 <= x2 <= 64
    // 0 <= y <= 96
    // color tiene los dos nibbles de ambos pixels
    

    *( ( unsigned char * ) ( pantallaActual + ( (  unsigned int ) x2 ) + ( ( (  unsigned int ) y ) << 6 ) ) ) = color;

}

extern void radasPintarRectangulo( unsigned char x2, unsigned char y, unsigned char ancho2, unsigned char alto, unsigned char valor ) {

    // x2 es en pares de pixels, o sea x/2, al igual que ancho2
    // 0 <= x2 <= 64
    // 0 <= y <= 96
    // 1 <= ancho2 <= 64
    // 1 <= alto <= 96
    // color tiene los dos nibbles para cada par de pixels
    
    unsigned char i, j;
    unsigned char *p = pantallaActual + ( (  unsigned int ) x2 ) + ( ( (  unsigned int ) y ) << BITS_ANCHO_PANTALLA );
    unsigned int dx = PANTALLA_X2 - ancho2;
    for ( j = 0; j < alto; j++ ) {
        for ( i = 0; i < ancho2; i++ ) {
            *p++ = valor;
        }
        p += dx;
    }
}

extern void radasBorrarRectangulo( unsigned char x2, unsigned char y, unsigned char ancho2, unsigned char alto ) {

    // x2 es en pares de pixels, o sea x/2, al igual que ancho2
    // 0 <= x2 <= 64
    // 0 <= y <= 96
    // 1 <= ancho2 <= 64
    // 1 <= alto <= 96
    
    unsigned char i, j;
    unsigned char *p = pantallaActual + ( (  unsigned int ) x2 ) + ( ( (  unsigned int ) y ) << BITS_ANCHO_PANTALLA );
    unsigned int dx = PANTALLA_X2 - ancho2;
    for ( j = 0; j < alto; j++ ) {
        for ( i = 0; i < ancho2; i++ ) {
            *p++ = 0;
        }
        p += dx;
    }
}

extern void radasPintarImagen( unsigned char x2, unsigned char y, Imagen *imagen ) {
    

    // x2 es en pares de pixels, o sea x/2, al igual que ancho2
    // Se hace clipping con los bordes de la pantalla
    
    unsigned char i, j;
    unsigned char *p = pantallaActual + ( (  unsigned int ) x2 ) + ( ( (  unsigned int ) y ) << BITS_ANCHO_PANTALLA );
    unsigned int dx = PANTALLA_X2 - imagen->tamx2;
    unsigned char tamy = imagen->tamy;
    unsigned char tamx2 = imagen->tamx2;
    unsigned char *pImg = imagen->pixels;
    for ( j = 0; j < tamy; j++ ) {
        for ( i = 0; i < tamx2; i++ ) {
            *p++ = *pImg++;
        }
        p += dx;
    }

}

extern void radasScrollRectanguloArriba( unsigned char *pantalla, unsigned char x2, unsigned char y, unsigned char ancho2, unsigned char alto ) {

    // Se hace scroll arriba un pixel, insertando color 0 por debajo
    // x2 es en pares de pixels, o sea x/2, al igual que ancho2
    // 0 <= x2 <= 64
    // 0 <= y <= 96
    // 1 <= ancho2 <= 64
    // 1 <= alto <= 96
    
    unsigned char i, j;
    unsigned char *p = pantalla + ( (  unsigned int ) x2 ) + ( ( (  unsigned int ) y ) << BITS_ANCHO_PANTALLA );
    unsigned int dx = PANTALLA_X2 - ancho2;
    unsigned char *pdx = p + PANTALLA_X2;
    for ( j = 0; j < alto - 1; j++ ) {
        for ( i = 0; i < ancho2; i++ ) {
            *p++ = *pdx++;
        }
        p += dx;
        pdx += dx;
    }
    for ( i = 0; i < ancho2; i++ ) {
        *p++ = 0;
    }
}

extern void radasPintarImagenEscalada( unsigned char invEscala, unsigned char x2, unsigned char y, Imagen *imagen ) {

    unsigned char i, j;
    unsigned char *p0 = pantallaActual + ( (  unsigned int ) x2 ) + ( ( (  unsigned int ) y ) << BITS_ANCHO_PANTALLA );
    unsigned char *p;
    unsigned int dx2 = ( PANTALLA_X2 - imagen->tamx2 ) << 1;
    unsigned char tamy = imagen->tamy;
    unsigned char tamx2 = imagen->tamx2;
    unsigned char *pImg0 = imagen->pixels;
    unsigned char *pImg;
    for ( j = 0; j < tamy; j+= invEscala ) {
        p = p0;
        pImg = pImg0;
        for ( i = 0; i < tamx2; i+= invEscala ) {
            *p++ = *pImg;
            pImg += invEscala;
        }
        p0 += dx2;
        pImg0 += tamx2 << 1;
    }

}

