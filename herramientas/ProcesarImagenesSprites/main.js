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

//
// *** Entrada:
//
// Se acepta un parametro path de una imagen
// La imagen contiene paletas (filas de 16 pixels) e imagenes rectangulares:
// - Alineadas al borde izquierdo de la imagen completa.
// - El tamaño de pixels en horizontal debe ser par.
// - Max. tam. horizontal: 510 pixels (255 pares)
// - Max. tam. vertical: 255 pixels
// - Las paletas se aplican a las imagenes que siguen, hasta que llega otra paleta
// - El primer elemento debe ser una paleta, no una imagen
// - Cada elemento (paleta o imagen) esta separada por unos pixels especiales indicados por el pixel (0, 0)
// de la imagen completa.
// - Entre cada elemento debe haber al menos una linea de separacion antes del siguiente pixel especial.

// Formato de los pixeles especiales:
// El pixel (0, 0) indica el color del pixel especial (por ejemplo magenta)
// A continuacion viene una linea de separacion ignorada (puede ser negra)
//
// En los restantes elementos, un solo pixel especial indica que la siguiente linea contiene 16 pixels
// con la informacion de una paleta de 16 registros.

// Dos pixeles especiales consecutivos indican que en vez de una paleta a continuacion hay una imagen.
// Ademas debe haber un pixel especial a la derecha del ultimo pixel de la primera linea de la imagen,
// y otro pixel especial debajo del ultimo pixel de la primera columna de la imagen. De esta forma se delimita la imagen.
// Por comodidad, la imagen entera puede estar rodeada de pixeles especiales, excepto en el borde izquierdo.
//
// Tres pixeles especiales consecutivos indican como en el caso anterior una imagen, que sera duplicada espejada horizontalmente.
//
// Formato de paleta (# = pixel especial):
//
//     #
//     (16 pixels de la paleta)
//
//
// Formato de imagen (# = pixel especial)
// Puede haber dos pixeles especiales o tres en la primera linea:
//
//     ##
//     imagenimagenimagenimagenimagenimagen#
//     imagenimagenimagenimagenimagenimagen
//     imagenimagenimagenimagenimagenimagen
//     imagenimagenimagenimagenimagenimagen
//     imagenimagenimagenimagenimagenimagen
//     imagenimagenimagenimagenimagenimagen
//     imagenimagenimagenimagenimagenimagen
//     imagenimagenimagenimagenimagenimagen
//     #
//
//
// *** Salida:
//
//  La salida es el fichero imagenes.h con informacion para compilador C
//
//  Además, las paletas e imagenes de la imagen de entrada entrada se sobreescriben con los valores RGB332 corregidos para el modo radastan
//


// ***** Librerias *****

var fs = require( 'fs' );
var Canvas = require( 'canvas' );
var Image = Canvas.Image;


console.log( "ProcesarImagenesSprites");
console.log( "Uso: node main.js <path_relativo_imagen>" );
console.log( "" );


var PANTALLA_X  = 128;
var PANTALLA_X2 = PANTALLA_X * 0.5;
var PANTALLA_Y = 96;


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

    if ( tx < 16 ) {
        console.error( "Error: la imagen debe tener al menos 16 pixels de ancho." );
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

    function getPixelEspecial( x, y ) {
        var p = getPixelP( x, y );
        return datosPixels[ p ] === colorEspecial[ 0 ] &&
               datosPixels[ p + 1 ] === colorEspecial[ 1 ] &&
               datosPixels[ p + 2 ] === colorEspecial[ 2 ];
    }

    function convertirColorRGBARadasRGB332( r, g, b ) {

        // r, g, b de 0 a 255

        var rr = r & 0xE0;
        var gr = g & 0xE0;
        var br = b & 0xC0;
        if ( br !== 0 ) {
            br |= 0x20;
        }

        return [ rr, gr, br ];
    }

    function colorEstaEnPaleta( color, paleta ) {

        // Devuelve indice en la paleta, de 0 a 15, o -1 si no se encuentra

        for ( var i = 0; i < 16; i++ ) {
            if ( color[ 0 ] === paleta[ i ][ 0 ] &&
                 color[ 1 ] === paleta[ i ][ 1 ] &&
                 color[ 2 ] === paleta[ i ][ 2 ] ) {
             return i;
            }
        }

        return -1;
    }

    var paletas = [];
    var paletaActual = null;
    var imagenes = [];
    var colorEspecial = [];

    var posicionY = 0;

    // Obtiene el color especial = [ r, g, b ]
    colorEspecial.push( getPixelPR( 0 ) );
    colorEspecial.push( getPixelPG( 0 ) );
    colorEspecial.push( getPixelPB( 0 ) );

    while ( posicionY < ty ) {

        // Baja una linea
        posicionY += 2;

        if ( posicionY >= ty ) {
            break;
        }

        // Salta lineas
        while ( ! getPixelEspecial( 0, posicionY ) && posicionY < ty ) {
            posicionY++;
        }
        if ( posicionY >= ty ) {
            break;
        }

        if ( getPixelEspecial( 1, posicionY ) ) {

            // Es una imagen

            var espejar = getPixelEspecial( 2, posicionY );

            if ( paletaActual === null ) {
                console.error( "Error: El primer elemento debe ser una paleta, no una imagen." );
                process.exit( -1 );
            }

            posicionY++;
            if ( posicionY >= ty ) {
                console.error( "Error: Pixeles especiales indicando imagen al final de la imagen de entrada." );
                break;
            }

            var tamXImagen = 0;
            var tamYImagen = 0;

            // Busca el ancho de la imagen
            for ( var i = 0; i < tx; i++ ) {
                if ( getPixelEspecial( i, posicionY ) ) {
                    tamXImagen = i;
                    break;
                }
            }
            if ( tamXImagen === 0 ) {
                console.error( "Error: imagen de tamaño X = 0, o no está bien definida. (x,y)=(" + i + ", " + posicionY + ")" );
                process.exit( -1 );
            }

            if ( tamXImagen % 2 !== 0 ) {
                console.error( "Error: imagen de tamaño X impar. TamX=" + tamXImagen );
                process.exit( -1 );
            }

            // Busca el alto de la imagen
            for ( var j = posicionY; j < ty; j++ ) {
                if ( getPixelEspecial( 0, j ) ) {
                    tamYImagen = j - posicionY;
                    break;
                }
            }
            if ( tamYImagen === 0 ) {
                console.error( "Error: imagen de tamaño Y = 0, o no está bien definida. (x,y)=(" + 0 + ", " + j + ")" );
                process.exit( -1 );
            }

            var indicesImagen = [];
            for ( var j = 0; j < tamYImagen; j++ ) {
                for ( var i = 0; i < tamXImagen; i++ ) {
                    var p = getPixelP( i, posicionY + j );
                    var r = getPixelPR( p );
                    var g = getPixelPG( p );
                    var b = getPixelPB( p );
                    var color = convertirColorRGBARadasRGB332( r, g, b );

                    var indice = colorEstaEnPaleta( color, paletaActual );

                    if ( indice === -1 ) {
                        console.error( "Error: pixel no está en la paleta actual. (x,y) del pixel = (" + i + ", " + ( posicionY + j ) + ", (r,g,b) = (" + r + ", " + g + ", " + b + ")" );
                        process.exit( -1 );
                    }

                    indicesImagen.push( indice );

                    setPixel( i, posicionY + j, color[ 0 ], color[ 1 ], color[ 2 ], 255 );
                }
            }

            var imagen = {
                tamX: tamXImagen,
                tamY: tamYImagen,
                indices: indicesImagen
            };

            imagenes.push( imagen );


            if ( espejar ) {
                indicesImagen = [];
                for ( var j = 0; j < tamYImagen; j++ ) {
                    for ( var i = tamXImagen - 1; i >= 0; i-- ) {
                        var p = getPixelP( i, posicionY + j );
                        var r = getPixelPR( p );
                        var g = getPixelPG( p );
                        var b = getPixelPB( p );
                        var color = convertirColorRGBARadasRGB332( r, g, b );

                        var indice = colorEstaEnPaleta( color, paletaActual );

                        indicesImagen.push( indice );
                    }
                }

                imagen = {
                    tamX: tamXImagen,
                    tamY: tamYImagen,
                    indices: indicesImagen
                };

                imagenes.push( imagen );

            }

            posicionY += tamYImagen;

        }
        else {
            // Es una paleta
            posicionY++;
            if ( posicionY >= ty ) {
               break;
            }
            var paleta = [];
            for ( var i = 0; i < 16; i++ ) {
                p = getPixelP( i, posicionY );
                var r = getPixelPR( p );
                var g = getPixelPG( p );
                var b = getPixelPB( p );
                var color = convertirColorRGBARadasRGB332( r, g, b );
                paleta.push( color );
                setPixel( i, posicionY, color[ 0 ], color[ 1 ], color[ 2 ], 255 );
            }
            paletas.push( paleta );
            paletaActual = paleta;
        }

    }

    if ( imagenes.length === 0 ) {
        console.error( "Error: no hay imagenes que procesar." );
        process.exit( -1 );
    }

    ctx2dNivel.putImageData( datosImagen, 0, 0 );

    console.log( "Guardando imagen..." );

    grabarImagenPNG( elCanvas, pathImagen );




    var pathImagenes = __dirname + "/imagenes.h";


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

    texto += "\n\n#ifndef IMAGENES_H\n";
    texto += "#define IMAGENES_H\n\n";

    texto += "#include \"modoRadastan.h\"\n\n";

    texto += "#define NUM_PALETAS " + paletas.length + "\n\n";

    // Escribe las paletas

    texto += "static unsigned char paletas [] = {\n\n";

    texto += "    // Pixels en formato gggrrrbb\n\n"

    for ( var ip = 0, lp = paletas.length; ip < lp; ip++ ) {
        var paleta = paletas[ ip ];
        for ( var ic = 0; ic < 16; ic++ ) {
            var col = paleta[ ic ];
            var rr = ( col[ 1 ] & 0xE0 ) >> 5;
            var gr = ( col[ 0 ] & 0xE0 ) >> 5;
            var br = ( col[ 2 ] & 0xC0 ) >> 6;

            var valorBytePaleta = ( rr << 5 ) | ( gr << 2 ) | br;

            var char2 = valorBytePaleta.toString( 16 );

            texto += "    0x" + char2 + ( ip === lp - 1 && ic === 15 ? "\n" : ",\n" );
        }
    }

    texto += "};\n\n";


    texto += "\n";
    texto += "\n";

    // Escribe imagenes

    function getNombreImagen( i ) {
        var nombreImagen = "" + i;
        while ( nombreImagen.length < 3 ) {
            nombreImagen = "0" + nombreImagen;
        }
        return nombreImagen;
    }

    for ( var iImagen = 0, nImagenes = imagenes.length; iImagen < nImagenes; iImagen++ ) {

        var nombreImagen = getNombreImagen( iImagen );

        texto += "static unsigned char pixelsImagen" + nombreImagen + "[] = {\n"

        var imagen = imagenes[ iImagen ];

        var p = 0;
        for ( var iPixel = 0, nPixels = imagen.indices.length; iPixel < nPixels; iPixel+= 2 ) {
            var indicePixel1 = imagen.indices[ p++ ];
            var indicePixel2 = imagen.indices[ p++ ];
            texto += "    0x" + indicePixel1.toString( 16 ) + indicePixel2.toString( 16 ) + ( iPixel === nPixels - 2 ? "\n" : ", \n" );
        }

        texto +=  "};\n\n";

    }

    texto += "#define NUM_IMAGENES " + imagenes.length + "\n\n\n";

    texto += "static Imagen imagenes[] = {\n\n"

    texto += "    // tamx2, tamy, imagen\n\n";

    for ( var iImagen = 0, nImagenes = imagenes.length; iImagen < nImagenes; iImagen++ ) {

        var imagen = imagenes[ iImagen ];

        var nombreImagen = getNombreImagen( iImagen );

        texto += "    {\n";
        texto += "        " + ( imagen.tamX * 0.5 ) + ", " + imagen.tamY + ", pixelsImagen" + nombreImagen + "\n";
        texto += iImagen === nImagenes - 1 ? "    }\n" : "    },\n";
    }

    texto += "};\n\n\n";

    texto += "#endif // IMAGENES_H\n\n\n";

    console.log( "Guardando fichero imagenes.h ..." );

    grabarFicheroTexto( texto, pathImagenes, function() {
        console.log( "Fichero con imagenes guardado." );
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
