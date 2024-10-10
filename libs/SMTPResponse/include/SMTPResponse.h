#pragma once 

#include <string>
#include <regex>
#include <cstdint>

#include "StatusType.h"

namespace ISXResponseI
{
class SMTPResponse {
public:
    explicit SMTPResponse(const std::string& response);

    const std::uint16_t& get_code() const;
    const std::string& get_enhanced_code() const;
    const std::string& get_text() const;

    const StatusType& get_status() const;
    bool CodeEquals(std::uint16_t code) const;
    bool StatusEquals(StatusType status) const;
    const std::string& get_formated_response() const;
    const std::string& get_raw_response() const;

    static void CheckStatus(const SMTPResponse& response, StatusType status);

private:    
    void ParseResponse(const std::string& response);
    void FormatResponse(const std::string& response);
    bool IsValidResponse(const std::string& response, std::smatch& matches) const;
    auto SplitAtEndline(const std::string& response) const -> std::pair<std::string, std::string>;

    std::uint16_t m_code;
    std::string m_enhanced_code;
    std::string m_text;
    std::string m_formated_response;
    std::string m_raw_response;
    StatusType m_status;
};
} // namespace ISXResponseI