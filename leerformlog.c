#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sqlite3.h>

#define MAXLEN 1024


int callback(void * arg, int numberColumns, char ** value, char ** columName){
	(void)arg;
	int i;
	printf("<tr>");
	for (i = 0; i < numberColumns; i++){
		printf("<td style = \"text-align: center;\"> %s</td>", value[i]);
		//printf("<p> %s:%s </p>", columName[i], value[i]);
	}
	printf("</tr>");
	return 0;
}
int callback2(void * arg, int numberColumns, char ** value, char ** columName){
	(void)arg;
	printf("<h2>La cantidad de archivos es: %s</h2>",value[0]);
}
int callback3(void * arg, int numberCOlumns, char ** value, char ** columName){
	(void)arg;
	printf("<h2>Velocidad promedio de transferencia: %s Kbyte/sec </h2>",value[0]);
}
int execQuery(sqlite3 * db, int query, char * startDate, char * endDate,char * event){
	char sql[200];
	int rc;
	if(strcmp(event,"LOGIN") == 0 || strcmp(event,"FAILLOGIN") == 0){
		sprintf(sql,"select * from eventLogin where date > '%s' and date < '%s' and evenType = '%s'  ;",startDate, endDate, event);
		printf("<table>");
		printf("<tr>");
		printf("<th>Id</th>");
		printf("<th>Fecha</th>");
		printf("<th>Pid</th>");
		printf("<th>Host</th>");
		printf("<th>Evento</th>");
		printf("<th>Cliente</th>");
		printf("</tr>");
		rc = sqlite3_exec(db,sql,callback,NULL,NULL);
		printf("</table>");
		return 0;
	}else{


	if(query == 1){
                if(strcmp(event,"ALL") == 0){
                        sprintf(sql,"select count(*)  from event where pathFile != 'NULL' and date > '%s' and date < '%s';",startDate,endDate);
                }else{
                        sprintf(sql,"select count(*)  from event where pathFile != 'NULL' and date > '%s' and date < '%s' and evenType = '%s'  ;",startDate, endDate, event);
                }
		rc = sqlite3_exec(db,sql,callback2,NULL,NULL);
		return 0;
	}

	if(query == 3){
                if(strcmp(event,"ALL") == 0){
                        sprintf(sql,"select avg(time)  from event where date > '%s' and date < '%s';",startDate,endDate);
                }else{
                        sprintf(sql,"select avg(time)  from event where date > '%s' and date < '%s' and evenType = '%s'  ;",startDate, endDate, event);
                }
        	rc = sqlite3_exec(db,sql,callback3,NULL,NULL);
                return 0;
	}
	if(query == 4){
                if(strcmp(event,"ALL") == 0){
                        sprintf(sql,"select DISTINCT customer from event where date > '%s' and date < '%s';",startDate,endDate);
                }else{
                        sprintf(sql,"select DISTINCT customer from event where date > '%s' and date < '%s' and evenType = '%s'  ;",startDate, endDate, event);
                }
	printf("<h2>Clientes encontrados:</h2>");
	printf("<table style = \"width: 200px\">");
        printf("<tr>");
	printf("<th>Cliente</th>");
	printf("</th>");
		rc = sqlite3_exec(db,sql,callback,NULL,NULL);
	printf("</table>");
        	return 0;
	}
	
	if(query == 2){
                if(strcmp(event,"ALL") == 0){
                        sprintf(sql,"select file, size  from event where size > 10000000  and date > '%s' and date < '%s';",startDate,endDate);
                }else{
                        sprintf(sql,"select file, size  from event where size > 10000000 and date > '%s' and date < '%s' and evenType = '%s'  ;",startDate, endDate, event);
                }
		printf("<h2>Archivos mayores a 10 MB:</h2>");
		printf("<table style = \"width: 200px\">");
	        printf("<tr>");
       	 	printf("<th>Archivo</th>");
        	printf("<th>Peso</th>");
		printf("</tr>");
		rc = sqlite3_exec(db,sql,callback,NULL,NULL);
		printf("</table>");
		return 0;
	}

	if(query == 5){
		if(strcmp(event,"ALL") == 0){
//                     select count(*), customer from event where evenType != 'FAILDOWNLOAD' and evenType != 'FAILUPLOAD' and date > '2022-05-06' and date < '2022-05-30 'group by customer
                	sprintf(sql,"select count(*) , customer from event where date > '%s' and date < '%s' and  evenType != 'FAILDOWNLOAD' and evenType != 'FAILUPLOAD' group by customer",startDate,endDate );
		}else{

                   sprintf(sql,"select count(*), customer from event where evenType != 'FAILDOWNLOAD' and evenType != 'FAILUPLOAD' and date > '%s' and date < '%s' and evenType = '%s' group by customer",startDate,endDate,event);

                }

                printf("<h2>Cantidad de archivos/Cliente</h2>");
                printf("<table style = \"width: 200px\">");
                printf("<tr>");
                printf("<th>Cantidad archivos</th>");
                printf("<th>Cliente</th>");
                printf("</tr>");
                rc = sqlite3_exec(db,sql,callback,NULL,NULL);
                printf("</table>");
                return 0;
	}


	 if(query == 6){
                if(strcmp(event,"ALL") == 0){
                        sprintf(sql,"select count(*),file  from event where file != 'NULL' and date > '%s' and date < '%s' group by file order by count(*) desc;",startDate,endDate);
                }else{
                        sprintf(sql,"select count(*),file  from event where file != 'NULL' and date > '%s' and date < '%s' and evenType = '%s' group by file order by count(*) desc ;",startDate, endDate, event);
                }
                printf("<h2>Frecuencia de archivos</h2>");
                printf("<table style = \"width: 200px\">");
                printf("<tr>");
                printf("<th>Cantidad</th>");
                printf("<th>Archivo</th>");
                printf("</tr>");
                rc = sqlite3_exec(db,sql,callback,NULL,NULL);
                printf("</table>");
                return 0;
        }



	
	printf("<table>");
	printf("<tr>");
	printf("<th>Id</th>");
	printf("<th>Fecha </th>");
	printf("<th>Pid</th>");
	printf("<th>Host</th>");
	printf("<th>Event </th>");
	printf("<th>Cliente</th>");

	printf("<th>Ruta</th>");
	printf("<th>File</th>");
        printf("<th>Tamaño</th>");
        printf("<th>Velocidad</th>");
	//int res = strcmp(event,"ALL");
		
	//printf("<p>Numero:%s</p>",res);
	if(query == 0){
		if(strcmp(event,"ALL") == 0){
			sprintf(sql,"Select * from event where date > '%s' and date < '%s';",startDate,endDate);
		}else{
	        	sprintf(sql,"Select * from event where date > '%s' and date < '%s' and evenType = '%s';",startDate,endDate,event);			}
	}

	rc = sqlite3_exec(db,sql,callback,NULL,NULL);
	printf("</tr>");


	if(rc != SQLITE_OK){
		fprintf(stderr,"Cant open database%s\n",sqlite3_errmsg(db));
		printf("Error2%s",sqlite3_errmsg(db));
	}
	printf("</table>");
	}
	return 0;
}
void separar(char *cadena, char *linea, char separador)
{
     int x, y;

    x = 0; y = 0;

    while ((linea[x]) && (linea[x] != separador))
    {
        cadena[x] = linea[x];
        x = x + 1;
    }
    cadena[x] = '\0';
    if (linea[x]) ++x;

    y = 0;

    while (linea[y] = linea[x])
    {
        linea[y] = linea[x];
        y++;
        x++;
    }


}

int pullApart(char * query,int n, char * inputBuffer, char sep){
        char str [10] = "query";
        char aux [100];
        char *  num;
        asprintf(&num, "%d", n);
        strcat(str,num);
        int i = 0;int j = 0;
        while(i < strlen(inputBuffer)){
                if(inputBuffer[i] == 'q'){
                        while(inputBuffer[i] != '='){
                                aux[j] = inputBuffer[i];
                                j++;
                                i++;
                        }
                                if(str[j-1] == aux[j-1]){
                                strcat(query,"on");
                           //     printf("Aux2:  %s\n",aux);
                                return 0;

                        }
                        memset(aux,0,100);
                }
                j=0;
                i++;
        }

}



int main(void)
{
    char *lenstr;
    char inputBuffer[MAXLEN];
    int contentLength;
    int i;
    char x;
    char startDate[80];
    char endDate[80];
    char event[80];
    char query1[50] ="";
    char query2[50] ="";
    char query3[50] ="";
    char query4[50] ="";
    char query5[50] ="";
    char query6[50] ="";
    sqlite3 *db;
  	 	
    int rc = sqlite3_open("logftp.db",&db);
    if(rc != SQLITE_OK){
                fprintf(stderr,"Cant open database%s\n",sqlite3_errmsg(db));
                return 1;
        }

    printf ("Content-type:text/html\n\n");
    printf("<html>");
    printf("<head>");
    printf("<TITLE>Response</TITLE>\n");
    printf("<style> body{background-color: white;max-width: 1200px; margin: 0 auto; display: flex; justify-content: center; flex-direction: column}</style>");
    printf("</head>");

    printf("</body>");
    printf("<h1 style = \"text-align: center\">Resultados de las consultas</h1>");
    lenstr = getenv("CONTENT_LENGTH");
    //lenstr = (char *)getenv("CONTENT_LENGTH");
    if (lenstr!= NULL)
    {
	contentLength = atoi(lenstr);   
    }
    else contentLength = 0;

    if (contentLength > sizeof(inputBuffer)-1)
	contentLength = sizeof(inputBuffer)-1;

    i = 0;

    while (i < contentLength){

	x = fgetc(stdin);
	if (x==EOF) break;
	inputBuffer[i] = x;
	i++;
    }

    inputBuffer[i] = '\0';
    contentLength = i;
	
		//printf("<br>Datos Formulario: %s\n", inputBuffer);
    //printf("<br>Tama&ntildeo: %d\n",contentLength);

    separar(startDate, inputBuffer, '=');
    separar(startDate, inputBuffer, '&');
    separar(endDate, inputBuffer, '=');
    separar(endDate, inputBuffer, '&');
    separar(event, inputBuffer, '=');
    separar(event, inputBuffer, '&');

    pullApart(query1,1, inputBuffer, '=');
    pullApart(query2,2, inputBuffer, '=');
    pullApart(query3,3, inputBuffer, '=');
    pullApart(query4,4, inputBuffer, '=');
    pullApart(query5,5, inputBuffer, '=');
    pullApart(query6,6, inputBuffer, '=');	

    printf("<style> span{font-weight:bold;font-size:20px }</style>");
    printf("<h2>Datos del formulario</h2>");
        printf("<p><span>Fecha de Inicio:</span> %s</p>",startDate);
        printf("<p><span>Fecha Final:</span> %s</p>",endDate);
        printf("<p><span>Evento:</span> %s</p>",event);
    
    printf("<style> table,th, td {border:1px solid black; border-collapse: collapse;} </style>");
	printf("<style> td {padding: 5px; }</style>");

	if(query1[0] == '\0'  && query2[0] == '\0' && query3[0] == '\0'  && query4[0] == '\0' && query5[0] == '\0' && query6[0] =='\0'){
		execQuery(db,0,startDate,endDate,event);
	}
	
    	//Ejecutar consultas selecionadas

	if(strcmp(event,"LOGIN") != 0){
		if(strcmp(event,"FAILLOGIN") != 0){
		if(query1[0] != '\0')execQuery(db,1,startDate,endDate,event);
		if(query2[0] != '\0')execQuery(db,2,startDate,endDate,event);
       		if(query3[0] != '\0')execQuery(db,3,startDate,endDate,event);
		if(query4[0] != '\0')execQuery(db,4,startDate,endDate,event);
		if(query5[0] != '\0')execQuery(db,5,startDate,endDate,event);
		if(query6[0] != '\0')execQuery(db,6,startDate,endDate,event);
		}
	}
	printf("</body>");


/*    printf("<p> Fecha de Inicio: %s",startDate);
    printf("<p> Fecha de fin: %s",endDate);
    printf("<p> Evento: %s",event);
    printf("<p> Consulta1: %s",query1);
    printf("<p> Consulta2: %s",query2);
    printf("<p> Consulta3: %s",query3);
    printf("<p> Consulta4: %s",query4);
    printf("<p> Consulta5: %s",query5);
	printf("<p>Consulta6: %s",query6);
    printf("</html>");
*/
	sqlite3_close(db);

    return 0;
}
