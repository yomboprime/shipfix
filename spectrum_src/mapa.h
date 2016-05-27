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


#ifndef MAPA_H
#define MAPA_H

#include "modoRadastan.h"

#define ALTURA_TERRENO 32

#define OFFSET_ALTURA_TERRENO ( PANTALLA_X2 * ((unsigned int) ( PANTALLA_Y - ALTURA_TERRENO ) ) )

extern unsigned char *getMapaAlturas();
extern unsigned char getNumPantallasX();

extern void pintarMapaColorPlano( unsigned char *pantalla, int posX2, unsigned char color );
extern void borrarMapa( unsigned char *pantalla );

#endif /* MAPA_H */

