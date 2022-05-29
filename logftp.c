#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sqlite3.h>

struct Evento{

        char  day[10];
        char  month[10];
        char  dayNum[10];
        //char hour[10];
        char year[10];
        char process[10];
        char host[10];
        char evenType[15];
        char client[40];
        char filePath[100];
        char file[80];
        char size[20];
        char  time[20];
};
void showEvents(struct Evento *event,int i,char date[]){
        printf("%s\n",event->day);
        printf("%s\n",event->month);
        printf("%s\n",event->dayNum);
        printf("%s\n",event->year);
        printf("%s\n",event->process);
        printf("%s\n",event->host);
        printf("%s\n",event->evenType);
        printf("%s\n",event->client);
       if(i == 2 || i == 3){
        printf("%s\n",event->filePath);
        printf("%s\n",event->file);
        if(i == 2){
        printf("%s\n",event->size);
        printf("%s\n",event->time);
        }
       }

       printf("Date:%s\n",date);
       printf("\n");
}


 bool putValueProcess(struct Evento *event,char str []){
     int i = 0;
     if(str[0] != '['){
         while (str[i] != ']')
         {
             event->process[i] = str[i];
            i++;
         }
         event->process[i] = '\0';
        return true;
     }
     return false;
 } 
 bool putValueHost(struct Evento *event,char str []){
     int i = 0;
     int j = 0;
     if(str[0] == '['){
         i++;
         while (str[i] != ']')
         {
             event->host[j] = str[i];
            i++;
            j++;
         }
         event->host[j] = '\0';
        return true;
     }
     return false;
 }
 bool verifyFail(char str []){
     if(str[0] == 'O'){
         return false;
     }
     return true;
 } 
void putValueEvent(struct Evento *event,char str [],bool fail){
    memset(event->evenType,0,15);
    char aux[15];
    int i = 0;
    while (str[i] != ':')
         {
             aux[i] = str[i];
            i++;
         }
    aux[i] = '\0';

    if(fail){
        strcat(event->evenType,"FAIL");
    }
    strcat(event->evenType,aux);
 }

void putValueClient(struct Evento *event, char str[])
{
    int i = 1;
    int j = 0;
    while (str[i] != '"')
    {
        event->client[j] = str[i];
        i++; 
	j++; 
    }
    event->client[j] = '\0';
}

void putFile(char file[] , char path []){
        int len = strlen(path);
        int i = len;

        while(path[i-1] != '/' ){
                i--;
        }
        int j = 0;
        while(i < len-2){
                file[j] = path[i];
                j++;
                i++;
        }

}
void putValuePath(struct Evento *e,char str[]){
        int i = 1;
        int j = 0;
        char file2[80]="";
        memset(e->file,0,80);
        memset(e->filePath,0,100);

        //strcat(e->filePath,str);

        while(str[i] != '"'){
                e->filePath[j] = str[i];
                i++;
                j++;
        }
        e->filePath[j] = '\0';
        
        putFile(file2,str);
        strcat(e->file,file2);
        //printf("FilePath:%s\n",e->filePath);
        //printf("File: %s\n",e->file);
}

void putValueTime(struct Evento *event,char str []){
    int i = 0;
    while(str[i] != 'K'){
        event->time [i] = str[i];
        i++;
    }
    event->time[i] = '\0';
 } 
int getMonth(char month[]){
    int nMonth = 0;

        if(strcmp(month,"Dec") == 0)nMonth = 12;
	if(strcmp(month,"Jan") == 0)nMonth = 1;
        if(strcmp(month,"Jan") == 0)nMonth = 1;
	if(strcmp(month,"Feb") == 0)nMonth = 2;
     	if(strcmp(month,"Mar") == 0)nMonth = 3;
        if(strcmp(month,"Apr") == 0)nMonth = 4;
        if(strcmp(month,"Jun") == 0)nMonth = 6;
       	if(strcmp(month,"Jul") == 0)nMonth = 7;
      	if(strcmp(month,"Aug") == 0)nMonth = 8;
  	if(strcmp(month,"Sep") == 0)nMonth = 9;
        if(strcmp(month,"Oct") == 0)nMonth = 10;
        if(strcmp(month,"Nov") == 0)nMonth = 11;
        if(strcmp(month,"May") == 0)nMonth = 5;
        
	return nMonth;
}

void putDateEvent(char date[],struct Evento *e){
	strcat(date,e->year);
        strcat(date,"-");
        int mes = getMonth(e->month);
        char * num;
        asprintf(&num, "%d", mes);
        if(mes < 10){
                strcat(date,"0");
        }
        strcat(date,num);
        strcat(date,"-");
        strcat(date,e->dayNum);
}

int  gestionaError(sqlite3 *db){
	fprintf(stderr,"Error:%s\n",sqlite3_errmsg(db));
	return sqlite3_errcode(db);
}
void insertDataL(struct Evento *e,sqlite3 *db, char date [], int num){
        char sql[250];
        int rc;

//	printf("AL ingresar a la base de datos:\n\n");


/*	printf("dateEvent:%s\n",date);
	printf("host:%s\n",e->host);
	printf("EvenType:%s\n",e->evenType);
	printf("client:%s\n",e->client);
  */
  	char * err_msg;
                sprintf(sql,"INSERT INTO event(date,nProcess,host,evenType,customer) values('%s','%s','%s','%s','%s');",date,e->process,e->host,e->evenType,e->client);
        

	 rc = sqlite3_exec(db,sql,NULL,NULL,NULL);

        if(rc != SQLITE_OK){
    	  gestionaError(db);
		//fprintf(stderr, "SQL Error %s%d\n",err_msg,num);
                //sqlite3_free(err_msg);
                //sqlite3_close(db);
        }
	
}


void insertData(struct Evento *e,sqlite3 *db, char dateEvent [], int num){
	char sql[250];
	int rc;
	char * err_msg = 0;
	
	if(num == 1){
		sprintf(sql,"INSERT INTO event(date,nProcess,host,evenType,customer) values('%s','%s','%s','%s','%s');",dateEvent,e->process,e->host,e->evenType,e->client);
		
	}
	if(num == 3){
		sprintf(sql,"INSERT INTO event(date,nProcess,host,evenType,customer,pathFile,file) values('%s','%s','%s','%s','%s','%s','%s')",dateEvent,e->process, e->host,e->evenType,e->client,e->filePath,e->file);
	}
	if(num == 2){
		sprintf(sql,"INSERT INTO event(date,nProcess,host,evenType,customer,pathFile,file,size,time) values('%s','%s','%s','%s','%s','%s','%s','%s','%s')",dateEvent,e->process, e->host,e->evenType,e->client,e->filePath,e->file,e->size, e->time);
	}
	 
	rc = sqlite3_exec(db,sql,0,0,&err_msg);
	
	if(rc != SQLITE_OK){
		fprintf(stderr, "SQL Error %s%d\n",err_msg,num);
	//	sqlite3_free(err_msg);
	//	sqlite3_close(db);
	}

}

int main(){

    
printf("Content-type:text/html\n\n");
printf(" <html>");
printf(" <head>");
printf("    <title>FtpLog</title>");
printf("    <style>body{font-size: 150 px;font-family: Krub, sans-serif;}</style>");
printf("    <style>html{background: linear-gradient(#EAEDED, #BFC9CA);background-repeat: no-repeat;height: 100%;position: relative;font-size: 150 px;font-size: 100%;box-sizing: border-box;}</style> ");

printf(" <style>FORM{margin: auto;padding: 10px 20px;background-color: #757575;display: flex;flex-direction:column;justify-content:center;");
printf("align-items: center;border-radius: 4px;color: white;box-shadow: 4px 4px 10px #000;");
printf("margin-bottom: 10px;position: absolute;top: 50%;left: 50%;transform: translate(-50%,-50%);}</style>");
printf("    <style>h1{position: absolute;top: 20%;left: 50%;transform: translate(-50%,-80%);}</style>");
printf("    <style>option{font-size: 10px;}</style>");
printf("    <style>select{font-size: 12px;padding: 2px;}</style>");
printf("</head>");
printf("<body >");
printf("<div>");
printf("    <h1>Analizador log FTP</h1>");
printf("    <FORM ACTION=\"/cgi-bin/leerformlog.cgi\" METHOD=\"POST\">");
printf("        <div style=\"display: flex; justify-content:center; align-items:center; gap:20px\">");
printf("            <div style=\"text-align:center\">");
printf("                <p> Fecha Inicial</p>");
printf("                <input name=\"fechaInicio\" type = \"date\"><BR>");
printf("            </div>");
printf("            <div style=\"text-align:center\">    ");
printf("                <p>Fecha Final</p>");
printf("                <input name=\"fechaFinal\" type = \"date\">");
printf("            </div>");
printf("            <div style=\"text-align:center\">");
printf("                <p>Evento</p>");
printf("                <select name=evento>");
printf("                    <option value=\"DOWNLOAD\">DOWNLOAD </option>");
printf("                    <option value=\"UPLOAD\">UPLOAD </option>");
printf("                    <option value=\"FAILLOGIN\">FAILLOGIN </option>");
printf("                    <option value=\"ALL\">TODOS</option>");
printf("                    <option value=\"FAILDOWNLOAD\">FAILDOWNLOAD </option>");
printf("                    <option value=\"FAILUPLOAD\">FAILUPLOAD </option>");
printf("                    <option value=\"LOGIN\">LOGIN</option>");
printf("                </select><br>");
printf("            </div>");
printf("        </div>");
printf("        <div  >");
printf("            <p>Selecione las consultas que desee(Opcional)</p>");
printf("            <div style=\"margin-bottom:15px\">");
printf("                <input name =\"query1\" type =\"checkbox\">Cantidad de archivos</input><br>");
printf("                <input name =\"query2\" type =\"checkbox\">Archivos mayores a 10MB</input><br>");
printf("                <input name =\"query3\" type =\"checkbox\">Velocidad promedio de transferencia</input><br>");
printf("                <input name =\"query4\" type =\"checkbox\">Listar clientes</input><br>");
printf("                <input name =\"query6\" type =\"checkbox\">Frecuencia de archivos</input><br>");
printf("                <input name =\"query5\" type =\"checkbox\">Listar hosts</input><br>");
printf("            </div>");
printf("        </div>");
printf("     <INPUT  TYPE=\"SUBMIT\" VALUE=\"Send\"  style=\"margin-bottom:10px;border-radius:4px;text-decoration:none;border:none;font-si");printf("ze:16px;padding: 5px 12px; \">");
printf("    </FORM>");
printf("</div>");
printf("</body>");
printf("</html>");




	struct Evento event;
    FILE * file = fopen("/var/log/vsftpd.log","r");
    //FILE * file = fopen("e3.txt","r");
    char character;
    char *err_msg = 0;
    if(file == NULL){
        perror("Error al abrir el archivo");
    }
    sqlite3 *db;
    int rc  = sqlite3_open("logftp.db",&db);
    if(rc != SQLITE_OK){
    	fprintf(stderr,"Cant open database%s\n",sqlite3_errmsg(db));
                return 1;
    }

    char * sql = "DROP TABLE IF EXISTS event;"
                      "CREATE TABLE event(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,date TEXT,nProcess TEXT, host TEXT,evenType TEXT,customer TEXT,pathFile TEXT,file TEXT, size INTEGER, time TEXT);";

    rc = sqlite3_exec(db,sql,0,0,&err_msg);

    if(rc != SQLITE_OK){
    	fprintf(stderr,"SQL error%s\n",err_msg);
	sqlite3_free(err_msg);
	sqlite3_close(db);
	return 1;
    }


    char cad[2];
    char str[100]="";
    int col = 0;
    bool fail = false;
    int x = 0;
    bool valid = true;
    bool login = false;
    char dateEvent[15];    
    
   bool valid2 = false; 
    
    while(feof(file) == 0){       // 0  1   2    3      4      5   6      7      8    9       10        11
        character = fgetc(file);             //Thu May 19 12:03:32 2022 [pid 102421] [elmer] OK UPLOAD: Client "::ffff:192.168.0.50", "/home/elmer/log.db", 20480 bytes, 6727.21
        if(character == '\n'){  //465k 
            printf("Entro\n");
            printf("Columna:%d\n",col);
	    memset(dateEvent,0,15);
            putDateEvent(dateEvent,&event);
            
	    if(login){
                putValueClient(&event,str);
                showEvents(&event,1,dateEvent);
                col = 0;
                login = false;
    		insertData(&event,db,dateEvent,1);	
    	    }if(col == 13){
           //     showEvents(&event,3);
		insertData(&event,db,dateEvent,3);

            }
            if(col == 15){
                putValueTime(&event,str);
         //       showEvents(&event,2);
		insertData(&event,db,dateEvent,2);
               
            }
            
            col = 0;
            valid = true;
            memset(str,0,100);
        }else{
            if(character != ' ' ){
                cad[0] = character;
                cad[1] = '\0';      //t
                strcat(str,cad);
            
            }else if(valid){
                switch (col){
                case 0: memset(event.day,0,10);strcat(event.day,str) ;break;
                case 1: memset(event.month,0,10);strcat(event.month,str) ;break;
                case 2: memset(event.dayNum,0,10);strcat(event.dayNum,str) ;break;
                case 4: memset(event.year,0,10);strcat(event.year,str) ;break;
                case 6: valid = putValueProcess(&event,str) ;break;
                case 7: valid = putValueHost(&event,str) ;break;
                case 8: fail = verifyFail(str) ;break; //fail/ok
                case 9: putValueEvent(&event,str,fail);
                        if(str[0] == 'L')login = true ;break;
                case 11: putValueClient(&event,str);break;
                case 12: putValuePath(&event,str);break;
                case 13: memset(event.size,0,20);strcat(event.size,str);break;
            
                }
                col++;
		//printf("Str: %s\n",str);
		memset(str,0,100);
            }
        
    }   }
   
}


