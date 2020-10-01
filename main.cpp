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
#define THSIZE 10
//Nombre de los txt
char nameTemp[] = "temperatura.txt";
char nameHum[] = "humedad.txt";
char name[] = "ultrasonic.txt"; 

// Dato leido actual
int ACTUAL = 0;

	// Arrays sin encriptar
// Arrays de escritura de los sensores
int ultraSonicW[USSIZE];
float humidityW[THSIZE];
float temperatureW[THSIZE];

// Semaforos
sem_t save;

// Barreras
bool write;	// Cuando sea false empieza a escribir

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
		
		// Seguira escribiendo mientras el semaforo no le diga lo contrario
		sem_wait(&save);
		string fragment = to_string(ultraSonicW[ACTUAL-1]);
		archivo <<  encrypt(fragment) << ","; 	// Escribe el dato 	
		sem_post(&save);
		if (ACTUAL == USSIZE){
			// Salto de linea y se cierra el archivo
			archivo <<endl;
			archivo.close();
		}
			
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
		
		// Seguira escribiendo mientras el semaforo no le diga lo contrario
		sem_wait(&save);
		string fragment = to_string(humidityW[ACTUAL-1]);
		archivo <<  encrypt(fragment) << ","; 	// Escribe el dato 	
		sem_post(&save);
		if (ACTUAL == THSIZE){
			// Salto de linea y se cierra el archivo
			archivo <<endl;
			archivo.close();
		}
			
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
		
		// Seguira escribiendo mientras el semaforo no le diga lo contrario
		sem_wait(&save);
		string fragment = to_string(temperatureW[ACTUAL-1]);
		archivo <<  encrypt(fragment) << ","; 	// Escribe el dato 	
		sem_post(&save);
		if (ACTUAL == THSIZE){
			// Salto de linea y se cierra el archivo
			archivo <<endl;
			archivo.close();
		}
			
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
//Borrar los archivos txt anteriores para 
//caerle encima a los datos para vez que corre el programa
void removeTxt(){
	try{ 
		remove("temperatura.txt");
		remove("humedad.txt");
	} catch (...) {

	} 
	try {
		remove("ultrasonic.txt");
	} catch (...) {

	}

}


//Programa principal
int main() {
	removeTxt(); //Borrando txt pasados
    bool seguir = true;
    // SEMAFORO
    sem_init(&save,0,1);
    
    //Inicializando wiringPi
    setup();

    while(seguir){
	printf("\nBIENVENIDO\n1. Leer Datos\n2. Generar Llave\n3. Salir\n");
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
	    	pthread_t writeUltra; //Hilos a usar
	    	for (int i = 0; i < USSIZE; ++i)
	    	{
	    		printf("\nEl valor de distancia %d es: %dcm" ,ACTUAL,ultrasonic());
	    		pthread_create(&writeUltra, NULL, writeUltraSonic, (void *)&name);
				delay( 1000 ); /* wait 1sec to refresh */
				
	    	}	
				
	    } else if (sensorElegido == 2){
	    	pthread_t writeTemp, writeHum; //Hilos a usar
	    	for (int i = 0; i < THSIZE; ++i)
	    	{
	    		int error = DHT11();
				while (error == -1){
				    error = DHT11();
				    delay( 1000 ); /* wait 1sec to refresh */
				}
				pthread_create(&writeTemp, NULL, writeTemperature, (void *)&nameTemp);
				pthread_create(&writeHum, NULL, writeHumidity, (void *)&nameHum);
				printf("Humedad = %.1f % Temperatura = %.1f *C \n", res[0], res[1]);
				
	    	}			
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
