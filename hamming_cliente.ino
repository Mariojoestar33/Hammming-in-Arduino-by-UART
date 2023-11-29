//Codigo cliente

const int H_T[7][3] = {
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1},
  {1, 1, 0},
  {0, 1, 1},
  {0, 0, 1},
  {1, 1, 1}
};

const int error [8][7] = {
  {1, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 1}
};

/*const int admitidos[16][8] = {
  {1, 1, 0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 0, 0, 0, 1},
  {0, 0, 1, 1, 0, 0, 1, 0},
  {1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 1, 1, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 1},
  {0, 0, 0, 1, 0, 1, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 1},
  {1, 0, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 1},
  {0, 1, 0, 1, 1, 0, 1, 0},
  {1, 1, 1, 1, 1, 0, 1, 1},
  {1, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 1},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 0, 1, 1, 1, 1, 1}
};*/

const int admitidos[16][7] = {
  {0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 1},
  {0, 0, 1, 0, 0, 1, 0},
  {1, 1, 0, 0, 0, 1, 1},
  {0, 1, 1, 0, 1, 0, 0},
  {1, 0, 0, 0, 1, 0, 1},
  {0, 1, 0, 0, 1, 1, 0},
  {1, 0, 1, 0, 1, 1, 1},
  {1, 1, 0, 1, 0, 0, 0},
  {0, 0, 1, 1, 0, 0, 1},
  {1, 1, 0, 1, 0, 1, 0},
  {0, 0, 0, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 0, 0},
  {0, 1, 0, 1, 1, 0, 1},
  {1, 0, 0, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1}
};

int mensajes = 0;
int errores = 0;
int erroresCorregidos = 0;
int erroresNOCorregidos = 0;

float bera = 0;
float berd = 0;

bool sindrome[3];

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() >= 7) {
    bool receivedBool[7];

    // Lee los 8 caracteres en el arreglo
    for (int i = 0; i < 7; i++) {
      char receivedChar = Serial.read();
      receivedBool[i] = (receivedChar == '1') ? true : false;
    }

    // Imprime el arreglo recibido
    Serial.println();
    Serial.print("Codigo recibido: ");
    for (int i = 0; i < 7; i++) {
      Serial.print(receivedBool[i]);
      //Serial.print(" ");
    }
    
    Serial.println();
    
    //Creacion de la nueva matriz
    int nuevaHT [8][3];
    for (int i = 0; i < 8; i++) {
      if (receivedBool[i] == true) {
        for(int j = 0; j < 3; j++) {
        	nuevaHT[i][j] = H_T[i][j];
        }
      } else {
        for(int j = 0; j < 3; j++) {
        	nuevaHT[i][j] = 0;
        }
      }
    }
    
    //Impresion de la matriz
    /*for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 3; j++) {
      	Serial.print(nuevaHT[i][j]);
        Serial.print(" ");
      }
      Serial.println();
    }*/
    
    for (int fila = 0; fila < 7 - 1; fila++) {
    // Realiza el XOR de la fila actual con la siguiente
      for (int columna = 0; columna < 3; columna++) {
        nuevaHT[fila + 1][columna] ^= nuevaHT[fila][columna];
        // Almacena el resultado de la última operación XOR en el vector "codigo"
        if (fila == 7 - 2) {
          sindrome[columna] = static_cast<bool>(nuevaHT[fila + 1][columna]);
        }
      }
  	}
    
    int erroract = 0;
    Serial.println("Sindrome");
	//Impresion del sindrome
    int bandera = 0;
    for (int i = 0; i < 3; i++) {
    	Serial.print(sindrome[i]);
      	Serial.print(" ");
        if(sindrome[i] != 0) {
        	bandera++;
        } if(i == 2 && bandera != 0) {
          	Serial.println(" ERROR DETECTADO");
        	  errores++;
            //bera = ((errores * 8)/(mensajes * 8));
          	erroract = 1;
            //Serial.println();
        }
    }

    mensajes++;
    
    
    //Correcion de error
    if(erroract != 0) {
      int bandera2 = 0;
      int opcion = -1;
      if (erroract != 0) {
        for (int i = 0; i < 7; i++) {
          bandera2 = 0;
          for (int j = 0; j < 3; j++) {
            if (H_T[i][j] == sindrome[j]) {
              bandera2++;
            }//Fin if
            if(bandera2 == 3) {
              opcion = i;
            }//Fin if2
          }//Fin for columnas
        }//Fin for filas
        if(opcion != -1) { //Se encontro el indice
        /*Serial.println();
        Serial.print("Opcion = ");
        Serial.print(opcion);
        Serial.println(); AQUI*/
        bool corregido[7];
        for (int i = 0; i < 7; i++) {
          corregido[i] = receivedBool[i] ^ error[opcion][i];
        }

        Serial.print("CORRECCION ESTIMADA: ");
        Serial.println();
        
        for (int i = 0; i < 7; i++) {
          Serial.print(corregido[i]);
        }
        Serial.println();


        //Validacion

        int b = 0;
        int o = -1;
        for(int i = 0; i < 16; i++) {
          b = 0;
          for(int j = 0; j < 7; j++) {
            if(corregido[j] == admitidos[i][j]) {
              b++;
            }
            if(b == 7) {
              o = i;
            }
          }
        }

        if(o != -1) {
          Serial.println();
          if(o == 0) Serial.print("El mensaje es 16");
          if(o == 1) Serial.print("El mensaje es 17");
          if(o == 2) Serial.print("El mensaje es 18");
          if(o == 3) Serial.print("El mensaje es 19");
          if(o == 4) Serial.print("El mensaje es 20");
          if(o == 5) Serial.print("El mensaje es 21");
          if(o == 6) Serial.print("El mensaje es 22");
          if(o == 7) Serial.print("El mensaje es 23");
          if(o == 8) Serial.print("El mensaje es 24");
          if(o == 9) Serial.print("El mensaje es 25");
          if(o == 10) Serial.print("El mensaje es 26");
          if(o == 11) Serial.print("El mensaje es 27");
          if(o == 12) Serial.print("El mensaje es 28");
          if(o == 13) Serial.print("El mensaje es 29");
          if(o == 14) Serial.print("El mensaje es 30");
          if(o == 15) Serial.print("El mensaje es 31");
          erroresCorregidos++;
          Serial.println();
            Serial.print("Errores detectados: ");
            Serial.println(errores);
            Serial.print("Errores Corregidos = ");
            Serial.print(erroresCorregidos);
            Serial.println();
            Serial.print("Errores NO Corregidos = ");
            Serial.print(erroresNOCorregidos);
            Serial.println();
            Serial.print("Mensajes recibidos: ");
            Serial.print(mensajes);
            Serial.println();
        } else {
          Serial.print("No se ha podido corregir...");
          erroresNOCorregidos++;
          //berd = ((erroresNOCorregidos * 8)/(mensajes * 8));
          Serial.println();
            Serial.print("Errores detectados: ");
            Serial.println(errores);
            Serial.print("Errores Corregidos = ");
            Serial.print(erroresCorregidos);
            Serial.println();
            Serial.print("Errores NO Corregidos = ");
            Serial.print(erroresNOCorregidos);
            Serial.println();
            Serial.print("Mensajes recibidos: ");
            Serial.print(mensajes);
            Serial.println();
        }

        //

          /*for (int i = 0; i < 8; ++i) {
            corregido[i] = receivedBool[i] ^ error[opcion][i];
            for (int j = 0; j < 8; j++) {
                Serial.print(corregido[j]);
                Serial.print(" ");
          }
        Serial.println();
          } */

          //Verificacion de la correccion

          //Creacion de la nueva matriz
          /*int nuevaHT2 [8][3];
          for (int i = 0; i < 8; i++) {
            if (corregido[i] == true) {
              for(int j = 0; j < 3; j++) {
                nuevaHT2[i][j] = H_T[i][j];
              }
            } else {
              for(int j = 0; j < 3; j++) {
                nuevaHT2[i][j] = 0;
              }
            }
          }*/

          //Impresion de la matriz
          /*for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 3; j++) {
              Serial.print(nuevaHT2[i][j]);
              Serial.print(" ");
            }
            Serial.println();
          }*/

          /*bool sindrome2[3];
          for (int fila = 0; fila < 8 - 1; fila++) {
          // Realiza el XOR de la fila actual con la siguiente
            for (int columna = 0; columna < 3; columna++) {
              nuevaHT2[fila + 1][columna] ^= nuevaHT2[fila][columna];
              // Almacena el resultado de la última operación XOR en el vector "codigo"
              if (fila == 8 - 2) {
                sindrome2[columna] = static_cast<bool>(nuevaHT2[fila + 1][columna]);
              }
            }
          }*/

          int erroract2 = 0;
          /*Serial.println("Sindrome nuevo");
          //Impresion del sindrome
            int bandera2 = 0;
            for (int i = 0; i < 3; i++) {
              Serial.print(sindrome2[i]);
                Serial.print(" ");
                if(sindrome2[i] != 0) {
                  bandera2++;
                } if(i == 2 && bandera2 != 0) {
                    Serial.println(" ERROR DETECTADO");
                    //errores++;
                    erroract2 = 1;
                    //Serial.println();
                }
            }*/
            Serial.println();

            /*if(erroract2 != 0) {
              Serial.print("No se pudo corregir el ERROR...");
              Serial.println();
              erroresNOCorregidos++;
            } else {
              Serial.print("ERROR CORREGIDO!!!");
              erroresCorregidos++;
            }*/

            //AQui

          //

        } else { //Fin encuentro indice
            Serial.println(" - Imposible correguir, no se encuentra el sindrome...");       
          }
      }
    } else {

        int b = 0;
        int o = -1;
        for(int i = 0; i < 16; i++) {
          b = 0;
          for(int j = 0; j < 7; j++) {
            if(receivedBool[j] == admitidos[i][j]) {
              b++;
            }
            if(b == 7) {
              o = i;
            }
          }
        }

        if(o != -1) {
          Serial.println();
          if(o == 0) Serial.print("El mensaje es 16");
          if(o == 1) Serial.print("El mensaje es 17");
          if(o == 2) Serial.print("El mensaje es 18");
          if(o == 3) Serial.print("El mensaje es 19");
          if(o == 4) Serial.print("El mensaje es 20");
          if(o == 5) Serial.print("El mensaje es 21");
          if(o == 6) Serial.print("El mensaje es 22");
          if(o == 7) Serial.print("El mensaje es 23");
          if(o == 8) Serial.print("El mensaje es 24");
          if(o == 9) Serial.print("El mensaje es 25");
          if(o == 10) Serial.print("El mensaje es 26");
          if(o == 11) Serial.print("El mensaje es 27");
          if(o == 12) Serial.print("El mensaje es 28");
          if(o == 13) Serial.print("El mensaje es 29");
          if(o == 14) Serial.print("El mensaje es 30");
          if(o == 15) Serial.print("El mensaje es 31");
          //erroresCorregidos++;
        }

            Serial.println();
            Serial.print("Errores detectados: ");
            Serial.println(errores);
            Serial.print("Errores Corregidos = ");
            Serial.print(erroresCorregidos);
            Serial.println();
            Serial.print("Errores NO Corregidos = ");
            Serial.print(erroresNOCorregidos);
            Serial.println();
            Serial.print("Mensajes recibidos: ");
            Serial.print(mensajes);
            Serial.println();

    }

    /*Serial.println();
    Serial.print("La BER ANTES de codificar es: ");
    bera = ((errores * 8)/(mensajes * 8));
    Serial.print(bera);
    Serial.println();
    Serial.print("La BER DESPUES de codificar es: ");
    berd = ((erroresNOCorregidos * 8)/(mensajes * 8));
    Serial.print(berd);*/
    
  } 

  //Calcular BER antes de codificar
    Serial.println();
    Serial.print("La BER ANTES de codificar es: ");
    bera = (static_cast<float>(errores * 7) / (mensajes * 7));
    Serial.print(bera);
    Serial.println();
    Serial.print("La BER DESPUES de codificar es: ");
    berd = (static_cast<float>(erroresNOCorregidos * 7) / (mensajes * 7));
    Serial.print(berd);
  
  //delay(100);
  
}