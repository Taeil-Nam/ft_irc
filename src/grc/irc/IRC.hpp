/**
 * @file IRC.hpp
 * @author Taeil-Nam (nam0314@gmail.com)
 * @brief IRC 로직을 처리하는 정적 클래스이다.
 * @version 0.1
 * @date 2024-03-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "common.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"

using namespace gdf;

/**
 * @brief 
 * IRC 서버의 버전을 나타내는 매크로이다.
 */
#define IRC_VERSION "v0.1"

/**
 * @brief Carriage Return Line Feed(CRLF)를 나타내는 매크로이다.
 */
#define CRLF "\r\n"

////////////////////////////////////////////////////////////
// Error Replies
////////////////////////////////////////////////////////////

/**
 * @brief 해당 nick을 가진 유저가 없는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> <nick> :No such nick/channel"
 */
#define ERR_NOSUCHNICK "401"

/**
 * @brief 해당 channel name을 가진 채널이 없는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<channel name> :No such channel"
 */
#define ERR_NOSUCHCHANNEL "403"

/**
 * @brief PRIVMSG에 받는 사람이 없는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> :No recipient given (<command>)"
 */
#define ERR_NORECIPIENT "411"

/**
 * @brief PRIVMSG에 전달할 메시지 내용이 없는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> :No text to send"
 */
#define ERR_NOTEXTTOSEND "412"

/**
 * @brief PRIVMSG / NOTICE 메시지가 주어진 채널에 전달될 수 없을 때 응답하는 에러 코드이다.
 * 유저가 채널에 메시지를 보낼 수 있는 권한이 없거나, 채널에 입장되어 있는 상태가 아닌 경우에 해당된다.
 *
 * 메시지 형식: "<client> <channel> :Cannot send to channel"
 */
#define ERR_CANNOTSENDTOCHAN "404"

/**
 * @brief 서버의 Message of the Day(MOTD) 파일이 존재하지 않는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> :MOTD File is missing"
 */
#define ERR_NOMOTD "422"

/**
 * @brief nick이 꼭 필요한 메시지에 nick이 없는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: ":No nick given"
 */
#define ERR_NONICKNAMEGIVEN "431"

/**
 * @brief NICK 메시지의 nick에 사용할 수 없는 문자가 들어있는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<nick> :Erroneus nick"
 */
#define ERR_ERRONEUSNICKNAME "432"

/**
 * @brief NICK 메시지의 nick을 이미 다른 유저가 사용하고 있는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<nick> :nick is already in use"
 */
#define ERR_NICKNAMEINUSE "433"

/**
 * @brief channel + nick의 조합을 사용하는 메시지에서, 해당 nick이 채널에 없는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> <nick> <channel> :They aren't on that channel"
 */
#define ERR_USERNOTINCHANNEL "441"

/**
 * @brief user가 속하지 않은 채널에 특정 메시지를 전달하려고 하는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> <channel> :You're not on that channel"
 */
#define ERR_NOTONCHANNEL  "442"

/**
 * @brief 채널에 이미 있는 nick을 초대했을 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> <nick> <channel> :is already on channel"
 */
#define ERR_USERONCHANNEL "443"

/**
 * @brief 메시지에 필요한 매개변수가 부족한 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<command> :Not enough parameters"
 */
#define ERR_NEEDMOREPARAMS "461"

/**
 * @brief 연결 등록이 되지 않은 user로부터 메시지를 받은 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: ":You may not reregister"
 */
#define ERR_ALREADYREGISTERED "462"

/**
 * @brief 연결 등록을 위한 password가 다른 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: ":Password incorrect"
 */
#define ERR_PASSWDMISMATCH "464"

/**
 * @brief 유저가 가득찬 채널에 입장하려고 하는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<channel> :Cannot join channel (+l)"
 */
#define ERR_CHANNELISFULL "471"

/**
 * @brief 유저가 초대를 받지 않고 초대 전용 채널에 입장하는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<channel> :Cannot join channel (+i)"
 */
#define ERR_INVITEONLYCHAN "473"

/**
 * @brief 채널 입장에 필요한 key(password)가 없거나 다른 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<channel> :Cannot join channel (+k)"
 */
#define ERR_BADCHANNELKEY "475"

/**
 * @brief operator 권한이 없는 user가 operator 권한이 필요한 메시지를 사용하는 경우 응답하는 에러 코드이다.
 *
 * 메시지 형식: "<client> <channel> :You're not channel operator"
 */
#define ERR_CHANOPRIVSNEEDED "482"


////////////////////////////////////////////////////////////
// Command responses.
////////////////////////////////////////////////////////////

/**
 * @brief user가 등록되면 첫 번째로 응답하는 메시지이며, 해당 user의 정보를 알려준다.
 *
 * 메시지 형식: "<nick> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
 */
#define RPL_WELCOME "001"

/**
 * @brief user가 등록되면 두 번째로 응답하는 메시지이며, 해당 user가 연결된 서버의 정보를 알려준다.
 *
 * 메시지 형식: "<nick> :Your host is <servername>, running version <version>"
 */
#define RPL_YOURHOST "002"

/**
 * @brief user가 등록되면 세 번째로 응답하는 메시지이며, 해당 user가 연결된 서버가 생성된 날짜를 알려준다.
 *
 * 메시지 형식: "<nick> :This server was created <datetime>"
 */
#define RPL_CREATED "003"

/**
 * @brief user가 등록되면 네 번째로 응답하는 메시지이며, 서버 이름 및 version을 알려준다. 
 *
 * 메시지 형식: "<nick> <servername> <version> <available user modes> ""
 */
#define RPL_MYINFO "004"

/**
 * @brief user가 등록되면 다섯 번째로 응답하는 메시지이며, 서버에서 지원 가능한 목록들을 알려준다.
 *
 * 메시지 형식: "<nick> <1-13 tokens> :are supported by this server"
 */
#define RPL_ISUPPORT "005"

/**
 * @brief 현재 채널에 어떤 mode가 적용되어 있는지 알려준다.
 *
 * 메시지 형식: "<nick> <channel> <modestring> <mode arguments>..."
 */
#define RPL_CHANNELMODEIS "324"

/**
 * @brief 유저가 채널에 입장했을 때, 채널에 topic이 없는 경우 topic이 설정되어 있지 않다고 알려준다.
 *
 * 메시지 형식: "<nick> <channel> :No topic is set"
 */
#define RPL_NOTOPIC "331"

/**
 * @brief 유저가 채널에 입장했을 때, 채널에 topic이 있는 경우 topic을 알려준다.
 *
 * 메시지 형식: "<nick> <channel> :<topic>"
 */
#define RPL_TOPIC "332"

/**
 * @brief 유저가 특정 유저를 채널에 초대했을 때, 정상적으로 초대했음을 알려준다.
 *
 * 메시지 형식: "<client> <target nick> <channel>"
 */
#define RPL_INVITING "341"

/**
 * @brief 채널의 모든 유저 목록을 알려준다.
 *
 * 메시지 형식: "<client> <symbol> <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
 */
#define RPL_NAMREPLY "353"

/**
 * @brief 채널의 모든 유저 목록의 끝을 알려준다.
 * RPL_NAMREPLY 메시지의 뒤에 붙는다
 *
 * 메시지 형식: "<client> <channel> :End of /NAMES list"
 */
#define RPL_ENDOFNAMES "366"

namespace grc
{

class IRC
{
public:
    static void HandleMessage(const int32 IN socket, Network& IN network, const std::string& IN password);
private:
    IRC(); // = delete
    typedef void (*TcommandFunctionPTR)(const int32,
                                        const std::string&,
                                        const std::vector<std::string>&,
                                        const std::string&,
                                        const std::string&,
                                        Network&);
    static void initializeCommandFunctionMap();
    static void parseMessage(const std::string& IN message,
                             std::string& OUT command,
                             std::vector<std::string>& OUT parameters,
                             std::string& OUT trailing);
    static void PASS(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void NICK(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void USER(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void QUIT(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void JOIN(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void PART(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void MODE(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void TOPIC(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void INVITE(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void KICK(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void PRIVMSG(const int32 IN socket,
                        const std::string& IN command,
                        const std::vector<std::string>& IN parameters,
                        const std::string& IN trailing,
                        const std::string& IN password,
                     Network& IN OUT network);
    static void PING(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);
    static void PONG(const int32 IN socket,
                     const std::string& IN command,
                     const std::vector<std::string>& IN parameters,
                     const std::string& IN trailing,
                     const std::string& IN password,
                     Network& IN OUT network);

    static void sendWelcomeMessage(const int32 IN socket, Network& IN OUT network);
    static bool isNicknameInUse(const std::string& IN nickname);
private:
    static std::map<std::string, TcommandFunctionPTR> sStaticCommandFunctionMap;
};

}
