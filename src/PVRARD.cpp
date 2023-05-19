/*
 *  Copyright (C) 2011-2021 Team Kodi (https://kodi.tv)
 *  Copyright (C) 2011 Pulse-Eight (http://www.pulse-eight.com/)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "PVRARD.h"
#include "Globals.h"

#include <algorithm>

#include <kodi/General.h>
#include "Utils.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

/***********************************************************
  * PVR Client AddOn specific public library functions
  ***********************************************************/

std::string CPVRARD::GetStreamingUrl(const std::string& id, const std::string& name)
{
  std::string jsonString;
  int statusCode = 0;

  jsonString = m_httpClient->HttpGet(API_MEDIATHEK + name + "/item/" + id + "?embedded=true&mcV6=true", statusCode);

  rapidjson::Document doc;
  doc.Parse(jsonString.c_str());
  if (doc.GetParseError())
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to get channelinfo");
    return "";
  }
  const rapidjson::Value& widgets = doc["widgets"];

  for (rapidjson::Value::ConstValueIterator itr1 = widgets.Begin();
      itr1 != widgets.End(); ++itr1)
  {
    const rapidjson::Value& widgetItem = (*itr1);
    const rapidjson::Value& streams = widgetItem["mediaCollection"]["embedded"]["streams"];
    for (rapidjson::Value::ConstValueIterator itr2 = streams.Begin();
        itr2 != streams.End(); ++itr2)
    {
      const rapidjson::Value& streamItem = (*itr2);
      const rapidjson::Value& media = streamItem["media"];
      for (rapidjson::Value::ConstValueIterator itr3 = media.Begin();
          itr3 != media.End(); ++itr3)
      {
        const rapidjson::Value& mediaItem = (*itr3);
        std::string url = Utils::JsonStringOrEmpty(mediaItem, "url");
        kodi::Log(ADDON_LOG_DEBUG, "URL: %s", url.c_str());
        return url;
      }
    }
  }

  return "";
}

CPVRARD::CPVRARD() :
  m_settings(new CSettings())
{
  m_settings->Load();
  m_httpClient = new HttpClient();

  LoadChannels();
}

CPVRARD::~CPVRARD()
{
  m_channels.clear();
}

ADDON_STATUS CPVRARD::SetSetting(const std::string& settingName, const std::string& settingValue)
{
  ADDON_STATUS result = m_settings->SetSetting(settingName, settingValue);
  if (!m_settings->VerifySettings()) {
    return ADDON_STATUS_NEED_SETTINGS;
  }
  return result;
}

bool CPVRARD::LoadChannels()
{
  kodi::Log(ADDON_LOG_DEBUG, "Load ARD Channels");
  std::string jsonString;
  int statusCode = 0;

  jsonString = m_httpClient->HttpGet(PROGRAM_API_HOST + "now", statusCode);

  rapidjson::Document doc;
  doc.Parse(jsonString.c_str());
  if (doc.GetParseError())
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to load channels");
    return false;
  }

  int currentnumber = m_settings->GetStartNum()-1;
  std::string channame;
  int ref_id = 0;
  const rapidjson::Value& channels = doc["channels"];

  for (int i = 0; i < ard_channels.size()/2; i++)
  {
  //  const rapidjson::Value& channelItem = Utils::JsonStringOrEmpty(channels, ard_channels[i].c_str());
    const rapidjson::Value& channelItem = channels[ard_channels[i*2].c_str()];

    ARDChannel ard_channel;

    channame = Utils::JsonStringOrEmpty(channelItem, "name");
    int dvbid = Utils::JsonIntOrZero(channelItem, "dvbId");
    if (dvbid == 0) {
      ref_id = stoi(Utils::JsonStringOrEmpty(channelItem, "dvbId"));
    } else {
      ref_id = dvbid;
    }
    ard_channel.strChannelName = channame;
    ard_channel.iChannelNumber = ++currentnumber;
    ard_channel.iUniqueId = ref_id;
    ard_channel.crid = Utils::JsonStringOrEmpty(channelItem, "id");
    ard_channel.strMediathekName = Utils::JsonStringOrEmpty(channelItem, "mediathekName");
    ard_channel.page = ard_channels[i*2+1];
    ard_channel.strIconPath = ClientPath() + "resources/images/tv-" + ard_channels[i*2+1] + ".png";

    kodi::Log(ADDON_LOG_DEBUG, "%i. Channel Name: %s ID: %i", currentnumber, channame.c_str(), ref_id);
    m_channels.emplace_back(ard_channel);
  }

  return true;
}

void CPVRARD::SetStreamProperties(std::vector<kodi::addon::PVRStreamProperty>& properties,
                                    const std::string& url,
                                    bool realtime, bool playTimeshiftBuffer,
                                    const std::string& license)
{
  kodi::Log(ADDON_LOG_DEBUG, "[PLAY STREAM] url: %s", url.c_str());

  properties.emplace_back(PVR_STREAM_PROPERTY_STREAMURL, url);
  properties.emplace_back(PVR_STREAM_PROPERTY_INPUTSTREAM, "inputstream.adaptive");
  properties.emplace_back(PVR_STREAM_PROPERTY_ISREALTIMESTREAM, realtime ? "true" : "false");

  kodi::Log(ADDON_LOG_DEBUG, "[PLAY STREAM] hls");
  properties.emplace_back("inputstream.adaptive.manifest_type", "hls");
  properties.emplace_back(PVR_STREAM_PROPERTY_MIMETYPE, "application/x-mpegURL");
  properties.emplace_back("inputstream.adaptive.stream_selection_type", "adaptive");
  properties.emplace_back("inputstream.adaptive.play_timeshift_buffer", playTimeshiftBuffer ? "true" : "false");

  properties.emplace_back("inputstream.adaptive.manifest_update_parameter", "full");
}


PVR_ERROR CPVRARD::GetCapabilities(kodi::addon::PVRCapabilities& capabilities)
{
  capabilities.SetSupportsEPG(true);
  capabilities.SetSupportsTV(true);
  capabilities.SetSupportsRadio(false);
  capabilities.SetSupportsChannelGroups(false);
  capabilities.SetSupportsRecordings(false);
  capabilities.SetSupportsRecordingsDelete(false);
  capabilities.SetSupportsRecordingsUndelete(false);
  capabilities.SetSupportsTimers(false);
  capabilities.SetSupportsRecordingsRename(false);
  capabilities.SetSupportsRecordingsLifetimeChange(false);
  capabilities.SetSupportsDescrambleInfo(false);
  capabilities.SetSupportsProviders(false);

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetBackendName(std::string& name)
{
  name = "ard pvr add-on";
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetBackendVersion(std::string& version)
{
  version = "0.1";
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetConnectionString(std::string& connection)
{
  connection = "connected";
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetBackendHostname(std::string& hostname)
{
  hostname = "";
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetDriveSpace(uint64_t& total, uint64_t& used)
{
  total = 1024 * 1024 * 1024;
  used = 0;
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetEPGForChannel(int channelUid,
                                     time_t start,
                                     time_t end,
                                     kodi::addon::PVREPGTagsResultSet& results)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  for (const auto& channel : m_channels)
  {

    if (channel.iUniqueId != channelUid)
      continue;


    std::string jsonEpg;
    int statusCode = 0;

    kodi::Log(ADDON_LOG_DEBUG, "EPG Request for Channel %u Start %u End %u", channel.iUniqueId, start, end);

    jsonEpg = m_httpClient->HttpGet(PROGRAM_API_HOST + "channel?channel=" + channel.crid + "&pastHours=5&futureEvents=1", statusCode);

    rapidjson::Document epgDoc;
    epgDoc.Parse(jsonEpg.c_str());
    if (epgDoc.GetParseError())
    {
      kodi::Log(ADDON_LOG_ERROR, "[GetEPG] ERROR: error while parsing json");
      return PVR_ERROR_SERVER_ERROR;
    }
    const rapidjson::Value& epgitems = epgDoc["events"];
    for (rapidjson::Value::ConstValueIterator itr1 = epgitems.Begin();
      itr1 != epgitems.End(); ++itr1)
    {
      const rapidjson::Value& epgItem = (*itr1);

      kodi::addon::PVREPGTag tag;

      tag.SetUniqueBroadcastId(Utils::JsonIntOrZero(epgItem,"id"));
      tag.SetUniqueChannelId(channelUid);
      if (epgItem.HasMember("title"))
      {
        const rapidjson::Value& title = epgItem["title"];
        kodi::Log(ADDON_LOG_ERROR, "[GetEPG] Title %s", Utils::JsonStringOrEmpty(title,"short").c_str());
        tag.SetTitle(Utils::JsonStringOrEmpty(title,"short"));
      }
      time_t startdate = Utils::StringToTime(Utils::JsonStringOrEmpty(epgItem,"startDate"));
      time_t enddate = Utils::StringToTime(Utils::JsonStringOrEmpty(epgItem,"endDate"));
      tag.SetStartTime((int) startdate);
      tag.SetEndTime((int) enddate);
      tag.SetPlot(Utils::JsonStringOrEmpty(epgItem,"synopsis"));
      if (epgItem.HasMember("image"))
      {
        const rapidjson::Value& image = epgItem["image"];
        std::string imageurl = Utils::JsonStringOrEmpty(image, "contentUrl");
        std::string toReplace("{width}");
        size_t pos = imageurl.find(toReplace);
        if (pos != std::string::npos) {
          imageurl.replace(pos, toReplace.length(), "720");
        }
        kodi::Log(ADDON_LOG_ERROR, "[GetEPG] Image URL %s", imageurl.c_str());
        tag.SetIconPath(imageurl);
      }

      if (startdate > start && enddate < end)
      {
        results.Add(tag);
      }
    }
  }

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::IsEPGTagPlayable(const kodi::addon::PVREPGTag& tag, bool& bIsPlayable)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  bIsPlayable = false;

  for (const auto& channel : m_channels)
  {
    if (channel.iUniqueId == tag.GetUniqueChannelId())
    {
      auto current_time = time(NULL);
      if (current_time > tag.GetStartTime())
      {
        bIsPlayable = true;
      }
    }
  }

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetEPGTagStreamProperties(
    const kodi::addon::PVREPGTag& tag, std::vector<kodi::addon::PVRStreamProperty>& properties)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  for (const auto& channel : m_channels)
  {
    if (channel.iUniqueId == tag.GetUniqueChannelId())
    {
      return GetStreamProperties(channel, properties, tag.GetStartTime(), false);
    }
  }
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetProvidersAmount(int& amount)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetProviders(kodi::addon::PVRProvidersResultSet& results)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetChannelsAmount(int& amount)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  amount = m_channels.size();
  std::string amount_str = std::to_string(amount);
  kodi::Log(ADDON_LOG_DEBUG, "Channels Amount: [%s]", amount_str.c_str());
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetChannels(bool bRadio, kodi::addon::PVRChannelsResultSet& results)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  for (const auto& channel : m_channels)
  {

    if (!bRadio)
    {
      kodi::addon::PVRChannel kodiChannel;

      kodiChannel.SetUniqueId(channel.iUniqueId);
      kodiChannel.SetIsRadio(false);
      kodiChannel.SetChannelNumber(channel.iChannelNumber);
//      kodiChannel.SetSubChannelNumber(channel.iSubChannelNumber);
      kodiChannel.SetChannelName(channel.strChannelName);
//      kodiChannel.SetEncryptionSystem(channel.iEncryptionSystem);
      kodiChannel.SetIconPath(channel.strIconPath);
      kodiChannel.SetIsHidden(false);
      kodiChannel.SetHasArchive(true);

//       PVR API 8.0.0
//      kodiChannel.SetClientProviderUid(channel.iProviderId);

      results.Add(kodiChannel);
    }
  }

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetStreamProperties(
    const ARDChannel& channel, std::vector<kodi::addon::PVRStreamProperty>& properties, int starttime, bool isLive)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetChannelStreamProperties(
    const kodi::addon::PVRChannel& channel, std::vector<kodi::addon::PVRStreamProperty>& properties)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  ARDChannel addonChannel;
  GetChannel(channel, addonChannel);

  std::string url = GetStreamingUrl(addonChannel.crid, addonChannel.page);

  SetStreamProperties(properties, url, true, false, "");

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetChannelGroupsAmount(int& amount)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);
  amount = 0;
  std::string amount_str = std::to_string(amount);
  kodi::Log(ADDON_LOG_DEBUG, "Groups Amount: [%s]", amount_str.c_str());

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetChannelGroups(bool bRadio, kodi::addon::PVRChannelGroupsResultSet& results)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetChannelGroupMembers(const kodi::addon::PVRChannelGroup& group,
                                           kodi::addon::PVRChannelGroupMembersResultSet& results)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetSignalStatus(int channelUid, kodi::addon::PVRSignalStatus& signalStatus)
{
  signalStatus.SetAdapterName("pvr ard backend");
  signalStatus.SetAdapterStatus("OK");

  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetRecordingsAmount(bool deleted, int& amount)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetRecordings(bool deleted, kodi::addon::PVRRecordingsResultSet& results)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetRecordingStreamProperties(
    const kodi::addon::PVRRecording& recording,
    std::vector<kodi::addon::PVRStreamProperty>& properties)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetTimerTypes(std::vector<kodi::addon::PVRTimerType>& types)
{
  /* TODO: Implement this to get support for the timer features introduced with PVR API 1.9.7 */
  return PVR_ERROR_NOT_IMPLEMENTED;
}

PVR_ERROR CPVRARD::GetTimersAmount(int& amount)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::GetTimers(kodi::addon::PVRTimersResultSet& results)
{
  return PVR_ERROR_NO_ERROR;
}

PVR_ERROR CPVRARD::CallEPGMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                                    const kodi::addon::PVREPGTag& item)
{
  kodi::Log(ADDON_LOG_DEBUG, "function call: [%s]", __FUNCTION__);


  return CallMenuHook(menuhook);
}

PVR_ERROR CPVRARD::CallChannelMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                                        const kodi::addon::PVRChannel& item)
{
  return CallMenuHook(menuhook);
}

PVR_ERROR CPVRARD::CallTimerMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                                      const kodi::addon::PVRTimer& item)
{
  return CallMenuHook(menuhook);
}

PVR_ERROR CPVRARD::CallRecordingMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                                          const kodi::addon::PVRRecording& item)
{
  return CallMenuHook(menuhook);
}

PVR_ERROR CPVRARD::CallSettingsMenuHook(const kodi::addon::PVRMenuhook& menuhook)
{
  return CallMenuHook(menuhook);
}

PVR_ERROR CPVRARD::CallMenuHook(const kodi::addon::PVRMenuhook& menuhook)
{
  return PVR_ERROR_NO_ERROR;
}

bool CPVRARD::GetChannel(const kodi::addon::PVRChannel& channel, ARDChannel& myChannel)
{
  for (const auto& thisChannel : m_channels)
  {

    if (thisChannel.iUniqueId == (int)channel.GetUniqueId())
    {
      myChannel.iUniqueId = thisChannel.iUniqueId;
      myChannel.iChannelNumber = thisChannel.iChannelNumber;
      myChannel.strChannelName = thisChannel.strChannelName;
      myChannel.strMediathekName = thisChannel.strMediathekName;
      myChannel.crid = thisChannel.crid;
      myChannel.page = thisChannel.page;
      myChannel.strIconPath = thisChannel.strIconPath;

      return true;
    }
  }

  return false;
}

std::string CPVRARD::GetRecordingURL(const kodi::addon::PVRRecording& recording)
{
  return "";
}

ADDONCREATOR(CPVRARD)
