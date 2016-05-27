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


#ifndef MODO_RADASTAN
#define MODO_RADASTAN

#define DIRECCION_PANTALLA0 ( ( unsigned char * ) 0x4000U )
#define DIRECCION_PANTALLA1 ( ( unsigned char * ) 0x6000U )
#define PANTALLA_X 128
#define PANTALLA_Y 96
#define BITS_ANCHO_PANTALLA 6
#define PANTALLA_X2 ( PANTALLA_X >> 1 )
#define NUM_BYTES_PANTALLA 6144

typedef struct {
    
    unsigned char tamx2;
    unsigned char tamy;
    unsigned char *pixels;
    
} Imagen;

extern void radasPonerModoRadastan( unsigned char atributos, unsigned char borde, unsigned char *paleta );

extern void radasPonerPaleta( unsigned char *paleta );

// TODO: funcion para poner un solo registro de la paleta

extern unsigned char *radasObtenerPantallaActual();
extern unsigned char *radasObtenerLaOtraPantalla();

extern void radasCambiarPantalla();

extern void radasCambiarVariablesPantalla();

extern void radasCls( unsigned char *pantalla, unsigned char valor );
extern void radasClsPantalla0( unsigned char valor );
extern void radasClsPantalla1( unsigned char valor );

extern void radasCopiarPantalla( unsigned char *origen, unsigned char *destino );


extern void radasPintarPixel( unsigned char x, unsigned char y, unsigned char color );
extern void radasPintar2Pixels( unsigned char x2, unsigned char y, unsigned char color );

extern void radasPintarRectangulo( unsigned char x2, unsigned char y, unsigned char ancho2, unsigned char alto, unsigned char valor );
extern void radasBorrarRectangulo( unsigned char x2, unsigned char y, unsigned char ancho2, unsigned char alto );
extern void radasPintarImagen( unsigned char x2, unsigned char y, Imagen *imagen );

extern void radasScrollRectanguloArriba( unsigned char *pantalla, unsigned char x2, unsigned char y, unsigned char ancho2, unsigned char alto );

extern void radasPintarImagenEscalada( unsigned char invEscala, unsigned char x2, unsigned char y, Imagen *imagen );

#endif //MODO_RADASTAN

