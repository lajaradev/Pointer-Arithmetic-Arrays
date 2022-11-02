#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib.h"


int main(){ // gcc main.c lib.c -o main.exe

int i, j, k, l, aux1, aux2, aux3, aux4, nArgument, nArgumentAux;

char comando[100], comandoAux[100];
char prompt[100] = "*"; 
char delimitador[8] = " \n\t"; // DELIMITARDOR HASTA ENCONTRAR ESPACIO
char nameDB[100] = "*";  // NOMBRE BASE DE DATOS 
char nameTable[100] = "*"; // NOMBRE DE LA TABLA
char *tok[100]; // TOKEN
char *comillas, *comillasAux; // PARA LA INSERCION DE VALORES

char **nombre, **valores; // PUNTERO DOBLE 


TABLE *tabla = NULL, *tableAux, *tableCurrent = NULL;
TYPE *tipo;
ROW *lines = NULL, *lineAux;

Index(); // PRESENTACION DEL PROGRAMA

while(1){

///////////////////////// P R O M P T ///////////////////////// L E E R / C O M A N D O S /////////////////////////      

    printf("\n [%s]:> ", prompt);
    gets(comando);
    strcpy(comandoAux,comando);

    nArgument = 0;
    tok[nArgument] = strtok(comando, delimitador); // STRTOK ROMPE LA CADENA POR EL DELIMITADOR ESPECIFICADO 
    nArgument ++; // SEGUNDA PALABRA

    while((tok[nArgument] = strtok(NULL, delimitador)) != NULL){
        nArgument ++;
    }
    
///////////////////////// C O M A N D O S /////////////////////////

    if(tok[0] == NULL){ // VUELVE A SALIR EL MISMO PROMPT
        continue;
    }

    if (strcmp(tok[0], "exit") == 0){ // PARA SALIR DEL PROGRAMA
        
        if(nArgument != 1){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
            printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *EXIT* ES DE 1\n");
            continue;
        }
        else if(tabla != NULL){ // LIBERAR MEMORIA DE LA TABLA
            
            if(lines != NULL){ // LIBERAR MEMORIA DE LAS FILAS
                memoryLines(lines, tableCurrent -> numCampos);
            }

            memoryTable(tabla);
            tabla = NULL;
            return 0;
        }
        else if(tabla == NULL){ // TODO DESACTIVADO
             return 0;
        }
        else{
            printf("ERROR DESCONOCIDO\n");
        }

       
    }     

    else if(strcmp(tok[0], "new") == 0){ // COMANDO PARA NUEVA BASE DE DATOS O NUEVA TABLA
        
        if(nArgument == 1){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
            printf("ERROR: EL COMANDO 'NEW' LLEVA 2 ARGUMENTOS \n");
            continue;
        }

        else if(strcmp(tok[1], "db") == 0){ // CREAR UNA NUEVA BASE DE DATOS

            j = validateCharacters(tok[2]); // SI ESTA BIEN ESCRITO J VALDRA 0, SINO J VALDRA 1

            if(j == 0){ // SI ESTA BIEN ESCRITO CREA LA BASE DE DATOS
                createDB(tok[2]);
            }
        }
        
        else if(strcmp(tok[1], "table") == 0){
            
            if(nArgument < 5 || nArgument % 2 == 0){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
            printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *NEW TABLE* ES MIN 5 E IMPAR\n");
            continue;
            }
            if(strcmp(prompt, "*") == 0 ){
                printf("ERROR: EL COMANDO NEW TABLE NECESITA TENES UNA BASE DE DATOS ACTIVA");
            }

            for(i = 0; i < nArgument; i++){ // VALIDAR TODOS LOS ARGUMENTOS
                j = validateCharacters(tok[2]); // SI ESTA BIEN ESCRITO J VALDRA 0, SINO J VALDRA 1
                if(j != 0){ // SI ALGUNO ESTA MAL ESCRITO SAL DEL BUCLE
                    break;
                }
            }

            if(j != 0){ // SALIR
                continue;
            }

            nombre = (char**)malloc( ((nArgument-3)/2) * sizeof(char*) ); // RESERVA DE MEMORIA
            tipo = malloc( ((nArgument-3)/2) * sizeof(TYPE*) ); // RESERVA DE MEMORIA
            
            aux1 = 0; k = 0;

            for(i = 3; i < nArgument; i++){ // PARA <CAMPO> <TIPO> ... 
            
                if(i % 2 == 1){ // VARIABLE MODULO 2, SI ES 1 ES IMPAR (CAMPOS)
                    
                    j = validateCharacters(tok[i]); // SI ESTA BIEN ESCRITO J VALDRA 0

                    if(j != 0){
                        break; 
                    }
                    nombre[aux1] = strdup(tok[i]); // INTRODUCIMOS EL ARGUMENTO EN NOMBRE
                }

                else{ // SI ES 0 ES PAR (TIPOS)

                    k = validateTypes(tok[i]);
                    tipo[aux1] = k;
                    aux1 ++;
                }
            }


            l = validateDuplicateFields(nombre, aux1); // DEVUELVE 1 SI HAY DOS CAMPOS IGUALES

            if(j == 0 && l == 0){ // SI TODO ESTA BIEN CREAMOS LA TABLA
                tableAux = initializeTable(tok[2], ((i-3)/2), nombre, tipo); // ME DEVUELVE UNA TABLA

            //  printf("\n AAA: \nNombre tabla: %s || Numero de campos: %d\n", tableAux -> nombre, tableAux -> numCampos); for(i = 0; i < tableAux -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n", tableAux -> campos[i], tableAux -> tipos[i]); }
            //  printf("\nBBB\n%d", tableAux -> tipos[0]);
            }
            else{ // SI ALGO FALLA LIBERAMOS MEMORIA Y SALIMOS
                for(i = 0; i < aux1; i ++){
                    free(nombre[i]);
                }
                free(nombre);
                free(tipo);
                continue;
            }

            if(tableAux != NULL){ // SI LA TABLA AUXILIAR ESTA LLENA
                
                if(tabla == NULL){ // SI ES LA PRIMERA TABLA
                    tabla = tableAux;
                    createFieldTable(nameDB, tok[2], tabla); 
                // printf("\n CREAR PRIMERA TABLA: \nNombre tabla: %s || Numero de campos: %d\n", tabla -> nombre, tabla -> numCampos); for(i = 0; i < tabla -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n", tabla -> campos[i], tabla -> tipos[i]); }

                }
                else{ // SI YA HAY ALGUNA TABLA CREADA
                    
                //  printf("\n LE PASAMOS TABLA 1: \n Nombre tabla: %s || Numero de campos: %d\n", tabla -> nombre, tabla -> numCampos); for(i = 0; i < tabla -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n", tabla -> campos[i], tabla -> tipos[i]); }
                //   printf("\n LE PASAMOS TABLA AUXILIAR(NUEVA): \nNombre tabla: %s || Numero de campos: %d\n", tableAux -> nombre, tableAux -> numCampos); for(i = 0; i < tableAux -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n", tableAux -> campos[i], tableAux -> tipos[i]); }

                    insertNextTable(tabla, tableAux);
                    createFieldTable(nameDB, tok[2], tabla);
                
                }
            }
            else{
                printf("ERROR\n");
            }
        }
    }

    else if(strcmp(tok[0], "use") == 0){ // COMANDO PARA ACTIVAR UNA BASE DE DATOS O UNA TABLA
        
        if(nArgument == 1){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
            printf("ERROR: EL COMANDO 'USE' LLEVA 2 ARGUMENTOS \n");
            continue;
        }

        else if(strcmp(tok[1], "db") == 0){
           
            if(nArgument != 3){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
                printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *USE DB* ES DE 3\n");
                continue;
            }

            if(strcmp(tok[2], ".") == 0 || (tok[2], "..") == 0){ // NO PODEMOS VOLVER ATRAS EN DIRECTORIOS
                printf("ERROR: NO EXISTE ESA BASE DE DATOS\n");
                continue;
            }
        
            aux1 = enterDir(tok[2]); // SI EXISTE AUX = 0, SI NO EXISTE AUX = 1

            if (aux1 == 0){ // EXISTE LA BASE DE DATOS
                strcpy(prompt, tok[2]);
                strcpy(nameDB, tok[2]);
                strcpy(nameTable, "*"); 

                if(tabla != NULL){ // LIBERAR MEMORIA DE LA TABLA
                
                    if(lines != NULL){ // LIBERAR MEMORIA DE LAS FILAS
                        memoryLines(lines, tableCurrent -> numCampos);
                        lines = NULL;
                    }

                    memoryTable(tabla);
                    tabla = NULL;
                }
                tabla = loadTable(tok[2]); // CARGAR TABLAS          
                displayTables(tabla, nameDB); // MOSTRAR TABLAS
            }     
        }

        else if(strcmp(tok[1], "table") == 0){

            if(nArgument != 3){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
                printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *USE TABLE* ES DE 3\n");
                continue;
            }  

            aux1 =  enterTable(nameDB, tok[2]); // SI EXISTE LA TABLA AUX = 1, SI NO EXISTE AUX = 0

            if(aux1 == 1){ // EXISTE

                strcpy(prompt, nameDB);
                strcpy(nameTable, tok[2]);
                strcat(prompt, "/");
                strcat(prompt, tok[2]);

                if(lines != NULL){ // LIBERAR MEMORIA LINEA
                    memoryLines(lines, tableCurrent -> numCampos);
                    lines = NULL;
                }

                tableCurrent = lookForTable(tok[2], tabla); // BUSCAMOS LA TABLA         
                lines = loadRows(nameDB, tok[2], tableCurrent -> numCampos, tableCurrent -> tipos);   //CARGAR TABLA
            
                /* MOSTRAR */ // lineAux = lines;  while (lineAux != NULL){ for (i = 0; i < tableCurrent -> numCampos; i ++){ printf("[%s]", lineAux -> valores[i]);  } printf("\n"); lineAux = lineAux -> sig; }
            }
            else{ // NO EXISTE
                printf("ERROR: LA TABLA NO EXISTE\n");
            }
        }       
    }
   
    else if (strcmp(tok[0], "insert") == 0 ) { // INSERTAR VALORES

        if(tabla == NULL){ 
            printf("ERROR: NO HAS SELECCIONADO NINGUNA TABLA\n");
            continue;
        }
    
        aux4 = 0;
        comillas = strchr(comandoAux, '"');

        if(comillas != NULL){  
            comillasAux = strchr(comillas+1, '"');
            if(comillasAux == NULL){ // si no encontramos la pareja error
                printf("ERROR de comillas\n");
                aux4++;
                continue;
            }
            else{
                nArgumentAux = 0;
                tok[nArgumentAux] = MiStrTok(comandoAux, delimitador, 0); 
                nArgumentAux++;

                    while ((tok[nArgumentAux] = MiStrTok(NULL, delimitador,1)) != NULL){
                        nArgumentAux++;   
                    }    
                }
            }
        else{ // SINO HAY COMILLAS EN EL COMANDO
            nArgumentAux = 0;
            tok[nArgumentAux] = MiStrTok(comandoAux, delimitador, 0);
            nArgumentAux ++;
            while((tok[nArgumentAux] = MiStrTok(NULL, delimitador, 0)) != NULL){
                nArgumentAux ++;
            }   
        }

        if (nArgumentAux != (tableCurrent->numCampos + 1)){ // +1 POR EL ARGUMENTO INSERT + CAMPOS DE LA TABLA
            printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *INSERT* ES DE 1 + NUMERO DE CAMPOS DE LA TABLA\n");
            continue;
        }

        for(i = 0; i < tableCurrent -> numCampos; i ++){ // PARA TODOS LOS CAMPOS DE LA TABLA

            if(tableCurrent -> tipos[i] == NUM){
                if(validateNUM(tok[i + 1]) == 0){ // SI ESTUVIESE BIEN DEVUELVE UN 1
                    printf("ERROR: EL DATO TIPO 'NUM' ESTA MAL ESCRITO\n");
                    aux4 ++;
                    break;
                }
            }
            if(tableCurrent -> tipos[i] == DATE){
                if(validateDate(tok[i + 1]) == 0){
                    printf("ERROR: EL DATO TIPO 'DATE' ESTA MAL ESCRITO\n");
                    aux4 ++;
                    break;
                }
            }
            if(tableCurrent -> tipos[i] == EMAIL){
                if(validateEmail(tok[i + 1]) == 0){ 
                    printf("ERROR: EL DATO TIPO 'EMAIL' ESTA MAL ESCRITO\n");
                    aux4 ++;
                    break;
                }
            }
            if(tableCurrent -> tipos[i] == TEXT){
               
                if(comillas == NULL){ // SI NO HAY COMILLAS
                    if(validateCharacters(tok[i + 1]) == 1){ // AQUI ESTA ALREVES, DEVUELVE 1 SI ESTA MAL
                        printf("ERROR: EL DATO TIPO 'TEXT' ESTA MAL ESCRITO\n");
                        aux4 ++;
                        break;
                    }
                }
                else{ // SI HAY COMILLAS
                    // NO VALIDAMOS
                }  
            } 
        }

        if(aux4 != 0){
            continue;
        }

        valores = malloc(sizeof(char*) * tableCurrent -> numCampos); // RESERVAR DE MEMORIA PARA LA CANTIDAD DE CAMPOR

        for(i = 0; i < tableCurrent -> numCampos; i ++){
            valores[i] = strdup(tok[i + 1]); // INTRODUCIMOS LOS VALORES
        }
        
        lineAux = initializeRow(valores); // CREAMOS LA ESTRUCTURA PARA LAS LINEAS
          
        if(lineAux != NULL){ 
            if(lines == NULL){ // PRIMERA LINEA
                lines = lineAux;
                createFieldRow(nameDB, tableCurrent, lines);
            }
            else{ // SI YA HAY ALGUNA LINEA
                insertNextRow(lines, lineAux);
                createFieldRow(nameDB, tableCurrent, lines);
            }
        }
        else{
            printf("ERROR: NO SE HA INICIADO BIEN LA ESTRUCTURA DE LA LINEA\n");
        }
    }

    else if(strcmp(tok[0], "select") == 0){ // MOSTRAR VALORES SELECCIONADOS
        
        if(tabla == NULL){ 
            printf("ERROR: NO HAS SELECCIONADO NINGUNA TABLA\n");
            continue;
        }
        if(strcmp(prompt, nameDB) == 0){
             printf("ERROR: NO HAS SELECCIONADO NINGUNA TABLA\n");
             continue;
        }
        
        comillas = strchr(comandoAux, '"'); // BUSCA LA PRIMERA APARICION DEL CARACTER " EN EL COMANDO
        
        if(comillas != NULL){ // SI HAY COMILLAS EN EL COMANDO

            comillasAux = strchr(comillas + 1, '"'); // BUSCAMOS EL CIERRE DE COMILLAS

            if(comillasAux != NULL){

                nArgumentAux = 0;
                tok[nArgumentAux] = MiStrTok(comandoAux, delimitador, 0); // STRTOK ROMPE LA CADENA POR EL DELIMITADOR ESPECIFICADO 
                nArgumentAux ++; // SEGUNDA PALABRA

                while((tok[nArgumentAux] = MiStrTok(NULL, delimitador, 1)) != NULL){ // EL 1 ES PARA ACTIVAR LAS COMILLAS
                    nArgumentAux ++;
                }
            }
            else{
                printf("ERROR: NO SE HA ENCONTRADO EL CIERRE DE COMILLAS\n");
                continue;
            }


        }
        else{ // SINO HAY COMILLAS EN EL COMANDO
            nArgumentAux = 0;
            tok[nArgumentAux] = MiStrTok(comandoAux, delimitador, 0);
            nArgumentAux ++;
            while((tok[nArgumentAux] = MiStrTok(NULL, delimitador, 0)) != NULL){
               // printf("%d, %s\n ", nArgumentAux, tok[nArgumentAux]);
                nArgumentAux ++;
            }   
        }

        if(nArgument == 1){ // [*] SELECT
            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
            showBody(tableCurrent -> numCampos, lines);// IMPRIMIMOS LOS REGISTROS
        }
        else if(nArgument == 2){ // [*] SELECT ASC|DES         
            if(strcmp(tok[1], "ASC") == 0){
                showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                showBodyASC(tableCurrent -> numCampos, lines, tableCurrent);// IMPRIMIMOS ASCENDENTE
            }
            else if(strcmp(tok[1], "DES") == 0){
                showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                showBodyDES(tableCurrent -> numCampos, lines, tableCurrent);// IMPRIMIMOS DESCENCENTE
            }
            else{
                printf("ERROR: LOS PARAMETROS SON: ASC || DES\n");
            }
        }
        else if(nArgument == 4){ // [*] SELECT [<CAMPO> <OPERADOR> <VALOR>] 

            for(i = 0; i < tableCurrent -> numCampos; i ++){

                if(strcmp(tableCurrent -> campos[i], tok[1]) == 0){ // PARA VALIDAR LA COLUMNA

                    if(tableCurrent -> tipos[i] == TEXT){
                        showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                        showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                        break; 
                    }
                    else if(tableCurrent -> tipos[i] == NUM && validateNUM(tok[3]) == 1){
                        showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                        showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                        break; 
                    }      
                    else if(tableCurrent -> tipos[i] == DATE && validateDate(tok[3]) == 1){
                        showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                        showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                        break; 
                    } 
                    if(tableCurrent -> tipos[i] == EMAIL && validateEmail(tok[3]) == 1){
                        showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                        showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                        break; 
                    }
                    else{
                        printf("ERROR DE TIPOS\n");
                    }                         
                }
                if(i + 1 == tableCurrent -> numCampos){
                        printf("ERROR: CAMPO NO ENCONTRADO\n");
                }
            }
        }  
        else if(nArgument == 5){ // [*] SELECT [<CAMPO> <OPERADOR> <VALOR>] [ASC|DES] 

            if(strcmp(tok[4], "ASC") == 0){
                
                for(i = 0; i < tableCurrent -> numCampos; i ++){

                    if(strcmp(tableCurrent -> campos[i], tok[1]) == 0){ // PARA VALIDAR LA COLUMNA

                        if(tableCurrent -> tipos[i] == TEXT){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderASC(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }
                        else if(tableCurrent -> tipos[i] == NUM && validateNUM(tok[3]) == 1){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderASC(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }      
                        else if(tableCurrent -> tipos[i] == DATE && validateDate(tok[3]) == 1){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderASC(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }
                        else if(tableCurrent -> tipos[i] == EMAIL && validateEmail(tok[3]) == 1){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderASC(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }
                        else{
                            printf("ERROR DE TIPOS\n");
                        }                           
                    }
                    if(i + 1 == tableCurrent -> numCampos){
                            printf("ERROR: CAMPO NO ENCONTRADO\n");
                        }
                }

            }
            else if(strcmp(tok[4], "DES") == 0){
               
                for(i = 0; i < tableCurrent -> numCampos; i ++){

                    if(strcmp(tableCurrent -> campos[i], tok[1]) == 0){ // PARA VALIDAR LA COLUMNA

                        if(tableCurrent -> tipos[i] == TEXT){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderDES(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }
                        else if(tableCurrent -> tipos[i] == NUM && validateNUM(tok[3]) == 1){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderDES(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }      
                        else if(tableCurrent -> tipos[i] == DATE && validateDate(tok[3]) == 1){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderDES(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }
                        else if(tableCurrent -> tipos[i] == EMAIL && validateEmail(tok[3]) == 1){
                            showHeader(tableCurrent); // IMPRIMIMOS EN ENCABEZADO
                            orderDES(lines, tableCurrent, 0);
                            showFieldValue(lines, tableCurrent -> tipos[i], tok[2], tok[3], i, tableCurrent -> numCampos); //
                            break; 
                        }
                        else{
                            printf("ERROR DE TIPOS\n");
                        }                           
                    }
                    if(i + 1 == tableCurrent -> numCampos){
                            printf("ERROR: CAMPO NO ENCONTRADO\n");
                        }
                }
            }
            else{
                printf("ERROR: EL COMANDO VALIDO ES: SELECT CAMPO OPERADOR VALOR ASC|DES\n");
            }
        } 
        else{
            printf("ERROR: NUMERO DE PARAMETROS INCORRECTO\n");
        }
    }
    
    else if(strcmp(tok[0], "list") == 0){ // MOSTRAR LAS BASES DE DATOS EXISTENTES
              
        if(nArgument != 1){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
            printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *LIST* ES DE 1\n");
            continue;
        }

        showDB();
    }

    else if(strcmp(tok[0], "delete") == 0){ // [*] DELETE <CAMPO> <OPERADOR> <VALOR>
        
        if(nArgument != 4){ // VALIDAR LA CANTIDAD DE ARGUMENTOS
            printf("ERROR: EL NUMERO DE ARGUMENTOS PARA *DELETE* ES DE 4 \n");
            continue;
        }

        if(tabla == NULL){
            printf("NO SE HA CARGADO NINGUNA BASE DE DATOS\n");
        }        
        if(strcmp(prompt, nameDB) == 0){
            printf("NO SE HA CARGADO NINGUNA TABLA\n");
        }

        comillas = strchr(comandoAux, '"'); // BUSCA LA PRIMERA APARICION DEL CARACTER " EN EL COMANDO
        
        if(comillas != NULL){ // SI HAY COMILLAS EN EL COMANDO

            comillasAux = strchr(comillas + 1, '"'); // BUSCAMOS EL CIERRE DE COMILLAS

            if(comillasAux != NULL){

                nArgumentAux = 0;
                tok[nArgumentAux] = MiStrTok(comandoAux, delimitador, 0); // STRTOK ROMPE LA CADENA POR EL DELIMITADOR ESPECIFICADO 
                nArgumentAux ++; // SEGUNDA PALABRA

                while((tok[nArgumentAux] = MiStrTok(NULL, delimitador, 1)) != NULL){ // EL 1 ES PARA ACTIVAR LAS COMILLAS
                    nArgumentAux ++;
                }
            }
            else{
                printf("\nERROR: NO SE HA ENCONTRADO EL CIERRE DE COMILLAS\n");
                break;
            }


        }
        else{ // SINO HAY COMILLAS EN EL COMANDO
            nArgumentAux = 0;
            tok[nArgumentAux] = MiStrTok(comandoAux, delimitador, 0);
            nArgumentAux ++;
            while((tok[nArgumentAux] = MiStrTok(NULL, delimitador, 0)) != NULL){
                nArgumentAux ++;
            }   
        }

        if(nArgument == 4){ 
             
             for(i = 0; i < tableCurrent -> numCampos; i ++){

                if(strcmp(tableCurrent -> campos[i], tok[1]) == 0){ // PARA VALIDAR LA COLUMNA

                    if(tableCurrent -> tipos[i] == TEXT){
                        lines = deleteLineValueSpecified(tableCurrent -> numCampos, lines, tableCurrent ->tipos[i], tok[3], tok[2], i);
                        updateRow(lines, tableCurrent, nameDB);
                        break; 
                    }
                    else if(tableCurrent -> tipos[i] == NUM && validateNUM(tok[3]) == 1){
                        lines = deleteLineValueSpecified(tableCurrent -> numCampos, lines, tableCurrent ->tipos[i], tok[3], tok[2], i);
                        updateRow(lines, tableCurrent, nameDB);
                        break; 
                    }      
                    else if(tableCurrent -> tipos[i] == DATE && validateDate(tok[3]) == 1){
                        lines = deleteLineValueSpecified(tableCurrent -> numCampos, lines, tableCurrent ->tipos[i], tok[3], tok[2], i);
                        updateRow(lines, tableCurrent, nameDB);
                        break; 
                    } 
                    else if(tableCurrent -> tipos[i] == EMAIL && validateEmail(tok[3]) == 1){
                        lines = deleteLineValueSpecified(tableCurrent -> numCampos, lines, tableCurrent ->tipos[i], tok[3], tok[2], i);
                        updateRow(lines, tableCurrent, nameDB);
                        break; 
                    } 
                    else{
                        printf("ERROR DE TIPOS\n");
                    }                         
                }
                if(i + 1 == tableCurrent -> numCampos){
                        printf("ERROR: CAMPO NO ENCONTRADO\n");
                }
            }
        }
    }
    

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

    else if(strcmp(tok[0], "modificavalor") == 0){ // (RENOMBRAR) MODIFICAR UN VALOR DE UNA LINEA 
                                    // modificavalor nombre = samuel paco
        if (tabla == NULL){
                printf("Todavia no ha cargado ninguna bd\n");
                continue;
            }  
            if (strcmp(prompt, nameDB) == 0){
                printf("Todavia no ha cargado ninguna tabla\n");
                continue;
            }             

            comillas = strchr(comandoAux, '"');
            
            if(comillas != NULL){
                        
                comillasAux = strchr(comillas+1, '"');
                
                if(comillasAux == NULL){
                    printf("ERROR de comillas");
                    break;
                }
                else{
                    nArgument = 0;
                    tok[nArgument] = MiStrTok(comandoAux, delimitador, 0);
                    nArgument++;

                    while ((tok[nArgument] = MiStrTok(NULL, delimitador,1)) != NULL){                        
                        nArgument++;  
                    }
                }
            }
            else{
                nArgument = 0;
                tok[nArgument] = MiStrTok(comandoAux, delimitador, 0);
                nArgument++;

                while ((tok[nArgument] = MiStrTok(NULL, delimitador ,0)) != NULL){
                    nArgument++;
                }
            }

            if (nArgument == 5){
                printf("AAAAAAA\n\n");
                //MOSTRAR LOS CAMPOS
                for(i=0;i<tableCurrent->numCampos;i++){
                    if(strcmp(tableCurrent->campos[i], tok[1]) == 0){ // comprobamos que la columna existe
                        if(tableCurrent->tipos[i] == NUM && validateNUM(tok[3])==1){
                            showHeader(tableCurrent);
                            lines = renombrar_linea_valor_deseado(tableCurrent->numCampos, lines, tableCurrent->tipos[i], tok[3], tok[2], i, tok[4]);
                            updateRow(lines, tableCurrent, nameDB);
                            break;
                        }
                        else if(tableCurrent->tipos[i] == DATE && validateDate(tok[3])==1){
                            showHeader(tableCurrent);
                            lines = renombrar_linea_valor_deseado(tableCurrent->numCampos, lines, tableCurrent->tipos[i], tok[3], tok[2], i, tok[4]);
                            updateRow(lines, tableCurrent, nameDB);
                            break;
                        }
                        else if(tableCurrent->tipos[i] == TEXT){
                            showHeader(tableCurrent);
                            lines = renombrar_linea_valor_deseado(tableCurrent->numCampos, lines, tableCurrent->tipos[i], tok[3], tok[2], i, tok[4]);
                            updateRow(lines, tableCurrent, nameDB);
                            break;
                        }
                        else if(tableCurrent->tipos[i] == EMAIL && validateEmail(tok[3]) == 1){
                            showHeader(tableCurrent);
                            lines = renombrar_linea_valor_deseado(tableCurrent->numCampos, lines, tableCurrent->tipos[i], tok[3], tok[2], i, tok[4]);
                            updateRow(lines, tableCurrent, nameDB);
                            break;
                        }
                        else{
                            printf("ERROR de tipos\n");
                        }
                    }
                }
            }
    }

    else if(strcmp(tok[0], "renombrarDB") == 0){ // RENOMBRAMOS UNA BASE DE DATOS 
    //entramos a la base de datos y renombrarDB 'nombrebasedatos' 'nomnuevabase'
    //// comtrolar el numer de argumentos y que no hayan tabla activas
        j = validateCharacters(tok[2]);

            if(j != 0){
                printf("ERROR de caracteres");
                continue;
            }

            j = renombrar_BD(nameDB, tok[2]);

            if(j<2){
                continue;
            }
            else{

                strcpy(nameDB, tok[2]);
                strcpy(prompt, tok[2]);

            }
    }

    else if(strcmp(tok[0], "renombrartablas") == 0){ // RENOMBRAMOS UNA TABLA
        
        j = validateCharacters(tok[2]);

            if(j != 0){
                printf("ERROR de caracteres");
                continue;
            }

            //validar que no este en la tabla a cambiar y este en la base de datos activa

            tabla = renombrar_tabla(tabla, nameDB, tok[1], tok[2]);
        
            editMeta(nameDB, tabla);
            displayTables(tabla, nameDB);


    }

    else if(strcmp(tok[0], "borrarDB") == 0){ // BORRAR BASE DE DATOS
                                                // borrar base1
        tabla = borrar_bd(tabla, tok[1]);
            editMeta(tok[1], tabla);
            char aux6[100] = "";
            strcpy(aux6, tok[1]);
            strcat(aux6, "\\meta.db");
            remove(aux6);
            rmdir(tok[1]);
            strcpy(nameDB, "*");
            strcpy(prompt, "*");
    }
   
    else if(strcmp(tok[0], "deleTablas") == 0){ //BORRAR TABLA
        
            if(nArgument != 2){
                printf("ERROR de argumentos en 'borrar' ");
                continue;
            }
            if(strcmp(nameDB, "*") == 0 ){
                printf("ERROR no hay base de datos activa ");
                continue;
            }
            if(strcmp(nameTable, "*") != 0 ){
                printf("ERROR tabla activa, salga a la bd de datos");
                continue;
            }

            tabla = borrar_tabla(tabla, nameDB, tok[1]);
            editMeta(nameDB, tabla);
            tabla = loadTable(nameDB);
    }

    else if(strcmp(tok[0], "borrarlinea") == 0){ // BORRAR TODAS LAS LINEA
                                                // 
        if (nArgument != 1){
                printf("	## ERROR: comando 'exit' numero de parametros incorrecto\n\n");
                continue;
            }

            if (!strcmp(prompt, nameDB) == 0) //este caso se da cuando hay una db y tabla activa 
            {
                borrar_lineas(lines,tableCurrent->numCampos, prompt, nameDB);
                lines=NULL;
            }
            else{
                printf("Debe haber una tabla activa\n");
            } 
    }
 
    else if (strcmp(tok[0], "hola") == 0){

        if(nArgument == 1){
            printf("ERROR\n");
            continue;
        }
        if(strcmp(tok[1], "adios") == 0){
            printf("hola y adios");
            continue;
        }
        if(strcmp(tok[1], "hastaluego") == 0){
            printf("hola y hasta luego");
            continue;
        }
    }
    
    else{ // COMANDO DESCONOCIDO
        printf("ERROR: COMANDO DESCONOCIDO\n");
    }
}


return 0;
}