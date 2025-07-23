// smtpmanager.h
//
// Copyright (c) 2019-2021 Kristofer Berggren
// All rights reserved.
//
// falaclient is distributed under the MIT license, see LICENSE for details.

#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <unistd.h>

#include "contact.h"
#include "log.h"
#include "smtp.h"
#include "status.h"

class SmtpManager
{
public:
  struct Action
  {
    bool m_IsSendMessage = false;
    bool m_IsCreateMessage = false;
    bool m_IsSendCreatedMessage = false;
    std::string m_From;
    std::string m_To;
    std::string m_Cc;
    std::string m_Bcc;
    std::string m_Att;
    std::string m_Subject;
    std::string m_Body;
    std::string m_HtmlBody;
    std::string m_RefMsgId;
    std::string m_ComposeTempDirectory;
    uint32_t m_ComposeDraftUid = 0;
    std::string m_CreatedMsg;
    bool m_FormatFlowed = false;
  };

  struct Result
  {
    SmtpStatus m_SmtpStatus = SmtpStatusFailed;
    std::string m_Message;
    Action m_Action;
  };

public:
  SmtpManager(const std::string& p_User, const std::string& p_Pass, const std::string& p_Host,
              const uint16_t p_Port, const std::string& p_Name, const std::string& p_Address,
              const bool p_Connect, const int64_t p_Timeout,
              const std::function<void(const SmtpManager::Result&)>& p_ResultHandler,
              const std::function<void(const StatusUpdate&)>& p_StatusHandler);
  virtual ~SmtpManager();

  void Start();
  void AsyncAction(const Action& p_Action);
  Result SyncAction(const Action& p_Action);
  std::string GetAddress();

private:
  void Process();
  Result PerformAction(const Action& p_Action);
  void SetStatus(uint32_t p_Flags);
  void ClearStatus(uint32_t p_Flags);

private:
  std::string m_User;
  std::string m_Pass;
  std::string m_Host;
  uint16_t m_Port = 0;
  std::string m_Name;
  std::string m_Address;
  bool m_Connect = false;
  int64_t m_Timeout = 0;
  std::function<void(const SmtpManager::Result&)> m_ResultHandler;
  std::function<void(const StatusUpdate&)> m_StatusHandler;
  std::atomic<bool> m_Running;
  std::thread m_Thread;

  std::condition_variable m_ExitedCond;
  std::mutex m_ExitedCondMutex;

  std::deque<Action> m_Actions;
  std::mutex m_QueueMutex;

  int m_Pipe[2] = { -1, -1 };
};
