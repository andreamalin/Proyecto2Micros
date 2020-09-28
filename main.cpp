/*
*----------------------------------------
* main.cpp
* Proyecto #2 Grupo #
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

// VARIABLES
// Temperatura
#define DHTPIN 4
float res[2] = { 0, 0 };
// Ultrasonico
#define TRIG 18
#define ECHO 24
// Tamaño lectura/escritura
#define ASIZE 25

	// Arrays sin encriptar
// Arrays de escritura de los sensores
int ultraSonicW[ASIZE];
float humidityW[ASIZE];
float temperatureW[ASIZE]:

// Arrays de lectura del txt
int ultraSonicR[ASIZE];
float humidityR[ASIZE];
float temperatureR[ASIZE]:

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
int readUltraSonic(char* nombreArchivo){
	ifstream archivo;
	int largo, inicio, guardar, pos;
	string linea, numero;
	archivo.open(nombreArchivo, ios::in); // Abriendo el archivo
	
	if(archivo.fail()){ // Verificando si hay un fallo
		return 1;
	}
 
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
			
				if(!(pos >= ASIZE)){				// Verificando que se cumplan los 25 espacios
					ultraSonicR[pos] = guardar;			
				}
				
				pos++;
			}
			
		}
		
		while (!read){		// Si se quiere leer empieza read tiene que ser true
			sem_wait(&save); // Espera a que le digan que lea la proxima linea si hay
		}
	}
	archivo.close();
	return 0;
}

/**
* Lee el archivo de texto y llena el array correspondiente con una linea
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
int readHumidity(char* nombreArchivo){
	ifstream archivo;
	int largo, inicio, pos;
	float guardar;
	string linea, numero;
	archivo.open(nombreArchivo, ios::in); // Abriendo el archivo
	
	if(archivo.fail()){ // Verificando si hay un fallo
		return 1;
	}
 
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
			
				if(!(pos >= ASIZE)){				// Verificando que se cumplan los 25 espacios
					humidityR[pos] = guardar;			
				}
				
				pos++;
			}
			
		}
		
		while (!read){		// Si se quiere leer empieza read tiene que ser true
			sem_wait(&save); // Espera a que le digan que lea la proxima linea si hay
		}
	}
	archivo.close();
	return 0;
}

/**
* Lee el archivo de texto y llena el array correspondiente con una linea
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
int readTemperature(char* nombreArchivo){
	ifstream archivo;
	int largo, inicio, pos;
	float guardar;
	string linea, numero;
	archivo.open(nombreArchivo, ios::in); // Abriendo el archivo
	
	if(archivo.fail()){ // Verificando si hay un fallo
		return 1;
	}
 
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
			
				if(!(pos >= ASIZE)){				// Verificando que se cumplan los 25 espacios
					temperatureW[pos] = guardar;			
				}
				
				pos++;
			}
			
		}
		
		while (!read){		// Si se quiere leer empieza read tiene que ser true
			sem_wait(&save); // Espera a que le digan que lea la proxima linea si hay
		}
	}
	archivo.close();
	return 0;
}

/**
* Escribe en el array del sensor ultra (Escribe 25 datos)
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
int writeUltraSonic(char* nombreArchivo){
	// Con respecto al archivo
	ofstream archivo;
	archivo.open(nombreArchivo, ios::app);
	
	// Verificando que nada fallara
	if(archivo.fail()){
		return 1;			
	}
	
	while(write){
		sem_wait(&save); // Espera a que le digan que guarde los datos
	}
	
	// Seguira escribiendo mientras el semaforo no le diga lo contrario
	for(int i = 0; i < ASIZE; i++){
		archivo << ultraSonicW[i] << ","; 	// Escribe el dato 
	}
	
	// Salto de linea y se cierra el archivo
	archivo <<endl;
	archivo.close();
	return 0;
}

/**
* Escribe en el array de humedad (Escribe 25 datos)
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
int writeHumidity(char* nombreArchivo){
	// Con respecto al archivo
	ofstream archivo;
	archivo.open(nombreArchivo, ios::app);
	
	// Verificando que nada fallara
	if(archivo.fail()){
		return 1;			
	}
	
	while(write){
		sem_wait(&save); // Espera a que le digan que guarde los datos
	}
	
	// Seguira escribiendo mientras el semaforo no le diga lo contrario
	for(int i = 0; i < ASIZE; i++){
		archivo <<  encrypt(humidityW[i]) << ","; 	// Escribe el dato 
	}
	
	// Salto de linea y se cierra el archivo
	archivo <<endl;
	archivo.close();
	return 0;
}

/**
* Escribe en el array de temperatura (Escribe 25 datos)
* nombreArchivo: con .txt al final
* retorna 0 si fue un exito y 1 si algo no salio bien
*/
int writeTemperature(char* nombreArchivo){
	// Con respecto al archivo
	ofstream archivo;
	archivo.open(nombreArchivo, ios::app);
	
	// Verificando que nada fallara
	if(archivo.fail()){
		return 1;			
	}
	
	while(write){
		sem_wait(&save); // Espera a que le digan que guarde los datos
	}
	
	// Seguira escribiendo mientras el semaforo no le diga lo contrario
	for(int i = 0; i < ASIZE; i++){
		archivo << encrypt(temperatureW[i]) << ","; 	// Escribe el dato 
	}
	
	// Salto de linea y se cierra el archivo
	archivo <<endl;
	archivo.close();
	return 0;
}

void setup(){
    //Setup para los pines del ultrasonico
    wiringPiSetupGpio();
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    //TRIG pin must start LOW
    digitalWrite(TRIG, LOW);
    delay(30);
    
    //Setup pines de temperatura
    pinMode( DHTPIN, OUTPUT );
}
//Sensor humedad temperatura
int DHT11(){
    uint8_t laststate	= HIGH;
    uint8_t counter		= 0;
    uint8_t j		= 0, i;
    res[0] = res[1] = 0;
    int dht11_dat[5] = { 0, 0, 0, 0, 0 };

    /* pull pin down for 18 milliseconds */
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    /* then pull it up for 40 microseconds */
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    /* prepare to read the pin */
    pinMode( DHTPIN, INPUT );
    
    /* detect change and read data */
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

	/* ignore first 3 transitions */
	if ( (i >= 4) && (i % 2 == 0) ){
	    /* shove each bit into the storage bytes */
	    dht11_dat[j / 8] <<= 1;
	    if ( counter > 50 )
		dht11_dat[j / 8] |= 1;
		j++;
	}
    }
    /* check we read 40 bits (8bit x 5 ) + verify checksum in the last byte */
    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ){
    	/**
    	¡¡¡Poner que ya se escribe!!!
    	*/
	res[0] = stof(to_string(dht11_dat[0])+"."+ to_string(dht11_dat[1])); //Humedad
	res[1] = stof(to_string(dht11_dat[2])+"."+ to_string(dht11_dat[3])); //Temperatura
	
	return 0; //Se sale correctamente si el dato leido es bueno
    }
    return -1; 
}

int ultrasonic() {
    
    //Send trig pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG, LOW);
 
    //Wait for echo start
    while(digitalRead(ECHO) == LOW);
 
    //Wait for echo end
    long startTime = micros();
    while(digitalRead(ECHO) == HIGH);
    long travelTime = micros() - startTime;
 
    //Get distance in cm
    int distance = travelTime / 58;
 
    return distance;
}


//Programa principal
int main() {
    bool seguir = true;
    write = true;
    read = true;
    
    // SEMAFORO
    sem_init(&guardar,0,1);
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
	    printf("\nSENSOR A USAR\n1. Ultrasonido\n2. Temperatura y Humedad\n");
	    int sensorElegido; //Ingreso de una opcion
	    cout<<"Ingrese el sensor a utilizar: ";
	    cin>>sensorElegido;
	    
	    if (sensorElegido == 1){
		printf("Distance: %d cm\n", ultrasonic());
	    } else if (sensorElegido == 2){
		int error = DHT11();
		while (error == -1){
		    error = DHT11();
		    delay( 1000 ); /* wait 1sec to refresh */
		}
		printf("Humedad = %.1f % Temperatura = %.1f *C \n", res[0], res[1]);
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

