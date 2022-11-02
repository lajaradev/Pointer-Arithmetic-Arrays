#include "lib.h"

////////////////////////////////////////////////// F U N C I O N E S //////////////////////////////////////////////////

void Index(){ // PRESENTACION DEL PROGRAMA
    
    printf("\n Nombre:    Samuel \n Apellidos: Lajara Agullo \n Correo:    samuel.lajara@goumh.umh.es");
    
    printf("\n Comandos: ");
    printf("\n   [*] exit ");
    printf("\n   [*] new db <nombre_base_de_datos> ");
    printf("\n   [*] use db <nombre_base_de_datos> ");
    printf("\n   [*] new table <nombre_tabla> <campo> <tipo>");
    printf("\n   [*] use table <nombre_tabla>");
    printf("\n   [*] insert <valor> ");
    printf("\n   [*] select [<campo> <operador> <valor>] <orden> ");
    printf("\n   [*] list ");
    printf("\n   [*] delete [<campo> <operador> <valor>]"); 
}

///////////////////////// M E T A /////////////////////////

void createMeta(char *nameDB, char *nameMeta2){ // CREAMOS EL FICHERO META, FUNCION DENTRO DE CREAR NUEVA BASE DE DATOS
   
    DIR *dir;

    if(dir = opendir(nameDB)){ // ABRIMOS LA BASE DE DATOS
       
        FILE *fichero; // DECLARAMOS LA VARIABLE FICHERO COMO PUNTERO A FILE
        fichero = fopen(nameMeta2,"w+"); // ABRIMOS EL ARCHIVO TXT EN MODO ESCRITURA Y GUARDAMOS SU DIRECCION EN EL PUNTERO
        if(fichero != NULL){
            fprintf(fichero, "TABLAS: 0");
            fprintf(fichero,"\n------------");

            fclose(fichero); // CERRAR FICHERO
        }
        else{
            printf("\nERROR");
        }
    }
    else{
         printf("\nNO ABRE LA BASE DE DATOS");
    }
}

void editMeta(char *nameDB, TABLE *tabla){ // EDITAR EL META AL AÑADIR UNA NUEVA TABLA

    DIR *dir;
    char aux1[10] = "\\meta.db";
    char aux2[50] = "";
    char aux3[50] = "";
    int i, nTablas = countTable(nameDB);

    strcpy(aux2, nameDB);
    strcat(aux2, aux1);

    if(dir = opendir(nameDB)){ // ABRIMOS LA BASE DE DATOS
      
        FILE *fichero;
        fichero = fopen(aux2, "w");

        if(fichero != NULL){ // SI EL FICHERO EXISTE
         
            fprintf(fichero, "TABLAS: %i", nTablas);
            fprintf(fichero,"\n------------");

            if(nTablas == 1){ // SOLO HAY UNA TABLA

                fprintf(fichero,"\n%s: %i", tabla -> nombre, tabla -> numCampos); // NOMBRE Y CANTIDAD DE CAMPOS DE LA TABLA

                for(i = 0; i < tabla -> numCampos; i ++){ // ALMACENAMOS EL TIPO DE CADA CAMPO 

                    if(tabla -> tipos[i] == 0){
                        strcpy(aux3,"NUM");
                    }
                    if(tabla -> tipos[i] == 1){
                        strcpy(aux3,"DATE");
                    }
                    if(tabla -> tipos[i] == 2){
                        strcpy(aux3,"EMAIL");
                    }
                    if(tabla -> tipos[i] == 3){
                        strcpy(aux3,"TEXT");
                    }
                    
                    
                fprintf(fichero,"\n%s: %s", tabla -> campos[i], aux3); // ESCRIBIMOS LOS CAMPOS Y TIPOS

                }

            }
            else{ // HAY MAS DE UNA TABLA

                while (tabla != NULL){ // MIENTRAS EXISTAN TABLAS
                    
                    fprintf(fichero,"\n%s: %i", tabla -> nombre, tabla -> numCampos); // NOMBRE Y CANTIDAD DE CAMPOS DE LA TABLA
                   
                    for(i = 0; i < tabla -> numCampos; i ++){ // ALMACENAMOS EL TIPO DE CADA CAMPO 

                        if(tabla -> tipos[i] == 0){
                            strcpy(aux3,"NUM");
                        }
                        if(tabla -> tipos[i] == 1){
                            strcpy(aux3,"DATE");
                        }
                        if(tabla -> tipos[i] == 2){
                            strcpy(aux3,"EMAIL");
                        }
                        if(tabla -> tipos[i] == 3){
                            strcpy(aux3,"TEXT");
                        }
                       
                    
                    fprintf(fichero,"\n%s: %s", tabla -> campos[i], aux3); // ESCRIBIMOS LOS CAMPOS Y TIPOS

                    }
                
                    if(tabla -> sig == NULL){ // ESTAMOS EN LA ULTIMA TABLA, SALIMOS
                        break;
                    }
                fprintf(fichero, "\n------------------");
                tabla = tabla -> sig;
                }
                
            }

        fclose(fichero);

        }
        else{
            printf("ERROR: EL FICHERO NO EXISTENTE");
        }
    }
    else{
        printf("ERROR: NO ABRE LA BASE DE DATOS");
    }

}

int countTable(char *nameDB){ // FUNCION PARA CONTAR LAS TABLAS DENTRO DE UNA BASE DE DATOS

    int numTable = 0;
    DIR *dir;
    struct dirent *f; // #INCLUDES

    if(dir = opendir(nameDB)){
        while(f = readdir(dir)){ // FUNCION QUE LEE TODO LO DE LA CARPETA
            numTable ++;
        }
    }
    
    numTable = numTable - 3; // RESTAMOS EL ARCHIVO META Y DOS MAS (SIEMPRE HAY DOS DE MAS NOSE QUE SON)

    return numTable;
}

///////////////////////// B A S E / D E / D A T O S /////////////////////////

void createDB(char *name_db){ // CREAR BASE DE DATOS

    char name_meta1[11]="\\meta.db"; 
    char name_meta2[50]="";

    strcpy(name_meta2, name_db);
    strcat(name_meta2,name_meta1); // CONCATENAMOS
 
    mkdir(name_db); // CREAR EL DIRECTORIO
    createMeta(name_db, name_meta2); // CREAMOS EL META DENTRO DEL DIRECTORIO
    
}

int enterDir(char *nameDB){ // ENTRAR A UN DIRECTORIO
    
    DIR *dir;

    if(dir = opendir(nameDB)){
        return 0;
    }
    else{
        printf("\nERROR: LA BASE DE DATOS NO EXISTE");
        return 1;
    }
}

///////////////////////// T A B L A S /////////////////////////

TABLE *initializeTable(char *name_table, int nFields, char **fields, TYPE *tipo){ // INICIALIZAMOS LA TABLA

    int i;

    TABLE *tabla = malloc(sizeof(TABLE)); // RESERVA DE MEMORIA PARA LA TABLA

    if(tabla == NULL){ // ERROR
        return NULL;
    }

    strcpy (tabla -> nombre, name_table); // COPIAMOS EL VALOR DEL PUNTERO RECIBIDO EN LA TABLA
            tabla -> numCampos = nFields;
            tabla -> campos = fields;
            tabla -> tipos = tipo;
            tabla -> sig = NULL;
            tabla -> ant = NULL;
    
    //  printf("Nombre tabla: %s || Numero de campos: %d\n", tabla -> nombre, tabla -> numCampos); for(i = 0; i < tabla -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n", tabla -> campos[i], tabla -> tipos[i]); }

    return tabla;

}

void insertNextTable(TABLE *tabla, TABLE *tableAux){ // BUSCAMOS LA SIGUIENTE TABLA QUE ESTE VACIA
    int i;
    TABLE *nextTable;

    nextTable = tabla; // NEXTTABLE ES LA PRIMERA TABLA

    while (nextTable -> sig != NULL){ // MIENTRAS QUE LA SIGUIENTE TABLA NO ESTE VACIA
        nextTable = nextTable -> sig; // PASAMOS A LA SIGUIENTE TABLA
    }

    nextTable -> sig = tableAux; // ASIGNAMOS LOS VALORES A LA SIGUIENTE TABLA (ESTARA VACIA)
    tableAux -> ant = nextTable;
   // printf("\n ULTIMA TABLA: \n Nombre tabla: %s || Numero de campos: %d\n", nextTable -> sig -> nombre, nextTable->sig -> numCampos); for(i = 0; i < nextTable ->sig -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n", nextTable->sig -> campos[i], nextTable->sig -> tipos[i]); }
   // printf("\n TABLA ANTERIOR : \n Nombre tabla: %s || Numero de campos: %d\n",  tableAux -> ant -> nombre,  tableAux -> ant -> numCampos); for(i = 0; i <  tableAux -> ant -> numCampos; i++){ printf("Campos: %s || Tipo:  %d\n",  tableAux -> ant -> campos[i],  tableAux -> ant -> tipos[i]); }

}

void createFieldTable(char *nameDB, char *nameTable, TABLE *tabla){ // CREAMOS EL FICHERO DE LA TABLA
 
    char extension[5] = ".tab";
    char barras[3] = "\\";
    char nameTableAux[50] = "";

    strcpy(nameTableAux, nameDB);
    strcat(nameTableAux, barras);
    strcat(nameTableAux, nameTable);
    strcat(nameTableAux, extension);

    DIR *dir;
   

    if(access(nameTableAux, F_OK) == 0){
        printf("TABLA YA EXISTENTE");
    }
    else{

        if(dir = opendir(nameDB)){ // ABRIMOS LA BASE DE DATOS
        
            FILE *fichero; // DECLARAMOS LA VARIABLE FICHERO COMO PUNTERO A FILE
            fichero = fopen(nameTableAux,"w+"); // ABRIMOS EL ARCHIVO TXT EN MODO ESCRITURA Y GUARDAMOS SU DIRECCION EN EL PUNTERO
           
            if(fichero != NULL){
                editMeta(nameDB, tabla); 
            }
            else{
                printf("\nERROR");
            }

            fclose(fichero); // CERRAR FICHERO
        }
        else{
            printf("ERROR: NO ABRE LA BASE DE DATOS");
        }



    }

    
}

int enterTable(char *nameDB, char *nameTable){ // ENTRAR EN UNA TABLA
 
    char extension[5] = ".tab";
    char barras[3] = "\\";
    char nameTableAux[100] = "";

    strcpy(nameTableAux, nameDB);
    strcat(nameTableAux, barras);
    strcat(nameTableAux, nameTable);
    strcat(nameTableAux, extension);

    DIR *dir;

    if(access(nameTableAux, F_OK) == 0){ // ACCESS: F_OK (¿EXISTE EL ARCHIVO?), SI TIENE EXITO DEVUELVE 0, SI NO -1
        if(dir = opendir(nameDB)){ 
                return 1;       
        }   
    }        
    else{
        return 0;
    }
}

TABLE* lookForTable(char *nameTable, TABLE *tabla){ // IMPORTANTE
    
    if(tabla != NULL ){ 
        while(tabla != NULL){ 
            if(strcmp(tabla->nombre, nameTable) == 0){ // COMPARACION DE QUE SE LLAMEN IGUAL
                return tabla;
            }
            tabla=tabla->sig; // SINO COMPARAMOS LA SIGUIENTE TABLA
        }
    }
    else{
        printf("\nERROR DESCONOCIDO");
        return tabla;
    }
}

void memoryTable (TABLE *tabla){ // LIBERAR MEMORIA DE LA TABLA

    TABLE *nextTable;
    int i;

    if(tabla == NULL){ // SI LA TABLA ESTA VACIA SALIMOS
        return;
    }
    else{ 
        while( tabla != NULL){ // MIENTRAS LA TABLA NO ESTE VACIA
            
            nextTable = tabla -> sig;

            for(i = 0; i < tabla -> numCampos; i ++){
                free(tabla -> campos[i]); // LIBERAR CAMPOS DE LA TABLA
            }
            free(tabla -> tipos); // LIBERAMOS LA CAJA DE TIEMPOS
            free(tabla -> campos); //  LIBERAMOS LA CAJA DE CAMPOS
            free(tabla); // LIBERAMOS TABLA

            tabla = nextTable;
        }
        
    }
}

TABLE* loadTable(char *nameDB){ // CARGAMOS LA TABLA

    int i,j,nTable,nFields;

    char aux1[11] = "\\meta.db";
    char aux2[50] = "";
    char row[1000], value[100];
    char *nameTable;
    char **nameField;

    TABLE *tabla = NULL, *tableNew;
    FILE *meta;
    TYPE *types;

    strcpy(aux2, nameDB);
    strcat(aux2, aux1); // NOMBRE DE LA BASE DE DATOS Y ACCESO AL META

    meta = fopen(aux2, "r+"); // ABRIMOS EL META

    if(meta == NULL){
        printf("ERROR: EL ARCHIVO META ESTA VACIO");
        return 0;
    }

    fgets(row, 1000, meta); // LEEMOS LOS PRIMEROS 1000 CARACTERES DE LA FILA || TABLA: NUMERO
    getField(row, ":", 1, value); // OBTENEMOS EL VALOR DEL CAMPO 1
    trim(value); // QUITAMOS LOS ESPACIOS AL VALOR
    nTable = atoi(value); // LO CONVERTIMOS EN NUMERO

    for (i = 0; i < nTable; i ++){ // PARA CADA FILA DE LA TABLA

        fgets(row, 1000, meta); // IGNORAMOS LOS GUIONES DEL META
        fgets(row, 1000, meta); // LA LINEA CON EL NOMBRE DE LA TABLA: NUMERO DE CAMPOS
        getField(row, ":", 0, value); // EL PRIMER VALOR SERA EL NOMBRE
        trim(value); // QUITA ESPACIOS
        nameTable = strdup(value); // APUNTAMOS NAMETABLE AL VALOR DE VALUE
        getField(row, ":", 1, value); // COGEMOS EL SEGUNDO CAMPO QUE ES EL NUMERO
        trim(value);
        nFields = atoi(value);

        nameField = malloc(sizeof(char*)*nFields); 
        types = malloc(sizeof(TYPE*)*nFields);

        for(j = 0; j < nFields; j ++){ // PARA CADA COLUMNA DE LA TABLA

            fgets(row,1000,meta); // MIRAMOS LA LINEA
            getField(row,":",0,value);  // NOMBRE DEL CAMPO
            trim(value);
            nameField[j]=strdup(value); 
            getField(row,":",1,value); // EL TIPO DEL CAMPO
            trim(value);
            types[j]=validateTypes(value); 
        }

        tableNew = initializeTable(nameTable, nFields, nameField, types);
        
        if(tabla == NULL){
            tabla = tableNew;
        }
        else{
            insertNextTable(tabla, tableNew);
        }
    }
    fclose(meta);
    return(tabla);
    
}

void displayTables(TABLE* tabla, char *nameDB){ // MOSTRAR TABLAS
    
    int i;
    TABLE *tableAux;

    tableAux = tabla;

    if(countTable(nameDB) == 0){
        printf("    - NO HAY TABLAS\n");
        return;
    }
   
    for(i = 0; i < countTable(nameDB); i ++){
        printf("    - %s\n", tableAux -> nombre);
      
        tableAux = tableAux -> sig;
    }
}

///////////////////////// L I N E A S /////////////////////////

ROW *initializeRow(char **valores){ // INICIALIZAMOS LAS FILAS

    ROW *row = malloc(sizeof(ROW)); // RESERVA DE MEMORIA PARA LA FILA

    if(row == NULL){ // ERROR
        return NULL;
    }

    row -> valores = valores; 
    row -> sig = NULL;
    row -> ant = NULL;

    return row;

}

void insertNextRow(ROW *row, ROW *rowAux){ // BUSCAMOS LA SIGUIENTE LINEA QUE ESTE VACIA

    ROW *nextRow;

    nextRow = row;

    while (nextRow -> sig != NULL){ // MIENTRAS QUE LA SIGUIENTE LINEA NO ESTE VACIA
        nextRow = nextRow -> sig; // PASAMOS A LA SIGUIENTE LINEA
    }

    nextRow -> sig = rowAux; // ASIGNAMOS LOS VALORES A LA SIGUIENTE TABLA (ESTARA VACIA)
    rowAux -> ant = nextRow;
    rowAux -> sig = NULL;
}

void createFieldRow(char *nameDB, TABLE *tableCurrent, ROW *row){ // ESCRIBIR EN EL FICHERO LOS VALORES  

    DIR *dir;
  

    char aux1[11] = ".tab";
    char aux2[50] = "\\";
    char aux3[50] = "";

    int i;

    strcpy(aux3, nameDB);
    strcat(aux3, aux2);
    strcat(aux3, tableCurrent -> nombre);
    strcat(aux3, aux1);

    if(dir = opendir(nameDB)){
        FILE *fichero;
        fichero = fopen(aux3, "w+");
        if(fichero != NULL){

            while (row != NULL){ // MIENTRAS QUEDEN FILAS
                
                for(i = 0; i < tableCurrent -> numCampos - 1; i ++){ // MENOS LA ULTIMA COLUMNA
                    if(tableCurrent -> tipos[i] == TEXT){ // ESCRIBIR LAS COMILLAS EN EL TXT
                        fprintf(fichero, "\"%s\", ",row -> valores[i]); // ESCRIBIMOS LOS VALORES DE LA LINEA
                    } 
                    else{
                        fprintf(fichero, "%s, ", row -> valores[i]); // RESTO DE TIPOS DE CAMPOS
                    }
                }

                if(tableCurrent -> tipos[tableCurrent -> numCampos - 1] == TEXT){ // ULTIMA COLUMNA PARA AÑADIR SALTO DE LINEA
                    fprintf(fichero, "\"%s\"\n", row -> valores[tableCurrent -> numCampos - 1]);
                }
                else{
                    fprintf(fichero, "%s\n", row -> valores[tableCurrent -> numCampos - 1]);
                }

                if(row -> sig == NULL){
                    break;
                }

                row = row -> sig;
            }
            fclose(fichero);
        }
        else{
            printf("ERROR: FICHERO VACIO");
        }
    } 
}

ROW *loadRows(char *nameDB, char *nameTable, int numFields, TYPE *tipos){ // CARGAMOS LOS REGISTROS
   
    int i, comillas;

    char extension[5] = ".tab";
    char barras[3] = "\\";
    char nameTableAux[100] = "";
    char row[1000] = "";
    char **valores; // DE LA ESTRUCTURA
    char *token, *rowAux;

    long lastPosition, final; // 4 BYTE = SIGNED LONG INT
    
    FILE *fichero;
    ROW *lines = NULL, *newLine;
    
    strcpy(nameTableAux, nameDB);
    strcat(nameTableAux, barras);
    strcat(nameTableAux, nameTable);
    strcat(nameTableAux, extension);

    if(access(nameTableAux,F_OK) == 0 ){ 

        fichero = fopen(nameTableAux, "r+"); 

        fseek(fichero, 0, SEEK_END ); // PONEMOS EL CURSOR EN LA ULTIMA POSICION

        if (ftell( fichero) == 0 ){ // DONDE ESTA EL CURSOR
            fclose(fichero);
            return lines;
        }

        final = ftell(fichero); // DONDE ESTA EL CURSOR
        fseek(fichero, 0, SEEK_SET); // MOVER EL CURSOR A LA PRIMERA POSICION, [0]


        while(final != ftell(fichero)){ // MIENTRAS NO LLEGUEMOS AL FINAL DEL FICHERO
           
            fgets(row, 1000, fichero);// LEEMOS LA CADENA (*CADENA, MAX CARACTERES PARA LEER, FICHERO)

            valores = (char**)malloc( numFields  * sizeof(char*) ); // RESERVA DE MEMORIA
            
            for(i = 0; i < numFields; i++){
               
                comillas = (tipos[i] == TEXT); // TEXT -> COMILAS = 1

                if(i == 0){
                    rowAux = row;
                }
                else{
                    rowAux = NULL;
                }

                token = MiStrTok(rowAux, ",\n", comillas);
                valores[i] = strdup(token); // COPIA CADENAS
            }
              
            newLine = initializeRow(valores); // CREAR LA ESTRUCTURA DE LA LINEA
             
            if(lines == NULL){ // PRIMERA LINEA
                lines = newLine;
            }
            else{
                insertNextRow(lines, newLine);
            }
        }        
    }
    else{
        printf("ERROR: NO HAY ACCESO AL FICHERO");
    }
    fclose(fichero);
    return lines;
}

void memoryLines (ROW *lines, int nFields){ // LIBERAR MEMORIA DE LAS FILAS

    ROW *nextLine;
    int i;

    if(lines == NULL){
        return;
    }
    else{
        while(lines != NULL){
            
            nextLine = lines -> sig; // SIGUIENTE FILA

            for(i = 0; i < nFields; i ++){ // PARA CADA CAMPO DE LA FILA
                free(lines -> valores[i]);
            }
            free(lines -> valores);
            free(lines);
            lines = nextLine;
        }
        

    }

}

///////////////////////// V A L I D A C I O N E S /////////////////////////

int validateDuplicateFields(char **chain, int nArg){ // DUPLICADOS DE CAMPOS

    int i, j;
    for(i = 0; i < nArg; i ++){
        for(j = i + 1; j < nArg; j ++){
            if(strcmp(chain[i], chain[j]) == 0){
                printf("ERROR: NO PUEDE HABER DOS CAMPOS IGUALES\n");
                return 1;
            }
        }
    }
    return 0;
}

int validateCharacters (char *chain){ // VALIDAR LA CADENA DE CARACTERES
    
    int i = 0, error = 0;
    char aux[50] = "";

    strcpy(aux, chain);

    while (aux[i] != '\0'){ // MIENTRAS SEA DIFERENTE DE UN ESPACIO EN BLANCO
    
       if(aux[i] >= 'a' && aux[i] <= 'z'){  // SI EL CARACTER [0] ESTA ENTRE LOS VALORES
           i ++;                            // CARACTER [1]
           continue;                        // Y REINICIAMOS EL BUCLE CON EL CARACTER [1]
       }
        if(aux[i] >= 'A' && aux[i] <= 'Z'){
            i ++;
            continue;
        }
        if(aux[i] >= '0' && aux[i] <= '9'){
            i ++;
            continue;
        }
        if(aux[i] == '_'){
            i ++;
            continue;
        }
        // SI LLEGA AL FINAL SALE DEL BUCLE ANTES DE SEGUIR
        printf("Caracter incorrecto");
        i ++;
        error ++; // SI HAY ALGO MAL ESCRITO DEVOLVERA 1
        break;
    }

    return error;    
}

TYPE validateTypes(char *value){ // TIPOS DE CAMPOS
   
    if(strcmp(value, "NUM") == 0){
        return NUM;
    }
    else if(strcmp(value, "DATE") == 0){
        return DATE;
    }
    else if(strcmp(value, "TEXT") == 0){
        return TEXT;
    }
    else if(strcmp(value, "EMAIL") == 0){
        return EMAIL;
    }
    else{
        return 0;
    }
}

int validateNUM(char *chain){ // VALIDAR UN NUMERO
    
    int i, j, inicio, p = 0;

    if(chain[0] == '+' || chain[0] == '-'){
        inicio = 1;
    }
    else{
        inicio = 0;
    }

    for(i = inicio; i < strlen(chain); i ++){ // DEVUELVE LA LOSNGITUD DE LA CADENA
        if(chain[i] < '0' || chain[i] > '9'){
            if (chain[i] == '.'){
                p ++;
            }
            else{
                return 0;
            }
        }

        if(p == 2){
            return 0;
        }
    }

    if(inicio == 1 && strlen(chain) == 1){
        return 0;
    }
    
    if (strlen(chain) == 0){
        return 0;
    }

    if(inicio == 1 && strlen(chain) == 2 && chain[1] == '.'){
        return 0;
    }

    if(inicio == 0 && strlen(chain) == 1 && chain[0] == '.'){
        return 0;
    }

    return 1;
}

int validateDate(char *chain){ // VALIDAR UNA FECHA

    int i, d, m, a, error;

    for(i = 0; i < 10; i ++){
        if(i != 2 && i != 5){
            if(chain[i] < '0' && chain[i] > '9'){
                error = 1;
            } 
        }
    }

    if(chain[2] != '/' || chain[5] != '/'){ // CON GUIONES && chain[2] != '-' || chain[5] != '-'
        error = 1;
    }

    d = (chain[0] - '0') * 10 + (chain[1] - '0');
    m = (chain[3] - '0') * 10 + (chain[4] - '0');
    a = (chain[6] - '0') * 1000 + (chain[7] - '0') * 100 + (chain[8] - '0') * 10 + (chain[9] - '0');

    if(error == 1){
        return 0;
    }
    else if(strlen(chain) != 10){
        return 0;
    }
    else if(d < 1 || m < 1 || a < 1 || d > 31 || m > 12 || a > 9999 ){
        return 0;
    }
    else if((m == 4 || m == 6 || m == 9 || m == 11) && d > 30){
        return 0;
    }
    else if(m == 2 && a % 4 == 0 && d > 29){
        return 0;
    }
    else if(m == 2 && a % 4 != 0 && d > 28){
        return 0;
    }
    else{
        return 1;
    }

}

int validateEmail(char *chain){ // [ALFANUMERICO] @ [ALFANUMERICO] . [ALFANUMERICO]
        
    chain = strchr(chain, '@'); // BUSCA EL @
    if(chain == NULL) return 0; // SI NO ENCUENTRA EL @ SERA FALSE

    return strchr(chain, '.') != NULL; //  LUEGO BUSCA EL PUNTO DESDE NOS HEMOS QUEDADO

}

int validateNameDB(char *argument){ // VALIDAR EL NOMBRE DE UNA BASE DE DATOS

    char chain[100] = "";
    int j = 0, error = 0;

    strcpy(chain, argument);
    
    while (chain[j] != '\0'){
        if(chain[j] != '.'){
            j ++;
            continue;
        }
        else{
            error ++;
            break;
        }
    }
    return error;
}

char* MiStrTok(char *cad1, char *sep, int comillas){ // FUNCION PARA PARTIR LA CADENA POR LAS COMILLAS                                                   
	static char *pt1=NULL;
	static char *pt2=NULL;
	static char *ptFinal;

	if(cad1!=NULL)
	{
		pt1 = cad1;
		ptFinal = pt1+strlen(cad1);
	}
	else
	{
		pt1=pt2+1;
	}

	if(comillas == 1)
	{
		while( pt1[0]!='\0' && pt1<ptFinal && strchr(sep, pt1[0])!=NULL && pt1[0]!='"')
			pt1++;
		if(pt1[0]=='"')
		{
			pt1++;
			pt2=pt1+1;
			while( pt2[0]!='\0' && pt2[0]!='"' )
				pt2++;
			if(pt2[0]=='\0')
			{
				pt1=pt2=ptFinal=NULL;
				return NULL;
			}
			else
			{
				pt2[0]='\0';
				return pt1;
			}
		}
	}
	else
	{
		while( pt1[0]!='\0' && pt1<ptFinal && strchr(sep, pt1[0])!=NULL )
			pt1++;
	}

	if(pt1>=ptFinal)
	{
		pt1=pt2=ptFinal=NULL;
		return NULL;
	}

	pt2=pt1+1;
	while( pt2[0]!='\0' &&  strchr(sep, pt2[0])==NULL )
		pt2++;
	pt2[0]='\0';

	return pt1;
}

char *getField(char *row, char *delimiter, int id, char *value){ // DEVUELVE EL VALOR DEL CAMPO 

    int i, delimiterSize = 1;
    char *x, *y;

    if(row == NULL || row[0] == '\0'){
        return NULL;
    }
    x = row;
    y = strstr(row, delimiter);

    for (i = 0; i < id; i ++){
        x = y + delimiterSize;
        y = strstr(x, delimiter);
    }
    if(y == NULL){
        strcpy(value,x);
    }
    else{
        strncpy(value, x, y - x);
        value [y - x] = '\0';
    }
    return value;
}

void trim(char *cad){ // ESTA FUNCION QUITA LOS ESPACIOS EN BLANCO
	int c, i, j, n=strlen(cad);
	
	if (cad==NULL || n<1)
		return;
	n--;
	while(n>=0 && ((c=cad[n])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		n--;
	cad[n+1]='\0';
	i=0;
	while (i<=n && ((c=cad[i])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		i++;
	for (j=0 ; j<=n-i+1; j++)
		cad[j]=cad[j+i];
}

///////////////////////// I M P R I M I R / D A T O S /////////////////////////

void showHeader(TABLE *tableCurrent){ // MOSTRAMOS LA CABECERA DE LA TABLA

    int i;

    for(i = 0; i < tableCurrent -> numCampos; i ++){
        printf("%s\t", tableCurrent -> campos[i]); // \t ES COMO PRESIONAR EL TAB (DEJAR UN ESPACIO)
    }
    printf("\n");
}

void showBody(int nFields, ROW *lines){ // MOSTRAMOS TODOS LOS REGISTROS DE LA TABLA
    
    int i, accountant = 1;
    ROW *lineAux = lines;

    if(lines == NULL){
        printf("ERROR: NO HAY REGISTROS");
    }
    else{
        while(lineAux != NULL){
           
            printf("[%d]", accountant); // CONTADOR
            for(i = 0; i < nFields - 1; i ++){
                printf("%s, ", lineAux -> valores[i]); // ESCRIBIMOS LOS REGISTROS
            }

            printf("%s\n", lineAux -> valores[nFields - 1]); // AÑADIMOS EL SALTO DE LINEA AL ULTIMO VALOR
            accountant ++;

            lineAux = lineAux -> sig;
        }   
    }
}

void showBodyASC(int nFields, ROW *lines, TABLE *tableCurrent){ // MOSTRAMOS TODOS LOS REGISTROS DE FORMA ASCENDENTE

    int i, accountant = 1;
    ROW *lineAux = lines;

    if(lines == NULL){
        printf("ERROR: NO HAY REGISTROS");
    }
    else{
        orderASC(lineAux, tableCurrent, 0); // MODIFICAMOS EL 0 PARA ORDENAR POR UN CAMPO QUE NO SEA EL PRIMERO
        while(lineAux != NULL){
           
            printf("[%d]", accountant); // CONTADOR
            for(i = 0; i < nFields - 1; i ++){
                printf("%s, ", lineAux -> valores[i]); // ESCRIBIMOS LOS REGISTROS
            }

            printf("%s\n", lineAux -> valores[nFields - 1]); // AÑADIMOS EL SALTO DE LINEA AL ULTIMO VALOR
            accountant ++;

            lineAux = lineAux -> sig;
        }   
    }
}

void orderASC(ROW *lines, TABLE *tableCurrent, int field){ // ORDENAR ASCENDENTE

    ROW *i, *j;

    if(tableCurrent -> tipos[field] == NUM){

        for(i = lines; i != NULL; i = i -> sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(atof(i -> valores[field]) > atof(j -> valores[field])){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
    if(tableCurrent->tipos[field] == DATE){
        for(i = lines; i != NULL; i = i->sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(DATEtoNUM(i -> valores[field]) > DATEtoNUM(j -> valores[field])){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
    if(tableCurrent -> tipos[field] == TEXT){
        for(i = lines; i != NULL; i = i -> sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(strcmp(i -> valores[field], j -> valores[field]) > 0){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
    if(tableCurrent -> tipos[field] == EMAIL){
        for(i = lines; i != NULL; i = i -> sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(strcmp(i -> valores[field], j -> valores[field]) > 0){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
}

void showBodyDES(int nFields, ROW *lines, TABLE *tableCurrent){ // MOSTRAMOS TODOS LOS REGISTROS DE FORMA DESCENDENTE

    int i, accountant = 1;
    ROW *lineAux = lines;

    if(lines == NULL){
        printf("ERROR: NO HAY REGISTROS");
    }
    else{
        orderDES(lineAux, tableCurrent, 0); // MODIFICAMOS EL 0 PARA ORDENAR POR UN CAMPO QUE NO SEA EL PRIMERO
        while(lineAux != NULL){
           
            printf("[%d]", accountant); // CONTADOR
            for(i = 0; i < nFields - 1; i ++){
                printf("%s, ", lineAux -> valores[i]); // ESCRIBIMOS LOS REGISTROS
            }

            printf("%s\n", lineAux -> valores[nFields - 1]); // AÑADIMOS EL SALTO DE LINEA AL ULTIMO VALOR
            accountant ++;

            lineAux = lineAux -> sig;
        }   
    }
}

void orderDES(ROW *lines, TABLE *tableCurrent, int field){ // ORDENAR DESCENDENTE

    ROW *i, *j;

    if(tableCurrent -> tipos[field] == NUM){

        for(i = lines; i != NULL; i = i -> sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(atof(i -> valores[field]) < atof(j -> valores[field])){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
    if(tableCurrent -> tipos[field] == DATE){
        for(i = lines; i != NULL; i = i->sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(DATEtoNUM(i -> valores[field]) < DATEtoNUM(j -> valores[field])){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
    if(tableCurrent -> tipos[field] == TEXT){
        for(i = lines; i != NULL; i = i -> sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(strcmp(i -> valores[field], j -> valores[field]) < 0){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
     if(tableCurrent -> tipos[field] == EMAIL){
        for(i = lines; i != NULL; i = i -> sig){
   
            for (j = i; j != NULL; j = j -> sig){
               
                if(strcmp(i -> valores[field], j -> valores[field]) < 0){
                   // printf("%s > %s\n", i->valores[field], j->valores[field]);
                   // printf("CAMBIANDO\n");
                    exchangeValues(i, j);

                }   
            }
        }
    }
}

void exchangeValues(ROW *linesAux1, ROW *linesAux2){ // INTERCAMBIAR VALORES

    char **linesAux;

    linesAux = linesAux1 -> valores;
    linesAux1 -> valores = linesAux2 -> valores;
    linesAux2 -> valores = linesAux;
}

int DATEtoNUM(char* chain){ // CONVERTIMOS LA FECHA A UN NUMERO
    // DIA/MES/AÑO
	int res, dia, mes, anyo;
	dia = (chain[0] - '0') * 10 + (chain[1] - '0');
    mes = (chain[3] - '0') * 10 + (chain[4] - '0');
    anyo = (chain[6] - '0') * 1000 + (chain[7] - '0') * 100 + (chain[8] - '0') * 10 + (chain[9] - '0');
	res = dia;

	if (mes == 1)
		res = res;
	else if (mes == 2)
		res = res + 31;
	else if (mes == 3 && (anyo%4) != 0)
		res = res + 59;
	else if (mes == 3 && (anyo%4) == 0)
		res = res + 60;
	else if (mes == 4 && (anyo%4) != 0)
		res = res + 90;
	else if (mes == 4 && (anyo%4) == 0)
		res = res + 91;
	else if (mes == 5 && (anyo%4) != 0)
		res = res + 120;
	else if (mes == 5 && (anyo%4) == 0)
		res = res + 121;
	else if (mes == 6 && (anyo%4) !=0)
		res = res + 151;
	else if (mes == 6 && (anyo%4) == 0)
		res = res + 152;
	else if (mes == 7 && (anyo%4) != 0)
		res = res + 181;
	else if (mes == 7 && (anyo%4) == 0)
		res = res + 182;
	else if (mes == 8 && (anyo%4) != 0)
		res = res + 212;
	else if (mes == 8 && (anyo%4) == 0)
		res = res + 213;
	else if (mes == 9 && (anyo%4) !=0 )
		res = res + 243;
	else if (mes == 9 && (anyo%4) == 0)
		res = res + 244;
	else if (mes == 10 && (anyo%4) != 0)
		res = res + 273;
	else if (mes == 10 && (anyo%4) == 0)
		res = res + 274;
	else if (mes == 11 && (anyo%4) != 0)
		res = res + 304;
	else if (mes == 11 && (anyo%4) == 0)
		res = res + 305;
	else if (mes == 12 && (anyo%4) != 0)
		res = res + 334;
	else if (mes == 12 && (anyo%4) == 0)
		res = res + 335;

	res = (res + (anyo - 1) * 365);
	res = (res + (anyo - 1) / 4);

	return res;
}

void showFieldValue(ROW *lines, TYPE types, char *operator, char *value, int position, int nFields){ // 

    int i, accountant = 1;
    ROW *lineAux = lines;

    if(lines == NULL){
        printf("ERROR: FILA VACIA");
    }
    else{

        if(types == TEXT){ // PARA EL TIPO TEXTO

            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){
                    if(strcmp(value, lineAux -> valores[position]) == 0){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, ">") == 0){

                 while(lineAux != NULL){
                    if(strcmp(value, lineAux -> valores[position]) > 0){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, "<") == 0){

                 while(lineAux != NULL){
                    if(strcmp(value, lineAux -> valores[position]) < 0){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }

            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' ");
            }

        }
        else if(types == DATE){ // PARA EL TIPO DATE
             
            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){
                    if(DATEtoNUM(lineAux -> valores[position]) == DATEtoNUM(value)){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, ">") == 0){
                while(lineAux != NULL){
                    if(DATEtoNUM(lineAux -> valores[position]) > DATEtoNUM(value)){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, "<") == 0){
                while(lineAux != NULL){
                    if(DATEtoNUM(lineAux -> valores[position]) < DATEtoNUM(value)){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' ");
            }
        }
        else if(types == NUM){ // PARA EL TIPO NUM
            
            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){
                    if(atof(lineAux -> valores[position]) == atof(value)){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, ">") == 0){
                 while(lineAux != NULL){
                    if(atof(lineAux -> valores[position]) > atof(value)){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, "<") == 0){
                 while(lineAux != NULL){
                    if(atof(lineAux -> valores[position]) < atof(value)){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' ");
            }
        }
        if(types == EMAIL){ // PARA EL TIPO TEXTO

            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){
                    if(strcmp(value, lineAux -> valores[position]) == 0){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, ">") == 0){

                 while(lineAux != NULL){
                    if(strcmp(value, lineAux -> valores[position]) > 0){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }
            }
            else if(strcmp(operator, "<") == 0){

                 while(lineAux != NULL){
                    if(strcmp(value, lineAux -> valores[position]) < 0){ // COMPARAMOS LA CADENA DE CARACTERES
                        printf("[%d] ", accountant); // MOSTRAMOS EL CONTADOR
                        for(i = 0; i < nFields - 1; i ++){
                            printf("%s, ", lineAux -> valores[i]); // MOSTRAMOS TODOS LOS VALORES MENOS EL ULTIMO
                        }
                        printf("%s\n", lineAux -> valores[nFields - 1]); // ULTIMO VALOR CON SALTO DE LINEA AÑADIDO
                        accountant ++;
                    }
                    lineAux = lineAux -> sig;
                }

            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' ");
            }

        }
        
        else{
           
        }

    }
}

void showDB(){ // MOSTRAR LAS BASES DE DATOS QUE EXISTEN

    DIR *d;
    struct dirent *f;
    int j = 0;

    d = opendir(".");

    if(d == NULL){
        printf("NO HAY BASES DE DATOS\n");
        return;
    }

    while ((f = readdir(d)) != NULL){
        if((strcmp(f -> d_name, ".") != 0) && (strcmp(f -> d_name, "..") != 0)){
            if(validateNameDB(f -> d_name)  == 0){
                printf("    - %s\n", f -> d_name);
                j ++;
            }
        }
    }

    if(j == 0){
        printf("NO HAY BASES DE DATOS\n");
        return;
    }
    closedir(d);
}


///////////////////////// B O R R A D O / D E / D A T O S /////////////////////////

ROW* deleteLineValueSpecified(int nFields, ROW *lines, TYPE type, char *value, char *operator, int position){ // BORRAR EL VALOR DESEADO

    int i, j = 1, k;
    ROW *lineAux = lines, *nextLine, *prevLine;

    if (lines == NULL){ // SI EL FICHERO ESTA VACIO
        printf("ERROR: NO HAY DATOS\n");
    }
    else{
        if(type == NUM){ // POR PARAMETRO
            if(strcmp(operator, "=") == 0){ // EN CASO DE IGUALDAD DE NUMEROS
                while(lineAux != NULL){ // MIENTRAS QUEDEN LINEAS
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(atof(value) == atof(lineAux -> valores[position])){ // CONVERTIMOS EL TEXTO A NUMERO Y COMPARAMOS
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, ">") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(atof(lineAux -> valores[position]) > atof(value)){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux->ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                        j++; 
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, "<") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(atof(lineAux -> valores[position]) < atof(value)){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux->ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux->sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux->valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else{
               printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' \n ");
            }
        }
        else if(type == DATE){ 
            printf("CUANDO BORRO CON UN TIPO FECHA, LA PRIEMRA VEZ BORRA TODO(NOSE PORQUE) PERO SI INSERTAS LOS VALORES Y LO HACES DE NUEVO SI LO HACE BIEN");
            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){ // MIENTRAS QUEDEN LINEAS
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(DATEtoNUM(value) == DATEtoNUM(lineAux->valores[position])){ // CONVERTIMOS LAS FECHAS A NUMEROS PARA COMPARARLAS
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){                        
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, ">") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(DATEtoNUM(lineAux -> valores[position]) > DATEtoNUM(value)){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;                           
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, "<") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(DATEtoNUM(lineAux -> valores[position]) < DATEtoNUM(value)){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' \n ");            
                }
        }
        else if(type == TEXT){ 
            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){ 
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(strcmp(value, lineAux -> valores[position]) == 0){ // COMPARAMOS LA CADENA DE CARACTERES 
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, ">") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(strcmp(lineAux -> valores[position], value) > 0){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, "<") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(strcmp(lineAux -> valores[position],value) < 0){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux->ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0 ; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' \n ");            
            }
        }
           else if(type == EMAIL){ 
            if(strcmp(operator, "=") == 0){
                while(lineAux != NULL){ 
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(strcmp(value, lineAux -> valores[position]) == 0){ // COMPARAMOS LA CADENA DE CARACTERES 
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, ">") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(strcmp(lineAux -> valores[position], value) > 0){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux -> ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else if(strcmp(operator, "<") == 0){
                while(lineAux != NULL){
                    prevLine = lineAux -> ant;
                    nextLine = lineAux -> sig;
                    if(strcmp(lineAux -> valores[position],value) < 0){
                        if(lineAux == lines && lineAux -> sig == NULL){
                            lines = NULL;
                        }
                        else if(lineAux == lines && lineAux->ant == NULL){
                            lines = lines -> sig;
                            nextLine -> ant = NULL;
                            lineAux -> sig = NULL;
                        }
                        else if(lineAux -> sig == NULL){
                            prevLine -> sig = NULL;
                            lineAux -> ant = NULL;
                        }
                        else{
                            prevLine -> sig = lineAux -> sig;
                            nextLine -> ant = lineAux -> ant;
                        }                        
                        for(i = 0 ; i < nFields; i ++){
                            free(lineAux -> valores[i]);
                        }
                        free(lineAux -> valores);
                        free(lineAux);
                    }
                    lineAux = nextLine;
                }
                return lines;
            }
            else{
                printf("ERROR: SOLO ACEPTA LOS OPERADORES '=' / '<' / '>' \n ");            
            }
        }    
    }
}

void updateRow(ROW *lines, TABLE *tableCurrent, char *nameDB){ // ESCRIBIR EN EL FICHERO TODOS LOS VALORES DE LAS LINEAS

    DIR *dir;
    char aux1[11] = ".tab";
    char aux2[50] = "\\";
    char aux3[50] = "";
    int i;

    strcpy(aux3, nameDB);
    strcat(aux3, aux2);
    strcat(aux3, tableCurrent -> nombre);
    strcat(aux3, aux1); // RUTA AL DIRECTORIO

    if(dir = opendir(nameDB)){
        FILE *fichero;
        fichero = fopen(aux3, "w+");
        if(fichero != NULL){
            
            while (lines != NULL){
                for(i = 0; i < tableCurrent -> numCampos - 1; i ++){
                    if(tableCurrent -> tipos[i] == TEXT){ // ESTE CASO ES PARA EL TIPO TEXT
                        fprintf(fichero, "\"%s\", ", lines -> valores[i]);
                    }
                    else{
                        fprintf(fichero, "%s, ", lines -> valores[i]); // ESTE CASO ES PARA NUM DATE EMAIL
                    }
                }

                if(tableCurrent -> tipos[tableCurrent -> numCampos - 1] == TEXT){
                fprintf(fichero, "\"%s\"\n",lines -> valores[tableCurrent -> numCampos - 1]);
                }
                else{
                    fprintf(fichero, "%s\n", lines -> valores[tableCurrent -> numCampos - 1]);
                }    
                if(lines -> sig == NULL){
                    break;
                }
                lines = lines -> sig;
            }
            fclose(fichero);
        }
        else{
            printf("\nERROR");
        }
    }
}




////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

char* NUMtoDATE(int res){ // CONVERTIMOS EL NUMERO A UNA FECHA
	
    int anyo, mes=0, dia, bi,i;
	char *fecha;
	float a;

	fecha = malloc(sizeof(char) * 10);

	a = 0.00273785463;
	anyo = ((res) * a);
	bi = ((anyo) / 4);
	anyo = anyo + (bi / 365);
	if (anyo % 4 != 0)
		res = (res - bi - ((anyo - 1) * 365));
	else if (anyo % 4 == 0)
		res = (res - bi - ((anyo - 1) * 365));

	if (res <= 31)
		mes = 1;
	else if (res <= 59 && (anyo % 4) != 0)
	{
		mes = 2;
		res = res - 31;
	}
	else if (res <= 60 && (anyo % 4) == 0)
	{
		mes = 2;
		res = res - 31;
	}
	else if (res <= 90 && (anyo % 4) != 0)
	{
		mes = 3;
		res = res - 59;
	}
	else if (res <= 91 && (anyo % 4) == 0)
	{
		mes = 3;
		res = res - 60;
	}
	else if (res <= 120 && (anyo % 4) != 0)
	{
		mes = 4 ;
		res = res - 90;
	}
	else if (res <= 121 && (anyo % 4) == 0)
	{
		mes = 4;
		res = res - 91;
	}
	else if (res <= 151 && (anyo % 4) != 0)
	{
		mes = 5;
		res = res - 120;
	}
	else if (res <= 152 && (anyo % 4) == 0)
	{
		mes = 5;
		res = res - 121;
	}
	else if (res <= 181 && (anyo % 4) != 0)
	{
		mes = 6;
		res = res - 151;
	}
	else if (res <= 182 && (anyo % 4) == 0)
	{
		mes = 6;
		res = res - 152;
	}
	else if (res <= 212 && (anyo % 4) != 0)
	{
		mes = 7;
		res = res - 181;
	}
	else if (res <= 213 && (anyo % 4) == 0)
	{
		mes = 7;
		res = res - 182;
	}
	else if (res <= 243 && (anyo % 4) !=0)
	{
		mes = 8;
		res = res - 212;
	}
	else if (res <= 244 && (anyo % 4) == 0)
	{
		mes = 8;
		res = res - 213;
	}
	else if (res <= 273 && (anyo % 4) != 0)
	{
		mes = 9;
		res = res - 243;
	}
	else if (res <= 274 && (anyo % 4) == 0)
	{
		mes = 9;
		res = res - 244;
	}
	else if (res <= 304 && (anyo % 4) != 0)
	{
		mes = 10;
		res = res - 273;
	}
	else if (res <= 305 && (anyo % 4) == 0)
	{
		mes = 10;
		res = res - 274;
	}
	else if (res <= 334 && (anyo % 4) != 0)
	{
		mes = 11;
		res = res - 304;
	}
	else if (res <= 335 && (anyo % 4) == 0)
	{
		mes = 11;
		res = res - 305;
	}
	else if (res <= 365 && (anyo % 4) != 0)
	{
		mes = 12;
		res = res - 334;
	}
	else if (res <= 366 && (anyo % 4) == 0)
	{
		mes = 12;
		res = res - 335;
	}
	dia = res;
	if (dia == 0)
	{
		dia = 31;
		mes = 12;
		anyo = anyo - 1;
	}
	if (dia == -1)
	{
		dia = 30;
		mes = 12;
		anyo = anyo - 1;
	}
    if(dia < 10 && mes < 10)
		sprintf(fecha, "0%i/0%i/%i", dia, mes, anyo);
	else if (dia < 10 && mes >= 10)
		sprintf(fecha, "0%i/%i/%i", dia, mes, anyo);
	else if (dia >= 10 && mes < 10)
		sprintf(fecha, "%i/0%i/%i", dia, mes, anyo);
	else
		sprintf(fecha, "%i/%i/%i", dia, mes, anyo);

	return fecha;
}

ROW* renombrar_linea_valor_deseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion, char *nuevovalor){ 

    int i, j=1, k;
    ROW *lineaaux = linea, *lineasiguiente, *lineaanterior;

    if (linea == NULL){ // por si el fichero estuviese vacio
        printf("No se han encontrado datos\n");
    }
    else{
        printf("hola");
        if(tipo == NUM){ // esto se lo pasamos por parametro
            printf("hola2");
            if(strcmp(simbolo,"=")==0){ // en el caso de que quiera igualdad de numeros
                printf("hola3");
                while(lineaaux != NULL){ // mientras hayan lineas
                    printf("hola4");

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(atof(valor) == atof(lineaaux->valores[posicion])){ // convertimos el texto a numeros reales y los comparamos

                        if(validateNUM(nuevovalor)){ // comprobamos q sea un numero el nuevo valor
                            lineaaux->valores[posicion] = strdup(nuevovalor);
                        }
                        else{
                           printf("El nuevo valor no es un numero\n");
                        } 

                    }

                    lineaaux = lineasiguiente;
                }
                return linea;
            }
        }
        else if(tipo == DATE){ // este es el caso de que sea tipo fecha
            printf("hola2");
            if(strcmp(simbolo,"=")==0){
                printf("hola3");
                while(lineaaux != NULL){ // mientras hayan lineas
                    printf("hola4");

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;


                    if(DATEtoNUM(valor) == DATEtoNUM(lineaaux->valores[posicion])){ // convertimos las fechas a numeros para asi poder compararlas
                       
                        if(validateDate(nuevovalor)){ // comprobamos q sea un numero el nuevo valor
                            lineaaux->valores[posicion] = strdup(nuevovalor);
                        }
                        else{
                           printf("El nuevo valor no es una fecha\n");
                        } 

                    }

                    lineaaux = lineasiguiente;
                }

                return linea;
            }
        }
        else if(tipo == TEXT){ // en el caso de que sea tipo texto
            if(strcmp(simbolo,"=")==0){
                while(lineaaux != NULL){ // mientras hayan lineas

                    lineaanterior = lineaaux->ant;
                    lineasiguiente = lineaaux->sig;

                    if(strcmp(valor,lineaaux->valores[posicion])==0){ // comparamos la cadena de caracteres
                        
                        lineaaux->valores[posicion] = strdup(nuevovalor);
                    }

                    lineaaux = lineasiguiente;

                }

                return linea;
            }
        }    
    }
}

int renombrar_BD(char *nombre_bd, char *nuevo_nombre_bd){

    DIR *dir;

    if(dir = opendir(nombre_bd)){// entramos al directorio 

        if(rename(nombre_bd, nuevo_nombre_bd) != 0){
            
            printf("No se pudo renombrar");
            return 0;
        }
        else{
            
            return 2;
        }
    }
    else{
        printf("La db: %s no fue encontrada\n", nombre_bd);
        return 1;
    }

}

TABLE* borrar_bd (TABLE *tabla, char *nombre_bd){
    TABLE *tablasiguiente, *tableaux, *tablaanterior;
    DIR *dir;
    int i, aux;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";


    if(dir = opendir(nombre_bd)){
    
        if(tabla == NULL)
        {
            printf("Tabla vacia");
            return tabla;
        }

        while(tabla != NULL){

            tableaux = tabla;
            tablaanterior = tableaux->ant;
            tablasiguiente = tableaux->sig;

            strcpy(aux3,nombre_bd);
            strcat(aux3,aux2);
            strcat(aux3,tableaux->nombre);
            strcat(aux3,aux1);
            
            if(tableaux == tabla && tableaux->sig == NULL){
                tabla=NULL;
            }
            else if(tableaux == tabla && tableaux->ant == NULL){

                tabla = tabla->sig;
                tablasiguiente->ant = NULL;
                tableaux ->sig = NULL;

            }
            else if(tableaux->sig == NULL){

                tablaanterior->sig = NULL;
                tableaux->ant = NULL;
            }
            

            for(i=0;i<tableaux->numCampos;i++){

                free(tableaux->campos[i]);
            }

            free(tableaux->tipos);
            free(tableaux->campos);
            free(tableaux);

            remove(aux3);

        }

    }
    return tabla;
}

TABLE* borrar_tabla(TABLE *tabla, char *nombre_bd, char *tablaborrar){ // BORRAR UN NODO CONCRETO DE UNA LISTA

    TABLE *tablasiguiente, *tableaux, *tablaanterior;
    DIR *dir;
    int i, aux;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";


    if(dir = opendir(nombre_bd)){
    
        if(tabla == NULL)
        {
            printf("Tabla vacia");
            return tabla;
        }

        aux = enterTable(nombre_bd, tablaborrar);
        printf("3");

        if(aux == 1){

            strcpy(aux3,nombre_bd);
            strcat(aux3,aux2);
            strcat(aux3,tablaborrar);
            strcat(aux3,aux1);

            printf("4");
            tableaux = lookForTable(tablaborrar, tabla);

            tablaanterior = tableaux->ant;
            tablasiguiente = tableaux->sig;
            
            if(tableaux == tabla && tableaux->sig == NULL){
                printf("primer caso");
                tabla=NULL;
            }
            else if(tableaux == tabla && tableaux->ant == NULL){

                printf("segundo caso");
                tabla = tabla->sig;
                tablasiguiente->ant = NULL;
                tableaux ->sig = NULL;

            }
            else if(tableaux->sig == NULL){
                
                printf("tercer caso");
                tablaanterior->sig = NULL;
                tableaux->ant = NULL;
            }
            else{

                printf("ultimo caso");
                tablaanterior->sig = tableaux->sig;
                tablasiguiente->ant = tableaux->ant;
            }

            for(i=0;i<tableaux->numCampos;i++){

                free(tableaux->campos[i]);
            }

            free(tableaux->tipos);
            free(tableaux->campos);
            free(tableaux);

            remove(aux3);

        }

    }
    return tabla;
}

TABLE* renombrar_tabla(TABLE* tabla, char *nombre_bd, char *nombre_tabla, char *nombre_nuevo_tabla){

    DIR *dir;
    TABLE *tablaaux = tabla;
    char aux1[5]=".tab";
    char aux2[3]="\\";
    char aux3[100]="";
    char aux4[100]="";
    char aux5[100]="";
    int i;
    

    strcpy(aux3, nombre_bd);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3,aux1);

    if(dir = opendir(nombre_bd)){// entramos al directorio 

        strcpy(aux5,nombre_bd);
        strcat(aux5,aux2);
        strcat(aux5,nombre_nuevo_tabla);
        strcat(aux5,aux1);

        i = enterTable(nombre_bd, nombre_nuevo_tabla);

        if(i == 1){
            
            printf("La tabla: %s no puede actualizarse porque %s ya existe en nuestra bd",nombre_tabla, nombre_nuevo_tabla);
            return tabla;
        }

        if(rename(aux3, aux5) != 0){
            printf("No se pudo renombrar");
            return tabla;
        }
        else{
            
            tablaaux = lookForTable(nombre_tabla, tablaaux);
            strcpy(tablaaux->nombre,nombre_nuevo_tabla);
            return tabla;
        }
    }
    else{
        printf("La tabla: %s no fue encontrada\n", nombre_tabla);
        return tabla;
    }
}

void borrar_lineas(ROW *lineas, int num_campos, char *prompt, char *nombre_bd){ // BORRAR TODAS LAS LINEAS DE UN FICHERO

    ROW *lineasiguiente;
    DIR *dir;
    char aux1[10] = ".tab";
    char aux2[110] = "";    
    int i;

    printf("%s\n",prompt);
    strcpy(aux2, prompt);
    strcat(aux2, aux1);

    printf("%s\n", aux2);

     if(dir = opendir(nombre_bd)){

        printf("Entramos en la carpeta\n");

        FILE *fichero;
        fichero = fopen(aux2, "w");
        if(fichero != NULL){
            printf("Entramos en el fichero\n");
            if(lineas != NULL){

                memoryLines(lineas, num_campos);

                fprintf(fichero, "");
                fclose(fichero);
            }
            printf("ya esta todo borrado\n");
        }
        else{
            printf("Ya esta todo borrado\n");
        }
    }

    else{
        printf("ERROR al abrir el directorio");
    }


}

int EnteroValidar (char *cad)
{
	int i, ini;
	
	if(cad[0]=='+' || cad[0]=='-') // en el caso de que queramos q sea un numero entero positivo, quitaremos la parte del -
		ini=1;
	else 
		ini=0;
	for (i=ini; i<strlen(cad); i++)
		{
			if (cad[i]<'0' || cad[i]>'9')
				return 0;
		}
	if (ini==1 && strlen(cad)==1)
		return 0;
	if (ini==0 && strlen(cad)==0)
		return 0;
return 1;
}
