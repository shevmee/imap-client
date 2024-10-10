#ifndef IMAPRESPONSE_H
#define IMAPRESPONSE_H

#include <string>
#include <regex>
#include <stdexcept>
#include <utility>

#include "StatusType.h"

namespace ISXResponseI
{
    struct SplitParts {
        std::string part1;
        std::string part2;
        std::string part3;
    };

    class IMAPResponse
    {
    public:
        explicit IMAPResponse(const std::string response);

        const std::string& get_tag() const;
        const std::string& get_status() const;
        const std::string& get_text() const;
        const StatusType& get_response_status() const;
        const std::string& get_formatted_response() const;
        const std::string& get_raw_response() const;
        bool StatusEquals(StatusType status) const;

        static void CheckStatus(const IMAPResponse& response, StatusType status);

    private:
        std::string m_tag;
        std::string m_status;
        std::string m_text;
        StatusType m_response_status;
        std::string m_formatted_response;
        std::string m_raw_response;

        void ParseResponse(const std::string& raw_response);
        void FormatResponse(const std::string& response);
        bool IsValidResponse(const std::string& response, std::smatch& matches) const;
        SplitParts splitByFirstTwoSpaces(const std::string& input);
        //auto SplitAtSpace(const std::string& response) const -> std::pair<std::string, std::string>;
    };

} // namespace ISXResponse

#endif // IMAPRESPONSE_H
