/*
 *  Copyright (C) 2020 Team Kodi (https://kodi.tv)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include <kodi/AddonBase.h>

class ATTR_DLL_LOCAL CSettings
{
public:
  CSettings() = default;

  bool Load();
  ADDON_STATUS SetSetting(const std::string& settingName, const std::string& settingValue);
  bool VerifySettings();

  const int& GetARDServiceProvider() const { return m_ardServiceProvider; }
  const std::string& GetARDUsername() const { return m_ardUsername; }
  const std::string& GetARDPassword() const { return m_ardPassword; }
  const std::string& GetARDAccessToken() const { return m_ardAccessToken; }
  const std::string& GetARDRefreshToken() const { return m_ardRefreshToken; }
  const std::string& GetARDDeviceNumber() const { return m_ardDeviceNumber; }
  const std::string& GetARDDeviceID() const { return m_ardDeviceID; }
  const std::string& GetARDDeviceSerial() const { return m_ardDeviceSerial; }
  const std::string& GetARDSubscriberID() const { return m_ardSubscriberID; }
  const std::string& GetARDStreamKey() const { return m_ardStreamKey; }
  const std::string& GetARDStreamUser() const { return m_ardStreamUser; }
  const std::string& GetGenericAccessToken() const { return m_Generic_AccessToken; }
  const std::string& GetSSAccessToken() const { return m_SS_AccessToken; }
  const std::string& GetSSRefreshToken() const { return m_SS_RefreshToken; }
  const std::string& GetSSIdentity() const { return m_SS_Identity; }
  const int& GetStartNum() const { return m_start_num; }
  const bool HideUnsubscribed() const { return m_hideunsubscribed; }
  const bool IsTVenabled() const { return m_enabletv; }
  const bool IsRadioenabled() const { return m_enableradio; }
  const bool IsGroupsenabled() const  { return m_enablegroups; }

private:
  int m_ardServiceProvider;
  std::string m_ardUsername;
  std::string m_ardPassword;
  std::string m_ardAccessToken;
  std::string m_ardRefreshToken;
  std::string m_ardDeviceNumber;
  std::string m_ardDeviceID;
  std::string m_ardDeviceSerial;
  std::string m_ardSubscriberID;
  std::string m_ardStreamKey;
  std::string m_ardStreamUser;
  std::string m_Generic_AccessToken;
  std::string m_SS_AccessToken;
  std::string m_SS_RefreshToken;
  std::string m_SS_Identity;
  int m_start_num;
  bool m_hideunsubscribed;
  bool m_enabletv;
  bool m_enableradio;
  bool m_enablegroups;
};
