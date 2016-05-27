#    This file is part of Shipfix
# 
#    Shipfix  is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.



# configura aqui tu path a Node js:
node = /media/datos/soft/nodejs/node-v4.4.3-linux-x64/bin/node

# Todos los targets:
all: generarCargadorBASIC generarPantallaCarga compilarPantallaCarga crearTAPPantallaCarga generarTerreno generarImagenes generarTextosES compilarJuegoES crearTAPJuegoES concatenarTAPsES generarWavES mostrarTamTAP


# Targets comunes a todos los lenguajes:

generarCargadorBASIC:
	../herramientas/bas2tap/bas2tap -q -a10 -sShipFix ./cargadorBASIC/cargador.bas

generarPantallaCarga:
	$(node) ../herramientas/ProcesarImagenesSprites/main.js ../../graficos/pantallaCarga.png > ultimolog.txt
	cp ../herramientas/ProcesarImagenesSprites/imagenes.h ./imagenCarga.h

compilarPantallaCarga:
	zcc +zx -o p.bin -lndos modoRadastan.c cargadorPantallaCarga.c > ultimolog.txt
	
crearTAPPantallaCarga:
	$(node) ../herramientas/bin2tap-js/bin2tap.js ../../spectrum_src/p.bin > ultimolog.txt

generarTerreno:
	$(node) ../herramientas/ProcesarImagenTerreno/main.js ../../graficos/terreno1.png > ultimolog.txt
	cp ../herramientas/ProcesarImagenTerreno/alturas.h ./alturas.h

generarImagenes:
	$(node) ../herramientas/ProcesarImagenesSprites/main.js ../../graficos/imagenes1.png > ultimolog.txt
	cp ../herramientas/ProcesarImagenesSprites/imagenes.h ./imagenes.h


# Targets Lenguaje ES

generarTextosES:
	$(node) ../herramientas/ProcesarTextos/main.js ../../textos/textos_ES.txt > ultimolog.txt
	cp ../textos/textos_ES.h ./textos.h

compilarJuegoES:
	zcc +zx -o g_ES.bin -lndos modoRadastan.c mapa.c sonido.c juego.c > ultimolog.txt

crearTAPJuegoES:
	$(node) ../herramientas/bin2tap-js/bin2tap.js ../../spectrum_src/g_ES.bin > ultimolog.txt

concatenarTAPsES:
	cat ./cargadorBASIC/cargador.tap p.tap g_ES.tap > ../ShipFix.tap

generarWavES:
	tape2wav ../ShipFix.tap ../ShipFix.wav

# Fin Targets Lenguaje ES

mostrarTamTAP:
	ls -l ./g_ES.bin

clean:
	$(RM) *.bin *.i *.asm *.tap *.op* *.o *.reloc *~ zcc_opt.def
