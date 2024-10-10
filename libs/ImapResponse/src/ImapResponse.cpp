#include "ImapResponse.h"
#include <iostream>

namespace ISXResponse
{
    IMAPResponse::IMAPResponse(const std::string response)
    {
        FormatResponse(response);
        ParseResponse(response);
    }

    const std::string& IMAPResponse::get_tag() const
    {
        return m_tag;
    }

    const std::string& IMAPResponse::get_status() const
    {
        return m_status;
    }

    const std::string& IMAPResponse::get_text() const
    {
        return m_text;
    }

    const StatusType& IMAPResponse::get_response_status() const
    {
        return m_response_status;
    }

    const std::string& IMAPResponse::get_formatted_response() const
    {
        return m_formatted_response;
    }

    const std::string& IMAPResponse::get_raw_response() const
    {
        return m_raw_response;
    }

    void IMAPResponse::CheckStatus(const IMAPResponse& response, StatusType status)
    {
        if (!response.StatusEquals(status))
        {
            throw std::runtime_error("IMAP Unexpected status code in response: " + response.get_raw_response() );
        }
    }

    void IMAPResponse::ParseResponse(const std::string& raw_response)
    {
        bool is_unsolicited = false;
        // parse unsolicited response (for now assume it is single line)
        if (raw_response[0] == '*')
        {
            // parsing (todo next sprint)
            m_raw_response = raw_response.substr(raw_response.find("\r\n") + 2);
            is_unsolicited = true;
        }

        std::smatch matches;      

        // A regex pattern that matches an IMAP response: TAG STATUS TEXT
        std::regex imapResponsePattern(R"(^(\*|\w+)\s+(OK|NO|BAD|BYE|PREAUTH|FETCH|FLAGS|EXISTS|RECENT|LIST|SEARCH|CAPABILITY|[A-Z]+)?\s*(\((?:[^\(\)]*)\)|\[.*\]|[^\r\n]*)?\r?\n?)");
        

        if (std::regex_match(m_raw_response, matches, imapResponsePattern))
        {
            // m_raw_response = raw_response;
            m_tag = matches[1];  // '*' for untagged or actual tag (e.g., 'A001')
            m_status = matches[2];  // OK, NO, BAD, etc.
            m_text = matches[3];  // Remaining text message

            if (m_status == "OK")
            {
                m_response_status = StatusType::OK;
            }
            else if (m_status == "NO")
            {
                m_response_status = StatusType::NO;
            }
            else if (m_status == "BAD")
            {
                m_response_status = StatusType::BAD;
            }
            else
            {
                m_response_status = StatusType::Undefined;
            }
        }
        else
        {
            if (is_unsolicited)
                m_response_status = StatusType::OK;
            else
                throw std::invalid_argument("Invalid response format imap");
        }
    }


    void IMAPResponse::FormatResponse(const std::string& response)
    {
        // Prefix "S: " to each line of the response
        m_formatted_response = "S: " + response;

        // Replace all CRLF (\r\n) or LF (\n) with "\nS: "
        m_formatted_response = std::regex_replace(m_formatted_response, std::regex("\r?\n"), "\nS: ");

        // Check if we added an extra "S: " at the end and remove it if necessary
        if (m_formatted_response.size() >= 3 && 
            m_formatted_response.substr(m_formatted_response.size() - 3) == "S: ")
        {
            m_formatted_response.erase(m_formatted_response.end() - 3, m_formatted_response.end());
        }
        // std::cout<<"TEST RESP "<<m_formatted_response<<std::endl;
    }


    bool IMAPResponse::IsValidResponse(const std::string& response, std::smatch& matches) const
    {
        std::regex responsePattern(R"(^([*+A-Z]+) (OK|NO|BAD)(?: (.*))?$)");
        return std::regex_match(response, matches, responsePattern);
    }

    SplitParts IMAPResponse::splitByFirstTwoSpaces(const std::string& input) {
        SplitParts parts;
        size_t firstSpaceIndex = input.find(' ');

        if (firstSpaceIndex == std::string::npos) {
            // No spaces found
            parts.part1 = input;
            parts.part2 = "";
            parts.part3 = "";
            return parts;
        }

        size_t secondSpaceIndex = input.find(' ', firstSpaceIndex + 1);
        
        if (secondSpaceIndex == std::string::npos) {
            // One space found
            parts.part1 = input.substr(0, firstSpaceIndex);
            parts.part2 = input.substr(firstSpaceIndex + 1);
            parts.part3 = "";
            return parts;
        }

        // Split the string into three parts
        parts.part1 = input.substr(0, firstSpaceIndex);
        parts.part2 = input.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1);
        parts.part3 = input.substr(secondSpaceIndex + 1);
        
        return parts;
    }

    bool IMAPResponse::StatusEquals(StatusType status) const
    {
        return m_response_status == status;
    }

} // namespace ISXIResponse
