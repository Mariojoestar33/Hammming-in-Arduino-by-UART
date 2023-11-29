const int sensorPin = A0;
const int led = 7;

const int G[4][7] = {
  {1, 1, 0, 1, 0, 0, 0},
  {0, 1, 1, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0},
  {1, 1, 1, 0, 0, 0, 1}
};

int m[4];  // Arreglo para almacenar los valores binarios de 4 bits
bool codigo[7]; // Arreglo booleano a enviar

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Toma de temperatura
  int sensorValue = analogRead(sensorPin);
  //float voltage = sensorValue * (5.0 / 1024.0);
  int temperatureC = (sensorValue * 5.0 / 1024) * 100;

  // Redondeo de la temperatura y ajuste del rango
  temperatureC = round(temperatureC);
  if (temperatureC <= 16) {
    temperatureC = 16;
  } else if (temperatureC >= 31) {
    temperatureC = 31;
  }

  // Imprimir la temperatura
  /*Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.println();*/
  //Serial.print(" C - Binario: ");

  // Imprimir la temperatura redondeada en formato binario
  for (int i = 3; i >= 0; i--) {
    m[i] = (temperatureC & (1 << i)) ? 1 : 0;
    //Serial.print(m[i]);
  }
  
  // Crear una copia invertida en el mismo array
  for (int i = 0; i < 2; i++) {
    int temp = m[i];
    m[i] = m[3 - i];
    m[3 - i] = temp;
  }

  // Imprimir la copia invertida
  /*Serial.print(" - Copia invertida: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(m[i]);
  }*/
  

  // Creacion de la nueva matriz
  int nuevaG[4][7];
  for (int i = 0; i < 4; i++) {
    if (m[i] == 1) {
      for (int j = 0; j < 7; j++) {
        nuevaG[i][j] = G[i][j];
      }
    } else {
      for (int j = 0; j < 7; j++) {
        nuevaG[i][j] = 0;
      }
    }
  }

  // Realiza la operación XOR por filas en nuevaG
  for (int fila = 0; fila < 3; fila++) { // Cambiado a 3 filas para (7,4)
    // Realiza el XOR de la fila actual con la siguiente
    for (int columna = 0; columna < 7; columna++) {
      nuevaG[fila + 1][columna] ^= nuevaG[fila][columna];
    }
  }

  // Almacena el resultado de la última operación XOR en el vector "codigo"
  for (int i = 0; i < 7; i++) {
    codigo[i] = static_cast<bool>(nuevaG[3][i]);
  }

  // Imprime cada elemento del vector "codigo" antes de enviarlo
  //Serial.print(" - Transmitir: ");
  for (int i = 0; i < 7; i++) {
    Serial.print(codigo[i]);
  }
  //Serial.println();

  digitalWrite(led, HIGH);
  //delay(100);
}