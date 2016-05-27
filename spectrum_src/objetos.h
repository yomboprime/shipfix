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


#ifndef __OBJETOS_H__
#define __OBJETOS_H__

// Numero de bits a desplazar una posicion en punto fijo
// En el eje x es uno mas porque se obtienen bytes, y hay dos pixels por byte
#define BITS_DECIMALES_X 7
#define BITS_DECIMALES_Y 6

typedef struct {
    
    // Punto fijo 16 bits con signo
    int x, y;
    int velx, vely;
    
    // Valores enteros (las ordenadas X son pares de pixels)
    int xant, yant;
    int xant2, yant2;

    // tamx2 es dividido por 2
    unsigned char tamx2, tamy;

    Imagen *imagen;

} Jugador;


// estado/flags de un objeto
#define ESTADO_OBJETO_ESTATICO 0
#define ESTADO_OBJETO_CAYENDO 1
#define ESTADO_OBJETO_COGIDO 2
#define ESTADO_OBJETO_EN_LA_NAVE 3
#define ESTADO_OBJETO_ROTO 4
#define FLAG_OBJETO_POR_BORRAR 128

#define PANTALLA_INVALIDA 255

typedef struct {

    unsigned char indice;
    
    unsigned char flagsEstado;
    
    // Necesario par arreglar la nave
    unsigned char necesario;

    // tamx2 es dividido por 2
    unsigned char tamx2, tamy;    
    
    Imagen *imagen;    

    // Indice de pantalla en la que esta el objeto
    unsigned char pantalla;

    // Coordenada entera X / 2 (pares de pixels) dentro de la pantalla
    unsigned char xEnt2;
    
    // Coordenada entera Y dentro de la pantalla
    unsigned char yEnt;
    
    // ** Variables para cuando esta cayendo

    // Posicion y velocidad en Y cuando esta cayendo (punto fijo)
    int y;
    int vely;

    // Posiciones anteriores enteras de pantalla en Y cuando el objeto esta cayendo
    int yant, yant2;    


} Objeto;

#endif /* __OBJETOS_H__ */

