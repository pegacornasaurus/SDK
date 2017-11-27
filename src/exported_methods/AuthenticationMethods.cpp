#include "exported_methods/AuthenticationMethods.h"

typedef void (*ScriptFunction)(void);

struct EmailRequestParams
{
  void* object;
  void (*callback)(void* object, ModioResponse response, char* message);
};

struct EmailExchangeParams
{
  void* object;
  void (*callback)(void* object, ModioResponse response);
};

std::map< int,EmailRequestParams* > email_request_params;
std::map< int,EmailExchangeParams* > email_exchange_params;

void onEmailRequested(int call_number, int response_code, json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;

  std::string message_str = "";
  if(modio::hasKey(response_json,"message"))
    message_str = response_json["message"];

  char* message = new char[message_str.size() + 1];
  strcpy(message, message_str.c_str());
  email_request_params[call_number]->callback(email_request_params[call_number]->object, response, message);
  email_request_params.erase(call_number);
  delete email_request_params[call_number];
}

void modioEmailRequest(void* object, char* email, void (*callback)(void* object, ModioResponse response, char* message))
{
  std::map<std::string, std::string> data;
  data["api_key"] = modio::API_KEY;
  data["email"] = email;

  std::vector<std::string> headers;

  int call_number = modio::curlwrapper::getCallCount();
  modio::curlwrapper::advanceCallCount();

  email_request_params[call_number] = new EmailRequestParams;
  email_request_params[call_number]->callback = callback;
  email_request_params[call_number]->object = object;

  std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "oauth/emailrequest";
  url += "?api_key=" + modio::API_KEY;
  url += "&email=" + std::string(email);

  modio::curlwrapper::post(call_number, url, headers, data, &onEmailRequested);
}

void onEmailExchanged(int call_number, int response_code, json response_json)
{
  ModioResponse response;
  modioInitResponse(&response, response_json);
  response.code = response_code;
  if(response.code == 200)
  {
    modio::ACCESS_TOKEN = response_json["access_token"];
    json token_json;
    token_json["access_token"] = response_json["access_token"];
    std::ofstream out(modio::getModIODirectory() + "token.json");
    out<<std::setw(4)<<token_json<<std::endl;
    out.close();
  }
  email_exchange_params[call_number]->callback(email_exchange_params[call_number]->object, response);
  email_exchange_params.erase(call_number);
}

void modioEmailExchange(void* object, char* security_code, void (*callback)(void* object, ModioResponse response))
{
  std::map<std::string, std::string> data;
  std::vector<std::string> headers;

  int call_number = modio::curlwrapper::getCallCount();
  modio::curlwrapper::advanceCallCount();
  email_exchange_params[call_number] = new EmailExchangeParams;
  email_exchange_params[call_number]->callback = callback;
  email_exchange_params[call_number]->object = object;
  std::string url = modio::MODIO_URL + modio::MODIO_VERSION_PATH + "oauth/emailexchange";
  url += "?api_key=" + modio::API_KEY;
  url += "&security_code=" + std::string(security_code);
  modio::curlwrapper::post(call_number, url, headers, data, &onEmailExchanged);
}

bool modioIsLoggedIn()
{
  return modio::ACCESS_TOKEN!="";
}

void modioLogout()
{
  modio::ACCESS_TOKEN = "";

  json empty_json;
  std::ofstream out(modio::getModIODirectory() + "token.json");
  out<<std::setw(4)<<empty_json<<std::endl;
  out.close();
}
