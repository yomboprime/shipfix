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


#include <spectrum.h>

#include "modoRadastan.h"
#include "imagenCarga.h"

void main( void ) {

    radasPonerModoRadastan( PAPER_BLACK, PAPER_WHITE | INK_BLACK, &paletas[ 0 ] );

    radasPintarImagen( 0, 0, &imagenes[ 0 ] );

    ula_sync();
    radasCambiarPantalla();
    
    // El programa termina con el modo radas puesto y mostrando la imagen.
    // Se vuelve al basic y se carga el programa principal.
    
}

