%macro calculateCursorPosition 2
   mov  rax, [spacePos]
   mov  rbx, [DimMatrix]
   
   idiv rbx                        ;rax/rbx = cursorPos/DimMatrix
   shl rax, %1                      ;corrimiento de bits a la izda (multiplicar por 2)
   add rax, %2
%endmacro

section .data               
;Cambiar Nombre y Apellido por vuestros datos.
developer db "Kevin Caballero",0

;Constantes que también están definidas en C.
DimMatrix    equ 3
SizeMatrix   equ DimMatrix*DimMatrix ;=9 

section .text            

;Variables definidas en Ensamblador.
global developer                        

;Subrutinas de ensamblador que se llaman desde C.
global copyMatrixP1, updateBoardP1, getSpacePosP1, posCurScreenP1,
global moveCursorP1, moveTileP1, checkEndP1, playP1

;Variables definidas en C.
extern spacePos, cursorPos, state, moves, rowScreen, colScreen, charac
extern tilesIni, tiles, tilesEnd

;Funciones de C que se llaman desde ensamblador.
extern clearScreen_C, printBoardP1_C, gotoxyP1_C, printchP1_C, getchP1_C
extern printMessageP1_C


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATENCIÓN: Recordad que en ensamblador las variables y los parámetros 
;;   de tipo 'char' se tienen que asignar a registros de tipo
;;   BYTE (1 byte): al, ah, bl, bh, cl, ch, dl, dh, sil, dil, ..., r15b
;;   las de tipo 'short' se tiene que asignar a registros de tipo 
;;   WORD (2 bytes): ax, bx, cx, dx, si, di, ...., r15w
;;   las de tipo 'int' se tiene que asignar a registros de tipo  
;;   DWORD (4 bytes): eax, ebx, ecx, edx, esi, edi, ...., r15d
;;   las de tipo 'long' se tiene que asignar a registros de tipo 
;;   QWORD (8 bytes): rax, rbx, rcx, rdx, rsi, rdi, ...., r15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Les subrutinas en ensamblador que hay que implementar son:
;;   copyMatrixP1, updateBoardP1, getSpacePosP1, posCurScreenP1 
;;   moveCursorP1, moveTileP1, checkEndP1
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Situar el cursor en una fila indicada por la variable (rowScreen) y en 
; una columna indicada por la variable (colScreen) de pantalla 
; llamando a la función gotoxyP1_C.
; 
; Variables globales utilizadas:	
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call gotoxyP1_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Mostrar un carácter guradado en la varaile (charac) en pantalla, en
; la posición donde está el cursor llamando a la función printchP1_C.
; 
; Variables globales utilizadas:	
; (charac): Carácter que queremos mostrar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printchP1_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Leer una tecla y guardar el carácter asociado en la varaible (charac) 
; sin mostrarlo en pantalla, llamando a la función getchP1_C
; 
; Variables globales utilizadas:	
; (charac): Carácter que queremos mostrar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call getchP1_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret 


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copiar la matriz (tilesIni), sobre la matriz (tiles).
; Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
; Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
; a 8 (posición [2][2]) con incrementos de 1 porque los datos son de 
; tipo char(BYTE) 1 byte.
;
; Variables globales utilizadas:   
; (tilesIni): Matriz con las fichas iniciales del juego
; (tiles)   : Matriz donde guardamos la fichas del juego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
copyMatrixP1:
   push rbp
   mov  rbp, rsp

   mov rsi, tilesIni
   mov rdi, tiles
   mov rcx, 0
   copy:
		mov ax, [rsi]
		mov [rdi], ax
		add rsi,1              ;mover el puntero del array un byte para situarlo al inicio del siguiente valor del source
		add rdi,1              ;mover el puntero del array un byte para situarlo al inicio del siguiente valor del target
		add rcx,1              ;incrementar indice
		cmp rcx, SizeMatrix    ;comparar indice con la longitud del array
		jl copy                ;si es menor salta a ciclo

   mov rsp, rbp
   pop rbp
   ret
 
 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar los valores de la matriz (tiles), en pantalla, 
; dentro del tablero en las posiciones correspondientes.
; Recorrer toda la matriz por filas de izquierda a derecha y de arriba a abajo.
; Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
; a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
; tipo char(BYTE) 1 byte, y para cada elemento de la matriz:
; Posicionar el cursor en el tablero llamando a la función gotoxyP1_C.
; La posición inicial del cursor es la fila 12 de la pantalla (fila 0 
; de la matriz), columna 8 de la pantalla (columna 0 de la matriz).
; Mostrar los caracteres de cada posición de la matriz (tiles) 
; llamando a la función printchP1_C.
; Actualizar la columna (colScreen) de 4 en 4 y al cambiar de fila
; (rowScreen) de 2 en 2.
; Mostrar los movimientos que quedan por hacer (moves) dentro del tablero 
; en la fila 8, columna 20 de la pantalla.
; 
; Variables globales utilizadas:   
; (tiles)    : Matriz donde guardamos las fichas del juego.
; (moves)    : Movimientos que quedan para ordenar las fichas.
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
; (charac)   : Carácter a escribir en pantalla.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
updateBoardP1:
   push rbp
   mov  rbp, rsp
   
   mov rdi, tiles                     ;posicionar en rdi el inicio de tiles
   
   mov byte [rowScreen], 8            ;posicion inicial row cursor
   mov byte [colScreen], 20           ;posicion inicial col cursor
   mov si, [moves]
   mov word [charac], si             ;mover el valor a charac
   call gotoxyP1
   call printchP1                     ;llamada a pintar en pantalla
   
   mov byte [rowScreen], 10           ;posicion inicial row cursor
   mov byte [colScreen], 8            ;posicion inicial col cursor
   mov byte [charac], '-'
   
   mov rcx, 1                         ;array index
   row_loop:
		mov rsi,0                     ;row index
		col_loop:
			mov al, [rdi]             ;mover a al el valor de la posicion actual de tiles(rdi)
			mov byte [charac], al     ;mover el valor a charac
			call gotoxyP1
			call printchP1            ;llamada a pintar en pantalla
			inc rdi                   ;mover el puntero del array un byte para situarlo al inicio del siguiente valor del target
			inc rcx                   ;incrementar indice array
			inc rsi                   ;incrementar indice de row
			add byte [colScreen], 4   ;posicionar la columna
			cmp rsi, DimMatrix        ;comparar el indice con la longitud de una row
			jl col_loop
			
			sub  byte [colScreen], 12 ;restarle 4*3 al cursor de columan, valor que hemos movido al recorrer la row
			add  byte [rowScreen], 2  ;sumar dos al cursor de la fila
			
			cmp rcx, SizeMatrix       ;comparar el indice del array con la longitud del mismo
			jl row_loop
		
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Buscar donde está el espacio en blanco dentro de la matriz (tiles), 
; y actualizar la posición (spacePos) del espacio dentro de la matriz.
; Recorrer toda la matriz por filas de izquierda a derecha y de arriba a bajo.
; Si el número de espacios no es 1 (spacePos = sizeMatrix).
; Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
; a 9 (posiciones [2][2]) con incrementos de 1 porquè los datos son de
; tipo char(BYTE) 1 byte.
; 
; Variables globales utilizadas:   
; (tiles)   : Matriz donde guardamos las fichas del juego.
; (spacePos): Posición del espacio dentro de la matriz.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getSpacePosP1:
   push rbp
   mov  rbp, rsp
   
   mov rdi, tiles
   mov rcx, 0                  ;array index
   mov rbx, 0                  ;space count
   
   look_for_space:
		mov al, [rdi]          ;mover a al el valor de la posicion actual de tiles(rdi)
		add rdi,1              ;mover el puntero del array un byte para situarlo al inicio del siguiente valor del target
		add rcx,1              ;incrementar indice
		cmp al, ' '
		jne checkIfEndOfArray
		
		mov [spacePos], rcx
		inc rbx
		
		checkIfEndOfArray:
		cmp rcx, SizeMatrix    ;comparar indice con la longitud del array
		jl look_for_space

		cmp rbx,1
		jne notAnOnlySpace
		je onlyOneSpace
   
   notAnOnlySpace:
   mov byte [spacePos], SizeMatrix
   
   onlyOneSpace:   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Posicionar el cursor en la pantalla, dentro del panel, en función de
; la variable (cursorPos).
; Para calcular la posición del cursor a pantalla (rowScreen) y
; (colScreen) utilizar estas fórmulas:
; rowScreen=12+(cursorPos/DimMatrix)*2
; colScreen= 8+(cursorPos%DimMatrix)*4
; Para posicionar el cursor llamar a la funció gotoxyP1_C.
; 
; Variables globales utilizadas:   
; (cursorPos): Posición del cursor en la matriu (tiles).
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
posCurScreenP1:
   push rbp
   mov  rbp, rsp
   
   mov  eax, [cursorPos]
   mov  ebx, 3                     ;sustitui la constante dimmatrix por si valor directamente ya que me daba violacion de segmento al intentar acceder a ella (realmente no se la razon)
   idiv ebx                        ;rax/rbx = cursorPos/DimMatrix
   shl eax, 1                      ;corrimiento de bits a la izda (multiplicar por 2)
   add eax, 12
   mov [rowScreen], eax

   mov  eax, [spacePos]
   mov  ebx, 3
   idiv ebx                        ;rax/rbx = cursorPos/DimMatrix
   shl eax, 2                      ;corrimiento de bits a la izda (multiplicar por 2)
   add eax, 8
   mov [colScreen], eax
   call gotoxyP1
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mover el cursor en la dirección indicada por la variable (charac),
; ('i':arriba, 'k':bajo, 'j':izquierda o 'l':derecha).
; Actualizar la variable (cursorPos) controlando que
; no salga del tablero.
;
; No se debe posicionar el cursor en la pantalla.
;
; Variables globales utilizadas:
; (charac)   : Carácter leído de teclado.
; (cursorPos): Posición del cursor en la matriz (tiles).
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
moveCursorP1:
   push rbp
   mov  rbp, rsp 
   
   mov  al, [charac]
   cmp  al, 'i'
   je   moveUp
   cmp  al, 'k'
   je   moveDown
   cmp  al, 'j'
   je   moveLeft
   cmp  al, 'l'
   je   moveRight
   jmp  endMoveCursorP1
   
moveUp:
   sub  dword [cursorPos], 80
   cmp  dword [cursorPos], 0
   jge  endMoveCursorP1
   add  dword [cursorPos], 80
   jmp  endMoveCursorP1
   
moveDown:
   add  dword [cursorPos], 80
   cmp  dword [cursorPos], 80*25-1
   jle  endMoveCursorP1
   sub  dword [cursorPos], 80
   jmp  endMoveCursorP1
   
moveLeft:
   sub  dword [cursorPos], 1
   ;cmp  dword [cursorPos], 80*(dword [cursorPos]/80)
   jge  endMoveCursorP1
   add  dword [cursorPos], 1
   jmp  endMoveCursorP1
   
moveRight:
   add  dword [cursorPos], 1
   ;cmp  dword [cursorPos], (80*(dword [cursorPos]/80)+79)
   jle  endMoveCursorP1
   sub  dword [cursorPos], 1
   jmp  endMoveCursorP1
   
endMoveCursorP1:

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mover la ficha de la casilla donde está el cursor (cursorPos)
; a la casilla donde está el espacio (spacePos) si están
; de lado (arriba, abajo, izquierda o derecha).
;
; Si la casilla donde está el espacio está al lado de la casilla donde
; está el cursor en la matriz (tiles):
; - Mover la ficha donde está el cursor a la casilla donde está el espacio y
; poner el espacio a la casilla donde está el cursor, en la matriz (tiles).
; Si no, no hacer el movimiento y poner (state=2).
;
; No se debe mostrar la matriz con los cambios, se hace en updateBoardP1.
;
; Variables globales utilizadas:
; (tiles)    : Matriz donde guardamos las fichas del juego.
; (cursorPos): Posición del cursor en la matriz (tiles).
; (spacePos) : Posición del espacio en la matriz (tiles).
; (state)    : Estado del juego.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
moveTileP1:
   push rbp
   mov  rbp, rsp
   
   
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Verificar si se han agotado los movimentos.
; Si (moves == 0) poner (state=3).
; 
; Variables globales utilizadas:   
; (moves): Movimientos que quedan para ordenar les fichas
; (state): Estado del juego.
;          3: Pierdes, no quedan movimientos.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkEndP1:
   push rbp
   mov  rbp, rsp
   
   cmp  qword [moves], 0
   jne  endCheckEndP1
   mov  dword [state], 3

endCheckEndP1:
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Mostrar un mensaje debajo del tablero según el valor de la variable 
; (state) llamando a la función printMessageP1_C().
; 
; Variables globales utilizadas:   
; (status)   : Estado del juego.
; (rowScreen): Fila de la pantalla donde posicionamos el cursor.
; (colScreen): Columna de la pantalla donde posicionamos el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printMessageP1:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   
   call printMessageP1_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Juego del 8-PUZZLE.
; Función principal del juego.
; Permite jugar al juego del 8-PUZZLE llamando todas las funcionalidades.
; 
; Pseudo-código:
; Mostrar el tablero de juego llamando a la función printBoardP1_C.
; Inicializar el estado del juego, (state=1).
; Inicializar los movimientos que se pueden realizar (moves = 9).
; Inicializar la matriz (tiles) con los valores de la matriz (tilesIni)
; llamando la función copyMatrixP1_C.
; Actualizar el tablero de juego llamando a la función updateBoardP1_C.
; Busca dónde está el espacio en blanco dentro de la matriz (tiles) llamando
; la función getSpacePosP1_C.
; Si sólo hay un espacio (spacePos<sizeMatrix) posicionar el
; cursor en el lugar donde está el espacio (cursorPos = spacePos),
; sino, poner (state=5) y mostrar el mensaje para
; indicarlo llamando la función printMessageP1_C.
; Mientras (state==1) hacer:
;   Posicionar el cursor dentro del tablero llamando a la función posCurScreenP1_C
;   Leer una tecla (charac) llamando a la función getchP1_C.
;   Según la tecla leída haremos lo siguiente:
;   - ['i','j','k' o 'l'] desplazar el cursor según la dirección
;     elegida (llamar a la función moveTileP1_C).
;   - [' '] Mover la ficha de la casilla donde está el cursor llamando a
;     la función moveTileP1_C, si se puede hacer el movimiento (state!=2)
;     poner el espacio donde ya está el cursor (spacePos=cursorPos)
;     y decrementar los movimientos que quedan (moves).
;   - '<ESC>' (código ASCII 27) poner (state = 0) para salir.
;   Actualizar el tablero de juego llamando a la función updateBoardP1_C.
;   Verificar si se ha agotado los movimientos llamando la función checkEndP1_C.
;   Mostrar un mensaje debajo del tablero según el valor de la
;   variable (state) llamando la función printMessageP1_C.
;   Si no se ha podido hacer movimiento (state == 2) poner
;   (state = 1) para continuar jugando.
; Fin mientras.
;
; Antes de salir, esperar a que se pulse una tecla llamando la función getchP1_C para terminar.
; 
; Variables globales utilizadas:   
; (moves)    : Movimientos que quedan para ordenar les fichas.
; (spacePos) : Posición del espacio dentro de la matriz (tiles(.
; (cursorPos): Posición del cursor en la matriz (tiles).
; (charac)   : Carácter a escribir en pantalla.
; (state)    : Estado del juego.
;              1: Continuamos jugando.
;              2: No se ha podido hacer el movimiento.
;              3: Pierdes, no quedan movimientos.
;              5: Error, no se ha encontrado el espacio.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
playP1:
   push rbp
   mov  rbp, rsp
   
   call printBoardP1_C   ;printBoard2_C();
   
   push rax
   push rbx
   
   mov QWORD[moves], 9        ;moves = 9
   mov WORD[state], 1         ;state = 1;
   
   call copyMatrixP1          ;copyMatrixP1_C();
   call updateBoardP1         ;updateBoardP1_C();
   call getSpacePosP1         ;getSpacePosP1_C();         
   cmp  DWORD[spacePos], SizeMatrix     ;if (spacePos < SizeMatrix) {
   jge  playP1_Else
      mov eax, DWORD[spacePos]            
      mov DWORD[cursorPos], eax         ;cursorPos = spacePos;  
      jmp playP1_While
   playP1_Else                ;} else {
      mov  WORD[state], 5     ;state = 5;
      call printMessageP1     ;printMessageP1_C();
      
   playP1_While:
   cmp  WORD[state], 1        ;while (state == 1)
   jne  playP1_EndWhile       
      call posCurScreenP1     ;posCurScreenP1_C(); 
            
      call getchP1            ;getchP1_C()
      mov  al, BYTE[charac]
      playP1_ReadKey_ijkl:
      cmp al, 'i'
      jl  playP1_ReadKey_Space;(charac>='i')
      cmp al, 'l'
      jg  playP1_ReadKey_Space;(charac<='l')
         call moveCursorP1    ;moveCursorP1_C();
         jmp playP1_ReadKey_End
      playP1_ReadKey_Space:
      cmp al, ' '
      jne playP1_ReadKey_ESC  ;(charac==' ')
         call moveTileP1      ;moveTileP1_C();
         cmp  WORD[state], 2  ;if (state != 2) {
         je  playP1_ReadKey_ESC
            mov eax, DWORD[cursorPos]            
            mov DWORD[spacePos], eax    ;spacePos = cursorPos; 
            dec QWORD[moves]            ;moves--;
            jmp playP1_ReadKey_End
      playP1_ReadKey_ESC:
      cmp al, 27              ;if (charac==27)
      jne playP1_ReadKey_End
         mov WORD[state], 0   ;state = 0  
      playP1_ReadKey_End:

      call updateBoardP1      ;updateBoardP1_C();
      call checkEndP1         ;checkEndP1_C();
      call printMessageP1     ;printMessageP1_C();
      cmp  WORD[state], 2     ;if (state == 2) 
      jne playP1_While
         mov WORD[state], 1   ;state = 1;
   jmp playP1_While
   playP1_EndWhile:

   call getchP1               ;getchP1_C();

   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret
