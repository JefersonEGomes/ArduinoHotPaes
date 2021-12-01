/*
FACULDADE SENAC PE
Projeto Hot Pães
Autores: Equipe Hot Pães
Envio de medida ( temperatura ) para APIs HTTP
Hardware
NODEMCU ESP8266
Sensor Temperatura e Umidade - DHT 22 ou DHT11
*/
#include <ESP8266WiFi.h> /* bilblioteca adequada para wifi esp8266 */
#include<ESP8266WebServer.h> /* Biblioteca para o ESP funcionar como servidor */
#include <ESP8266HTTPClient.h> /* biblioteca para acesso à API */
#include "DHT.h" /* bib sensor dht */
#include <WiFiClient.h> /* bibilioteca wifiClient */
#include "time.h" /*incluindo biblioteca para medição do tempo*/
#define api_header_key "ueyr123768565HGHgjhgHGHJghjghgHDFgdfdhgfklkjlkjuytty68"
#define DHTPIN 4 // pino D2 do nodemcu
#define DHTTYPE DHT11 // especificacao do modelo de sensor DHT
#define http_appjef "http://jefersonegomes.pythonanywhere.com/data_transfer"
#define http_appflavia "http://flaviab.pythonanywhere.com/data_temp";
#define IDMODULO "ID01-Jef" // definição da identificação do modulo de leitura e placa
#define KEYAPI "1A2b3C4E5f" // definição da chave de segurança - deve ser criptografada e sofrer mudanças temp
orárias
#define REDEWIFI "JACIEL" // wifi a ser utilizada
#define SENHA "a1s2d3f4g5" // senha da rede wifi a ser utilizada
ESP8266WebServer server(80); //server na porta 80
/*
Variáveis globais
*/
unsigned long intervalo = 30000; // variavel para definir intervalo entre leituras - cada 1000 equivale a um segundo
unsigned long tempoAnterior = 0; // variavel para guardar o momento da leitura / envio anterior
int num_medida = 0;
float temp_anterior = 0; // temperatura anterior
float temp_atual = 0; // temperatura atual
float umidade = 0; // umidade
float temp_pao = 45.00; // temperatura limite do pão
int status_mudanca = 0; // variavel para registrar midanca 0=nao, 1=sim

String frase_stat = ""; // Frase de feedback
DHT dht(DHTPIN, DHTTYPE); /* configura o pino e o tipo de sensor utilizado */
// Inicialização
void setup () {
// inicializando monitor serail, wifi e sensor
Serial.begin(115200);
WiFi.begin( REDEWIFI , SENHA );
dht.begin();
while (WiFi.status() != WL_CONNECTED) {
// Estabelecendo conexão com o wifi
delay(1000);
Serial.print("Connectando à rede WIFI..:");
Serial.println(REDEWIFI);
}
Serial.print("Connectado à rede WIFI ");
Serial.println(REDEWIFI);
Serial.print("IP: ");
Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP
server.on("/", handle_OnConnect); //Servidor recebe uma solicitação HTTP - chama a função handle_OnConnect
server.onNotFound(handle_NotFound); //Servidor recebe uma solicitação HTTP não especificada - chama a função
handle_NotFound
server.begin(); //Inicializa o servidor
Serial.println("Servidor HTTP inicializado");
}
void loop() {
String endpoint;
char medida[10];
static float temperatura;
unsigned long tempoatual = millis();
String fraseOK = "Olá venha buscar seu pão";
server.handleClient(); //Chama o método handleClient()
if ((tempoatual - tempoAnterior) > intervalo )
{
tempoAnterior = tempoatual;
if (WiFi.status() == WL_CONNECTED) { // Verificando o status da conexão WiFi
HTTPClient http; // Declaração de objeto da classe HTTPClient
num_medida++;
temperatura = dht.readTemperature();
temp_atual = temperatura;
if ( isnan( temperatura ) ) /* funcao paraa testar se a leitura foi bem sucedida */
{
Serial.println( "Erro na medicao\n" );
delay(100);
}
else
{
temperatura = (int) temperatura;

temp_atual = temperatura;
if (temp_atual != temp_anterior)
{
temp_anterior = temp_atual;
status_mudanca = 1;
frase_stat = "Vem buscar seu pão :)";
}
else {
status_mudanca = 0;
frase_stat = "Estou esfriando";
}

env_Dados( temperatura, status_mudanca, http_appjef, IDMODULO, KEYAPI );
env_Dados( temperatura, status_mudanca, http_appflavia, IDMODULO, KEYAPI );
}
}
}
}
void handle_OnConnect() {
temp_atual = dht.readTemperature(); //Realiza a leitura da temperatura
umidade = dht.readHumidity(); //Realiza a leitura da umidade
Serial.print("Temperatura: ");
Serial.print(temp_atual); //Imprime no monitor serial o valor da temperatura lida
Serial.println(" oC");
Serial.print("Umidade: ");
Serial.print(umidade); //Imprime no monitor serial o valor da umidade lida
Serial.println(" %");
if (temp_atual >= temp_pao) {
frase_stat = "Vem buscar seu pao quentinho";
} else {
frase_stat = "Espere a proxima fornada";
}
/*
Função para preparar e realização uma requisição HTTP com informações de
temperatura
Http da api
identificação do sensor
chave do modulo para testar segurança
*/
void env_Dados ( float temp_atual, int umidade, String frase_stat, char *http_api , char *id_api, char*chave_modulo) {
String endpoint;
char medida[10];
char umidade_pao[10];
String frase = frase_stat;
HTTPClient http; // Declarar um objeto da classe HTTPClient
WiFiClient wifiClient;
Serial.println( http_api );
http.begin( wifiClient , http_api ); // Especificando o endereço da requisição HTTP
http.addHeader("Content-Type", "application/x-www-form-urlencoded");
http.addHeader("Authorization-Token", api_header_key );
sprintf( medida, "%03d", (int)temperatura ); // Transforma numero em string (sprintf)
sprintf( umidade_pao, "%02d", (int)umidade );
endpoint = "api_key=";
endpoint += chave_modulo;
endpoint += "&id=";
endpoint += id_api;
endpoint += "&chave=";
endpoint += chave_modulo;
endpoint += "&medida=";
endpoint += medida;
endpoint += "&umidade_pao=";
endpoint += umidade_pao;
endpoint += "&frase=";
endpoint += frase;
int httpCode = http.POST(endpoint); // Enviando a requisição
if (httpCode > 0) { // Verificando o retorno da requisição
String retorno = http.getString(); // obtendo o retorno da requisição
Serial.println( retorno ); // mostrando o retorno
}
else {
Serial.print("Erro na requisição:");
Serial.println( http_api );
}
http.end(); //Close connection
}