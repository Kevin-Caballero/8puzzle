/**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas les funciones que tenéis 
 * que implementar en ensamblador.
 * Desde este código se hacen las llamadas a les subrutinas de ensamblador. 
 * ESTE CÓDIGO NO SE PUEDE MODIFICAR Y NO HAY QUE ENTREGARLO.
 * */
 
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>     //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>      //STDIN_FILENO

extern int developer;   //Variable declarada en ensamblador que indica el nombre del programador

/**
 * Constantes
 */
#define DimMatrix  3     //dimensión de la matriz 3x3
#define SizeMatrix DimMatrix*DimMatrix //=9


/**
 * Definición de variables globales
 */
int spacePos = 0; //Posición del espacio dentro de la matriz tiles (3x3),
                  //spacePos [0 : (SizeMatrix-1)].
                  //Fila = pos / DimMatrix  [0 : (DimMatrix-1)]
                  //Columna = pos % DimMatrix [0 : (DimMatrix-1)]
int cursorPos = 0;//Posición del cursor dentro de la matriz tiles (3x3),
                  //spacePos [0 : (SizeMatrix-1)].
                  //Fila = pos / DimMatrix  [0 : (DimMatrix-1)]
                  //Columna = pos % DimMatrix [0 : (DimMatrix-1)]
   
short state = 1;  //0: Salir, hemos pulsado la tecla 'ESC' para salir.
                  //1: Continuamos jugando.
                  //2: No se ha podido hacer el movimiento.
                  //3: Pierdes, no quedan movimientos.
                  //5: Error, no se ha encontrado el espacio.
   
long moves = 9;   //Movimientos que quedan para ordenar les fichas.
   
int  rowScreen;  //Fila para posicionar el cursor en pantalla.
int  colScreen;  //Columna para posicionar el cursor en pantalla.
char charac;     //Carácter leído de teclado y para escribir en pantalla. 

// Matriz 3x3 donde guardamos las fichas iniciales del juego.
char tilesIni[DimMatrix][DimMatrix] = { {'1','2','3'},
                                        {'4','C',' '},                                     
                                        {'B','D','A'} }; 

// Matriz 3x3 donde guardamos las fichas del juego.
char tiles[DimMatrix][DimMatrix]    = { {'1','2','3'},
                                        {'4','X','A'},
                                        {'B','C','D'} }; 

// matrizs 3x3 donde guardamos las fichas en la posición que queremos conseguir.
char tilesEnd[DimMatrix][DimMatrix] = { {'1','2','3'},
                                        {'4',' ','A'},
                                        {'B','C','D'} };
                                        
/**
 * Definición de les funciones de C
 */
void clearscreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();

void printMenuP1_C();
void printBoardP1_C();

void copyMatrixP1_C();
void updateBoardP1_C();
void getSpacePosP1_C();
void posCurScreenP1_C();
void moveCursorP1_C();
void moveTileP1_C();
void checkEndP1_C();
void printMessageP1_C();
void playP1_C();

/**
 * Definición de las subrutinas de ensamblador que se llaman desde C
 */
void copyMatrixP1();
void updateBoardP1();
void getSpacePosP1();
void posCurScreenP1();
void moveCursorP1();
void moveTileP1();
void checkEndP1();
void playP1();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:	
 * Ninguna
 * 
 * Esta función no es llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void clearScreen_C(){
	
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en una fila indicada por la variable (rowScreen) y 
 * en la columna indicada por la variable (colScreen) de la pantalla.
 * 
 * Variables globales utilizadas:	
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Se ha definido una subrutina en ensamblador equivalente 'gotoxyP1'  
 * para poder llamar a esta función guardando el estado de los registros 
 * del procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 */
void gotoxyP1_C(){
	
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter guardado en la variable (charac) en pantalla,
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:	
 * (charac): Carácter que queremos mostrar.
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printchP1' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 */
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Leer una tecla y guardar el carácter asociado en la variable (charac)
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:	
 * (charac): Caràcter que llegim de teclat.
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'getchP1' para
 * llamar a esta función guardando el estado de los registros del procesador.
 * Esto se hace porque las funciones de C no mantienen el estado de los 
 * registros.
 */
void getchP1_C(){

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándard (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

    /* ~ICANON para tratar la entrada de teclado carácter a carácter no como línea entera acabada en /n
    ~ECHO para que no se muestre el carácter leído.*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. */
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   charac = (char) getchar();              
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}


/**
 * Mostrar en pantalla el menú del juego y pedir una opción.
 * Sólo acepta una de las opciones correctas del menú ('0'-'9')
 * 
 * Variables globales utilizadas:		
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * (charac)   : Carácter que leemos de teclado.
 * (developer): ((char *)&developer): Variable definida en el código ensamblador.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printMenuP1_C(){

   clearScreen_C();
   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();
   printf("                           \n");
   printf("        Developed by:      \n");
   printf("     ( %s )    \n",(char *)&developer);
   printf(" _________________________ \n");
   printf("|                         |\n");
   printf("|         MAIN MENU       |\n");
   printf("|_________________________|\n");
   printf("|                         |\n");
   printf("|     1. copyMatrix       |\n");
   printf("|     2. updateBoard      |\n");
   printf("|     3. getSpacePos      |\n");
   printf("|     4. posCurScreen     |\n");
   printf("|     5. moveCursor       |\n");
   printf("|     6. moveTile         |\n");
   printf("|     7. checkEnd         |\n");
   printf("|     8. Play Game        |\n");
   printf("|     9. Play Game C      |\n");
   printf("|     0. Exit             |\n");
   printf("|_________________________|\n");
   printf("|                         |\n");
   printf("|        OPTION:          |\n");
   printf("|_________________________|\n"); 

   charac =' ';
   while (charac < '0' || charac > '9') {
      rowScreen = 21;
      colScreen = 18;
      gotoxyP1_C();           //Posicionar el cursor
      getchP1_C();            //Lleer una opción
   }
   
}


/**
 * Mostrar el tablero de juego en pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:	
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 *  
 * Esta función se llama des de C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printBoardP1_C(){

   rowScreen = 0;
   colScreen = 0;
   gotoxyP1_C();                                      //Filas
                                                      //Tablero
   printf(" _____________________________________ \n"); //01
   printf("|                                     |\n"); //02
   printf("|           8-PUZZLE  v1.0            |\n"); //03
   printf("|                                     |\n"); //04
   printf("|      Order the tiles as shown!      |\n"); //05
   printf("|_____________________________________|\n"); //06
   printf("|                                     |\n"); //07
   printf("|  Remaining Moves[_]    FINAL GOAL   |\n"); //08
   printf("|                                     |\n"); //09
   //             8   12  16                           <- Columnas Tablero               
   printf("|      0   1   2         0   1   2    |\n"); //10
   printf("|    +---+---+---+     +---+---+---+  |\n"); //11
   printf("|  0 |   |   |   |   0 | 1 | 2 | 3 |  |\n"); //12
   printf("|    +---+---+---+     +---+---+---+  |\n"); //13
   printf("|  1 |   |   |   |   1 | 4 |   | A |  |\n"); //14
   printf("|    +---+---+---+     +---+---+---+  |\n"); //15
   printf("|  2 |   |   |   |   2 | B | C | D |  |\n"); //16
   printf("|    +---+---+---+     +---+---+---+  |\n"); //17
   printf("|_____________________________________|\n"); //18
   printf("|                                     |\n"); //19
   printf("|                                     |\n"); //20
   printf("|_____________________________________|\n"); //21
   printf("|                                     |\n"); //22
   printf("|   (i)Up (j)Left (k)Down (l)Right    |\n"); //23
   printf("|   (Space) move tile  (ESC)  Exit    |\n"); //24
   printf("|_____________________________________|\n"); //25
            
}


/**
 * Copiar la matriz (tilesIni), sobre la matriz (tiles).
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
 * a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
 * tipo char(BYTE) 1 byte.
 * 
 * Variables globales utilizadas:   
 * (tilesIni): Matriz con las fichas iniciales del juego
 * (tiles)   : Matriz donde guardamos la fichas del juego.
 *
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'copyMatrixP1'.
 */
void copyMatrixP1_C(){
   
   int i, j;

   for (i=0;i<DimMatrix;i++){
      for (j=0;j<DimMatrix;j++){
         tiles[i][j] = tilesIni[i][j];
      }
   }

}


/**
 * Mostrar los valores de la matriz (tiles), en pantalla, 
 * dentro del tablero en las posiciones correspondientes.
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
 * a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
 * tipo char(BYTE) 1 byte, y para cada elemento de la matriz:
 * Posicionar el cursor en el tablero llamando a la función gotoxyP1_C.
 * La posición inicial del cursor es la fila 12 de la pantalla (fila 0 
 * de la matriz), columna 8 de la pantalla (columna 0 de la matriz).
 * Mostrar los caracteres de cada posición de la matriz (tiles) 
 * llamando a la función printchP1_C.
 * Actualizar la columna (colScreen) de 4 en 4 y al cambiar de fila
 * (rowScreen) de 2 en 2.
 * Mostrar los movimientos que quedan por hacer (moves) dentro del tablero 
 * en la fila 8, columna 20 de la pantalla.
 * 
 * Variables globales utilizadas:   
 * (tiles)    : Matriz donde guardamos las fichas del juego.
 * (moves)    : Movimientos que quedan para ordenar las fichas.
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * (charac)   : Carácter a escribir en pantalla.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'updateBoardP1'.
 */
void updateBoardP1_C(){

   int i, j;
   rowScreen = 12;
   for (i=0;i<DimMatrix;i++){
	  colScreen = 8;
      for (j=0;j<DimMatrix;j++){
         gotoxyP1_C();
         charac = tiles[i][j];
         printchP1_C();
         colScreen = colScreen + 4;
      }
      rowScreen = rowScreen + 2;
   }
   
   rowScreen = 8;
   
   gotoxyP1_C();
   charac = moves;
   charac = charac + '0';
   printchP1_C();

}

/**
 * Buscar donde está el espacio en blanco dentro de la matriz (tiles), 
 * y actualizar la posición (spacePos) del espacio dentro de la matriz.
 * Recorrer toda la matriz por filas de izquierda a derecha y de arriba a bajo.
 * Si el número de espacios no es 1 (spacePos = sizeMatrix).
 * Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
 * a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
 * tipo char(BYTE) 1 byte.
 * 
 * Variables globales utilizadas:   
 * (tiles)   : Matriz donde guardamos las fichas del juego.
 * (spacePos): Posición del espacio dentro de la matriz.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'getSpacePosP1'.
 */
void getSpacePosP1_C(){

   int i=0, j=0, spaces=0, pos=0; 
   for (i=0;i<DimMatrix;i++){
      for (j=0;j<DimMatrix;j++){
		 if (tiles[i][j]==' ') {
			 spaces++;
			 spacePos = pos;
         }
         pos++;
       }
   }
   
   if (spaces!=1){  
   	  spacePos = SizeMatrix;
   }  
    
}


/**
 * Posicionar el cursor en la pantalla, dentro del panel, en función de
 * la variable (cursorPos).
 * Para calcular la posición del cursor a pantalla (rowScreen) y
 * (colScreen) utilizar estas fórmulas:
 * rowScreen=12+(cursorPos/DimMatrix)*2
 * colScreen= 8+(cursorPos%DimMatrix)*4
 * Para posicionar el cursor llamar a la funció gotoxyP1_C.
 * 
 * Variables globales utilizadas:   
 * (cursorPos): Posición del cursor en la matriu (tiles).
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'posCurScreenP1'.
 */
void posCurScreenP1_C() {

   rowScreen=12+(cursorPos/DimMatrix)*2;
   colScreen= 8+(cursorPos%DimMatrix)*4;
   gotoxyP1_C();
   
}


/**
 * Mover el cursor en la dirección indicada por la variable (charac),
 * ('i':arriba, 'k':bajo, 'j':izquierda o 'l':derecha).
 * Actualizar la variable (cursorPos) controlando que
 * no salga del tablero.
 *
 * No se debe posicionar el cursor en la pantalla.
 *
 * Variables globales utilizadas:
 * (charac)   : Carácter leído de teclado.
 * (cursorPos): Posición del cursor en la matriz (tiles).
 *
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina en ensamblador equivalente 'moveCursorP1'.
 */
void moveCursorP1_C(){

   int i = cursorPos/DimMatrix;
   int j = cursorPos%DimMatrix;
   
   switch(charac){
      case 'i':
         if (i > 0) {
			cursorPos = cursorPos - DimMatrix;
         }
      break;
      case 'k':
         if (i < (DimMatrix-1)) {
			cursorPos = cursorPos + DimMatrix;
         }
      break;
      case 'j':
         if (j > 0) {
			cursorPos = cursorPos - 1;
         }
      break;
      case 'l':
         if (j < (DimMatrix-1)) {
			cursorPos = cursorPos + 1;
         }
      break;
   }

}


/**
 * Mover la ficha de la casilla donde está el cursor (cursorPos)
 * a la casilla donde está el espacio (spacePos) si están
 * de lado (arriba, abajo, izquierda o derecha).
 *
 * Si la casilla donde está el espacio está al lado de la casilla donde
 * está el cursor en la matriz (tiles):
 * - Mover la ficha donde está el cursor a la casilla donde está el espacio y
 * poner el espacio a la casilla donde está el cursor, en la matriz (tiles).
 * Si no, no hacer el movimiento y poner (state=2).
 *
 * No se debe mostrar la matriz con los cambios, se hace en updateBoardP1_C().
 *
 * Variables globales utilizadas:
 * (tiles)    : Matriz donde guardamos las fichas del juego.
 * (cursorPos): Posición del cursor en la matriz (tiles).
 * (spacePos) : Posición del espacio en la matriz (tiles).
 * (state)    : Estado del juego.
 *
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'moveTilesP1'.
 */
void moveTileP1_C(){

   int  rowCur  =(cursorPos/DimMatrix);
   int  colCur  =(cursorPos%DimMatrix);
   int  rowSpace=(spacePos/DimMatrix);
   int  colSpace=(spacePos%DimMatrix);
   
   if (  ( (rowCur==rowSpace) && ( (colCur==(colSpace-1)) || (colCur==(colSpace+1)) ) )
      || ( (colCur==colSpace) && ( (rowCur==(rowSpace-1)) || (rowCur==(rowSpace+1)) ) )  ){	 

      tiles[rowSpace][colSpace]	= tiles[rowCur][colCur];
      tiles[rowCur][colCur]		= ' ';
      
   } else {
      state = 2;
   }

}

/**
 * Verificar si se han agotado los movimentos.
 * Si (moves == 0) poner (state=3).
 * 
 * Variables globales utilizadas:   
 * (moves): Movimientos que quedan para ordenar les fichas
 * (state): Estado del juego.
 *          3: Pierdes, no quedan movimientos.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'checkEnP1'.
 */
void checkEndP1_C() {
   
   if (moves == 0) {
      state = 3;
   } 
   
}


/**
 * Mostrar un mensaje debajo del tablero según el valor de la variable 
 * (state).
 * state: 0: Salir, hemos pulsado la tecla 'ESC' para salir.
 *        1: Continuamos jugando.
 *        2: No se ha podido hacer el movimiento.
 *        3: Pierdes, no quedan movimientos.
 *        5: Error, no se ha encontrado el espacio.
 * Se espera que se pulse una tecla para continuar.
 * 
 * Variables globales utilizadas:   
 * (status)   : Estado del juego.
 * (rowScreen): Fila de la pantalla donde posicionamos el cursor.
 * (colScreen): Columna de la pantalla donde posicionamos el cursor.
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printMessageP1' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 */
void printMessageP1_C() {

   rowScreen = 20;
   colScreen = 3;
   gotoxyP1_C();
   
   switch(state){
      case 0:
         printf("<<<<<<< EXIT: (ESC) Pressed >>>>>>>");
      break;
      case 1:
         printf("===========  NEXT MOVE  ===========");
      break;
      case 2:
         printf("**********  CAN'T  MOVE  **********");
      break;
      case 3:
         printf("------ SORRY, NO MORE MOVES! ------");
      break;
      case 5:
         printf("xxxxxxxx ERROR -> NO SPACE xxxxxxxx");
      break;
   }
   
}


/**
 * Juego del 8-PUZZLE.
 * Función principal del juego.
 * Permite jugar al juego del 8-PUZZLE llamando todas las funcionalidades.
 * 
 * Pseudo-código:
 * Mostrar el tablero de juego llamando a la función printBoardP1_C.
 * Inicializar el estado del juego, (state=1).
 * Inicializar los movimientos que se pueden realizar (moves = 9).
 * Inicializar la matriz (tiles) con los valores de la matriz (tilesIni)
 * llamando la función copyMatrixP1_C.
 * Actualizar el tablero de juego llamando a la función updateBoardP1_C.
 * Busca dónde está el espacio en blanco dentro de la matriz (tiles) llamando
 * la función getSpacePosP1_C.
 * Si sólo hay un espacio (spacePos<sizeMatrix) posicionar el
 * cursor en el lugar donde está el espacio (cursorPos = spacePos),
 * sino, poner (state=5) y mostrar el mensaje para
 * indicarlo llamando la función printMessageP1_C.
 * Mientras (state==1) hacer:
 *   Posicionar el cursor dentro del tablero llamando a la función posCurScreenP1_C
 *   Leer una tecla (charac) llamando a la función getchP1_C.
 *   Según la tecla leída haremos lo siguiente:
 *   - ['i','j','k' o 'l'] desplazar el cursor según la dirección
 *     elegida (llamar a la función moveTileP1_C).
 *   - [' '] Mover la ficha de la casilla donde está el cursor llamando a
 *     la función moveTileP1_C, si se puede hacer el movimiento (state!=2)
 *     poner el espacio donde ya está el cursor (spacePos=cursorPos)
 *     y decrementar los movimientos que quedan (moves).
 *   - '<ESC>' (código ASCII 27) poner (state = 0) para salir.
 *   Actualizar el tablero de juego llamando a la función updateBoardP1_C.
 *   Verificar si se ha agotado los movimientos llamando la función checkEndP1_C.
 *   Mostrar un mensaje debajo del tablero según el valor de la
 *   variable (state) llamando la función printMessageP1_C.
 *   Si no se ha podido hacer movimiento (state == 2) poner
 *   (state = 1) para continuar jugando.
 * Fin mientras.
 *
 * Antes de salir, esperar a que se pulse una tecla llamando la función getchP1_C para terminar.
 * 
 * Variables globales utilizadas:   
 * (moves)    : Movimientos que quedan para ordenar les fichas.
 * (spacePos) : Posición del espacio dentro de la matriz (tiles(.
 * (cursorPos): Posición del cursor en la matriz (tiles).
 * (charac)   : Carácter a escribir en pantalla.
 * (state)    : Estado del juego.
 *              1: Continuamos jugando.
 *              2: No se ha podido hacer el movimiento.
 *              3: Pierdes, no quedan movimientos.
 *              5: Error, no se ha encontrado el espacio.
 * 
 * Esta función no se llama desde ensamblador.
 * Hay una subrutina de ensamblador equivalente 'playP1', para  
 * llamar les subrutinas del juego definidas en ensamblador.
 */
void playP1_C(){

   printBoardP1_C();
   state = 1;
   moves = 9;
   copyMatrixP1_C();
   updateBoardP1_C();
   getSpacePosP1_C();
   if (spacePos < SizeMatrix) {
      cursorPos = spacePos; 
   } else {
      state = 5;
      printMessageP1_C();
   }
   while (state == 1) {     //Bucle principal.
	   
	  posCurScreenP1_C();   

      getchP1_C();                   
      if (charac >= 'i' && charac <= 'l') {
		 moveCursorP1_C();
      } else if (charac == ' ') {
		  moveTileP1_C();
   	      if (state != 2) {
              spacePos = cursorPos; 
              moves--;
          }
      } else if (charac == 27) {
            state = 0;
      }
      
      updateBoardP1_C();
      checkEndP1_C();
      printMessageP1_C();
      if (state == 2) state = 1; 
      
   }
   getchP1_C();     
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓN: Podéis probar la funcionalidad de las subrutinas que se tienen
 * que desarrollar eliminando los comentarios de la llamada a la función 
 * equivalente implementada en C que hay debajo de cada opción.
 * Para al juego completo hay una opción para la versión en ensamblador y 
 * una opción para el juego en C.
 */
int main(void){   
   charac = ' ';
   while (charac != '0') {
     printMenuP1_C();   
     switch(charac){
        case '1'://Copiar la matriz tilesIni a tiles.
          printBoardP1_C();
          //=======================================================
            copyMatrixP1();
            //copyMatrixP1_C(); 
          //=======================================================
          updateBoardP1_C();
          rowScreen = 20;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press any key ...");
          getchP1_C();
        break;
        case '2'://Actualitzar el contenido del tablero.  
          printBoardP1_C();      
          //=======================================================
            updateBoardP1();
            //updateBoardP1_C();
          //=======================================================
          rowScreen = 20;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press any key ...");
          getchP1_C();
        break;
        case '3': //Buscar donde está el espacio en blanco dentro de la matriu (tiles).
          printBoardP1_C();
          updateBoardP1_C();
          //=======================================================        
            getSpacePosP1();
            //getSpacePosP1_C();
          //=======================================================
          if (spacePos < SizeMatrix) {
			 rowScreen = 20;
             colScreen = 12;
             gotoxyP1_C();
             printf("Press any key ...");
             cursorPos = spacePos;
             posCurScreenP1_C();
          } else {
             rowScreen = 20;
             colScreen = 3;
             gotoxyP1_C();
             printf("ERROR->ONLY 1 SPACE: Press any key ");
          } 
          getchP1_C();
        break;
        case '4'://Posicionar el cursor donde hay el espacio.  
          printBoardP1_C();
          updateBoardP1_C();
          getSpacePosP1_C();
          cursorPos = spacePos;
          rowScreen = 20;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press any key ..."); 
          //=======================================================
             posCurScreenP1();
             //posCurScreenP1_C();
          //=======================================================
          getchP1_C();
        break;
        case '5'://Mover el cursor en la dirección indicada.  
          printBoardP1_C();
          rowScreen = 20;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press i,j,k,l: ");
          updateBoardP1_C();
          getSpacePosP1_C();
          cursorPos = spacePos;
          posCurScreenP1_C();
          getchP1_C();   
          if (charac>='i' && charac<='l') {
          //=======================================================
             moveCursorP1();
             //moveCursorP1_C();
          //=======================================================
          }
          rowScreen = 20;
          colScreen = 12;
          gotoxyP1_C();
          printf(" Press any key ...");
          posCurScreenP1_C();
          getchP1_C();
        break;
        case '6'://Mover la fitxa donde está el espacio.
          state=1;
          printBoardP1_C();
          rowScreen = 20;
          colScreen = 12;
          gotoxyP1_C();
          printf("Press <SPACE>: ");
          updateBoardP1_C();
          getSpacePosP1_C();
          posCurScreenP1_C();
          getchP1_C();   
          if (charac>=' ') {
          //=======================================================
             moveTileP1();
             //moveTileP1_C();
          //=======================================================
	      }
          updateBoardP1_C();
          if (state != 2) {
             rowScreen = 20;
             colScreen = 12;
             gotoxyP1_C();
             printf(" Press any key ...");
             spacePos = cursorPos;
             posCurScreenP1_C();
          } else {			 
             printMessageP1_C();
          }
          getchP1_C();
        break;
        case '7': //Verificar el estado del juego.
          printBoardP1_C();    
          moves = 5;
          state = 1;
          ///moves = 0; //-> state=3 (NO MORE MOVES)
          updateBoardP1_C();
          posCurScreenP1_C();
          //=======================================================
            checkEndP1();
            //checkEndP1_C();
          //=======================================================
          printMessageP1_C();
          rowScreen = 26;
          colScreen = 12;
          gotoxyP1_C();
          printf(" Press any key ...");
          getchP1_C();  
        break;
        case '8': //Juego completo en Ensamblador. 
          //=======================================================
          playP1();
          //=======================================================
        break;
        case '9': //Juego completo en C.    
          //=======================================================
          playP1_C();
          //=======================================================
        break;
     }
  }
  printf("\n\n");

  return 0;
  
}
