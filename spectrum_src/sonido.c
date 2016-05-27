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


#include "sonido.h"


unsigned char sonidoInicialJetpac[] = {
  
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0xF7,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
    
};

unsigned char sonidoCogerObjeto[] = {
  
    0xE7,
    0x01,
    0x90,
    0x01,
    0x50,
    0x01,
    0x1F,
    0xF8,
    0x10,
    0x10,
    0x10,
    0x00,
    0x07,
    0x00
};

unsigned char sonidoSoltarObjeto[] = {
  
    0xE7,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x10,
    0x00,
    0x00,
    0x00,
    0x09,
    0x00
};

unsigned char sonidoDejarObjetoNave[] = {
  
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x10,
    0x00,
    0x00,
    0x00,
    0x09,
    0x00
};

unsigned char sonidoGolpe[] = {
  
    0x80,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x10,
    0x00,
    0x00,
    0x00,
    0x09,
    0x00
};

unsigned char sonidoBerbiqui[] = {
    
    0xFF,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x04,
    0xFE,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};

unsigned char sonidoMartillazo[] = {
  
    0x80,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFE,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};

unsigned char sonidoFinalNave[] = {
  
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF7,
    0x1F,
    0x00,
    0x00,
    0x00,
    0xA0,
    0x00
};

unsigned char *sonidos[] = {
    sonidoInicialJetpac,
    sonidoCogerObjeto,
    sonidoSoltarObjeto,
    sonidoDejarObjetoNave,
    sonidoGolpe,
    sonidoBerbiqui,
    sonidoMartillazo,
    sonidoFinalNave
};

extern void inicializarSonido() {
    
    // Se usa el chip por defecto para sonido del jetpac (solo ruido, canal A), y 
    // el segundo chip se usa para sonidos esporadicos.

    
    // Pone el sonido de jetpac con volumen a 0, listo para sonar.
    reproducirIndiceSonido( SONIDO_JETPAC, VALOR_CHIP_DEFECTO );

}

extern void setVolumenSonidoJetpac( unsigned char volumen ) {
    outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, REGISTRO_SONIDO_A_VOLUME );
    outp( PUERTO_SONIDO_DATO, volumen );
}

extern void reproducirIndiceSonido( unsigned char indiceSonido, unsigned char chip ) {
    
    // chip es VALOR_CHIP_DEFECTO o bien VALOR_CHIP_SEGUNDO
    
    unsigned char i;
    unsigned char *sonido = sonidos[ indiceSonido ];
    
    outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, chip );
    
    for ( i = 0; i < NUM_REGISTROS_SONIDO; i++ ) {
        outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, i );
        outp( PUERTO_SONIDO_DATO, sonido[ i ] );
    }

    outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, VALOR_CHIP_DEFECTO );
}

extern void reproducirSonido( unsigned char *sonido, unsigned char chip ) {

    unsigned char i;

    outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, chip );

    for ( i = 0; i < NUM_REGISTROS_SONIDO; i++ ) {
        outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, i );
        outp( PUERTO_SONIDO_DATO, sonido[ i ] );
    }

    outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, VALOR_CHIP_DEFECTO );

}

extern void apagarSonido( unsigned char chip ) {
    
    // chip es VALOR_CHIP_DEFECTO o bien VALOR_CHIP_SEGUNDO
    
    unsigned char i = 0;
    
    outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, chip );
    
    for ( i = 0; i < NUM_REGISTROS_SONIDO; i++ ) {
        outp( PUERTO_SONIDO_SELECCIONAR_CHIP_O_REGISTRO, i );
        outp( PUERTO_SONIDO_DATO, 0x00 );
    }
    
}

extern unsigned char *getSonido( unsigned char indiceSonido ) {

    return sonidos[ indiceSonido ];

}

