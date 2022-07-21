#include "../include/alml_webserver.h"
#include "../include/device/device.h"
#include "../include/display/remote.h"
#include "../include/pixel/effectcore.h"
#include "../include/display/lamp.h"
#include "../include/fastled/base.h"
#include "../include/fastled/fastled.h"

#include "alml_task.h"

extern EffectWorker  * _effects[];

#ifndef DEBUG
  #ifdef DEBUG_WEBSERVER
    #define DEBUG
  #endif
#endif
#ifdef DEBUG
  #define LOG(func, ...) ALT_TRACEC(func, __VA_ARGS__)
#else
  #define LOG(func, ...) ;
#endif

#define UDPMULTI_IN_MAXSIZE 2048
#define UDP_IN_MAXSIZE 2048


// char      _udpMulti_packetBuffer[UDP_IN_MAXSIZE + 1]; 
// char      _udp_packetBuffer[UDP_IN_MAXSIZE + 1]; 
// Udp *     _UdpPtr = nullptr;
// UdpMulti   _UdpMulti;
// Udp      _Udp;
// AsyncWebSocket    web_socket("/ws");
AsyncWebServer    web_server(80);
AsyncEventSource  event("/events");
Webserver         _Webserver;
DNSServer         dnsServer;
WCEVO_manager     _WCEVO_manager(ADS_NAME, (const char *)"alml1234", &dnsServer, &web_server); 
TaskSimple        * _task_httpCallback        = nullptr;
TaskSimple        * _task_socketCallback      = nullptr;
TaskSimple        * _task_socketCleanupClient = nullptr;

namespace alml_webserver {
  String ip2string(const IPAddress &a) {
      char buf[18];
      sprintf(buf,"%d.%d.%d.%d",a[0],a[1],a[2],a[3]);
      return String(buf);
  }
  IPAddress get_localIP() {
    IPAddress localIP;
    localIP = WiFi.localIP();
    if (localIP[0] != 0) {
      return localIP;
    }

    return INADDR_NONE;
  } 
}



void not_found_server(AsyncWebServerRequest * request);

// #define AP_WS_FS
#ifdef AP_WS_FS
const char FileSystemStatu_html[] PROGMEM = "<p id=\"FileSystem_statu\" >Free Storage: %FREESPIFFS% | Used Storage: %USEDSPIFFS% | Total Storage: %TOTALSPIFFS%</p>";
const char FileSystemIndex_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
  <style>
    #dbg{font-family:monaco;font-size:12px;line-height:13px;color:#aaa;margin:0;padding:0;padding-left:4px}body{font-family:Arial,Helvetica,sans-serif}*{box-sizing:border-box}.form-inline{display:flex;flex-flow:row wrap;align-items:center}.form-inline label{margin:5px 10px 5px 0}.form-inline input{vertical-align:middle;margin:5px 10px 5px 0;padding:10px;background-color:#fff;border:1px solid #ddd}.form-inline button{padding:10px 20px;background-color:#1e90ff;border:1px solid #ddd;color:#fff;cursor:pointer}.form-inline button:hover{background-color:#4169e1}@media (max-width:800px){.form-inline input{margin:10px 0}.form-inline{flex-direction:column;align-items:stretch}}.row{--bs-gutter-x:1.5rem;--bs-gutter-y:0;display:flex;flex-wrap:wrap;margin-top:calc(var(--bs-gutter-y) * -1);margin-right:calc(var(--bs-gutter-x) * -.5);margin-left:calc(var(--bs-gutter-x) * -.5)}.row>*{flex-shrink:0;width:100%;max-width:100%;padding-right:calc(var(--bs-gutter-x) * .5);padding-left:calc(var(--bs-gutter-x) * .5);margin-top:var(--bs-gutter-y)}.col{width:auto}

  </style>   
</head>
<body id="body"  onload="onBodyLoad()">
  <pre onclick="clearEvent()" id="dbg"></pre>
  <p><h1>FileSystem</h1></p>
  <p id="FileSystem_statu" >Free Storage: %FREESPIFFS% | Used Storage: %USEDSPIFFS% | Total Storage: %TOTALSPIFFS%</p>
  <div class=row>
    <div class="col">
      <div class="form-inline">
        <input id="fileupload" type="file" name="data">
        <button onclick="uploadFile()">Upload File</button>
      </div>
    </div>
    <div class="col">
      <div class="form-inline">
        <input type="text" id="filePath" value="%FOPATH%">
        <button onclick="sendPath()">update</button>
      </div>
    </div>
    <div class="col">
      <div class="form-inline">
        <button onclick="downloadDeleteButtonv2('download')">donwload</button>
        <button onclick="downloadDeleteButtonv2('delete')">delete</button>
      </div>
    </div>
  </div>
  <div id="FileSystem_list" >%FILELIST%</div>
  <script type="text/javascript">
    function ge(e){return document.getElementById(e)}function ce(e){return document.createElement(e)}function stb(){window.scrollTo(0,document.body.scrollHeight||document.documentElement.scrollHeight)}function clearEvent(){ge("dbg").innerText=""}function addMessage(e){var t=ce("div");t.innerText=e,ge("dbg").appendChild(t),stb()}function startEvents(){var e=new EventSource("/events");e.onopen=function(e){},e.onerror=function(e){e.target.readyState!=EventSource.OPEN&&addMessage("Events Closed")},e.onmessage=function(e){addMessage(e.data)},e.addEventListener("ota",function(e){addMessage(e.data)},!1)}function onBodyLoad(){startEvents()}function request_format_http(e,t,n){return"/"+e+"?"+t+"="+n}function request_format_httpX2(e,t,n,s,o){return"/"+e+"?"+t+"="+n+"&"+s+"="+o}function request_http(e,t){var n=new XMLHttpRequest;n.onreadystatechange=function(){4==this.readyState&&200==this.status&&t&&t(n.responseText)},n.open("GET",e,!0),n.send()}function request_send_http(e,t,n,s){request_http(request_format_http(e,t,n),s)}function request_send_httpX2(e,t,n,s,o,a){request_http(request_format_httpX2(e,t,n,s,o),a)}function sendPath(){request_send_http("FileSystem_upload","path",document.getElementById("filePath").value,null)}function downloadDeleteButton(e,t){if("delete"==t&&request_send_httpX2("FileSystem_set","name",e,"action",t,function(e){ge("FileSystem_list").innerHTML=e,request_http("/FileSystem_statu",function(e){ge("FileSystem_statu").innerHTML=e})}),"download"==t){var n="/FileSystem_set?name="+e+"&action="+t;window.open(n,"_blank")}}async function uploadFile(){let e=new FormData;e.append("file",fileupload.files[0]),await fetch("/FileSystem_upload",{method:"POST",body:e}),request_http("/FileSystem_list",function(e){ge("FileSystem_list").innerHTML=e,request_http("/FileSystem_statu",function(e){ge("FileSystem_statu").innerHTML=e})})}function downloadDeleteButton(e1,t){var table = document.getElementById('mytable');var e = "/" + table.rows[e1].cells[0].innerHTML + "/" + table.rows[e1].cells[1].innerHTML;if("delete"==t&&request_send_httpX2("FileSystem_set","name",e,"action",t,function(e){ge("FileSystem_list").innerHTML=e,request_http("/FileSystem_statu",function(e){ge("FileSystem_statu").innerHTML=e})}),"download"==t){var n="/FileSystem_set?name="+e+"&action="+t;window.open(n,"_blank")}}async function uploadFile(){let e=new FormData;e.append("file",fileupload.files[0]),await fetch("/FileSystem_upload",{method:"POST",body:e}),request_http("/FileSystem_list",function(e){ge("FileSystem_list").innerHTML=e,request_http("/FileSystem_statu",function(e){ge("FileSystem_statu").innerHTML=e})})}
    function downloadDeleteButtonv2(b){for(var e=document.getElementById("mytable").getElementsByTagName("INPUT"),a=0;a<e.length;a++)if(e[a].checked){var d,c=document.getElementById("mytable");if(d="/"==c.rows[a+1].cells[0].innerText?"/"+c.rows[a+1].cells[1].innerText:"/"+c.rows[a+1].cells[0].innerText+"/"+c.rows[a+1].cells[1].innerText,"delete"==b&&request_send_httpX2("FileSystem_set","name",d,"action",b,function(a){ge("FileSystem_list").innerHTML=a,request_http("/FileSystem_statu",function(a){ge("FileSystem_statu").innerHTML=a})}),"download"==b){var f="/FileSystem_set?name="+d+"&action="+b;window.open(f,"_blank")}break}}    
    }
  </script>  
</body>
</html>
)rawliteral";

String humanReadableSize(const size_t bytes);

void FileSystem_recursiveList(String & returnText) {
  returnText = "<table id='mytable'><tr><th align='left'>Folder</th><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
  DynamicJsonDocument doc(5000);
  JsonObject root = doc.to<JsonObject>();
  al_tools::SPIFFS_printFiles("/", root);
  // Serial.println("FileSystem_recursiveList");serializeJsonPretty(doc, Serial);Serial.println();
  JsonArray arr = doc[F("folders")].as<JsonArray>();
  // uint8_t tbP = 0;
  for (size_t i = 0; i < arr.size(); i++) {
    String path = arr[i].as<String>();
    JsonArray oPath;
    if (path == "/")  oPath = doc[path][F("items")].as<JsonArray>();
    else              oPath = doc[F("/")][path][F("items")].as<JsonArray>();
    for (size_t j = 0; j < oPath.size(); j++) {
      String file = oPath[j][F("file")].as<String>();
      size_t size = oPath[j][F("size")].as<size_t>();
      returnText += "<tr align='left'><td><input type='checkbox'>" + path+ "</td><td>" + file+ "</td><td>" + humanReadableSize(size) + "</td>";
      // String fullpath = (path=="/")?path+file:"/"+path+"/"+file;
      // Serial.printf("%3d fullpath: %s\n", j, fullpath.c_str());
      // returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(++tbP) + "\', \'download\')\">Download</button>";
      // returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(++tbP) + "\', \'delete\')\">Delete</button></tr>";
    }
  }
  returnText += "</table>";
}

// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

String FileSystem_processor(const String& var) {
  #if defined(ESP8266)
    FSInfo fs_info;
    FILESYSTEM.info(fs_info); 
  #endif   
  if (var == "FILELIST") {
    String result;
    FileSystem_recursiveList(result);
    return result;
  }
  if (var == "FREESPIFFS") {
    #if defined(ESP8266)
      return humanReadableSize((fs_info.totalBytes - fs_info.usedBytes));  
    #elif defined(ESP32)
      return humanReadableSize((FILESYSTEM.totalBytes() - FILESYSTEM.usedBytes()));
    #endif      
  }
  if (var == "USEDSPIFFS") {
    #if defined(ESP8266)
      return humanReadableSize(fs_info.usedBytes);
    #elif defined(ESP32)
      return humanReadableSize(FILESYSTEM.usedBytes());
    #endif
    
  }
  if (var == "TOTALSPIFFS") {
    #if defined(ESP8266)
      return humanReadableSize(fs_info.totalBytes);
    #elif defined(ESP32)
      return humanReadableSize(FILESYSTEM.totalBytes());
    #endif    
  }
  if (var == "FOPATH") {
    return _Webserver.uploadFOpath;
  }
  return String();
}

// https://github.com/smford/esp32-asyncwebserver-fileupload-example
void FileSystem_upload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  LOG(APPT_DEBUGREGION_WEBSERVER, "\n\tclient: %s - url: %s\n",  request->client()->remoteIP().toString().c_str(), request->url().c_str());
  if (!index) {
    request->_tempFile = FILESYSTEM.open(_Webserver.uploadFOpath + filename, "w");
    LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tUpload Start: %s\n", String(filename).c_str());
    event.send("Upload in progress", "ota"); 
  }
  if (len) {
    request->_tempFile.write(data, len);
    LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tWriting file: %s", String(filename).c_str());
    LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s: - index: %s", String(index).c_str());
    LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s: - len: %s\n", String(len).c_str());
  }
  if (final) {
    request->_tempFile.close();
    LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tUpload Complete: %s", String(filename).c_str());
    LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s: - size: %s\n", String(String(index) + len).c_str());
    event.send("Upload finish", "ota");
    request->redirect("/FileSystem");
  }
}
#endif

Webserver::Webserver() {
  #if defined(DEBUG)
  _httpTrace    = true;  
  #else
  _httpTrace    = false;  
  #endif
}


void Webserver::set_socketIsConnected(boolean v1) {_socketIsConnected = v1;}
void Webserver::set_socketClient(AsyncWebSocketClient * v1) {_socketClient = v1;}
void Webserver::set_socketServer(AsyncWebSocket * v1)       {_socketServer = v1;}
void Webserver::set_socketCallback(callback_function_t f)   {_socketCallback = std::move(f);}
// void Webserver::set_socketCallbackData(const String & v1)   {_socketCallbackData = v1;}
boolean Webserver::socketIsConnected() {return _socketIsConnected;}
void Webserver::socketHandle(const String & v1) {
  if (_socketCallback) _socketCallback(v1);
}
void socket_event(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
   LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] connect\n", server->url(), client->id());
    _Webserver.set_socketClient(client);
    _Webserver.set_socketServer(server);
    _Webserver.set_socketIsConnected(true);
    // client->printf_P(PSTR("Hello Client %u :)"), client->id());
    // client->ping();
    delay(0);

  } else if(type == WS_EVT_DISCONNECT){
   LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] disconnect\n", server->url(), client->id());
    _Webserver.set_socketIsConnected(false);
     delay(0);

  } else if(type == WS_EVT_ERROR){
   LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
   LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[12];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t)data[i] );
          msg += buff ;
        }
      }
      LOG(ALML_DEBUGREGION_WEBSERVER, "ws <<<[%s][%u] %s-message[%llu]\n\t%s\n", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len, msg.c_str());

      if(info->opcode == WS_TEXT)  {

        _task_socketCleanupClient->set_enabled(false);

        delay(0);

        // _Webserver.set_socketCallbackData(msg);
        _task_socketCallback->set_callbackOstart(std::bind(&Webserver::socketHandle, _Webserver, msg));
        _task_socketCallback->set_taskDelay(ETD::ETD_DELAY, true, 5000, 0);
        _task_socketCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
        _task_socketCallback->set_iteration_max(0);
        _task_socketCallback->set_enabled(true);         
      }
        // client->text("I got your text message");
      // else
        // client->binary("I got your binary message");
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
         LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
       LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[12];
        for(size_t i=0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] frame[%u] %s[%llu - %llu]: %s\n", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len, msg.c_str());

      if((info->index + len) == info->len){
       LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
         LOG(ALML_DEBUGREGION_WEBSERVER, "ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          // if(info->message_opcode == WS_TEXT)
            // client->text("I got your text message");
          // else
            // client->binary("I got your binary message");
        }
      }
    }
  }
}

void Webserver::socket_send(const String & message) {
  if ((_socketIsConnected) && (_socketClient!=NULL)) {

    delay(0);

    _socketClient->text(message);

    if (_socketTrace) {  
      #ifdef DEBUG
        LOG(ALML_DEBUGREGION_WEBSERVER, "ws >>>[%s][%u] message[%d]\n\t%s\n", _socketServer->url(), _socketClient->id(), message.length(), message.c_str());
      #endif 
    }
    
  }
  else
  {
    if (_socketTrace) {
      if (!_socketIsConnected)  {LOG(ALML_DEBUGREGION_WEBSERVER, "[ERROR] no socket connected\n");}
      if (_socketClient==NULL)  {LOG(ALML_DEBUGREGION_WEBSERVER, "[ERROR] no socket client available\n");}
    }
  }
}




void Webserver::http_send(AsyncWebServerRequest * request, const int & code, ENUM_WSTP ct, const String & data) {

  request->send(code, FPSTR(WSTP_ARR[ct]), data); 

  #ifdef DEBUG
    LOG(ALML_DEBUGREGION_WEBSERVER, "http >>>[%s][%s] %s-message[%d]\n\t%s\n", 
      request->host().c_str(), request->url().c_str(), WSTP_ARR[ct], data.length(), data.c_str());    
  #endif   
  
}

void Webserver::device_api(DynamicJsonDocument & doc, String & reply) {

  uint8_t op = 0;

  DynamicJsonDocument doc_reponse(3200);
  JsonObject root = doc_reponse.to<JsonObject>();
  JsonObject oOjbect = root.createNestedObject(F("device"));

  String ip = WiFi.localIP().toString();
  uint8_t oc;
  DevicePtrGet()->get_outputCount(oc);
  oOjbect[FPSTR(ALMLPT_IP)]= ip;
  oOjbect[FPSTR(ALMLPT_DN)]= DevicePtrGet()->get_name();
  oOjbect[FPSTR(ALMLPT_OC)]= oc;

  if (doc.containsKey(FPSTR(ALMLPT_API_OP))) {
    op = doc[FPSTR(ALMLPT_API_OP)].as<uint8_t>();
  } else {
    serializeJson(doc_reponse, reply);
    return;
  } 

  if (op == 1) {
    oOjbect[F("result")] = "server restarted in 5sec";
    DevicePtrGet()->restart_requiered();
  } 
  if (op == 2) {
    if (doc.containsKey(F("format"))) { 
      oOjbect[F("result")] = "fortmat done, server restarted in 5sec";
      DevicePtrGet()->format(doc[F("format")].as<uint8_t>());
      DevicePtrGet()->restart_requiered();
    }
  } 
  else if (op == 3) {
    if (doc.containsKey(FPSTR(ALMLPT_API_OUTPUT))) { 
      DynamicJsonDocument output(1024);  
      String sData = doc[FPSTR(ALMLPT_API_OUTPUT)].as<String>();
      serializeJson(output, sData);
      DeserializationError error = deserializeJson(output, sData);
      if (error) {
        oOjbect[F("result")] = "deserializeJson error";
      }  else {
        output[FPSTR(ALMLPT_DN)] = DevicePtrGet()->get_name();
        RemoteControlPtrGet()->handleJson(output, false);
        oOjbect = root.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
        DevicePtrGet()->outputs_toJson(oOjbect, false, true, false);        
        yield();
        RemoteControlPtrGet()->handleJson(4);
      }  
    }  
  }
  else if (op == 4) {
    if (doc.containsKey(FPSTR(ALMLPT_GET))) { 
      // DynamicJsonDocument output(2048); 
      JsonArray getArr = doc[FPSTR(ALMLPT_GET)].as<JsonArray>();
      for (JsonObject item : getArr) {
        String n = item[FPSTR(ALMLPT_N)].as<String>();
        String v = item[FPSTR(ALMLPT_V)].as<String>();
        if (n == FPSTR(ALMLPT_KKEY_004))        {reqNamIDList_json(0, doc_reponse);}
        else if (n ==  FPSTR(ALMLPT_KKEY_006))  {reqNamIDList_json(v, doc_reponse);}
        else if (n ==  FPSTR(ALMLPT_KKEY_002)) {
          oOjbect = root.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
          DevicePtrGet()->outputs_toJson(oOjbect, false, true, false);    
        }
        else if (n ==  FPSTR(ALMLPT_KKEY_003)) {
          oOjbect = root.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
          DevicePtrGet()->outputs_toJson(oOjbect, false, true, true);    
        }  
        else if ( n == FPSTR(ALMLPT_KKEY_001) ) {
          uint8_t oP = v.toInt();
          JsonArray arr = root.createNestedArray(F("effect"));
          JsonObject objEff = arr.createNestedObject();
          _effects[oP]->geteffconfig(oP, objEff, LAMPPTRGET()->get_globalBrightness(oP, 0));       
        }              
      }
    }  
  } 
  else if (op == 5) {
    if (doc.containsKey(FPSTR(ALMLPT_V))) { 
      String v = oOjbect[FPSTR(ALMLPT_V)].as<String>();
      oOjbect[F("result")] = "deepsleep requiered";
      if (v=="max") {
        _task_httpCallback->set_callbackOstart([](){
          FastLED.clear();
          FastLED.show();
          ESP.deepSleep(ESP.deepSleepMax());
        });
        _task_httpCallback->set_iteration_max(0);
        _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5, 2);
        _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
        _task_httpCallback->set_enabled(true);         
      } else if (v=="1") {
        _task_httpCallback->set_callbackOstart([](){
          FastLED.clear();
          FastLED.show();
          uint64_t t = 3600000000UL; // 3600000000UL
          ESP.deepSleep(t);
        });
        _task_httpCallback->set_iteration_max(0);
        _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5, 2);
        _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
        _task_httpCallback->set_enabled(true);   
      } else if (v=="030") {
        _task_httpCallback->set_callbackOstart([](){
          FastLED.clear();
          FastLED.show();          
          uint64_t t = 900000000UL; // 900000000UL
          ESP.deepSleep(t);
        });
        _task_httpCallback->set_iteration_max(0);
        _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5, 2);
        _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
        _task_httpCallback->set_enabled(true);   
      }

    }
  }      
  serializeJson(doc_reponse, reply);

}
void Webserver::httpHandle(AsyncWebServerRequest * request, const String & v1, uint8_t mod){
  if (request) {
    String json = v1;
    json.replace("\r\n", "");
    // Serial.printf_P(PSTR("[_callPost /json] %s\n"), json.c_str(()));  
    String reply = "";
    DynamicJsonDocument doc(2048);  
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
      Serial.printf_P(PSTR("[_http_post_cb][PARSSING ERROR] %s\n"), json.c_str()); 
      Serial.println(error.c_str()) ;
    } else {
      LOG(ALML_DEBUGREGION_WEBSERVER, "-\n");
      switch (mod) {
          case 0: _AP_Api.parsingRequest(doc, reply, "");break;
          case 1: device_api(doc, reply);break;
          default:break;
      }
    } 
    LOG(ALML_DEBUGREGION_WEBSERVER, "-\n");
    http_send(request, 200, WSTE_APPJSON, reply);
    LOG(ALML_DEBUGREGION_WEBSERVER, "-\n");
  }  
}

void Webserver::setupAp(){
  if(!_task_httpCallback)         _task_httpCallback          = new TaskSimple();
  if(!_task_socketCallback)       _task_socketCallback        = new TaskSimple();
  if(!_task_socketCleanupClient)  _task_socketCleanupClient   = new TaskSimple();

  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "AP register HTTP_POST request : /apapi\n");  
  web_server.on("/apapi", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, [=](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {  
      // _httpCallbackRequest  = request;
      String sData = "";
      for (size_t i = 0; i < len; i++) {sData += (char) data[i];}
      sData.replace("\r\n", "");
      LOG(ALML_DEBUGREGION_WEBSERVER, "http <<<[%s][%s][%s] %s-message[%d]\n%s\n", 
      request->host().c_str(), request->url().c_str(), WRMTP_ARR[request->method()-1], request->contentType().c_str(), sData.length(), sData.c_str());   
      LOG(ALML_DEBUGREGION_WEBSERVER, "--\n"); 
      _task_httpCallback->set_callbackOstart(std::bind(&Webserver::httpHandle, this, request, sData, 0));
      _task_httpCallback->set_iteration_max(0);
      _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5000, 0);
      _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
      _task_httpCallback->set_enabled(true); 
  }).setFilter(ON_AP_FILTER);

//   ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "AP register HTTP_POST request : /almlapi\n"); 
//   web_server.on("/almlapi", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, [=](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {  
//       // _httpCallbackRequest  = request;
//       String sData = "";
//       for (size_t i = 0; i < len; i++) {sData += (char) data[i];}
//       sData.replace("\r\n", "");
//       LOG(ALML_DEBUGREGION_WEBSERVER, "http <<<[%s][%s][%s] %s-message[%d]\n%s\n", 
//         request->host().c_str(), request->url().c_str(), WRMTP_ARR[request->method()-1], request->contentType().c_str(), sData.length(), sData.c_str()
//       );    
//       _task_httpCallback->set_callbackOstart(std::bind(&Webserver::httpHandle, this, request, sData, 1));
//       _task_httpCallback->set_iteration_max(0);
//       _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5000, 0);
//       _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
//       _task_httpCallback->set_enabled(true); 
//   }).setFilter(ON_AP_FILTER);   
}

void Webserver::setup(){
  if(!_task_httpCallback)         _task_httpCallback          = new TaskSimple();
  if(!_task_socketCallback)       _task_socketCallback        = new TaskSimple();
  if(!_task_socketCleanupClient)  _task_socketCleanupClient   = new TaskSimple();

  // web_socket.onEvent(socket_event);
  // web_server.addHandler(&web_socket);
  event.onConnect([](AsyncEventSourceClient *client){
    char buffer[100];
    String time;
    al_tools::on_time_d(time);
    sprintf_P(buffer, PSTR("client connected at : %s"), time.c_str());
    client->send(buffer,NULL,millis(),1000);
  });
  web_server.addHandler(&event);


  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "STA register HTTP_GET request : / to www/index.html\n");
  web_server.serveStatic("/", FILESYSTEM, "/").setDefaultFile("www/index.html").setFilter(ON_STA_FILTER);
  
  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "STA register HTTP_GET request : /api\n"); 
  web_server.on("/api", HTTP_GET, [](AsyncWebServerRequest * request) {
    DynamicJsonDocument doc(3200);
    reqNamIDList_json(0, doc);
    String result;
    serializeJson(doc, result);
    request->send(200, "application/json", result);
  }); 
  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "STA register HTTP_GET request : /api\n"); 
  web_server.on("/kodular", HTTP_GET, [](AsyncWebServerRequest * request) {
    String result;
    DynamicJsonDocument docReponse(1024);
    JsonObject objDevice = docReponse.createNestedObject(F("device"));
    objDevice[FPSTR(ALSI_STAIP)] = WiFi.localIP().toString();
    objDevice[FPSTR(ALSI_HOSTNAME)] = WiFi.hostname();
    serializeJson(docReponse, result);
    request->send(200, FPSTR(WSTP_002), result);
  }).setFilter(ON_STA_FILTER); 
  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "STA register HTTP_POST request : /apapi\n"); 
  web_server.on("/apapi", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, [=](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {  
      // _httpCallbackRequest  = request;
      String sData = "";
      for (size_t i = 0; i < len; i++) {sData += (char) data[i];}
      sData.replace("\r\n", "");
      LOG(ALML_DEBUGREGION_WEBSERVER, "http <<<[%s][%s][%s] %s-message[%d]\n\t%s\n", 
      request->host().c_str(), request->url().c_str(), WRMTP_ARR[request->method()-1], request->contentType().c_str(), sData.length(), sData.c_str());    
      LOG(ALML_DEBUGREGION_WEBSERVER, "--\n");
      _task_httpCallback->set_callbackOstart(std::bind(&Webserver::httpHandle, this, request, sData, 0));
      _task_httpCallback->set_iteration_max(0);
      _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5000, 0);
      _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
      _task_httpCallback->set_enabled(true); 
  });

  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "STA register HTTP_POST request : /almlapi\n"); 
  web_server.on("/almlapi", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, [=](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {  
      // _httpCallbackRequest  = request;
      String sData = "";
      for (size_t i = 0; i < len; i++) {sData += (char) data[i];}
      sData.replace("\r\n", "");
      LOG(ALML_DEBUGREGION_WEBSERVER, "http <<<[%s][%s][%s] %s-message[%d]\n\t%s\n", 
        request->host().c_str(), request->url().c_str(), WRMTP_ARR[request->method()-1], request->contentType().c_str(), sData.length(), sData.c_str()
      );    
      _task_httpCallback->set_callbackOstart(std::bind(&Webserver::httpHandle, this, request, sData, 1));
      _task_httpCallback->set_iteration_max(0);
      _task_httpCallback->set_taskDelay(ETD::ETD_DELAY, true, 5000, 0);
      _task_httpCallback->set_taskDelayEnabled(ETD::ETD_DELAY, true);
      _task_httpCallback->set_enabled(true); 
  });  
  // web_server.on("/",          [](AsyncWebServerRequest *request)  { 
  //   AsyncWebServerResponse *response = request->beginResponse(FILESYSTEM, "/www/index.html", "text/html");
  //   request->send(response);    
  // }).setFilter(ON_STA_FILTER);

  ALT_TRACEC(WCEVO_DEBUGREGION_WCEVO, "STA register onNotFound\n");
  web_server.onNotFound([](AsyncWebServerRequest *request) {
    not_found_server(request);
  });
  
/*
  web_server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index)
      LOG(ALML_DEBUGREGION_WEBSERVER, "UploadStart: %s\n", filename.c_str());
    // LOG(ALML_DEBUGREGION_WEBSERVER, "%s", (const char*)data);
    if(final)
      LOG(ALML_DEBUGREGION_WEBSERVER, "UploadEnd: %s (%u)\n", filename.c_str(), index+len);
  });
  web_server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    if(!index)
      LOG(ALML_DEBUGREGION_WEBSERVER, "BodyStart: %u\n", total);
    // LOG(ALML_DEBUGREGION_WEBSERVER, "%s", (const char*)data);
    if(index + len == total)
      LOG(ALML_DEBUGREGION_WEBSERVER, "BodyEnd: %u\n", total);
  });  
*/
#ifdef AP_WS_FS
  web_server.on("/FileSystem", HTTP_GET, [](AsyncWebServerRequest * request) {
    // request->send(200, "text/html", "webPAge");
    // request->send(FILESYSTEM, "/FileSystem_list.html", "text/html");
    request->send_P(200, "text/html", FileSystemIndex_html, FileSystem_processor);
  });
  web_server.on("/FileSystem_statu", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", FileSystemStatu_html, FileSystem_processor);
  });   


  web_server.on("/FileSystem_list", HTTP_GET, [](AsyncWebServerRequest * request) {
    String result;
    FileSystem_recursiveList(result);
    request->send(200, "text/html", result);
  }); 
  web_server.on("/FileSystem_set", HTTP_GET, [](AsyncWebServerRequest * request) {
    LOG(APPT_DEBUGREGION_WEBSERVER, "\n\tclient: %s - url: %s\n",  request->client()->remoteIP().toString().c_str(), request->url().c_str());
    if (request->hasParam("name") && request->hasParam("action")) {
      const char *fileName = request->getParam("name")->value().c_str();
      const char *fileAction = request->getParam("action")->value().c_str();

      LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tfile: %s\n", fileName);
      LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\taction: %s\n", fileAction);
      
      if (!FILESYSTEM.exists(fileName)) {
        LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tERROR: file does not exist\n");
        request->send(400, "text/plain", "ERROR: file does not exist");
      } else {
        LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tfile exists\n");;
        if (strcmp(fileAction, "download") == 0) {
          LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tdownloaded\n");
          request->send(FILESYSTEM, fileName, "application/octet-stream");
        } else if (strcmp(fileAction, "delete") == 0) {
          LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tdeleted\n");
          FILESYSTEM.remove(fileName);
          event.send("File deleted", "ota");  
          String result;
          FileSystem_recursiveList(result);          
          request->send(200, "text/plain", result);
        } else {
          LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\tERROR: invalid action param supplied\n");
          request->send(400, "text/plain", "ERROR: invalid action param supplied");
        }
      }
    } else {
      request->send(400, "text/plain", "ERROR: name and action params required");
    }
  });
  web_server.on("/FileSystem_upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      }, FileSystem_upload);
  web_server.on("/FileSystem_upload", HTTP_GET, [&](AsyncWebServerRequest * request) {
    LOG(APPT_DEBUGREGION_WEBSERVER, "http://%s%s\n", request->host().c_str(), request->url().c_str());
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if (p->isFile())      {LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\t_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());} 
      else if(p->isPost())  {LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\t_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());} 
      else                  {LOG(APPT_DEBUGREGION_WEBSERVER, "&c:1&s:\t_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());}
      if (p->name() == "path") {
        uploadFOpath = p->value();
      }
    }
    char buffer[100];
    sprintf_P(buffer, PSTR("filesystem download path set to : %s"), uploadFOpath.c_str());
    event.send(buffer, "ota");  
    request->send(200, "text/plain", uploadFOpath);
  });  
#endif
  // _task_socketCleanupClient->set_callback([](){/*Serial.println("-");*/ web_socket.cleanupClients();});
  // _task_socketCleanupClient->set_taskDelay(ETD::ETD_TIMER, true, 500, 1);
  // _task_socketCleanupClient->set_taskDelay(ETD::ETD_DELAY, true, 1, 2);
  // _task_socketCleanupClient->set_taskDelayEnabled(ETD::ETD_DELAY, true);
  // _task_socketCleanupClient->set_taskDelayEnabled(ETD::ETD_TIMER, false);
  // _task_socketCleanupClient->set_iteration_max(-1);
  // _task_socketCleanupClient->set_enabled(true);  
  LOG(ALML_DEBUGREGION_WEBSERVER, "--\n"); 
} 

void Webserver::begin() {
  web_server.begin();
}
void Webserver::handle()  {
  if(_task_httpCallback)        {if (_task_httpCallback->isEnabled()) _task_httpCallback->execute();}
  if(_task_socketCleanupClient) {if (_task_socketCleanupClient->isEnabled()) _task_socketCleanupClient->execute();}
  if(_task_socketCallback)      {if (_task_socketCallback->isEnabled()) _task_socketCallback->execute();}

}

void not_found_server(AsyncWebServerRequest * request){
    if (_Webserver._httpTrace){
      String result = "";
      if (request->method() == HTTP_GET)          {result = "GET";}
      else if(request->method() == HTTP_POST)     {result = "POST";}
      else if(request->method() == HTTP_DELETE)   {result = "DELETE";}
      else if(request->method() == HTTP_PUT)      {result = "PUT";}
      else if(request->method() == HTTP_PATCH)    {result = "PATCH";}
      else if(request->method() == HTTP_HEAD)     {result = "HEAD";}
      else if(request->method() == HTTP_OPTIONS)  {result = "OPTIONS";}
      else                                        {result = "UNKNOWN";}
      LOG(ALML_DEBUGREGION_WEBSERVER, "%s http:// -host:: %s -url: %s \n", result.c_str(), request->host().c_str(), request->url().c_str());

      if(request->contentLength()){
        LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\t_CONTENT_TYPE: %s\n", request->contentType().c_str());
        LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\t_CONTENT_LENGTH: %u\n", request->contentLength());
      }

      int headers = request->headers();
      int i;
      for(i=0;i<headers;i++){
        AsyncWebHeader * h = request->getHeader(i);
        LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\t_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
      }

      int params = request->params();
      for(i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if (p->isFile())      {LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\t_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());} 
        else if(p->isPost())  {LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\t_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());} 
        else                  {LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\t_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());}
      }
    }

    char err[100];
    sprintf_P(err, PSTR("Error: 'http://%s%s' was not found\n"), request->host().c_str(), request->url().c_str());
    
    // String filepath = request->url();
    // String content_type;
    // if (filepath=="/") filepath="/www/index.html";
    // boolean gzip = (filepath.indexOf(".gzip")!=-1);
    // if (FILESYSTEM.exists(filepath)) {
    //   int d=filepath.indexOf('.');
    //   if (d!=-1) {
    //     String suffix=filepath.substring(d+1);
    //     d=suffix.indexOf(".");
    //     if (d!=-1) suffix=suffix.substring(0,d);
    //     if    (suffix=="css") content_type="text/css";
    //     else if (suffix=="htm") content_type="text/html";
    //     else if (suffix=="ico") content_type="image/x-icon";
    //     else if (suffix=="js" ) content_type="text";
    //   }
    //   LOG(ALML_DEBUGREGION_WEBSERVER, "&c:1&s:\tsend file from spiff:\n\t\t%s\n\t\t%s\n", filepath.c_str(), content_type.c_str());
    //   AsyncWebServerResponse *response = request->beginResponse(FILESYSTEM, filepath, content_type);
    //   if (gzip) response->addHeader("Content-Encoding", "gzip");
    //   request->send(response);
    // }


    request->send(404, "text/html", String(err));
}
// Task              * _task_socketCallback      = nullptr;
// Task              * _task_socketCleanupClient = nullptr;
// 
// 
// 

UdpMulti::UdpMulti(){
}
void UdpMulti::send(const String & transmit_buffer) {
  int size = transmit_buffer.length();
  // Serial.printf_P(PSTR("tf: %d\n"), size);

  char buf[size+1];
  sprintf(buf, "%s", 
  transmit_buffer.c_str()
  );  
  #if defined(ESP8266)
  _server.beginPacketMulticast(_multiIp, _multiPort, WiFi.localIP());
  #elif defined(ESP32)
  _server.beginMulticastPacket();
  #else
  #endif      

  _server.print(buf);
  _server.endPacket();  


} 
boolean UdpMulti::receive() {
  uint16_t packet_size = _server.parsePacket();
  if (!packet_size) return false;
  if (packet_size > UDP_IN_MAXSIZE || packet_size < 3) return false;

  if (_packetBuffer) {delete _packetBuffer;_packetBuffer = nullptr;_packetBufferSize = 0;}
  _packetBuffer     = new uint8_t[packet_size+1];
  _packetBufferSize = packet_size;

  int length = _server.read(_packetBuffer, packet_size+1);
  _packetBuffer[length]=0;

  return true;  
}
void UdpMulti::get_packetBuffer(String & result) {
  if (!_packetBuffer) return;

  for (size_t i = 0; i < _packetBufferSize; i++) {result += (char) _packetBuffer[i];}

  delete _packetBuffer;
  _packetBuffer = nullptr;
  _packetBufferSize = 0; 
}
void UdpMulti::begin(){
  #if defined(ESP8266)
    IPAddress ip(239,0,0,57);
    _server.beginMulticast(WiFi.localIP(), ip, 9200);
  #elif defined(ESP32)
    _server.beginMulticast(_multiIp, _multiPort);
  #else
  #endif    
}
void UdpMulti::stop(){
  _server.stop();  
}

UdpClients::UdpClients(IPAddress ip,uint16_t port){
  _ip   = ip;
  _port = port;
}
UdpClients::~UdpClients(){

}

Udp::Udp(){
  // _UdpPtr = this;
}

void Udp::stop(){
  _server.stop();  
}

void Udp::set_port(uint16_t port) {
  _port = port;
}

void Udp::client_add(){
  // _list.add(new QueueItem());
  uint8_t size = _list.size();
  boolean isPresent = false;
  for(uint8_t i = 0; i < size; ++i) {
    if (_list[i]->get_ip()==_clientIp) isPresent = true;
  }
  if (isPresent) return;
  _list.add(new UdpClients(_clientIp, _clientPort));
  Serial.printf_P(PSTR("udp add client:\n\tip:%s\n\tport: %d\n"), alml_webserver::ip2string(_clientIp).c_str(), _clientPort);
}

void Udp::remote_print(){
  IPAddress   ip    = _server.remoteIP();
  uint16_t  port  = _server.remotePort();

  String ip_str = alml_webserver::ip2string(ip);

  Serial.printf_P(PSTR("\n[remote_print] ipStr: %10s port: %10d\n"), ip_str.c_str(), port );
}
IPAddress Udp::remoteIP(){
  return _server.remoteIP();
}

/*
void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w)
{
  boolean isTw;
  uint16_t numLeds;
  leds_managePtrGet()->get_ledsIsTw(0, isTw);
  CRGB * ledarray;
  CRGBW * ledarray_w;
  if (isTw) ledarray_w  = leds_managePtrGet()->get_crgwbArray(0);
  else      ledarray    = leds_managePtrGet()->get_crgbArray(0);
  leds_managePtrGet()->get_numLeds(0, numLeds);


  uint16_t pix = i ;
  if (pix < numLeds)
  {
    // if (!arlsDisableGammaCorrection && strip.gammaCorrectCol)
    // {
    //   strip.setPixelColor(pix, strip.gamma8(r), strip.gamma8(g), strip.gamma8(b), strip.gamma8(w));
    // } else {
    //   strip.setPixelColor(pix, r, g, b, w);
    // }
    ledarray[pix] = CRGB(r, g, b);
  }
}
*/

bool Udp::receive() {

  uint16_t packet_size = _server.parsePacket();
  if (!packet_size) return false;
  if (packet_size > UDP_IN_MAXSIZE || packet_size < 1) return false;

  if (_packetBuffer) {delete _packetBuffer;_packetBuffer = nullptr;_packetBufferSize = 0;}
  _packetBuffer     = new uint8_t[packet_size+1];
  _packetBufferSize = packet_size;


  int length = _server.read(_packetBuffer, packet_size+1);

  _packetBuffer[length]=0;


  // String ip_str = alml_webserver::ip2string(_server.remoteIP());

  _clientIp   = _server.remoteIP();
  _clientPort = _server.remotePort();
  return true;
}
void Udp::get_packetBuffer(String & result) {
  if (!_packetBuffer) return;

  for (size_t i = 0; i < _packetBufferSize; i++) {result += (char) _packetBuffer[i];}

  delete _packetBuffer;
  _packetBuffer = nullptr;
  _packetBufferSize = 0; 
}

void Udp::send(const String & transmit_buffer) {
  int size = transmit_buffer.length();
  if (size==0)return;
  // int start, end;
  char buf[size+1]; 
  sprintf(buf,"%s",transmit_buffer.c_str());
  _server.beginPacket(_server.remoteIP(), _server.remotePort());
  _server.print(buf);     
  _server.endPacket();  
}
void Udp::send_toIp(const String & transmit_buffer) {
  int sizeMsg = transmit_buffer.length();
  if (sizeMsg==0)return;

  char buf[sizeMsg+1]; 
  sprintf(buf,"%s",transmit_buffer.c_str());

  uint8_t size = _list.size();
  for(uint8_t i = 0; i < size; ++i) {
    _server.beginPacket(_list[i]->get_ip(), _list[i]->get_port());
    _server.print(buf);     
    _server.endPacket();    
  }   
}
void Udp::send_toIp(const String & transmit_buffer, const char * ip, uint16_t port) {
  int size = transmit_buffer.length();
  if (size==0)return;
  // int start, end;
  char buf[size+1]; 
  sprintf(buf,"%s",transmit_buffer.c_str());
  _server.beginPacket(ip, port);
  _server.print(buf);     
  _server.endPacket();
// #ifdef DEBUG_UDP
//  fsprintf("\n[size] %d\n%s\n",  size, transmit_buffer.c_str());
// #endif  
}
void Udp::send_toIp(const String & transmit_buffer, IPAddress ip, uint16_t port) {
  int size = transmit_buffer.length();
  if (size==0)return;
  // int start, end;
  char buf[size+1]; 
  sprintf(buf,"%s",transmit_buffer.c_str());
  _server.beginPacket(ip, port);
  _server.print(buf);     
  _server.endPacket();
// #ifdef DEBUG_UDP
//  fsprintf("\n[size] %d\n%s\n",  size, transmit_buffer.c_str());
// #endif  
}
void Udp::send_toIp(const String & transmit_buffer, const String & s_ip, const String & s_port) {
  char buff[80];
  sprintf(buff, "%s", s_ip.c_str());
  char * ip = buff;
  uint16_t port = s_port.toInt();
  send_toIp(transmit_buffer, ip, port) ;
}

void Udp::send_toIp(const char* buffer,const char * ip, uint16_t port) {
  _server.beginPacket(ip, port);
  _server.print(buffer);     
  _server.endPacket();
}
void Udp::send_toIp(const char* buffer,const String & s_ip, const String & s_port) {
  char buff[80];
  sprintf(buff, "%s", s_ip.c_str());
  char * ip = buff;
  uint16_t port = s_port.toInt();
  send_toIp(buffer, ip, port) ;
}

void Udp::parse() { 
  // if (_parseC != nullptr) _parseC(String(_udp_packetBuffer));
}
void Udp::parse_set_func(callback_function_t f) {
  _parseC = std::move(f);;
}

void Udp::begin(){
  _server.begin(_port);
}

void Udp::loop() {
  if (receive()) parse();
}


/*

    function downloadDeleteButtonv2(action){
      var grid        = document.getElementById("mytable");
      var checkBoxes  = grid.getElementsByTagName("INPUT");
      for (var i = 0; i < checkBoxes.length; i++) {
        if (checkBoxes[i].checked) {
          var table = document.getElementById('mytable'); 
          var filename; 
          if (table.rows[i+1].cells[0].innerText == "/") {
            filename = "/" + table.rows[i+1].cells[1].innerText;
          } else {
            filename = "/" + table.rows[i+1].cells[0].innerText + "/" + table.rows[i+1].cells[1].innerText;
          }
          if (action == "delete") {
            request_send_httpX2("FileSystem_set", "name", filename, "action", action, function(e) {
              ge("FileSystem_list").innerHTML =  e;
              request_http("/FileSystem_statu", function(e) {
                ge("FileSystem_statu").innerHTML =  e;
              } );
            } );
          }
          if (action == "download") {
            var urltocall = "/FileSystem_set?name=" + filename + "&action=" + action;
            window.open(urltocall,"_blank");
          }        
          break;   
        }
      }  
*/