TERCERA PRÁCTICA - JESÚS BALTASAR FERNÁNDEZ y CARLOS GÓMEZ FERNÁNDEZ

ORGANIZACIÓN DE ARCHIVOS Y DIRECTORIOS

	libros: directorio en el que se almacenan los archivos de texto sobre los que buscar palabra, los cuales son:
		17 LEYES DEL TRABAJO EN EQUIPO.txt: fichero de texto.
		El Oro Y La Ceniza - Abecassis Eliette.txt: fichero de texto.
		SEAMOS PERSONAS DE INFLUENCIA - JOHN MAXWELL.txt: fichero de texto.
		VIVE TU SUEÑO - JOHN MAXWELL.txt: fichero de texto.

	include: directorio en el que se almacenan librerias propias necesarias para la ejecución del programa, las cuales son:
		cliente.h: fichero con el código fuente correspondiente con la libreria cliente.
		colaProtegida.h: fichero con el código fuente correspondiente con la libreria colaProtegida.
		colores.h: fichero con el código fuente correspondiente con la libreria colores.
		semaforoContador.h: fichero con el código fuente correspondiente con la libreria semaforoContador.
		SSOOIIGLE.h: fichero con el código fuente correspondiente con la libreria SSOOIIGLE.

	src: directorio en el que se almacena el código del programa principal, el cual es:
		Buscador.cpp: fichero con el código fuente correspondiente con el programa Buscador.

	AutoevaluaciónP3_SSOOII_Baltasar_Gómez.pdf: fichero con la autoevaluación de los alumnos.

	MAKEFILE: fichero de ayuda a la compilación, ejecución y eliminación de archivos y directorios generados durante la compilación.

	README.txt: fichero de ayuda al usuario, sobre la distribución de archivos entre los directorios, la compilación del programa y su posterior ejecución.


COMPILACIÓN

	Generación de los directorios necesarios para la compilación:
		make dirs

	Compilación del programa principal:
		make SSOOIIGLE

	Generación de los directorios necesarios para la compilación y compilación del programa principal:
		make all


EJECUCIÓN

	Ejecución del programa "Buscador" buscando palabras aleatorias en los diferentes archivos de texto:
		make test


ELIMINACIÓN DE ARCHIVOS Y DIRECTORIOS GENERADOS EN LA COMPILACIÓN

	ELiminación de los archivos y directorios generados durante la compilación:
		make clean
