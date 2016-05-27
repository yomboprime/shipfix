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

#include <stdio.h>
#include <spectrum.h>
#include <input.h>
#include <stdlib.h>
#include <time.h>

#include "modoRadastan.h"
#include "objetos.h"
#include "imagenes.h"
#include "mapa.h"
#include "utilsTexto.h"
#include "textos.h"
#include "sonido.h"

//#define DEBUG_CPU_JUEGO

// ****** Prototipos de funciones ******

void inicializarEstadoDelJuego();
void bucleJuego();

void resetKeyboard();
unsigned char leerJoystickYTeclado();
void esperarTecla();
#define getTecla() in_Inkey()
void redefinirTeclas();

unsigned char mostrarMenu( Menu *menu, unsigned char atributosGeneral, unsigned char atributosSeleccion, unsigned char opcionSel, unsigned char offsetY );
void mostrarInventario( unsigned char offsetY, unsigned char forzar );
unsigned char listarInventario( unsigned char offsetY, unsigned char forzar );
void mostrarTextoInicio();
void mostrarLlegadaNave();
void mostrarMuertePorColision();
void mostrarMuerteExtraviado();
void mostrarExito();
void mostrarAcercaDe();

void secuenciaPulsarTecla();
void secuenciaFinal();

void llevarObjetoAlSuelo( Objeto *o );
void integrarObjetoEnTerreno( Objeto *o );
void integrarObjetoEnTerrenoAdaptandose( Objeto *o );
void restauraAlturasObjetosEstaticos();
void computarObjetosEnPantalla();
void redibujarEscenario();
void pintarObjetosEstaticos();

void cogerSoltarObjeto();
Objeto *detectarObjetoProximoAlJugador();
Objeto *detectarObjetoProximoAlJugadorEnX();
unsigned char detectarObjetoProximoANave( Objeto *obj );
void computarObjetosNecesarios();
unsigned char comprobarExito();

void actualizarXAntObjetosCayendo();
void actualizarXAnt2ObjetosCayendo();

void debugYSalir();

// ****** Variables globales ******
// Casi todas las variables son globales porque el acceso es más rápido.

// Joystick y teclado
struct in_UDK mapaTeclado;
unsigned char teclaInventario;

// Variables de juego
int i;
int tecla;
unsigned char opcion;
unsigned char valorJoystick = 0;
unsigned char disparoPulsadoFrameAnterior;

//#ahorroMemoria comentar
unsigned char debugStr[ 64 ];
#define NUM_DEBUG 8
int debug[ NUM_DEBUG ];


#define MAX_OBJETOS 8

#define OBJETO_0 0
#define OBJETO_1 1
#define OBJETO_2 2
#define OBJETO_3 3
#define OBJETO_4 4
#define OBJETO_5 5
#define OBJETO_CAPSULA 6
#define OBJETO_NAVE 7

#define NUM_OBJETOS_DINAMICOS OBJETO_CAPSULA

#define OBJETO_NULO 255

#define NUM_OBJETOS_NECESARIOS 3

Objeto objetos[ MAX_OBJETOS ];

unsigned char *nombresObjetos[ MAX_OBJETOS ];

// Objetos en pantalla actual, hay uno menos porque la capsula y la nave
// no estan en la misma pantalla nunca.
Objeto *objetosPantallaActual[ MAX_OBJETOS - 1 ];
// Objetos actualmente en pantalla
unsigned char numObjetosEnPantallaActual;
// Idem, que ademas estan cayendo
unsigned char numObjetosCayendo;
unsigned char flagObjetoCaido;

unsigned char numObjetosNecesarios;
unsigned char numObjetosObtenidos;

unsigned char naveVisitada;

Jugador jugador;

#define MAX_VELOCIDAD_IMPACTO 120

#define COLOR_TERRENO 0x11

unsigned char numPantallasX;
unsigned char cambioDePantalla;
unsigned char posicionPantallaX;
unsigned char posicionPantallaY;
unsigned char posicionPantallaYAnterior;
unsigned int posicionAlturaPantallaActual;
unsigned char *punteroAlturaPantallaActual;

// Valores de salida de la funcion bucleJuego (CONTINUAR es para no salir)
#define BUCLE_JUEGO_CONTINUAR 0
#define BUCLE_JUEGO_MENU 1
#define BUCLE_JUEGO_LLEGADA_NAVE 2
#define BUCLE_JUEGO_MUERTE_COLISION 3
#define BUCLE_JUEGO_MUERTE_EXTRAVIADO 4
#define BUCLE_JUEGO_EXITO 5
#define BUCLE_JUEGO_SALIDA_USUARIO 6

#define BUCLE_JUEGO_MIN_VALOR_CONTINUAR BUCLE_JUEGO_LLEGADA_NAVE

unsigned char valorBucleJuego;

// Objeto que esta portando el jugador. OBJETO_NULO = ninguno
unsigned char objetoCogido;

// Direccion del jugador ( 1 = izquierda )
unsigned char direccionJugador;

// Fotogramas del jugador sin contar los espejados horizontalmente (se multiplican por 2):
#define NUM_FOTOGRAMAS_JUGADOR 1
unsigned char fotogramaActualJugador;
Imagen *fotogramasJugador[ NUM_FOTOGRAMAS_JUGADOR * 2 ];

#define IMAGEN_OBJETO_0 ( NUM_FOTOGRAMAS_JUGADOR * 2 + 1 )
#define IMAGEN_OBJETO_1 ( NUM_FOTOGRAMAS_JUGADOR * 2 + 2 )
#define IMAGEN_OBJETO_2 ( NUM_FOTOGRAMAS_JUGADOR * 2 + 3 )
#define IMAGEN_OBJETO_3 ( NUM_FOTOGRAMAS_JUGADOR * 2 + 4 )
#define IMAGEN_OBJETO_4 ( NUM_FOTOGRAMAS_JUGADOR * 2 + 5 )
#define IMAGEN_OBJETO_5 ( NUM_FOTOGRAMAS_JUGADOR * 2 + 6 )
#define IMAGEN_CAPSULA ( NUM_FOTOGRAMAS_JUGADOR * 2 + 0 )
#define IMAGEN_NAVE ( NUM_FOTOGRAMAS_JUGADOR * 2 + 7 )

#define IMAGEN_JUGADOR 0

#define IMAGEN_PULSA_TECLA_0 ( IMAGEN_NAVE + 1 )
#define IMAGEN_PULSA_TECLA_1 ( IMAGEN_NAVE + 2 )

int xNueva, yNueva;
int xEntera, yEntera;

int difH1, difH2;

#define pintarJugador() radasPintarImagen( ( unsigned char )xEntera, ( unsigned char )yEntera, fotogramasJugador[ fotogramaActualJugador ] );

// ****** Programa principal ******

void main( void ) {

    radasCambiarVariablesPantalla();
    
    resetKeyboard();

    secuenciaPulsarTecla();

    radasPonerModoNormal( PAPER_BLACK | INK_CYAN, PAPER_BLACK );

    opcion = 255;

    // Mientras no se seleccione la opcion salir
    while ( opcion != 3 ) {

        clsTexto();
        opcion = mostrarMenu( menuPrincipal, PAPER_BLACK | INK_CYAN, PAPER_BLUE | INK_WHITE, 0, 0 );
        
        switch ( opcion ) {
            case 0:
                
                inicializarEstadoDelJuego();
                
                mostrarTextoInicio();
                
                do {

                    bucleJuego();

                    switch ( valorBucleJuego ) {
                        case BUCLE_JUEGO_MENU:
                            mostrarInventario( 0, 0 );
                            break;
                        case BUCLE_JUEGO_LLEGADA_NAVE:
                            mostrarLlegadaNave();
                            break;
                        case BUCLE_JUEGO_MUERTE_COLISION:
                            mostrarMuertePorColision();
                            break;
                        case BUCLE_JUEGO_MUERTE_EXTRAVIADO:
                            mostrarMuerteExtraviado();
                            break;
                        case BUCLE_JUEGO_EXITO:
                            secuenciaFinal();
                            break;
                    }

                } while ( valorBucleJuego <= BUCLE_JUEGO_MIN_VALOR_CONTINUAR );

                restauraAlturasObjetosEstaticos();

                break;
            case 1:
                // Redefinicion de teclas
                redefinirTeclas();
                break;
            case 2:
                // Acerca de
                mostrarAcercaDe();
                break;
            case 3:
                
                // Salir (confirmacion)
                clsTexto();
                opcion = mostrarMenu( menuConfirmarFinJuego, PAPER_BLACK | INK_CYAN, PAPER_BLUE | INK_WHITE, 0, 0 );
                if ( opcion == 1 ) {
                    opcion = 3;
                }
                else {
                    opcion = 255;
                }

                break;
        }

    }    

    // Fin del programa
    clsTexto();
    zx_border( INK_WHITE );
    zx_colour( PAPER_WHITE | INK_BLACK );
    imprimir( mensajeSalida );
    
}

// ****** Funciones ******

void inicializarEstadoDelJuego() {
    
    unsigned char x;
    
    // Estado inicial del juego
    
    srand( time(NULL) );
   
    numPantallasX = getNumPantallasX();
    posicionPantallaX = 0;
    posicionPantallaY = 0;
    numObjetosCayendo = 0;
    numObjetosEnPantallaActual = 0;
    posicionAlturaPantallaActual = 0;
    punteroAlturaPantallaActual = getMapaAlturas();
    objetoCogido = OBJETO_NULO;
    direccionJugador = 0;
    fotogramaActualJugador = 0;

    jugador.x = 16 << BITS_DECIMALES_X;
    jugador.y = 74 << BITS_DECIMALES_Y;
    jugador.tamx2 = imagenes[ IMAGEN_JUGADOR ].tamx2;
    jugador.tamy = imagenes[ IMAGEN_JUGADOR ].tamy;
    jugador.velx = 0;
    jugador.vely = 0;
    //jugador.imagen = &imagenes[ IMAGEN_JUGADOR ];

    for ( i = 0; i < NUM_FOTOGRAMAS_JUGADOR << 1; i++ ) {
        fotogramasJugador[ i ] = &imagenes[ IMAGEN_JUGADOR + i ];
    }

    jugador.xant = jugador.x >> BITS_DECIMALES_X;
    jugador.yant = jugador.y >> BITS_DECIMALES_Y;
    jugador.xant2 = jugador.xant;
    jugador.yant2 = jugador.yant;
    xNueva = jugador.x;
    yNueva = jugador.y;
    xEntera = xNueva >> BITS_DECIMALES_X;
    yEntera = yNueva >> BITS_DECIMALES_Y;

    for ( i = 0; i < NUM_OBJETOS_DINAMICOS; i++ ) {
        objetos[ i ].imagen = &imagenes[ IMAGEN_OBJETO_0 + i ];
        x = 0;
        while ( x == 0 || x == 15 ) {
            x = rand() & 31;
        }
        objetos[ i ].pantalla = x;
        x = 0;
        while ( x <= 2 || x >= PANTALLA_X2 - 3 ) {
            x = rand() & ( PANTALLA_X2 - 1);
        }
        objetos[ i ].xEnt2 = x;
        objetos[ i ].necesario = 0;
    }

    objetos[ OBJETO_CAPSULA ].flagsEstado = 0;
    objetos[ OBJETO_CAPSULA ].imagen = &imagenes[ IMAGEN_CAPSULA ];
    objetos[ OBJETO_CAPSULA ].pantalla = 0;
    objetos[ OBJETO_CAPSULA ].xEnt2 = 10;
    objetos[ OBJETO_CAPSULA ].yEnt = 75;

    objetos[ OBJETO_NAVE ].flagsEstado = 0;
    objetos[ OBJETO_NAVE ].imagen = &imagenes[ IMAGEN_NAVE ];
    objetos[ OBJETO_NAVE ].pantalla = 15;
    objetos[ OBJETO_NAVE ].xEnt2 = 20;
    objetos[ OBJETO_NAVE ].yEnt = PANTALLA_Y - ALTURA_TERRENO + 3;

    for ( i = 0; i <= MAX_OBJETOS; i++ ) {
        // Variables comunes a todos los objetos
        Objeto *o = &objetos[ i ];
        o->indice = i;
        o->tamx2 = o->imagen->tamx2;
        o->tamy = o->imagen->tamy;
        
        o->necesario = 0;
    }

    
    // Pone 3 objetos como necesarios
    for ( i = 0; i < NUM_OBJETOS_NECESARIOS; i++ ) {
        Objeto *o = NULL;
        while ( o == NULL || o->necesario ) {
            x = 8;
            while ( x >= NUM_OBJETOS_DINAMICOS ) {
                x = rand() & MAX_OBJETOS - 1;
            }

            o = &objetos[ x ];
        }
        o->necesario = 1;
    }

    
    integrarObjetoEnTerreno( &objetos[ OBJETO_CAPSULA ] );
    integrarObjetoEnTerrenoAdaptandose( &objetos[ OBJETO_NAVE ] );

    for ( i = 0; i < NUM_OBJETOS_DINAMICOS; i++ ) {

        // Pone los objetos moviles en el suelo
        
        Objeto *o = &objetos[ i ];

        o->flagsEstado = ESTADO_OBJETO_CAYENDO;
        o->y = 10;
        o->vely = 0;

        o->yEnt = o->y >> BITS_DECIMALES_Y;
        
        o->yant = o->yEnt;
        o->yant2 = o->yEnt;
        
        llevarObjetoAlSuelo( &objetos[ i ] );
    }

    // Asigna nombres de objetos al array nombresObjetos
    for ( i = 0; i < MAX_OBJETOS; i++ ) {
        // + 1 para saltar espacio inicial
        nombresObjetos[ i ] = menuNombresObjetos.opciones[ i ].texto + 1;
    }

    naveVisitada = 0;
}

void resetKeyboard() {
    mapaTeclado.fire = in_LookupKey( ' ' );
    mapaTeclado.left = in_LookupKey( 'o' );
    mapaTeclado.right = in_LookupKey( 'p' );
    mapaTeclado.down = in_LookupKey( 'a' );
    mapaTeclado.up = in_LookupKey( 'q' );
    teclaInventario = 'm';
}

unsigned char leerJoystickYTeclado() {
    return ((void*)in_JoyKeyboard)( & mapaTeclado ) | ((void*)in_JoySinclair1)( & mapaTeclado );
}

void esperarTecla() {
    while ( getTecla() || leerJoystickYTeclado() );
    while ( ! ( getTecla() || leerJoystickYTeclado() ) );
}

void redefinirTeclas() {
    
    unsigned char i, j, tecla, teclaOK;
    unsigned int codigo, codigo2;
    
    Menu *menu = &menuRedefinir;
    
    clsTexto();

    if ( menu->mensaje != NULL ) {
        imprimir( menu->mensaje );
    }
    
    for ( i = 0; i < menu->numOpciones; i++ ) {

        OpcionMenu *opcion = &menu->opciones[ i ];
        imprimir( opcion->texto );
        
        if ( i > 0 ) {
            pintarSegmentoConAtributos( menu->y + i - 1, menu->opciones[ i - 1 ].x0, menu->opciones[ i - 1 ].x1 - 1, PAPER_BLACK | INK_CYAN );
        }
        pintarSegmentoConAtributos( menu->y + i, opcion->x0, opcion->x1, PAPER_BLUE | INK_WHITE );

        while ( getTecla() );

        teclaOK = 0;        
        while ( ! teclaOK ) {
            tecla = 0;
            while ( ! tecla ) {
                tecla = getTecla();
            }
            while ( getTecla() );

            codigo = in_LookupKey( tecla );
            teclaOK = 1;
            for ( j = 0; j < i; j++ ) {
                switch ( j ) {
                    case 0:
                        codigo2 = mapaTeclado.left;
                        break;
                    case 1:
                        codigo2 = mapaTeclado.right;
                        break;
                    case 2:
                        codigo2 = mapaTeclado.up;
                        break;
                    case 3:
                        codigo2 = mapaTeclado.down;
                        break;
                    case 4:
                        codigo2 = mapaTeclado.fire;
                        break;
                    case 5:
                        codigo2 = in_LookupKey( teclaInventario );
                        break;
                }
                if ( codigo == codigo2 ) {
                   teclaOK = 0;
                   // Sonidillo de tecla invalida
                   reproducirIndiceSonido( SONIDO_GOLPE, VALOR_CHIP_SEGUNDO );
                   break;
                }
            }
        }
        
        switch ( i ) {
            case 0:
                mapaTeclado.left = codigo;
                break;
            case 1:
                mapaTeclado.right = codigo;
                break;
            case 2:
                mapaTeclado.up = codigo;
                break;
            case 3:
                mapaTeclado.down = codigo;
                break;
            case 4:
                mapaTeclado.fire = codigo;
                break;
            case 5:
                teclaInventario = tecla;
        }

        if ( tecla == ' ' ) {
            fputc( '_', stdout );
        }
        else {
            fputc( tecla, stdout );
        }

        pintarSegmentoConAtributos( menu->y + i, opcion->x1, opcion->x1 + 1, PAPER_BLACK | INK_YELLOW );

        imprimir( "\n" );
        
        // Sonidillo de tecla pulsada
        reproducirIndiceSonido( SONIDO_DEJAR_OBJETO_NAVE, VALOR_CHIP_SEGUNDO );
    }

}

unsigned char mostrarMenu( Menu *menu, unsigned char atributosGeneral, unsigned char atributosSeleccion, unsigned char opcionSel, unsigned char offsetY ) {

    // Muestra un menu en modo texto. Es necesario borrar la pantalla previamente, o mostrar los textos que se quiera
    // Se devuelve la opcion seleccionada.

    unsigned char i, iAnt;
    unsigned char valorAntJoystick;

    if ( menu->mensaje != NULL ) {
        imprimir( menu->mensaje );
    }

    for ( i = 0; i < menu->numOpciones; i++ ) {
        OpcionMenu *opcion = &menu->opciones[ i ];
        imprimir( opcion->texto );
        imprimir( "\n" );
    }

    i = opcionSel;
    valorAntJoystick = 0;
    valorJoystick = in_FIRE;
    while ( valorJoystick & in_FIRE ) {
        valorJoystick = leerJoystickYTeclado();
    }

    pintarSegmentoConAtributos( menu->y + offsetY + i, menu->opciones[ i ].x0, menu->opciones[ i ].x1, atributosSeleccion );

    while ( ! ( valorJoystick & in_FIRE ) ) {

        valorJoystick = leerJoystickYTeclado();

        iAnt = i;
        
        if ( ( valorJoystick & in_UP ) && ! ( valorAntJoystick & in_UP ) ) {
            if ( i > 0 ) {
                i--;
            }
            else {
                i = menu->numOpciones - 1;
            }
        }

        if ( ( valorJoystick & in_DOWN ) && ! ( valorAntJoystick & in_DOWN ) ) {
            if ( i < menu->numOpciones - 1 ) {
                i++;
            }
            else {
                i = 0;
            }
        }
        
        if ( iAnt != i ) {

            reproducirIndiceSonido( SONIDO_SOLTAR_OBJETO, VALOR_CHIP_SEGUNDO );

            pintarSegmentoConAtributos( menu->y + offsetY + iAnt, menu->opciones[ iAnt ].x0, menu->opciones[ iAnt ].x1, atributosGeneral );
            pintarSegmentoConAtributos( menu->y + offsetY + i, menu->opciones[ i ].x0, menu->opciones[ i ].x1, atributosSeleccion );
        }

        valorAntJoystick = valorJoystick;

    }

    return i;
}

void mostrarInventario( unsigned char offsetY, unsigned char forzar ) {

    offsetY = listarInventario( offsetY, forzar );
    
    // Menu continuar / salir de la partida
    
    opcion = mostrarMenu( menuInventario, PAPER_BLACK | INK_CYAN, PAPER_BLUE | INK_WHITE, 0, offsetY );
    if ( opcion == 1 ) {
        clsTexto();
        opcion = mostrarMenu( menuConfirmarFinPartida, PAPER_BLACK | INK_CYAN, PAPER_BLUE | INK_WHITE, 0, 0 );
        if ( opcion == 1 ) {
            valorBucleJuego = BUCLE_JUEGO_SALIDA_USUARIO;
        }
    }
    opcion = 255;

}

unsigned char listarInventario( unsigned char offsetY, unsigned char forzar ) {

    if ( offsetY == 0 ) {
        clsTexto();
    }
    
    if ( naveVisitada || forzar ) {

        // Objetos necesarios

        imprimir( mensajeInventarioEquipo );
        offsetY+= 4;

        for ( i = 0; i < NUM_OBJETOS_DINAMICOS; i++ ) {
            Objeto *o = &objetos[ i ];
            if ( o->necesario ) {
                imprimir( nombresObjetos[ i ] );
                imprimir( "\n" );
                offsetY++;
            }
        }
    }
    else {
        imprimir( mensajeAunNoLlegadoNave );
        offsetY += 3;
    }

    if ( ! forzar ) {
        if ( naveVisitada ) {

            // Objetos obtenidos

            computarObjetosNecesarios();

            if ( numObjetosObtenidos == 0 ) {
                imprimir( mensajeInventarioNaveVacio );
                offsetY+= 3;
            }
            else {
                imprimir( mensajeInventarioNave );
                offsetY+= 3;

                for ( i = 0; i < NUM_OBJETOS_DINAMICOS; i++ ) {
                    Objeto *o = &objetos[ i ];
                    if ( o->necesario && o->flagsEstado == ESTADO_OBJETO_EN_LA_NAVE ) {
                        imprimir( nombresObjetos[ i ] );
                        imprimir( "\n" );
                        offsetY++;
                    }
                }
            }
        }

        // Objeto que lleva el jugador

        if ( objetoCogido == OBJETO_NULO ) {
            imprimir( mensajeInventarioVacio );
            imprimir( "\n" );
            offsetY += 3;
        }
        else {
            imprimir( mensajeInventarioActual );
            imprimir( nombresObjetos[ objetoCogido ] );
            imprimir( "\n" );
            offsetY += 3 + 1;
        }
    }

    return offsetY;
    
}

void mostrarTextoInicio() {

    clsTexto();

    imprimir( mensajeInicio );
    imprimir( mensajePulsaUnaTecla );

    esperarTecla();

}

void mostrarLlegadaNave() {

    clsTexto();

    imprimir( mensajeLlegarNave );

    esperarTecla();

    clsTexto();

    imprimir( mensajeEquipoNecesario );
    
    listarInventario( 3, 1 );
    
    imprimir( "\n" );
    
    imprimir( mensajePulsaUnaTecla );
    
    esperarTecla();

    jugador.velx = 0;
}

void mostrarMuertePorColision() {
    
    clsTexto();
    
    imprimir( mensajeMuertePorColision );
    
    imprimir( mensajePulsaUnaTecla );
    
    esperarTecla();
    
}

void mostrarMuerteExtraviado() {
    
    clsTexto();
    
    imprimir( mensajeMuerteExtraviado );
    
    imprimir( mensajePulsaUnaTecla );
    
    esperarTecla();
    
}

void mostrarExito() {

    clsTexto();
    
    imprimir( mensajeExito );
    
    imprimir( mensajePulsaUnaTecla );
    
    esperarTecla();
    
}

void mostrarAcercaDe() {
    
    clsTexto();
    
    imprimir( mensajeAcercaDe );

    esperarTecla();

}

void secuenciaPulsarTecla() {

    // El estado de la pantalla en este punto es el que dejó el programa de pantalla de carga.
    
    // Pinta las imagenes de los dedos pulsando una tecla en las dos pantallas y las va alternando.
    
    unsigned int t;

    radasCopiarPantalla( radasObtenerLaOtraPantalla(), radasObtenerPantallaActual() );
    radasPintarImagen( 53, 59, &imagenes[ IMAGEN_PULSA_TECLA_0 ] );
    ula_sync();
    radasCambiarPantalla();
    radasPintarImagen( 53, 59, &imagenes[ IMAGEN_PULSA_TECLA_1 ] );
    ula_sync();
    radasCambiarPantalla();

    while ( getTecla() || leerJoystickYTeclado() );
    t = 0;
    while ( ! ( getTecla() || leerJoystickYTeclado() ) ) {
        t++;
        if ( t > 2000 ) {
            t = 0;
            ula_sync();
            radasCambiarPantalla();
        }
    };

    // TODO fundido de la paleta al negro

    if ( obtenerIndicePantalla() != 0 ) {
        ula_sync();
        radasCambiarPantalla();
    }
}

void secuenciaFinal() {

    // Secuencia de despegue de la nave
    
    unsigned char i, j, k, l;
    int n;
    
    static unsigned char sonidoTempBerbiqui[ 16 ];
    static unsigned char sonidoTempMartillo[ 16 ];
    static unsigned char sonidoTemp[ 16 ];
    
    unsigned char *sonidoBerbiqui = getSonido( SONIDO_BERBIQUI );
    unsigned char *sonidoMartillazo = getSonido( SONIDO_MARTILLAZO );
    
    unsigned char cambiarPant;
    unsigned char numPixelsBorrarPant;
    
    Objeto *nave = &objetos[ OBJETO_NAVE ];

    for ( i = 0; i < NUM_REGISTROS_SONIDO; i++ ) {    
        sonidoTempBerbiqui[ i ] = sonidoBerbiqui[ i ];
    }
    for ( i = 0; i < NUM_REGISTROS_SONIDO; i++ ) {
        sonidoTempMartillo[ i ] = sonidoMartillazo[ i ];
    }
    for ( i = 0; i < NUM_REGISTROS_SONIDO; i++ ) {
        sonidoTemp[ i ] = sonidoMartillazo[ i ];
    }

    posicionPantallaX = nave->pantalla;
    posicionPantallaY = 0;
    posicionAlturaPantallaActual = ((unsigned int)posicionPantallaX) << BITS_ANCHO_PANTALLA;

    inicializarSonido();
    radasPonerModoRadastan( PAPER_BLACK, PAPER_WHITE | INK_BLACK, &paletas[ 0 ] );
    redibujarEscenario();


    // Martillazos
    csleep( 200 );
    for ( i = 0; i < 5; i++ ) {
        sonidoTempMartillo[ REGISTRO_SONIDO_A_VOLUME ] = 0x0F;
        reproducirSonido( sonidoTempMartillo, VALOR_CHIP_DEFECTO );
        csleep( 20 );
        sonidoTempMartillo[ REGISTRO_SONIDO_A_VOLUME ] = 0;
        reproducirSonido( sonidoTempMartillo, VALOR_CHIP_DEFECTO );
        csleep( 50 );
    }
    csleep( 120 );

    // Berbiqui
    reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
    csleep( 70 );

    // Berbiqui ascendiendo volumen
    for ( i = 3; i >= 1; i-- ) {
        sonidoTempBerbiqui[ REGISTRO_SONIDO_A_COARSE_PITCH ] = i;
        for ( n = 255; n >= 0; n-= 10 ) {
            sonidoTempBerbiqui[ REGISTRO_SONIDO_A_FINE_PITCH ] = n;
            reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
            ula_sync();
        }
    }
    csleep( 120 );
    
    // Cuatro agujeros
    for ( j = 0; j < 4; j++ ) {
        
        sonidoTempBerbiqui[ REGISTRO_SONIDO_MIXER ] = 0xF6;
        reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
        csleep( 70 );
        
        sonidoTempBerbiqui[ REGISTRO_SONIDO_MIXER ] = 0xFE;
        reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
        csleep( 60 );
    }
    csleep( 100 );

    // Berbiqui descendiendo volumen
    for ( i = 1; i <= 3; i++ ) {
        sonidoTempBerbiqui[ REGISTRO_SONIDO_A_COARSE_PITCH ] = i;
        for ( n = 0; n <= 255; n+= 10 ) {
            sonidoTempBerbiqui[ REGISTRO_SONIDO_A_FINE_PITCH ] = n;
            reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
            ula_sync();
        }
    }
    sonidoTempBerbiqui[ REGISTRO_SONIDO_A_VOLUME ] = 0;
    reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
    csleep( 100 );

    
    for ( i = 0; i < 4; i++ ) {
        sonidoTempMartillo[ REGISTRO_SONIDO_A_VOLUME ] = 0x0F;
        reproducirSonido( sonidoTempMartillo, VALOR_CHIP_DEFECTO );
        csleep( 20 );
        sonidoTempMartillo[ REGISTRO_SONIDO_A_VOLUME ] = 0;
        reproducirSonido( sonidoTempMartillo, VALOR_CHIP_DEFECTO );
        csleep( 50 );
    }
    csleep( 70 );

    sonidoTempBerbiqui[ REGISTRO_SONIDO_A_VOLUME ] = 0x0F;
    for ( i = 3; i >= 2; i-- ) {
        sonidoTempBerbiqui[ REGISTRO_SONIDO_A_COARSE_PITCH ] = i;
        for ( n = 255; n >= 0; n-= 10 ) {
            sonidoTempBerbiqui[ REGISTRO_SONIDO_A_FINE_PITCH ] = n;
            reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
            ula_sync();
        }
    }
    for ( i = 2; i <= 3; i++ ) {
        sonidoTempBerbiqui[ REGISTRO_SONIDO_A_COARSE_PITCH ] = i;
        for ( n = 0; n <= 255; n+= 10 ) {
            sonidoTempBerbiqui[ REGISTRO_SONIDO_A_FINE_PITCH ] = n;
            reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
            ula_sync();
        }
    }
    sonidoTempBerbiqui[ REGISTRO_SONIDO_A_VOLUME ] = 0;
    reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
    csleep( 120 );

    for ( i = 0; i < 4; i++ ) {
        sonidoTempMartillo[ REGISTRO_SONIDO_A_VOLUME ] = 0x0F;
        reproducirSonido( sonidoTempMartillo, VALOR_CHIP_DEFECTO );
        csleep( 20 );
        sonidoTempMartillo[ REGISTRO_SONIDO_A_VOLUME ] = 0;
        reproducirSonido( sonidoTempMartillo, VALOR_CHIP_DEFECTO );
        csleep( 50 );
    }
    csleep( 100 );

    // Pitidos aleatorios
    for ( i = 0; i < 35; i++ ) {
        sonidoTemp[ REGISTRO_SONIDO_A_COARSE_PITCH ] = ( rand() & 3 ) + 1;
        sonidoTemp[ REGISTRO_SONIDO_A_FINE_PITCH ] = rand() & 0xFF;
        reproducirSonido( sonidoTemp, VALOR_CHIP_SEGUNDO );
        ula_sync();
        ula_sync();
        ula_sync();
        ula_sync();
        ula_sync();
    }
    sonidoTemp[ REGISTRO_SONIDO_A_VOLUME ] = 0;
    reproducirSonido( sonidoTemp, VALOR_CHIP_SEGUNDO );

    csleep( 200 );

    // Sonido nave ascendiendo volumen
    numPixelsBorrarPant = 1;
    sonidoTempBerbiqui[ REGISTRO_SONIDO_A_VOLUME ] = 0x0A;
    for ( i = 3; i >= 1; i-- ) {
        sonidoTempBerbiqui[ REGISTRO_SONIDO_A_COARSE_PITCH ] = i;
        for ( n = 255; n >= 0; n-= 1 ) {
            sonidoTempBerbiqui[ REGISTRO_SONIDO_A_FINE_PITCH ] = n;
            reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );

            cambiarPant = 0;
            if ( ( n & 0x1F ) == 0x10 ) {
                // Mueve nave hacia arriba
                nave->yEnt --;
                radasPintarImagen( nave->xEnt2, nave->yEnt, nave->imagen );
                if ( numPixelsBorrarPant > 0 ) {
                    radasBorrarRectangulo( nave->xEnt2, nave->yEnt + nave->tamy, nave->tamx2, numPixelsBorrarPant );
                }
                cambiarPant = 1;
                if ( numPixelsBorrarPant < 2 ) {
                    numPixelsBorrarPant++;
                }
            }
            
            ula_sync();
            if ( cambiarPant != 0 ) {
                radasCambiarPantalla();
            }
        }
    }
    csleep( 80 );

    
    // Sonido tren aterrizaje
    sonidoTempBerbiqui[ REGISTRO_SONIDO_B_VOLUME ] = 0x0B;
    sonidoTempBerbiqui[ REGISTRO_SONIDO_B_COARSE_PITCH ] = 0x05;
    sonidoTempBerbiqui[ REGISTRO_SONIDO_B_FINE_PITCH ] = 0x30;
    sonidoTempBerbiqui[ REGISTRO_SONIDO_MIXER ] = 0xFC;
    reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );

    // Sube el tren de aterrizaje
    for ( i = 0; i < 6; i++ ) {
        ula_sync();
        radasScrollRectanguloArriba( radasObtenerLaOtraPantalla(), nave->xEnt2, nave->yEnt + nave->tamy - 6, nave->tamx2, 6 );
        csleep( 45 );
    }
    
    // Fin sonido tren aterrizaje
    sonidoTempBerbiqui[ REGISTRO_SONIDO_B_VOLUME ] = 0x00;
    sonidoTempBerbiqui[ REGISTRO_SONIDO_MIXER ] = 0xFE;
    reproducirSonido( sonidoTempBerbiqui, VALOR_CHIP_SEGUNDO );
    
    csleep( 100 );

    // Sonido de escape de la nave
    reproducirIndiceSonido( SONIDO_FINAL_NAVE, VALOR_CHIP_SEGUNDO );
    
    // La nave se hace pequeñita
    k = nave->xEnt2;
    l = nave->yEnt;
    nave->xEnt2 += 10;
    for ( i = 2; i < 32; i++ ) {
        if ( i < 14 ) {
            nave->xEnt2 ++;
            if ( i & 1 ) {
                nave->yEnt ++;
            }
        }
        radasBorrarRectangulo( k, l, nave->tamx2, nave->tamy + 1 );
        radasPintarImagenEscalada( i, nave->xEnt2, nave->yEnt, nave->imagen );
        ula_sync();
        radasCambiarPantalla();
        csleep( 34 - i );
    }
    
    // Hace la estrella
    for ( i = 1; i <= 6; i++ ) {
        radasBorrarRectangulo( k, l, nave->tamx2, nave->tamy + 1 );
        radasPintar2Pixels( nave->xEnt2, nave->yEnt, 0x88 );
        for ( j = 1; j <= i; j++ ) {
            radasPintar2Pixels( nave->xEnt2, nave->yEnt - j, 0x80 );
            radasPintar2Pixels( nave->xEnt2, nave->yEnt + j, 0x80 );
        }
        for ( j = 1; j <= i >> 1; j++ ) {
            radasPintar2Pixels( nave->xEnt2 + j, nave->yEnt, 0x88 );
            radasPintar2Pixels( nave->xEnt2 - j, nave->yEnt, 0x88 );
        }
        ula_sync();
        radasCambiarPantalla();
        csleep( 2 );
    }
    for ( i = 6; i >= 2; i-- ) {
        radasBorrarRectangulo( k, l, nave->tamx2, nave->tamy + 1 );
        radasPintar2Pixels( nave->xEnt2, nave->yEnt, 0x88 );
        for ( j = 1; j <= i; j++ ) {
            radasPintar2Pixels( nave->xEnt2, nave->yEnt - j, 0x80 );
            radasPintar2Pixels( nave->xEnt2, nave->yEnt + j, 0x80 );
        }
        for ( j = 1; j <= i >> 1; j++ ) {
            radasPintar2Pixels( nave->xEnt2 + j, nave->yEnt, 0x88 );
            radasPintar2Pixels( nave->xEnt2 - j, nave->yEnt, 0x88 );
        }
        ula_sync();
        radasCambiarPantalla();
        csleep( 2 );
    }

    csleep( 220 );
    
    radasCls( radasObtenerPantallaActual(), 0x00 );
    radasCambiarPantalla();
    
    radasPonerModoNormal( PAPER_BLACK | INK_CYAN, PAPER_BLACK );

    mostrarExito();
    
}

void llevarObjetoAlSuelo( Objeto *o ) {

    unsigned char *alturas1;
    unsigned char i, sx, sy, x, y, yMin;
    
    if ( o->pantalla == objetos[ OBJETO_NAVE ].pantalla ) {
        // Se deja el objeto en la nave
        o->flagsEstado = ESTADO_OBJETO_EN_LA_NAVE;
        comprobarExito();
        return;
    }

    alturas1 = getMapaAlturas();

    alturas1 += ((unsigned int) o->pantalla ) << BITS_ANCHO_PANTALLA;
    
    x = o->xEnt2;
    sx = o->tamx2;
    sy = o->tamy;
    yMin = PANTALLA_Y;
    for ( i = 0; i < sx; i++ ) {
        y = PANTALLA_Y - alturas1[ x++ ] - 1 - sy;
        if ( y < yMin ) {
            yMin = y;
        }
    }
    
    if ( yMin < PANTALLA_Y - ALTURA_TERRENO ) {
        yMin = PANTALLA_Y - ALTURA_TERRENO;
    }
    
    o->yEnt = yMin;
    o->flagsEstado = ESTADO_OBJETO_ESTATICO;    

    integrarObjetoEnTerreno( o );
}

void integrarObjetoEnTerreno( Objeto *o ) {

    // Modifica la altura del terreno para cubrir el objeto, y aprovechar
    // la colision del terreno.
    // Se hace de forma simple, con el rectangulo del objeto

    unsigned char *alturas1;
    unsigned char i, l, x, h;
    
    h = PANTALLA_Y - o->yEnt - 1;

    if ( o->flagsEstado || h > ALTURA_TERRENO ) {
        // Error
        return;
    }

    alturas1 = getMapaAlturas();

    alturas1 += ((unsigned int) o->pantalla ) << BITS_ANCHO_PANTALLA;

    x = o->xEnt2;
    l = o->tamx2;
    for ( i = 0; i < l; i++ ) {
        if ( alturas1[ x ] < h ) {
            alturas1[ x ] = h;
        }
        x++;
    }

}

void integrarObjetoEnTerrenoAdaptandose( Objeto *o ) {

    // Modifica la altura del terreno para cubrir el objeto, y aprovechar
    // la colision del terreno.
    // Se hace adpatando el terreno al borde superior del objeto

    unsigned char *alturas1;
    unsigned char *pixels = o->imagen->pixels;
    unsigned char *pixelsi, *pixels1;
    
    unsigned char i, j, l, x, h;

    if ( o->flagsEstado ) {
        // Error
        return;
    }

    alturas1 = getMapaAlturas();

    alturas1 += ((unsigned int) o->pantalla ) << BITS_ANCHO_PANTALLA;

    x = o->xEnt2;
    l = o->tamx2;
    pixelsi = pixels;
    for ( i = 0; i < l; i++ ) {

        pixels1 = pixelsi;
        for ( j = 0; j < o->tamy; j++ ) {
            if ( *pixels1 ) {
                h = PANTALLA_Y - o->yEnt - j - 1;
                if ( alturas1[ x ] < h ) {
                    alturas1[ x ] = h;
                }
                break;
            }
            pixels1 += o->tamx2;
        }
        x++;
        pixelsi++;
    }

}

void restauraAlturasObjetosEstaticos() {
    for ( i = 0; i < NUM_OBJETOS_DINAMICOS; i++ ) {
        unsigned char j;
        Objeto *o = &objetos[ i ];
        if ( ! o->flagsEstado || ( o->flagsEstado & FLAG_OBJETO_POR_BORRAR ) ) {
            unsigned char *p = getMapaAlturas() + ( ((unsigned int) o->pantalla ) << BITS_ANCHO_PANTALLA );
            for ( j = 0; j < o->tamx2; j++ ) {
                p[ o->xEnt2 + j ] -= o->tamy;
            }
        }
    }
}

void computarObjetosEnPantalla() {

    unsigned int i;
    
    numObjetosEnPantallaActual = 0;

    for ( i = 0; i < MAX_OBJETOS; i++ ) {
        Objeto *o = &objetos[ i ];
        if ( posicionPantallaX == o->pantalla ) {
            // El objeto esta en la pantalla actual
            objetosPantallaActual[ numObjetosEnPantallaActual++ ] = o;
        }
    }

}

void redibujarEscenario() {

    computarObjetosEnPantalla();

    if ( posicionPantallaY == 0 ) {

        pintarMapaColorPlano( radasObtenerPantallaActual(), posicionAlturaPantallaActual, COLOR_TERRENO );

        pintarObjetosEstaticos();

        ula_sync();
        radasCambiarPantalla();

        pintarMapaColorPlano( radasObtenerPantallaActual(), posicionAlturaPantallaActual, COLOR_TERRENO );

        pintarObjetosEstaticos();

    }

}

void pintarObjetosEstaticos() {
    
    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
        Objeto *o = objetosPantallaActual[ i ];
        if ( ! o->flagsEstado ) {
            radasPintarImagen( o->xEnt2, o->yEnt, o->imagen );
        }
    }

}

void cogerSoltarObjeto() {
    
    Objeto *o;

    if ( objetoCogido == OBJETO_NULO ) {
        
        // El jugador no porta objeto. Detectar objeto para cogerlo.
        o = detectarObjetoProximoAlJugador();
        if ( o ) {
            if ( o->indice >= NUM_OBJETOS_DINAMICOS ) {
                if ( o->indice == OBJETO_NAVE ) {
                    if ( ! naveVisitada ) {
                        valorBucleJuego = BUCLE_JUEGO_LLEGADA_NAVE;
                        naveVisitada = 1;
                    }
                    comprobarExito();
                }
            }
            else {

                // Se ha cogido un objeto normal

                objetoCogido = o->indice;
                o->flagsEstado = ESTADO_OBJETO_COGIDO;

                // Reproduce sonido objeto cogido
                reproducirIndiceSonido( SONIDO_COGER_OBJETO, VALOR_CHIP_SEGUNDO );

                pintarJugador()
                radasBorrarRectangulo( o->xEnt2, o->yEnt, o->tamx2, o->tamy );
                ula_sync();
                radasCambiarPantalla();
                radasBorrarRectangulo( o->xEnt2, o->yEnt, o->tamx2, o->tamy );
                // Borra el jugador en ant
                radasBorrarRectangulo( ( unsigned char )( jugador.xant ),
                                       ( unsigned char )( jugador.yant ),
                                       jugador.tamx2, jugador.tamy );

                // Borra objetos cayendo en ant
                if ( numObjetosCayendo ) {
                    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                        Objeto *o2 = objetosPantallaActual[ i ];
                        if ( o2->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
                            radasBorrarRectangulo( o2->xEnt2, o2->yant2, o2->tamx2, o2->tamy );
                        }
                    }
                }
                // Actualiza alturas
                for ( i = 0; i < o->tamx2; i++ ) {
                    punteroAlturaPantallaActual[ o->xEnt2 + i ] -= o->tamy;
                }

                // Actualiza posiciones anteriores (2) al haber cambiado de pantalla
                jugador.xant2 = xEntera;
                jugador.yant2 = yEntera;

                if ( numObjetosCayendo ) {
                   actualizarXAnt2ObjetosCayendo();
                }
            }
        }
    }
    else {
        
        // El jugador porta un objeto. La accion es dejarlo
        
        // Si estamos en el borde de la pantalla, no podemos soltarlo
        if ( xEntera < 2 || xEntera >= PANTALLA_X2 -3 ) {
            // Sonidillo de que no se puede soltar el objeto
            reproducirIndiceSonido( SONIDO_GOLPE, VALOR_CHIP_SEGUNDO );
            return;
        }

        // Detecta objeto proximo
        o = detectarObjetoProximoAlJugador();
        if ( o ) {
            // Detecta si esta dejando el objeto en la nave
            if ( o->indice == OBJETO_NAVE ) {
                o = &objetos[ objetoCogido ];
                o->flagsEstado = ESTADO_OBJETO_EN_LA_NAVE;
                objetoCogido = OBJETO_NULO;

                reproducirIndiceSonido( SONIDO_DEJAR_OBJETO_NAVE, VALOR_CHIP_SEGUNDO );

                if ( ! naveVisitada ) {
                    valorBucleJuego = BUCLE_JUEGO_LLEGADA_NAVE;
                    naveVisitada = 1;
                }
                comprobarExito();
            }
            else {
                // No dejamos soltar un objeto cerca de otro.
                // Sonidillo de que no se puede soltar el objeto
                reproducirIndiceSonido( SONIDO_GOLPE, VALOR_CHIP_SEGUNDO );
                return;
            }

            return;
        }

        o = detectarObjetoProximoAlJugadorEnX();
        if ( o && o->indice != OBJETO_NAVE ) {
            // Tampoco dejamos soltar un objeto encima de otro
            // Sonidillo de que no se puede soltar el objeto
            reproducirIndiceSonido( SONIDO_GOLPE, VALOR_CHIP_SEGUNDO );
            return;
        }

        // Suelta el objeto

        reproducirIndiceSonido( SONIDO_SOLTAR_OBJETO, VALOR_CHIP_SEGUNDO );

        // Inserta el objeto en la escena
        o = &objetos[ objetoCogido ];
        o->flagsEstado = ESTADO_OBJETO_CAYENDO;
        o->pantalla = posicionPantallaX;
        o->xEnt2 = xEntera;
        o->yEnt = yEntera + jugador.tamy - o->tamy;
        o->y = ((int)o->yEnt) << BITS_DECIMALES_Y;
        o->vely = jugador.vely;
        o->yant = o->yEnt;
        o->yant2 = o->yEnt;

        numObjetosCayendo++;
        objetoCogido = OBJETO_NULO;
        
        computarObjetosEnPantalla();
        
    }
    
}

Objeto *detectarObjetoProximoAlJugador() {

    if ( posicionPantallaY > 0 ) {
        // Solo se pueden coger objetos en pantalla a nivel de terreno
        return NULL;
    }
    
    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
        Objeto *o = objetosPantallaActual[ i ];
        if ( o->flagsEstado ) {
            // Solo se puede coger un objeto estatico (que ha caido al suelo)
            continue;
        }
        if ( xEntera <= o->xEnt2 + o->tamx2 && 
             xEntera + jugador.tamx2 >= o->xEnt2 &&
             yEntera <= o->yEnt + o->tamy && 
             yEntera + jugador.tamy >= o->yEnt ) {
            return o;
        }
    }
    
    return NULL;
}

Objeto *detectarObjetoProximoAlJugadorEnX() {

    if ( posicionPantallaY > 0 ) {
        // Solo se pueden coger objetos en pantalla a nivel de terreno
        return NULL;
    }
    
    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
        Objeto *o = objetosPantallaActual[ i ];
        if ( o->flagsEstado ) {
            // Solo se puede coger un objeto estatico (que ha caido al suelo)
            continue;
        }
        if ( xEntera <= o->xEnt2 + o->tamx2 && 
             xEntera + jugador.tamx2 >= o->xEnt2 ) {
            return o;
        }
    }
    
    return NULL;
}

unsigned char detectarObjetoProximoANave( Objeto *obj ) {

    Objeto *o = &objetos[ OBJETO_NAVE ];
    
    if ( posicionPantallaY > 0 ) {
        // Solo se pueden coger objetos en pantalla a nivel de terreno
        return 0;
    }
    
    if ( obj->pantalla != o->pantalla ) {
        return 0;
    }
    
    if ( obj->xEnt2 <= o->xEnt2 + o->tamx2 && 
         obj->xEnt2 + obj->tamx2 >= o->xEnt2 &&
         obj->yEnt <= o->yEnt + o->tamy && 
         obj->yEnt + obj->tamy >= o->yEnt ) {
        return 1;
    }

    return 0;
}

void computarObjetosNecesarios() {

    numObjetosNecesarios = 0;
    numObjetosObtenidos = 0;
    for ( i = 0; i < NUM_OBJETOS_DINAMICOS; i++ ) {
        Objeto *o = &objetos[ i ];
        if ( o->necesario ) {
            numObjetosNecesarios++;
            if ( o->flagsEstado == ESTADO_OBJETO_EN_LA_NAVE ) {
                numObjetosObtenidos++;
            }
        }
    }

}

unsigned char comprobarExito() {
    computarObjetosNecesarios();
    if ( numObjetosNecesarios <= numObjetosObtenidos ) {
        valorBucleJuego = BUCLE_JUEGO_EXITO;
        return 1;
    }
    return 0;
}

void actualizarXAntObjetosCayendo() {

    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
        Objeto *o = objetosPantallaActual[ i ];
        if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
            o->yant = o->yEnt;
        }
    }

}

void actualizarXAnt2ObjetosCayendo() {

    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
        Objeto *o = objetosPantallaActual[ i ];
        if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
            o->yant2 = o->yant;
        }
    }

}

void bucleJuego() {

    unsigned char i;
    
    inicializarSonido();

    radasPonerModoRadastan( PAPER_BLACK, PAPER_WHITE | INK_BLACK, &paletas[ 0 ] );

    redibujarEscenario();

    // Bucle del juego
    
    valorBucleJuego = BUCLE_JUEGO_CONTINUAR;
    while ( valorBucleJuego == BUCLE_JUEGO_CONTINUAR ) {

#ifdef DEBUG_CPU_JUEGO
        zx_border( 3 );
#endif

        // Control jugador por joystick/teclado

        tecla = getTecla();

        if ( tecla == teclaInventario ) {
            valorBucleJuego = BUCLE_JUEGO_MENU;
            break;
        }
        
        
        // Debug
        /*
        if ( tecla == 'x' ) {
            valorBucleJuego = BUCLE_JUEGO_EXITO;
        }
        */
        
        
        //valorJoystick = funcionJoystickActual( &mapaTeclado );
        valorJoystick = leerJoystickYTeclado();
        i = 0;
        if ( valorJoystick & in_LEFT ) {
            jugador.velx --;
            direccionJugador = 1;
            i = 1;
        }
        if ( valorJoystick & in_RIGHT ) {
            jugador.velx ++;
            direccionJugador = 0;
            i = 1;
        }
        if ( valorJoystick & in_UP ) {
            jugador.vely -= 2;
            i = 1;
        }
        if ( valorJoystick & in_DOWN ) {
            jugador.vely ++;
            i = 1;
        }
        
        if ( i == 1 ) {
            setVolumenSonidoJetpac( VOLUMEN_SONIDO_JETPAC );
        }
        else {
            setVolumenSonidoJetpac( 0 );
        }

        // Control de fotogramas del jugador;
        fotogramaActualJugador = direccionJugador ? 1 : 0;

        radasBorrarRectangulo( ( unsigned char )( jugador.xant2 ),
                               ( unsigned char )( jugador.yant2 ),
                               jugador.tamx2, jugador.tamy );
        
        // Borra objetos dinamicos (cayendo)
        if ( numObjetosCayendo ) {
            for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                Objeto *o = objetosPantallaActual[ i ];
                if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
                    radasBorrarRectangulo( o->xEnt2, o->yant2, o->tamx2, o->tamy );
                }
            }
        }

        if ( ( valorJoystick & in_FIRE ) && ! disparoPulsadoFrameAnterior ) {
            // Disparo recien pulsado: coger o soltar objeto
            cogerSoltarObjeto();
        }
        disparoPulsadoFrameAnterior = valorJoystick & in_FIRE;
        
        // Gravedad
        jugador.vely ++;

        // Movimiento del jugador
        xNueva = jugador.x + jugador.velx;
        yNueva = jugador.y + jugador.vely;

        xEntera = xNueva >> BITS_DECIMALES_X;
        yEntera = yNueva >> BITS_DECIMALES_Y;

        // Movimiento de objetos cayendo
        if ( numObjetosCayendo ) {
            flagObjetoCaido = 0;
            for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                Objeto *o = objetosPantallaActual[ i ];
                if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {

                    // Movimiento
                    o->vely ++;
                    o->y += o->vely;
                    o->yEnt = o->y >> BITS_DECIMALES_Y;
                    
                    if ( posicionPantallaY > 0 ) {
                        
                        // Objeto cayendo por encima de la pantalla del terreno. Si llega
                        // al borde inferior, se le lleva al suelo.
                        if ( o->yEnt + o->tamy > PANTALLA_Y ) {

                            llevarObjetoAlSuelo( o );
                            
                            computarObjetosEnPantalla();
                            
                            numObjetosCayendo--;
                            
                            pintarJugador()
                            ula_sync();
                            radasCambiarPantalla();
                            // Borra el objeto en ant, que estaba cayendo aun
                            radasBorrarRectangulo( o->xEnt2, o->yant, o->tamx2, o->tamy );
                            // Borra el jugador en ant
                            radasBorrarRectangulo( ( unsigned char )( jugador.xant ),
                                                   ( unsigned char )( jugador.yant ),
                                                   jugador.tamx2, jugador.tamy );
                            flagObjetoCaido = 1;
   
                        }
                        else if ( o->y < 0 ) {
                            // Colision con borde superior: el objeto se para
                            o->vely = 0;
                            o->yEnt = 0;
                            o.y = 0;
                        }
                    }
                    else {

                        // Colision de objetos cayendo, con el terreno
                        difH1 = ( o->yEnt + o->tamy ) - ( PANTALLA_Y - punteroAlturaPantallaActual[ o->xEnt2 ] - 1 );
                        if ( o->tamx2 > 1 ) {
                            difH2 = ( o->yEnt + o->tamy ) - ( PANTALLA_Y - punteroAlturaPantallaActual[ o->xEnt2 + 1 ] - 1 );
                            if ( difH2 > difH1 ) {
                                difH1 = difH2;
                            }
                        }
                        if ( difH1 >= 0 ) {
                            if ( detectarObjetoProximoANave( o ) ) {
                                
                                reproducirIndiceSonido( SONIDO_DEJAR_OBJETO_NAVE, VALOR_CHIP_SEGUNDO );
                                
                                o->flagsEstado = ESTADO_OBJETO_EN_LA_NAVE;

                                comprobarExito();

                                // Borra el objeto en las dos shadow screen
                                pintarJugador()
                                ula_sync();
                                radasCambiarPantalla();
                                // Borra el objeto en ant, que estaba cayendo aun
                                radasBorrarRectangulo( o->xEnt2, o->yant, o->tamx2, o->tamy );
                                // Borra el jugador en ant
                                radasBorrarRectangulo( ( unsigned char )( jugador.xant ),
                                                       ( unsigned char )( jugador.yant ),
                                                       jugador.tamx2, jugador.tamy );
                            }
                            else {

                                // El objeto se asienta en el terreno
                                o->yEnt -= difH1;
                                if ( o->yEnt < PANTALLA_Y - ALTURA_TERRENO - 1 ) {
                                    o->yEnt = PANTALLA_Y - ALTURA_TERRENO - 1;
                                }

                                o->flagsEstado = ESTADO_OBJETO_ESTATICO;

                                // Si el jugador "molesta", lo subimos:
                                if ( xEntera <= o->xEnt2 + o->tamx2 && 
                                     xEntera + jugador.tamx2 >= o->xEnt2 &&
                                     yEntera < o->yEnt + o->tamy && 
                                     yEntera + jugador.tamy > o->yEnt ) {

                                    yEntera += ((int)o->yEnt) - ( yEntera + jugador.tamy );
                                    yNueva = ((int)yEntera) << BITS_DECIMALES_Y;

                                }

                                integrarObjetoEnTerreno( o );
                                
                                pintarJugador()
                                radasPintarImagen( o->xEnt2, o->yEnt, o->imagen );
                                ula_sync();
                                radasCambiarPantalla();
                                // Borra el objeto en ant, que estaba cayendo aun
                                radasBorrarRectangulo( o->xEnt2, o->yant, o->tamx2, o->tamy );
                                // Borra el jugador en ant
                                radasBorrarRectangulo( ( unsigned char )( jugador.xant ),
                                                       ( unsigned char )( jugador.yant ),
                                                       jugador.tamx2, jugador.tamy );
                                radasPintarImagen( o->xEnt2, o->yEnt, o->imagen );
                                
                                
                                
                                
                                flagObjetoCaido = 1;

                            }

                            numObjetosCayendo--;
                        }
                    }
                }
            }
            
            if ( flagObjetoCaido ) {

                // Actualiza posiciones anterior2 al haber cambiado de shadow screen
                jugador.xant = xEntera;
                jugador.yant = yEntera;
                jugador.xant2 = xEntera;
                jugador.yant2 = yEntera;

                if ( numObjetosCayendo ) {
                   actualizarXAnt2ObjetosCayendo();
                }                
            }
            
        }

        cambioDePantalla = 0;
        posicionPantallaYAnterior = posicionPantallaY;

        if ( ( xEntera + jugador.tamx2 ) > PANTALLA_X2 ) {
            xNueva = 0;
            xEntera = 0;
            cambioDePantalla = 1;
            posicionPantallaX++;
            if ( posicionPantallaX >= numPantallasX ) {
                posicionPantallaX = 0;
            }
        }
        else if ( xEntera < 0 ) {
            xNueva = ( (int)( PANTALLA_X2 ) - (int)( jugador.tamx2 ) ) << BITS_DECIMALES_X;
            xEntera = xNueva >> BITS_DECIMALES_X;
            cambioDePantalla = 1;
            if ( posicionPantallaX == 0 ) {
                posicionPantallaX = numPantallasX - 1;
            }
            else {
                posicionPantallaX--;
            }
        }

        if ( ( yEntera + jugador.tamy ) > PANTALLA_Y  ) {
            // Baja a la pantalla de abajo
            if ( posicionPantallaY == 0 ) {
                // No hay pantalla de abajo, choca contra el limite inferior del terreno
                if ( jugador.vely > MAX_VELOCIDAD_IMPACTO ) {
                    // Muerte por impacto excesivo
                    valorBucleJuego = BUCLE_JUEGO_MUERTE_COLISION;
                    reproducirIndiceSonido( SONIDO_GOLPE, VALOR_CHIP_SEGUNDO );
                }
                else {
                    jugador.vely = 0;
                    yNueva = jugador.y;
                    yEntera = yNueva >> BITS_DECIMALES_Y;
                }
            }
            else {
                yNueva = 0;
                yEntera = 0;
                cambioDePantalla = 1;
                posicionPantallaY--;
            }
        }
        else if ( yEntera < 0 ) {
            if ( posicionPantallaY >= 50 ) {
                // El jugador se ha perdido en el espacio
                valorBucleJuego = BUCLE_JUEGO_MUERTE_EXTRAVIADO;
                break;
            }
            else {
                // Sube a la pantalla de arriba
                yNueva = ( (int)( PANTALLA_Y ) - (int)( jugador.tamy ) ) << BITS_DECIMALES_Y;
                yEntera = yNueva >> BITS_DECIMALES_Y;
                cambioDePantalla = 1;
                posicionPantallaY++;
            }
        }
        
        if ( cambioDePantalla ) {
            
            punteroAlturaPantallaActual = getMapaAlturas() + ( ((unsigned int) posicionPantallaX ) << BITS_ANCHO_PANTALLA );

            if ( posicionPantallaY == 0 ) {
                
                // Estamos en pantalla a nivel de suelo, dibujar terreno y objetos estaticos

                posicionAlturaPantallaActual = ((unsigned int)posicionPantallaX) << BITS_ANCHO_PANTALLA;
                pintarMapaColorPlano( radasObtenerPantallaActual(), posicionAlturaPantallaActual, COLOR_TERRENO );

                if ( numObjetosCayendo ) {
                    // Lleva al suelo los objetos dinamicos de la anterior pantalla (que estaban cayendo)
                    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                        Objeto *o = objetosPantallaActual[ i ];
                        if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
                            llevarObjetoAlSuelo( o );
                            o->flagsEstado |= FLAG_OBJETO_POR_BORRAR;
                        }
                    }
                }

                // Pinta objetos estaticos recorriendo el array general
                for ( i = 0; i < MAX_OBJETOS; i++ ) {
                    Objeto *o = &objetos[ i ];
                    if ( o->pantalla == posicionPantallaX && ( ! o->flagsEstado || ( o->flagsEstado & FLAG_OBJETO_POR_BORRAR ) ) ) {
                        radasPintarImagen( o->xEnt2, o->yEnt, o->imagen );
                    }
                }
                
                ula_sync();
                radasCambiarPantalla();
                
                radasBorrarRectangulo( ( unsigned char )( jugador.xant ),
                                       ( unsigned char )( jugador.yant ),
                                       jugador.tamx2, jugador.tamy );

                if ( numObjetosCayendo ) {
                    // Borra objetos dinamicos de la anterior pantalla (que estaban cayendo) y los lleva al suelo
                    // Aunque se hayan llevado al suelo se conserva la variable yant
                    for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                        Objeto *o = objetosPantallaActual[ i ];
                        if ( o->flagsEstado & FLAG_OBJETO_POR_BORRAR ) {
                            radasBorrarRectangulo( o->xEnt2, o->yant, o->tamx2, o->tamy );
                            o->flagsEstado &= ~FLAG_OBJETO_POR_BORRAR;
                            numObjetosCayendo--;
                        }
                    }
                }

                computarObjetosEnPantalla();

                pintarMapaColorPlano( radasObtenerPantallaActual(), posicionAlturaPantallaActual, COLOR_TERRENO );
                
                // Pintar objetos estaticos recorriendo el array de pantalla actual
                pintarObjetosEstaticos();

                jugador.xant2 = xEntera;
                jugador.yant2 = yEntera;

            }
            else {
                
                // Estamos por encima de la pantalla del suelo
                
                if ( posicionPantallaYAnterior == 0 ) {
                    
                    borrarMapa( radasObtenerPantallaActual() );
                    ula_sync();
                    radasCambiarPantalla();
                    
                    radasBorrarRectangulo( ( unsigned char )( jugador.xant ),
                                           ( unsigned char )( jugador.yant ),
                                           jugador.tamx2, jugador.tamy );

                    if ( numObjetosCayendo ) {
                        // Borra objetos dinamicos de la anterior pantalla (que estaban cayendo) y los lleva al suelo
                        for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                            Objeto *o = objetosPantallaActual[ i ];
                            if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
                                radasBorrarRectangulo( o->xEnt2, o->yant, o->tamx2, o->tamy );
                                llevarObjetoAlSuelo( o );
                            }
                        }
                    }

                    computarObjetosEnPantalla();
                
                    borrarMapa( radasObtenerPantallaActual() );
                    
                }

                jugador.xant2 = jugador.xant;
                jugador.yant2 = jugador.yant;
            }

        }
        else {

            // Colision del jugador con el terreno
            if ( posicionPantallaY == 0 ) {
                difH1 = ( yEntera + jugador.tamy ) - ( PANTALLA_Y - punteroAlturaPantallaActual[ xEntera ] - 1 );
                difH2 = ( yEntera + jugador.tamy ) - ( PANTALLA_Y - punteroAlturaPantallaActual[ xEntera + 1 ] - 1 );
                if ( difH2 > difH1 ) {
                    difH1 = difH2;
                }
                if ( difH1 >= 0 ) {

                    if ( jugador.velx > MAX_VELOCIDAD_IMPACTO ||
                         jugador.velx < - MAX_VELOCIDAD_IMPACTO ||
                         jugador.vely > MAX_VELOCIDAD_IMPACTO ) {
                        // Muerte por impacto excesivo
                        valorBucleJuego = BUCLE_JUEGO_MUERTE_COLISION;
                        reproducirIndiceSonido( SONIDO_GOLPE, VALOR_CHIP_SEGUNDO );
                    }
                    else {
                        if ( difH1 >= 2 ) {
                            jugador.velx = 0;
                            xNueva = jugador.x;
                            xEntera = xNueva >> BITS_DECIMALES_X;
                            yNueva = jugador.y;
                            yEntera = yNueva >> BITS_DECIMALES_Y;
                        }
                        else {
                            yEntera -= difH1;
                            yNueva = yEntera << BITS_DECIMALES_Y;
                            jugador.vely = 0;
                        }
                    }
                }
            }

            jugador.xant2 = jugador.xant;
            jugador.yant2 = jugador.yant;

            if ( numObjetosCayendo ) {
                actualizarXAnt2ObjetosCayendo();
            }

        }

        jugador.xant = xEntera;
        jugador.yant = yEntera;
   
        if ( numObjetosCayendo ) {
            actualizarXAnt2ObjetosCayendo();
            actualizarXAntObjetosCayendo();
        }

        jugador.x = xNueva;
        jugador.y = yNueva;

        pintarJugador()

        // Pinta objetos dinamicos (cayendo)
        if ( numObjetosCayendo ) {
            for ( i = 0; i < numObjetosEnPantallaActual; i++ ) {
                Objeto *o = objetosPantallaActual[ i ];
                if ( o->flagsEstado == ESTADO_OBJETO_CAYENDO ) {
                    radasPintarImagen( o->xEnt2, o->yEnt, o->imagen );
                }
            }
        }
        

#ifdef DEBUG_CPU_JUEGO
        zx_border( 6 );
#endif
        
        ula_sync();
        radasCambiarPantalla();

    }
    
    radasPonerModoNormal( PAPER_BLACK | INK_CYAN, PAPER_BLACK );


}

void rotarPaletaAdelante( unsigned char *paleta ) {
    
    // TODO borrar
    
    unsigned char i;
    unsigned char v1 = paleta[ 0 ];
    unsigned char v2;
    for ( i = 1; i < 16; i++ ) {
        v2 = paleta[ i ];
        paleta[ i ] = v1;
        v1 = v2;
    }
    paleta[ 0 ] = v1;
}

void debugYSalir() {

    unsigned char i;
    
    radasPonerModoNormal( INK_BLACK | PAPER_WHITE, INK_WHITE );

    clsTexto();
    fputs( "Salida de debug:\n", stdout);
    for ( i = 0; i < NUM_DEBUG; i++ ) {
        itoa( debug[ i ], debugStr, 10 );
        puts( debugStr );
    }

    exit( -1 );
}

