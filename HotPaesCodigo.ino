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
server.send(200, "text/html", EnvioHTML(temp_atual, umidade, frase_stat)); //Envia as informações usando o códi
go 200, especifica o conteúdo como "text/html" e chama a função EnvioHTML
}
void handle_NotFound() { //Função para lidar com o erro 404
server.send(404, "text/plain", "Não encontrado"); //Envia o código 404, especifica o conteúdo como "text/pain" e e
nvia a mensagem "Não encontrado"
}
String EnvioHTML(float temperatura, float umidade, String frase_stat) { //Exibindo a página da web em HTML
String ptr = "<!DOCTYPE html> <html>\n"; //Indica o envio do código HTML
ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
//Torna a página da Web responsiva em qualquer navegador Web
ptr += "<meta http-equiv='refresh' content='2'>";//Atualizar browser a cada 30 segundos
ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
ptr += "<title>Monitor de Temperatura e Umidade</title>\n"; //Define o título da página
//Configurações de fonte do título e do corpo do texto da página web
ptr += "<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color
: #000000;}\n";
ptr += "body{margin-top: 50px;background-color:#b0cfdd;}\n";
ptr += "h1 {margin: 50px auto 30px;}\n";
ptr += "h2 {margin: 40px auto 20px;}\n";

ptr += "p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
ptr += "</style>\n";
ptr += "</head>\n";
ptr += "<body>\n";
ptr += "<div id=\"webpage\">\n";
ptr += "<h1>Plataforma Hot Paes</h1>\n";
ptr += "<h2>Seja Bem vindo a nossa pagina de informacoes</h2>\n";
//Exibe as informações de temperatura e umidade na página web
ptr += "<p><b>Temperatura do pao: </b>";
ptr += (float)temperatura;
ptr += " Graus Celsius</p>";
ptr += "<p><b>Umidade do pao: </b>";
ptr += (float)umidade;
ptr += " %</p>";
ptr += "<p><b>Status do Pao: </b>";
ptr += (String)frase_stat;
ptr += "</p>";
ptr += "</div>\n";
ptr += "</body>\n";
ptr += "</html>\n";
return ptr;
}
/*
Função para preparar e realização uma requisição HTTP com informações de
temperatura
Http da api
identificação do sensor
chave do modulo para testar segurança
*/
void env_Dados ( float temperatura, int stat_mudancas, char *http_api , char *id_api, char*chave_modulo) {
String endpoint;
char medida[10];
char stat[5];
HTTPClient http; // Declarar um objeto da classe HTTPClient
WiFiClient wifiClient;
Serial.println( http_api );
http.begin( wifiClient , http_api ); // Especificando o endereço da requisição HTTP
http.addHeader("Content-Type", "application/x-www-form-urlencoded");
http.addHeader("Authorization-Token", api_header_key );
sprintf( medida, "%03d", (int)temperatura ); // Transforma numero em string (sprintf)
sprintf( stat, "%02d", (int)stat_mudancas );
endpoint = "api_key=";
endpoint += chave_modulo;
endpoint += "&id=";
endpoint += id_api;
endpoint += "&chave=";
endpoint += chave_modulo;
endpoint += "&medida=";
endpoint += medida;

endpoint += "&status=";
endpoint += stat;
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