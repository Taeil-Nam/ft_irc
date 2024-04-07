/**
 * @file Channel.hpp
 * @author Taeil-Nam (nam0314@gmail.com)
 * @brief IRC Channel 클래스
 * @version 0.1
 * @date 2024-03-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "../../common.hpp"
#include "User.hpp"

namespace grc
{

class Channel
{
public:
    enum { UNLIMIT = 0 };

public:
    Channel();
    //const Channel& operator=(const Channel& IN channel);
    virtual ~Channel();

    bool IsUserExist(const std::string& IN nickname) const;
    bool IsOperator(const std::string& IN nickname) const;
    bool IsProtectedTopic() const;
    bool IsInviteOnly() const;
    bool IsKeyRequired() const;
    bool IsLimitedMaxUserCount() const;
    bool IsChannelEmpty() const;

    const std::string& GetName() const;
    const std::string& GetTopic() const;
    uint32 GetMaxUserCount() const;
    const std::string& GetKey() const;
    uint32 GetCurrentUserCount() const;
    const std::map<std::string, User>& GetUsers() const;
    const std::map<std::string, User>& GetOperators() const;
    std::string GetAllUsersNickname() const;
    std::string GetModeString() const;
    std::string GetModeArgument() const;

    void SetName(const std::string& IN channelName);
    void SetTopic(const std::string& IN topic);
    void SetMaxUserCount(uint32 IN maxUserCount);
    void SetKey(const std::string& IN key);
    void SetProtectedTopic();
    void SetInviteOnly();
    void SetLimitedMaxUserCount();
    void SetKeyRequired();

    void UnsetProtectedTopic();
    void UnsetInviteOnly();
    void UnsetLimitedMaxUserCount();
    void UnsetKeyRequired();

    void AddUser(const std::string& IN nickname, const User& IN user);
    void AddOperator(const std::string& IN nickname, const User& IN user);
    void DeleteTopic();
    void DeleteUser(const std::string& IN nickname);
    void DeleteOperator(const std::string& IN nickname);
private:
    Channel(const Channel& IN channel); // = delete
    const Channel& operator=(const Channel& IN channel); // = delete

private:
    std::string mName;
    std::string mTopic;
    uint32 mMaxUserCount;
    std::string mKey;
    std::map<std::string, User> mUsers;
    std::map<std::string, User> mOperators;
    bool mbIsProtectedTopic;
    bool mbIsInviteOnly;
    bool mbIsLimitedMaxUserCount;
    bool mbIsKeyRequired;
};

}
