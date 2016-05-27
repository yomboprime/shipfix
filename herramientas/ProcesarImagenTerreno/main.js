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


// Se acepta un parametro path de una imagen
// La imagen debe tener 96 pixeles de altura, y N * 128 de anchura
// Los primeros 16 pixels de la imagen son la paleta.
// Colores:
// 0 : Fondo vacio
// 1 : Terreno
// 2 : Separador de pantallas (se ignora)
// 3 : Linea de maxima altura del terreno
// >= 4, <= 15 : posicion inicial fija de hasta 12 sprites (no pueden estar por debajo del terreno) (No terminado)



// ***** Librerias *****

var fs = require( 'fs' );
var Canvas = require( 'canvas' );
var Image = Canvas.Image;


console.log( "ProcesarImagenTerreno");
console.log( "Uso: node main.js <path_relativo_imagen>" );
console.log( "" );


var PANTALLA_X  = 128;
var PANTALLA_X2 = PANTALLA_X * 0.5;
var PANTALLA_Y = 96;

var ALTURA_TERRENO = 32;


var pathImagen = __dirname + "/" + process.argv[ 2 ];

console.log( "Leyendo imagen en: " + pathImagen );

fs.readFile( pathImagen, function( err, buferImagen ) {

    if ( err ) {
        console.error( "Hubo un error al leer la imagen en: " + pathImagen );
        throw err;
    }

    console.log( "Procesando imagen..." );

    var img = new Image;
    img.src = buferImagen;
    var tx = img.width;
    var ty = img.height;

    if ( tx % PANTALLA_X !== 0 ) {
        console.error( "La dimension X de la imagen no es multiplo de " + PANTALLA_X );
        process.exit( -1 );
    }

    if ( ty !== PANTALLA_Y ) {
        console.error( "La dimension Y de la imagen no es igual a " + PANTALLA_Y );
        process.exit( -1 );
    }

    var elCanvas = new Canvas( tx, ty );
    ctx2dNivel = elCanvas.getContext( '2d' );
    ctx2dNivel.drawImage( img, 0, 0 );

    var datosImagen = ctx2dNivel.getImageData( 0, 0, tx, ty );
    var datosPixels = datosImagen.data;

    function getPixelPR( p ) {
        return datosPixels[ p ];
    }

    function getPixelPG( p ) {
        return datosPixels[ p + 1 ];
    }

    function getPixelPB( p ) {
        return datosPixels[ p + 2 ];
    }

    function getPixelPA( p ) {
        return datosPixels[ p + 3 ];
    }

    function getPixelP( x, y ) {
        return 4 * ( Math.round( x ) + Math.round( y ) * tx );
    }

    function setPixel( x, y, r, g, b, a ) {
        var p = getPixelP( x, y );
        datosPixels[ p ] = r;
        datosPixels[ p + 1 ] = g;
        datosPixels[ p + 2 ] = b;
        datosPixels[ p + 3 ] = a;
    }

    function getPixelNegro( x, y ) {
        var p = getPixelP( x, y );
        return datosPixels[ p ] === 0 &&
               datosPixels[ p + 1 ] === 0 &&
               datosPixels[ p + 2 ] === 0;
    }

    function getColorPaleta( r, g, b ) {
        // Devuelve indice paleta o -1 si no coincide con ninguno
        for ( var indicePaleta = 0; indicePaleta < 16; indicePaleta++ ) {
            if ( getPixelPR( 4 * indicePaleta ) === r && getPixelPG( 4 * indicePaleta ) === g && getPixelPB( 4 * indicePaleta ) === b ) {
                return indicePaleta;
            }
        }
        return -1;
    }

    // Bucle por las pantallas
    for ( var x = 0; x < tx; x += PANTALLA_X ) {

        if ( x === 0 ) continue;

        // Pinta linea roja vertical
        for ( var y = 0; y < PANTALLA_Y; y++ ) {
            if ( getPixelNegro( x, y ) ) {
                setPixel( x - 1, y, getPixelPR( 4 * 2 ), getPixelPG( 4 * 2 ), getPixelPB( 4 * 2 ), 255 );
            }
        }
    }

    // Pinta linea que indica altura maxima del terreno
    var yTerreno = PANTALLA_Y - ALTURA_TERRENO;
    var rTerreno = getPixelPR( 4 * 1 );
    var gTerreno = getPixelPG( 4 * 1 );
    var bTerreno = getPixelPB( 4 * 1 );
    for ( var x = 0; x < tx; x++ ) {
        if ( getPixelNegro( x, yTerreno ) ) {
            setPixel( x, yTerreno, getPixelPR( 4 * 3 ), getPixelPG( 4 * 3 ), getPixelPB( 4 * 3 ), 255 );
        }
    }

    // Altura de cada par de pixels
    var alturas = [];
    var sprites = [];
    for ( var x = 0; x < tx && x + 1 < tx; x += 2 ) {
        for ( var y = 1; y < PANTALLA_Y; y++ ) {
            var p0 = getPixelP( x, y );
            var indicePaleta0 = getColorPaleta( getPixelPR( p0 ), getPixelPG( p0 ), getPixelPB( p0 ) );
            var p1 = getPixelP( x + 1, y );
            var indicePaleta1 = getColorPaleta( getPixelPR( p1 ), getPixelPG( p1 ), getPixelPB( p1 ) );
            if ( indicePaleta0 === 1 || indicePaleta1 === 1 ) {

                // Terreno

                if ( y < yTerreno && y > 0 ) {
                    console.error( "Error: Píxel de terreno por encima del nivel permitido. X = " + x );
                    process.exit( -1 );
                }

                var altura = PANTALLA_Y - y;
                alturas.push( altura );

                for ( var y2 = y; y2 < PANTALLA_Y; y2++ ) {
                    setPixel( x, y2, rTerreno, gTerreno, bTerreno, 255 );
                    setPixel( x + 1, y2, rTerreno, gTerreno, bTerreno, 255 );
                }

                break;

            }
            else {
                // Solo puede haber un sprite en un byte
                var ip = indicePaleta0;
                if ( indicePaleta0 < 4 ) {
                    if ( indicePaleta1 >= 4 ) {
                        ip = indicePaleta1;
                    }
                }
                if ( ip >= 4 ) {
                    sprites.push( {
                        x: x,
                        y: y,
                        indice: ip
                    } );
                }
            }
        }
    }

    ctx2dNivel.putImageData( datosImagen, 0, 0 );

    console.log( "Guardando imagen..." );

    grabarImagenPNG( elCanvas, pathImagen );

    console.log( "Guardando fichero alturas.h ..." );

    var pathAlturas = __dirname + "/alturas.h";

    var textoAlturas = "/*\n";
    textoAlturas += "This program is free software: you can redistribute it and/or modify\n";
    textoAlturas += "it under the terms of the GNU General Public License as published by\n";
    textoAlturas += "the Free Software Foundation, either version 3 of the License, or\n";
    textoAlturas += "any later version.\n";
    textoAlturas += "\n";
    textoAlturas += "This program is distributed in the hope that it will be useful,\n";
    textoAlturas += "but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
    textoAlturas += "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
    textoAlturas += "GNU General Public License for more details.\n";
    textoAlturas += "\n";
    textoAlturas += "You should have received a copy of the GNU General Public License\n";
    textoAlturas += "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";
    textoAlturas += "*/\n";

    textoAlturas += "\n";
    textoAlturas += "#ifndef ALTURAS_H\n";
    textoAlturas += "#define ALTURAS_H\n";
    textoAlturas += "\n";
    textoAlturas += "\n";
    textoAlturas += "#define NUM_ALTURAS " + alturas.length + "\n";
    textoAlturas += "\n";
    textoAlturas += "#define NUM_PANTALLAS_X " + ( alturas.length / PANTALLA_X2 ) + "\n";
    textoAlturas += "\n";

    if ( alturas.length > 0 ) {
        textoAlturas += "static unsigned char alturas [] = {\n";
        textoAlturas += "\n";

        for ( var i = 0; i < alturas.length; i++ ) {
            textoAlturas += "    0x" + alturas[ i ].toString( 16 );
            if ( i < alturas.length - 1 ) {
                textoAlturas += ",";
            }
            textoAlturas += "\n";
        }

        textoAlturas += "\n";
        textoAlturas += "};\n";
    }
    textoAlturas += "\n";
    textoAlturas += "\n";

    textoAlturas += "#endif // ALTURAS_H\n";
    textoAlturas += "\n";
    textoAlturas += "\n";

    grabarFicheroTexto( textoAlturas, pathAlturas, function() {
        console.log( "Alturas guardadas." );
    } );

    console.log( "Fin." );

} );


function grabarImagenPNG( canvas, pathImagen ) {

    var out = fs.createWriteStream( pathImagen );
    var stream = canvas.createPNGStream();

    stream.on( 'data', function( chunk ) {
        out.write( chunk );
    } );

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
