#ifndef _lib_H_
#define _lib_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h> // FUNCION ACCESS

///////////////////////// E S T R U C T U R A S /////////////////////////

typedef enum{ // TIPOS DE DATOS
    NUM,
    DATE,
    EMAIL,
    TEXT
}TYPE;

typedef struct tab{
    char nombre[100];
    int numCampos;
    char **campos;
    TYPE *tipos;
    struct tab *sig, *ant;    
}TABLE;

typedef struct line {
    char **valores;
    struct line *sig, *ant;
}ROW;

////////////////////////////////////////////////// C A B E C E R A S //////////////////////////////////////////////////

void Index(); // PRESENTACION DEL PROGRAMA

///////////////////////// M E T A /////////////////////////

void createMeta(char *name_db, char *name_meta2); // CREAMOS EL FICHERO META, FUNCION DENTRO DE CREAR NUEVA BASE DE DATOS

void editMeta(char *nameDB, TABLE *tabla); // EDITAR EL META AL AÑADIR UNA NUEVA TABLA

int countTable(char *nameDB); // FUNCION PARA CONTAR LAS TABLAS DENTRO DE UNA BASE DE DATOS

///////////////////////// B A S E / D E / D A T O S /////////////////////////

void createDB(char *name_db); // CREAR BASE DE DATOS

int enterDir(char *name_db); // ENTRAR A UN DIRECTORIO

///////////////////////// T A B L A S /////////////////////////

TABLE *initializeTable(char *name_table, int nFields, char **fields, TYPE *tipo); // INICIALIZAMOS LA TABLA

void insertNextTable(TABLE *tabla, TABLE *tableAux); // BUSCAMOS LA SIGUIENTE TABLA QUE ESTE VACIA

void createFieldTable(char *name_db, char *name_table, TABLE *tabla); // CREAMOS EL FICHERO DE LA TABLA

int enterTable(char *nameDB, char *nameTable); // ENTRAR EN UNA TABLA

TABLE* lookForTable(char *nameTable, TABLE *tabla); // IMPORTANTE

void memoryTable (TABLE *tabla); // LIBERAR MEMORIA DE LA TABLA

TABLE* loadTable(char *nameDB); // CARGAMOS LA TABLA

void displayTables(TABLE* table, char *nameDB); // MOSTRAR TABLAS

///////////////////////// L I N E A S /////////////////////////

ROW *initializeRow(char **valores); // INICIALIZAMOS LAS FILAS

void insertNextRow(ROW *row, ROW *rowAux); // BUSCAMOS LA SIGUIENTE LINEA QUE ESTE VACIA

void createFieldRow(char *nameDB, TABLE *tableCurrent, ROW *row); // ESCRIBIR EN EL FICHERO LOS VALORES  

ROW *loadRows(char *nameDB, char *nameTable, int numFields, TYPE *tipos); // CARGAMOS LOS REGISTROS

void memoryLines (ROW *row, int nFields); // LIBERAR MEMORIA DE LAS FILAS

///////////////////////// V A L I D A C I O N E S /////////////////////////

int validateDuplicateFields(char **chain, int nArg); // DUPLICADOS DE CAMPOS

int validateCharacters (char *chain); // VALIDAR LA CADENA DE CARACTERES

TYPE validateTypes(char *value); // TIPOS DE CAMPOS

int validateNUM(char *chain); // VALIDAR UN NUMERO

int validateDate(char *chain); // VALIDAR UNA FECHA

int validateEmail(char *chain); // [ALFANUMERICO] @ [ALFANUMERICO] . [ALFANUMERICO]

int validateNameDB(char *argument); /// VALIDAR EL NOMBRE DE UNA BASE DE DATOS

char* MiStrTok(char *cad1, char *sep, int comillas);  // FUNCION PARA PARTIR LA CADENA POR LAS COMILLAS 

char *getField(char *row, char *delimiter, int id, char *value); // DEVUELVE EL VALOR DEL CAMPO 

void trim(char *cad); // ESTA FUNCION QUITA LOS ESPACIOS EN BLANCO

///////////////////////// I M P R I M I R / D A T O S /////////////////////////

void showHeader(TABLE *tableCurrent); // MOSTRAMOS LA CABECERA DE LA TABLA

void showBody(int nFields, ROW *lines); // MOSTRAMOS TODOS LOS REGISTROS DE LA TABLA

void showBodyASC(int nFields, ROW *lines, TABLE *tableCurrent); // MOSTRAMOS TODOS LOS REGISTROS DE FORMA ASCENDENTE

void orderASC(ROW *lines, TABLE *tableCurrent, int field); // ORDENAR ASCENDENTE

void showBodyDES(int nFields, ROW *lines, TABLE *tableCurrent); // MOSTRAMOS TODOS LOS REGISTROS DE FORMA DESCENDENTE

void orderDES(ROW *lines, TABLE *tableCurrent, int field); // ORDENAR DESCENDENTE

void exchangeValues(ROW *linesAux1, ROW *linesAux2); // INTERCAMBIAR VALORES

int DATEtoNUM(char* chain); // CONVERTIMOS LA FECHA A UN NUMERO

void showFieldValue(ROW *lines, TYPE types, char *operator, char *value, int position, int nFields); // 

void showDB(); // MOSTRAR LAS BASES DE DATOS QUE EXISTEN

///////////////////////// B O R R A D O / D E / D A T O S /////////////////////////

ROW* deleteLineValueSpecified(int nFields, ROW *lines, TYPE type, char *value, char *operator, int position); // BORRAR EL VALOR DESEADO

void updateRow(ROW *lines, TABLE *tableCurrent, char *nameDB); // ESCRIBIR EN EL FICHERO TODOS LOS VALORES DE LAS LINEAS



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

char* NUMtoDATE(int res); // CONVERTIMOS EL NUMERO A UNA FECHA

ROW* renombrar_linea_valor_deseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion, char *nuevovalor); 

int renombrar_BD(char *nombre_bd, char *nuevo_nombre_bd);

TABLE* borrar_bd (TABLE *tabla, char *nombre_bd);

TABLE* borrar_tabla(TABLE *tabla, char *nombre_bd, char *tablaborrar); // BORRAR UN NODO CONCRETO DE UNA LISTA

TABLE* renombrar_tabla(TABLE* tabla, char *nombre_bd, char *nombre_tabla, char *nombre_nuevo_tabla);

void borrar_lineas(ROW *lineas, int num_campos, char *prompt, char *nombre_bd); // BORRAR TODAS LAS LINEAS DE UN FICHERO

int EnteroValidar (char *cad);

#endif
