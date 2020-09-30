/*
*----------------------------------------
* main.cpp
* Proyecto #2 Grupo #7
*Integrantes: Andrea Amaya, Martin Amado, Brandon Hernandez
*			Oliver de Leon, Laura Tamath
* ---------------------------------------
* Sensores: 
* Ultrasonico
* Humedad y temperatura
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream> 
#include <fstream> // Leer y escribir
#include <wiringPi.h> //Libreria a usar para los sensores
#include <semaphore.h>
// VARIABLES
// Temperatura
#define DHTPIN 4
float res[2] = { 0, 0 };
// Ultrasonico
#define TRIG 18
#define ECHO 24
// Tamaño lectura/escritura
#define USSIZE 75
#define THSIZE 40

// Dato leido actual
int ACTUAL = 0;

	// Arrays sin encriptar
// Arrays de escritura de los sensores
int ultraSonicW[USSIZE];
float humidityW[THSIZE];
float temperatureW[THSIZE];
// Arrays de lectura del txt
int ultraSonicR[USSIZE];
float humidityR[THSIZE];
float temperatureR[THSIZE];

// Semaforos
sem_t save, load;

// Barreras
bool write, read;	// Cuando sea false empieza a escribir

using namespace std;

/**
* Recoge un fragmento de texto y lo encripta
*/
string encrypt(string text) 
{ 
	string result = ""; 
	for (int i=0;i<text.length();i++) 
	{ 
		result += char(int(text[i]+18-97)%26 +97); 
	} 
	return result; 
} 


/**
* Lee el archivo de texto y llena el array correspondiente con una linea
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
void *readUltraSonic(void *argument){
	ifstream archivo;
	int largo, inicio, guardar, pos;
	string linea, numero;
	
	char *fileName;
	fileName = (char *)argument;
	archivo.open(fileName, ios::in); // Abriendo el archivo
	
	if(!archivo.fail()){ // Verificando si hay un fallo
		while(!archivo.eof()){
			getline(archivo, linea);	// Se obtiene la línea
	
			// Se ponen todas las variables para poder entrar a la linea
			guardar = 0;
			pos = 0;
			inicio = 0;
			for(int i = 0; i < linea.length(); i++){	// Para cada una de las letras
				if(linea[i] == ','){
					int fin = i - inicio;
					
					numero = linea.substr(inicio, fin);	// Obteniendo el numero en string
					stringstream convertido(numero);	// Pasandolo a otro formato :v
					convertido >> guardar;				
					inicio = i + 1;	
				
					if(!(pos >= USSIZE)){				// Verificando que se cumplan los 25 espacios
						ultraSonicR[pos] = guardar;			
					}
					
					pos++;
				}
				
			}
		
			while (!read){		// Si se quiere leer empieza read tiene que ser true
				sem_wait(&load); // Espera a que le digan que lea la proxima linea si hay
			}
			sem_post(&load);		// lo desbloquea
		}
		archivo.close();
	}
 
	pthread_exit(NULL);
}

/**
* Lee el archivo de texto y llena el array correspondiente con una linea
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
void *readHumidity(void *argument){
	ifstream archivo;
	int largo, inicio, pos;
	float guardar;
	string linea, numero;
	
	char *fileName;
	fileName = (char *)argument;
	archivo.open(fileName, ios::in); // Abriendo el archivo
	
	if(!archivo.fail()){ // Verificando si hay un fallo
		while(!archivo.eof()){
			getline(archivo, linea);	// Se obtiene la línea
	
			// Se ponen todas las variables para poder entrar a la linea
			guardar = 0;
			pos = 0;
			inicio = 0;
			for(int i = 0; i < linea.length(); i++){	// Para cada una de las letras
				if(linea[i] == ','){
					int fin = i - inicio;
					
					numero = linea.substr(inicio, fin);	// Obteniendo el numero en string
					stringstream convertido(numero);	// Pasandolo a otro formato :v
					convertido >> guardar;				
					inicio = i + 1;	
				
					if(!(pos >= THSIZE)){				// Verificando que se cumplan los 25 espacios
						humidityR[pos] = guardar;			
					}
					
					pos++;
				}
				
			}
			
			while (!read){		// Si se quiere leer empieza read tiene que ser true
				sem_wait(&load); // Espera a que le digan que lea la proxima linea si hay
			}
			sem_post(&load);		// lo desbloquea
		}
		archivo.close();
	}
 
 	pthread_exit(NULL);
}

/**
* Lee el archivo de texto y llena el array correspondiente con una linea
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
void *readTemperature(void *argument){
	ifstream archivo;
	int largo, inicio, pos;
	float guardar;
	string linea, numero;
	
	char *fileName;
	fileName = (char *)argument;
	archivo.open(fileName, ios::in); // Abriendo el archivo
	
	if(!archivo.fail()){ // Verificando si hay un fallo
		while(!archivo.eof()){
			getline(archivo, linea);	// Se obtiene la línea
	
			// Se ponen todas las variables para poder entrar a la linea
			guardar = 0;
			pos = 0;
			inicio = 0;
			for(int i = 0; i < linea.length(); i++){	// Para cada una de las letras
				if(linea[i] == ','){
					int fin = i - inicio;
					
					numero = linea.substr(inicio, fin);	// Obteniendo el numero en string
					stringstream convertido(numero);	// Pasandolo a otro formato :v
					convertido >> guardar;				
					inicio = i + 1;	
				
					if(!(pos >= THSIZE)){				// Verificando que se cumplan los 25 espacios
						temperatureW[pos] = guardar;			
					}
					
					pos++;
				}
				
			}
			
			while (!read){		// Si se quiere leer empieza read tiene que ser true
				sem_wait(&load); // Espera a que le digan que lea la proxima linea si hay
			}
			sem_post(&load);		// lo desbloquea
		}
		archivo.close();
	}
 
 	pthread_exit(NULL);
}

/**
* Escribe en el array del sensor ultra (Escribe 25 datos)
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
void *writeUltraSonic(void *argument){
	// Con respecto al archivo
	ofstream archivo;
	
	char *fileName;
	fileName = (char *)argument;
	archivo.open(fileName, ios::app);
	
	// Verificando que nada fallara
	if(!archivo.fail()){
		while(write){
			sem_wait(&save); // Espera a que le digan que guarde los datos
		}
		sem_post(&save);	// lo desbloquea
		
		// Seguira escribiendo mientras el semaforo no le diga lo contrario
		for(int i = 0; i < USSIZE; i++){
			string fragment = to_string(ultraSonicW[i]);
			archivo <<  encrypt(fragment) << ","; 	// Escribe el dato 
		}
		
		// Salto de linea y se cierra el archivo
		archivo <<endl;
		archivo.close();		
	}
	
	pthread_exit(NULL);
}

/**
* Escribe en el array de humedad (Escribe 25 datos)
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
void *writeHumidity(void *argument){
	// Con respecto al archivo
	ofstream archivo;
	char *fileName;
	fileName = (char *)argument;
	archivo.open(fileName, ios::app);
	
	// Verificando que nada fallara
	if(!archivo.fail()){
		while(write){
			sem_wait(&save); // Espera a que le digan que guarde los datos
		}
		sem_post(&save);	// lo desbloquea
		
		// Seguira escribiendo mientras el semaforo no le diga lo contrario
		for(int i = 0; i < THSIZE; i++){
			string fragment = to_string(humidityW[i]);
			archivo <<  encrypt(fragment) << ","; 	// Escribe el dato 
		}
	
		// Salto de linea y se cierra el archivo
		archivo <<endl;
		archivo.close();		
	}
	
	pthread_exit(NULL);
}

/**
* Escribe en el array de temperatura (Escribe 25 datos)
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
void *writeTemperature(void *argument){
	// Con respecto al archivo
	ofstream archivo;
	
	char *fileName;
	fileName = (char *)argument;
	archivo.open(fileName, ios::app);
	
	// Verificando que nada fallara
	if(!archivo.fail()){
		while(write){
			sem_wait(&save); // Espera a que le digan que guarde los datos
		}
		sem_post(&save);	// lo desbloquea
		
		// Seguira escribiendo mientras el semaforo no le diga lo contrario
		for(int i = 0; i < THSIZE; i++){
			string fragment = to_string(temperatureW[i]);
			archivo <<  encrypt(fragment) << ","; 	// Escribe el dato 
		}
		
		// Salto de linea y se cierra el archivo
		archivo <<endl;
		archivo.close();			
	}
	
	pthread_exit(NULL);
}

void setup(){
    //Setup para los pines del ultrasonico
    wiringPiSetupGpio();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
}

//Sensor humedad temperatura
int DHT11(){
    uint8_t laststate	= HIGH; 
    uint8_t counter		= 0;
    uint8_t j		= 0, i;
    res[0] = res[1] = 0; //Array para guardar los datos
    int dht11_dat[5] = { 0, 0, 0, 0, 0 }; //Array para tomar los decimales

    /* Se mantiene el pin bajo por 18 ms */
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    /* Se  deja arriba por 40 ms */
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    /* Se comienza a tomar el dato*/
    pinMode( DHTPIN, INPUT );
    
    /* Se verifica si hay cambio en los datos */
    for ( i = 0; i < 85; i++ ){
	counter = 0;
	while ( digitalRead( DHTPIN ) == laststate ){
	    counter++;
	    delayMicroseconds( 1 );
	    if ( counter == 255 ){
		break;
	    }
	}
	laststate = digitalRead( DHTPIN );

	if ( counter == 255 )
	    break;

	/* Se ignoran las primeras 3 leidas, pues son las que mayor error tienen */
	if ( (i >= 4) && (i % 2 == 0) ){
	    /* Se revisa si los decimales son correctos */
	    dht11_dat[j / 8] <<= 1;
	    if ( counter > 50 )
		dht11_dat[j / 8] |= 1;
		j++;
	}
    }
    /*Se revisa si se leyeron bien los decimales*/
    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ){
		res[0] = stof(to_string(dht11_dat[0])+"."+ to_string(dht11_dat[1])); //Humedad
		res[1] = stof(to_string(dht11_dat[2])+"."+ to_string(dht11_dat[3])); //Temperatura
		//Se meten los datos a los array de temperatura y humedad
		temperatureW[ACTUAL] = res[1];
		humidityW[ACTUAL] = res[0];
		++ACTUAL; //Sumo un dato leido

		return 0; //Se sale correctamente si el dato leido es bueno
    }
    return -1; 
}
//Sensor ultrasonico
int ultrasonic() {
    //Se manda un pulso al output
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);
 
    //Se espera a que el input comience
    while(digitalRead(ECHO) == LOW);
 
    //Se espera a que el input termine
    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;
 
    //Se obtiene la distancia en cm
    int distance = travelTime / 58;
    //Se guarda el dato al array de ultrasonico
	ultraSonicW[ACTUAL] = distance;
	ACTUAL ++;

    return distance; //Se vuelve la distancia
}


//Programa principal
int main() {
    bool seguir = true;
    write = true;
    read = true;
    
    // SEMAFORO
    sem_init(&save,0,1);
    sem_init(&load,0,1);
    
    //Inicializando wiringPi
    setup();

    while(seguir){
	printf("\nBIENVENIDO\n1. Leer Datos\n2. Mostrar Datos\n3. Generar Llave\n4. Salir\n");
	int opcion; //Ingreso de una opcion
	cout<<"Ingrese la opcion a realizar: ";
	cin>>opcion;
	
	//Se revisa cada opcion
	if (opcion == 1){
	    printf("\nSENSOR A USAR\n1. Ultrasonico\n2. Temperatura y Humedad\n");
	    int sensorElegido; //Ingreso de una opcion
	    cout<<"Ingrese el sensor a utilizar: ";
	    cin>>sensorElegido;
	    
	    
	    if (sensorElegido == 1){
	    	while(ACTUAL < USSIZE){
		    	printf("\nEl valor de distancia %d es: %dcm" ,ACTUAL,ultrasonic());
				delay( 1000 ); /* wait 1sec to refresh */
	    	}
		    //Se guardan los datos haciendo uso de los semafotos
		    pthread_t write; //Hilo a usar
		    char name[] = "ultrasonic.txt"; //Nombre del txt
		    sem_wait(&save); //Espera a que guarde
			pthread_create(&write, NULL, writeUltra, (void *)&name);
			write = false;
			sem_post(&save); //Desbloquea
				
	    } else if (sensorElegido == 2){
	    	while(ACTUAL < THSIZE){
		    	int error = DHT11();
				while (error == -1){
				    error = DHT11();
				    delay( 1000 ); /* wait 1sec to refresh */
				}
				printf("Humedad = %.1f % Temperatura = %.1f *C \n", res[0], res[1]);
	    	}
			//Se guardan los datos al mismo tiempo haciendo uso de los semaforos
			pthread_t writeTemp, writeHum; //Hilos a usar
			//Nombre de los txt
			char nameTemp[] = "temperatura.txt";
			char nameHum[] = "humedad.txt";
			sem_wait(&save); //Espera a que guarde
			    
			pthread_create(&writeTemp, NULL, writeTemperature, (void *)&nameTemp);
			pthread_create(&writeHum, NULL, writeHumidity, (void *)&nameHum);
				
			write = false;
			sem_post(&save); //Desbloquea

	    }	    
	    
	    	
	} else {
	    seguir = false;
	}
	printf("\n\n");
    }
    //Se sale con exito
    printf("\n --- Fin --- \n");
    return 0;
}

