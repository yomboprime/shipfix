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


// Utilidades de texto en modo de pantalla de ZX Spectrum normal

#ifndef UTILSTEXTO_H
#define UTILSTEXTO_H

#include <stdio.h>

#define DIRECCION_INICIO_ATRIBUTOS 22528

extern void clsTexto() {
    fputc_cons( 12 );
}

extern void imprimir( unsigned char *texto ) {
    fputs( texto, stdout );
}

extern void pintarSegmentoConAtributos( unsigned char y, unsigned char x0, unsigned char x1, unsigned char atributos ) {

    unsigned char x;
    unsigned char *ptrAtributo = (unsigned char *)DIRECCION_INICIO_ATRIBUTOS;
    for ( x = 0; x < y; x++ ) {
        ptrAtributo += 32;
    }
    ptrAtributo += x0;
    
    for ( x = x0; x <= x1; x++ ) {
        *ptrAtributo++ = atributos;
    }

}

#endif /* UTILSTEXTO_H */

