#include "HttpClient.h"
//#include "Cache.h"
#include <random>
#include <kodi/AddonBase.h>

static const std::string USER_AGENT = std::string("Kodi/")
    + std::string(STR(KODI_VERSION)) + std::string(" pvr.ard/")
    + std::string(STR(ARD_VERSION));

HttpClient::HttpClient()
{
  kodi::Log(ADDON_LOG_INFO, "Using useragent: %s", USER_AGENT.c_str());

}

HttpClient::~HttpClient()
{

}

void HttpClient::ClearSession() {
}

std::string HttpClient::GetUUID()
{
  if (!m_uuid.empty())
  {
    return m_uuid;
  }

  m_uuid = GenerateUUID();
  return m_uuid;
}

std::string HttpClient::GenerateUUID()
{
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "-";

    srand( (unsigned) time(NULL));

    for (int i = 0; i < 21; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}
/*
std::string HttpClient::HttpGetCached(const std::string& url, time_t cacheDuration, int &statusCode)
{

  std::string content;
  std::string cacheKey = md5(url);
  statusCode = 200;
  if (!Cache::Read(cacheKey, content))
  {
    content = HttpGet(url, statusCode);
    if (!content.empty())
    {
      time_t validUntil;
      time(&validUntil);
      validUntil += cacheDuration;
      Cache::Write(cacheKey, content, validUntil);
    }
  }
  return content;
}
*/
std::string HttpClient::HttpGet(const std::string& url, int &statusCode)
{
  return HttpRequest("GET", url, "", statusCode);
}

std::string HttpClient::HttpDelete(const std::string& url, int &statusCode)
{
  return HttpRequest("DELETE", url, "", statusCode);
}

std::string HttpClient::HttpPost(const std::string& url, const std::string& postData, int &statusCode)
{
  return HttpRequest("POST", url, postData, statusCode);
}

std::string HttpClient::HttpRequest(const std::string& action, const std::string& url, const std::string& postData, int &statusCode)
{
  Curl curl;

  curl.AddOption("acceptencoding", "gzip,deflate");

  curl.AddHeader("User-Agent", USER_AGENT);

  std::string content = HttpRequestToCurl(curl, action, url, postData, statusCode);

  if (statusCode >= 400 || statusCode < 200) {
    kodi::Log(ADDON_LOG_ERROR, "Open URL failed with %i.", statusCode);
    if (m_statusCodeHandler != nullptr) {
      m_statusCodeHandler->ErrorStatusCode(statusCode);
    }
    return "";
  }
  return content;
}

std::string HttpClient::HttpRequestToCurl(Curl &curl, const std::string& action,
    const std::string& url, const std::string& postData, int &statusCode)
{
  kodi::Log(ADDON_LOG_DEBUG, "Http-Request: %s %s.", action.c_str(), url.c_str());
  std::string content;
  if (action == "POST")
  {
    content = curl.Post(url, postData, statusCode);
  }
  else if (action == "DELETE")
  {
    content = curl.Delete(url, statusCode);
  }
  else
  {
    content = curl.Get(url, statusCode);
  }
  return content;

}
