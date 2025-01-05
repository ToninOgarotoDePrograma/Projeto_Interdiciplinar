// Inclua a biblioteca Servo
#include <ESP32Servo.h>
#include <WiFi.h>

// Configurações da rede Wi-Fi
const char* ssid = "Heber 2andar";
const char* password = "00000000";

// Configuração do servidor
const uint16_t porta = 8090;

// Declaração do servo motor
const int pinoServo1 = 12;
Servo servo1;

// Variável para armazenar o ângulo atual do servo
int anguloAtual = 90; // Começa em 90º

// Objeto para conexão Wi-Fi
WiFiServer servidor(porta);

void setup() 
{
  // Inicialize a comunicação serial para depuração
  Serial.begin(115200);

  // Conecte-se ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicialize o servidor
  servidor.begin();
  Serial.println("Servidor iniciado. Aguardando conexões...");

  // Inicialize o servo motor
  servo1.attach(pinoServo1);
  servo1.write(anguloAtual); // Posicione o servo no ângulo inicial
}

void loop() 
{
  // Aguarde conexões de cliente
  WiFiClient cliente = servidor.available();
  if (cliente) {
    Serial.println("Cliente conectado!");
    while (cliente.connected()) {
      if (cliente.available()) {
        // Leia o comando enviado pelo cliente
        String comando = cliente.readStringUntil('\n');
        comando.trim(); // Remove espaços e quebras de linha extras
        Serial.println("Comando recebido: " + comando);

        if (comando == "A") { // Movimento para a esquerda (decrementa ângulo)
          if (anguloAtual > 0) { // Limita o movimento a 0º
            anguloAtual -= 5;
            servo1.write(anguloAtual);
            Serial.println("Ângulo atual: " + String(anguloAtual) + "º");
          } else {
            Serial.println("Ângulo mínimo alcançado (0º).");
          }
        } else if (comando == "D") { // Movimento para a direita (incrementa ângulo)
          if (anguloAtual < 180) { // Limita o movimento a 180º
            anguloAtual += 5;
            servo1.write(anguloAtual);
            Serial.println("Ângulo atual: " + String(anguloAtual) + "º");
          } else {
            Serial.println("Ângulo máximo alcançado (180º).");
          }
        }
      }
    }

    // Feche a conexão com o cliente
    cliente.stop();
    Serial.println("Cliente desconectado.");
  }
}