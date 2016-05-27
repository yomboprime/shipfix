/*
    This program is free software: you can redistribute it and/or modify
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


// Se acepta un parametro, el path relativo de un fichero de texto para exportar como fichero .h con extos y menus.
// // Ver el formato del fichero en textos/textos_ES.txt


// ***** Librerias *****

var fs = require( 'fs' );



console.log( "ProcesarTextos v1.0.0");
console.log( "Uso: node main.js <path_relativo_fichero_de_texto>" );
console.log( "" );

var pathFicheroEntrada = __dirname + "/" + process.argv[ 2 ];

console.log( "Leyendo fichero de texto en: " + pathFicheroEntrada );

fs.readFile( pathFicheroEntrada, 'utf8', function( err, datosFichero ) {

    if ( err ) {
        console.error( "Hubo un error al leer el fichero en: " + pathFicheroEntrada );
        process.exit( -1 );
    }

    console.log( "Fichero cargado. Procesando..." );

    var resultado = procesarDatos( datosFichero );

    if ( resultado.error ) {
        console.error( resultado.error );
        process.exit( -1 );
        return;
    }

    var nombreH = pathFicheroEntrada;
    var lastIndexOfDot = nombreH.lastIndexOf( '.' );
    if ( lastIndexOfDot > 0 && nombreH.lastIndexOf( '/' ) < lastIndexOfDot ) {
        nombreH = nombreH.substring( 0, lastIndexOfDot ) + ".h";
    }
    else {
        nombreH += ".h";
    }

    console.log( "Guardando fichero .h en " + nombreH );

    grabarFicheroTexto( resultado.contenidoFicheroH, nombreH, function() {
        console.log( "Fichero .h guardado." );
    } );

} );

function procesarDatos( datos ) {

    var lineas = datos.split( "\n" );

    resultado = {
        error: null,
        pantallas: [],
        contenidoFicheroH: null
    };

    var terminar = false;
    var indiceLinea = 0;
    var primerIndiceLineaPantalla = 0;
    var label = null;
    while ( ! terminar && indiceLinea < lineas.length ) {

        var linea = lineas[ indiceLinea ];

        if ( linea.length >= 1 && linea[ 0 ] === '#' ) {
            indiceLinea++;
            continue;
        }

        if ( linea.length > 64 ) {
            resultado.error = "Línea demasiado larga (max = 64 caracteres) (número de línea = " + indiceLinea + ", longitud = " + linea.length + ")";
            return resultado;
        }

        if ( linea.startsWith( "=== " ) && linea.length >= 5 ) {
            if ( label !== null ) {
                procesarPantalla( label, lineas, primerIndiceLineaPantalla, indiceLinea - 1, resultado );
                if ( resultado.error ) {
                    return resultado;
                }
            }
            primerIndiceLineaPantalla = indiceLinea + 1;
            label = linea.substring( 4 );
        }

        indiceLinea++;
    }

    if ( label !== null && indiceLinea === lineas.length && primerIndiceLineaPantalla <= indiceLinea - 1 ) {

        procesarPantalla( label, lineas, primerIndiceLineaPantalla, indiceLinea - 1, resultado );
        if ( resultado.error ) {
            return resultado;
        }
    }

    generarSalida( resultado );

    return resultado;

}

function procesarPantalla( label, lineas, primerIndiceLinea, ultimoIndiceLinea, resultado ) {

    if ( primerIndiceLinea > ultimoIndiceLinea ) {
        resultado.error = "Error interno: Indices inválidos: " + primerIndiceLinea + ", " + ultimoIndiceLinea;
        return;
    }
    
    var pantalla = {
        label: label,
        lineasTexto: [],
        menu: null
    };

    function crearOpcionMenu( textoOpcion ) {
        return {
            texto: " " + textoOpcion.substring( 1 ),
            x0: 0,
            x1: 0
        };
    }

    var enMenu = false;
    for ( var i = primerIndiceLinea; i <= ultimoIndiceLinea; i++ ) {
        var linea = lineas[ i ];
        if ( ! enMenu ) {
            if ( linea.length >= 1 && linea[ 0 ] === '@' ) {
                enMenu = true;
                pantalla.menu = {
                    yInicial: i - primerIndiceLinea,
                    opciones: [ crearOpcionMenu( linea ) ]
                };
            }
            else {
                if ( linea.length < 64 ) {
                    linea += '\n';
                }
                pantalla.lineasTexto.push( linea );
            }
        }
        else {
            if ( ! ( linea.length >= 1 && linea[ 0 ] === '@' ) ) {
                resultado.error = "Error: no se puede poner una línea normal después del menú. Línea causante = " + ( i + 1 );
                return;
            }
            pantalla.menu.opciones.push( crearOpcionMenu( linea ) );
        }
        
        if ( pantalla.lineasTexto.length > 24 ) {
            resultado.error = "Error: pantalla con más de 24 líneas. Línea causante = " + ( i + 1 );
            return;
        }

    }

    resultado.pantallas.push( pantalla );
}

function generarSalida( resultado ) {


    // Procesa los menus, obteniendo las coordenadas de cada opcion:
    for ( var i = 0; i < resultado.pantallas.length; i++ ) {
        var menu = resultado.pantallas[ i ].menu;
        if ( menu !== null ) {
            for ( var j = 0; j < menu.opciones.length; j++ ) {
                var opcion = menu.opciones[ j ];
                var linea = opcion.texto;
                var numEspaciosIniciales = 0;
                for ( var k = 0; k < linea.length && linea[ k ] === ' '; k++ ) {
                    numEspaciosIniciales++;
                }
                opcion.x0 = Math.floor( numEspaciosIniciales * 0.5 );
                opcion.x1 = Math.floor( ( linea.length ) * 0.5 );
            }
        }
    }

    var texto = "/*\n";
    texto += "This program is free software: you can redistribute it and/or modify\n";
    texto += "it under the terms of the GNU General Public License as published by\n";
    texto += "the Free Software Foundation, either version 3 of the License, or\n";
    texto += "any later version.\n";
    texto += "\n";
    texto += "This program is distributed in the hope that it will be useful,\n";
    texto += "but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
    texto += "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
    texto += "GNU General Public License for more details.\n";
    texto += "\n";
    texto += "You should have received a copy of the GNU General Public License\n";
    texto += "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";
    texto += "*/\n";

    texto += "\n\n#ifndef TEXTOS_H\n";
    texto += "#define TEXTOS_H\n\n";

    texto += "#include \"menu.h\"\n\n";

    // Textos de las pantallas
    for ( var i = 0; i < resultado.pantallas.length; i++ ) {
        var pantalla = resultado.pantallas[ i ];
        if ( pantalla.lineasTexto.length > 0 ) {

            texto += "static unsigned char mensaje" + pantalla.label + "[] = {\n";

            for ( var j = 0; j < pantalla.lineasTexto.length; j++ ) {
                var linea = pantalla.lineasTexto[ j ];
                for ( var k = 0; k < linea.length; k++ ) {
                    var codigoCaracter = linea.charCodeAt( k );
                    if ( codigoCaracter === 13 ) {
                        continue;
                    }
                    if ( codigoCaracter !== 10 &&
                         ( codigoCaracter < 32 || codigoCaracter >= 128 ) ) {
                        // Caracter no valido, traducir como espacio
                        codigoCaracter = 32;
                    }

                    if ( codigoCaracter === 10 ) {
                        codigoCaracter = 13;
                    }

                    var representacionCaracterHex = codigoCaracter.toString( 16 );

                    texto += "    0x" + representacionCaracterHex + ",\n";
                }
            }

            // Byte nulo de fin de cadena
            texto += "    0x00\n";

            texto += "};\n\n";

        }
    }

    // Menus

    // Textos de las opciones
    for ( var i = 0; i < resultado.pantallas.length; i++ ) {
        var pantalla = resultado.pantallas[ i ];
        var menu = pantalla.menu;
        if ( menu ) {

            for ( var o = 0; o < menu.opciones.length; o++ ) {

                var opcion = menu.opciones[ o ];

                texto += "static unsigned char menu" + pantalla.label + "Opcion" + o + "[] = {\n";

                var linea = opcion.texto;
                for ( var k = 0; k < linea.length; k++ ) {
                    var codigoCaracter = linea.charCodeAt( k );
                    if ( codigoCaracter === 13 || codigoCaracter === 10 ) {
                        continue;
                    }
                    if ( codigoCaracter < 32 || codigoCaracter >= 128 ) {
                        // Caracter no valido, traducir como espacio
                        codigoCaracter = 32;
                    }

                    var representacionCaracterHex = codigoCaracter.toString( 16 );

                    texto += "    0x" + representacionCaracterHex + ",\n";
                }

                // Byte nulo de fin de cadena
                texto += "    0x00\n";

                texto += "};\n\n";
            }
        }
    }

    for ( var i = 0; i < resultado.pantallas.length; i++ ) {
        var pantalla = resultado.pantallas[ i ];
        var menu = pantalla.menu;
        if ( menu ) {

            // Array de opciones
            texto += "static OpcionMenu menu" + pantalla.label + "Opciones[] = {\n\n"
            texto += "    // x0, x1, texto\n\n";
            for ( var o = 0; o < menu.opciones.length; o++ ) {

                var opcion = menu.opciones[ o ];

                texto += "    {\n";
                texto += "        " + opcion.x0 + ", " + opcion.x1 + ", menu" + pantalla.label + "Opcion" + o + "\n";
                texto += o === menu.opciones.length - 1 ? "    }\n" : "    },\n";
            }

            texto += "};\n\n";

            // Struct con info del menu
            texto += "static Menu menu" + pantalla.label + " = {\n\n";
            texto += "    " + ( pantalla.lineasTexto.length > 0 ? "mensaje" + pantalla.label : "NULL" ) + ",\n";
            texto += "    // y, numOpciones, opciones\n\n";
            texto += "    " + menu.yInicial + ", " + menu.opciones.length + ", menu" + pantalla.label + "Opciones\n";
            texto += "};\n\n";
        }
    }

    texto += "#endif // TEXTOS_H\n\n\n";



    resultado.contenidoFicheroH = texto;
}

function grabarFicheroTexto( texto, path, onEnd ) {

    fs.writeFile( path, texto, function( err ) {

        if( err ) {
            console.error( "Error guardando fichero de texto: " + err + " fichero: " + path );
            process.exit( -1 );
        }

        if ( onEnd ) {
            onEnd();
        }

    } );
}
