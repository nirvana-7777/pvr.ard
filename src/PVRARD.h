/*
 *  Copyright (C) 2011-2021 Team Kodi (https://kodi.tv)
 *  Copyright (C) 2011 Pulse-Eight (http://www.pulse-eight.com/)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include <string>
#include <vector>

#include <kodi/addon-instance/PVR.h>
#include "Settings.h"
#include "http/HttpClient.h"
#include "rapidjson/document.h"

static const std::string PROGRAM_API_HOST = "https://programm-api.ard.de/nownext/api/";
static const std::string API_MEDIATHEK = "https://api.ardmediathek.de/page-gateway/pages/";

std::vector<std::string> ard_channels { "Y3JpZDovL2Rhc2Vyc3RlLmRlL2xpdmUvY2xpcC9hYmNhMDdhMy0zNDc2LTQ4NTEtYjE2Mi1mZGU4ZjY0NmQ0YzQ", "daserste",
                                        "Y3JpZDovL2JyLmRlL0xpdmVzdHJlYW0tQlItU8O8ZA", "br",
                                        "Y3JpZDovL2hyLmRlL0xpdmVzdHJlYW0tSFI", "hr",
                                        "Y3JpZDovL21kci5kZS9MaXZlc3RyZWFtLU1EUi1TYWNoc2Vu", "mdr",
                                        "Y3JpZDovL25kci5kZS9MaXZlc3RyZWFtLU5EUi1OaWVkZXJzYWNoc2Vu", "ndr",
                                        "Y3JpZDovL3JhZGlvYnJlbWVuLmRlL3JhZGlvX2JyZW1lbl9saXZlc3RyZWFt", "radiobremen",
                                        "Y3JpZDovL3JiYi1vbmxpbmUuZGUvcmJiZmVybnNlaGVuL2xpdmVfYnJhbmRlbmJ1cmcvc2VuZGVwbGF0ei0tLWxpdmVzdHJlYW0tLS1icmFuZGVuYnVyZy0tLWhsczE", "rbb",
                                        "Y3JpZDovL3NyLW9ubGluZS5kZS8yODQ4NjAvbGl2ZXN0cmVhbQ", "sr",
                                        "Y3JpZDovL3N3ci5kZS8xMzQ4MTA0Mg", "swr",
                                        "Y3JpZDovL3dkci5kZS9CZWl0cmFnLTNkYTY2NGRlLTE4YzItNDY1MC1hNGZmLTRmNjQxNDcyMDcyYg", "wdr",
                                        "Y3JpZDovL3dkci5kZS9CZWl0cmFnLTFlNjA0YWFlLTViODctNGMzNC04ZDhmLTg4OWI1ZjE2ZDU3Mw", "one",
                                        "Y3JpZDovL2FydGUuZGUvTGl2ZXN0cmVhbS1BUlRF", "arte",
                                        "Y3JpZDovL2tpa2EuZGUvTGl2ZXN0cmVhbS1LaUth", "kika",
                                        "Y3JpZDovLzNzYXQuZGUvTGl2ZXN0cmVhbS0zc2F0", "3sat",
                                        "Y3JpZDovL2JyLmRlL0xpdmVzdHJlYW0tQVJELUFscGhh", "alpha",
                                        "Y3JpZDovL2Rhc2Vyc3RlLmRlL3RhZ2Vzc2NoYXUvbGl2ZXN0cmVhbQ", "tagesschau24",
                                        "Y3JpZDovL3dkci5kZS9CZWl0cmFnLTE3YTg4ZDdmLWI5NTAtNDcyNy05M2E0LWE3NzI3YjkxNjVkZQ", "phoenix",
                                        "Y3JpZDovL2RldXRzY2hld2VsbGUuZGUvTGl2ZXN0cmVhbS1EZXV0c2NoZVdlbGxl", "dw"
                                      };

struct ARDChannel
{
//  bool bRadio;
//  bool bArchive;
  int iUniqueId;
  int iChannelNumber; //position
  std::string strChannelName;
  std::string strMediathekName;
  std::string crid;
  std::string page;
  std::string strIconPath;
//  std::string strStreamURL;
};

struct ARDEvent
{
};

class ATTR_DLL_LOCAL CPVRARD : public kodi::addon::CAddonBase,
                                public kodi::addon::CInstancePVRClient
{
public:
  CPVRARD();
  ~CPVRARD() override;

  PVR_ERROR GetBackendName(std::string& name) override;
  PVR_ERROR GetBackendVersion(std::string& version) override;
  PVR_ERROR GetConnectionString(std::string& connection) override;
  PVR_ERROR GetBackendHostname(std::string& hostname) override;

  PVR_ERROR GetCapabilities(kodi::addon::PVRCapabilities& capabilities) override;
  PVR_ERROR GetDriveSpace(uint64_t& total, uint64_t& used) override;

  PVR_ERROR CallEPGMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                            const kodi::addon::PVREPGTag& item) override;
  PVR_ERROR CallChannelMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                                const kodi::addon::PVRChannel& item) override;
  PVR_ERROR CallTimerMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                              const kodi::addon::PVRTimer& item) override;
  PVR_ERROR CallRecordingMenuHook(const kodi::addon::PVRMenuhook& menuhook,
                                  const kodi::addon::PVRRecording& item) override;
  PVR_ERROR CallSettingsMenuHook(const kodi::addon::PVRMenuhook& menuhook) override;

  PVR_ERROR GetEPGForChannel(int channelUid,
                             time_t start,
                             time_t end,
                             kodi::addon::PVREPGTagsResultSet& results) override;
  PVR_ERROR IsEPGTagPlayable(const kodi::addon::PVREPGTag& tag, bool& bIsPlayable) override;
  PVR_ERROR GetEPGTagStreamProperties(
      const kodi::addon::PVREPGTag& tag,
      std::vector<kodi::addon::PVRStreamProperty>& properties) override;
  PVR_ERROR GetProvidersAmount(int& amount) override;
  PVR_ERROR GetProviders(kodi::addon::PVRProvidersResultSet& results) override;
  PVR_ERROR GetChannelGroupsAmount(int& amount) override;
  PVR_ERROR GetChannelGroups(bool bRadio, kodi::addon::PVRChannelGroupsResultSet& results) override;
  PVR_ERROR GetChannelGroupMembers(const kodi::addon::PVRChannelGroup& group,
                                   kodi::addon::PVRChannelGroupMembersResultSet& results) override;
  PVR_ERROR GetChannelsAmount(int& amount) override;
  PVR_ERROR GetChannels(bool bRadio, kodi::addon::PVRChannelsResultSet& results) override;
  PVR_ERROR GetRecordingsAmount(bool deleted, int& amount) override;
  PVR_ERROR GetRecordings(bool deleted, kodi::addon::PVRRecordingsResultSet& results) override;
  PVR_ERROR GetTimerTypes(std::vector<kodi::addon::PVRTimerType>& types) override;
  PVR_ERROR GetTimersAmount(int& amount) override;
  PVR_ERROR GetTimers(kodi::addon::PVRTimersResultSet& results) override;
  PVR_ERROR GetSignalStatus(int channelUid, kodi::addon::PVRSignalStatus& signalStatus) override;
  PVR_ERROR GetChannelStreamProperties(
      const kodi::addon::PVRChannel& channel,
      std::vector<kodi::addon::PVRStreamProperty>& properties) override;
  PVR_ERROR GetRecordingStreamProperties(
      const kodi::addon::PVRRecording& recording,
      std::vector<kodi::addon::PVRStreamProperty>& properties) override;

  ADDON_STATUS SetSetting(const std::string& settingName,
                        const std::string& settingValue);

protected:
  std::string GetRecordingURL(const kodi::addon::PVRRecording& recording);
  bool GetChannel(const kodi::addon::PVRChannel& channel, ARDChannel& myChannel);

private:
  PVR_ERROR CallMenuHook(const kodi::addon::PVRMenuhook& menuhook);

  void SetStreamProperties(std::vector<kodi::addon::PVRStreamProperty>& properties,
                           const std::string& url,
                           bool realtime, bool playTimeshiftBuffer,
                           const std::string& license);

 PVR_ERROR GetStreamProperties(
   const ARDChannel& channel,
   std::vector<kodi::addon::PVRStreamProperty>& properties, int starttime, bool isLive);

  std::vector<ARDChannel> m_channels;

  HttpClient *m_httpClient;
  CSettings* m_settings;

  bool LoadChannels();
  std::string GetStreamingUrl(const std::string& id, const std::string& name);
};
