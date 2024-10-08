#include "SMTPResponse.h"

namespace ISXResponse
{
    SMTPResponse::SMTPResponse(const std::string& response) 
    {
        ParseResponse(response);
        FormatResponse(response);

        switch (m_code / 100) 
        {
        case 2:
            m_status = StatusType::PositiveCompletion;
            break;
        case 3:
            m_status = StatusType::PositiveIntermediate;
            break;
        case 4:
            m_status = StatusType::TransientNegative;
            break;
        case 5:
            m_status = StatusType::PermanentNegative;
            break;
        default:
            m_status = StatusType::Undefined;
            break;
        }
    }

    const std::uint16_t& SMTPResponse::get_code() const 
    {
        return m_code;
    }

    const std::string& SMTPResponse::get_enhanced_code() const 
    {
        return m_enhanced_code;
    }

    const std::string& SMTPResponse::get_text() const 
    {
        return m_text;
    }

    const StatusType& SMTPResponse::get_status() const 
    {
        return m_status;
    }

    const std::string& SMTPResponse::get_formated_response() const 
    {
        return m_formated_response;
    }

    const std::string& SMTPResponse::get_raw_response() const 
    {
        return m_raw_response;
    }

    void SMTPResponse::CheckStatus(const SMTPResponse& response, StatusType status)
    {
        if (!response.StatusEquals(status)) 
        {
            throw std::runtime_error("Unexpected status code in response: " + response.get_raw_response());
        }
    }

    void SMTPResponse::ParseResponse(const std::string& raw_response) 
    {
        std::smatch matches;
        auto [response, tail] = SplitAtEndline(raw_response);

        if (IsValidResponse(response, matches)) 
        {
            m_raw_response = response;
            if (matches.size() > 1) 
            {
                m_code = std::stoi(matches[1].str());
            }
            if (matches.size() > 2 && matches[2].matched) 
            {
                m_enhanced_code = matches[2].str();
            }
            if (matches.size() > 3 && matches[3].matched) 
            {
                m_text = matches[3].str() + tail;
            }
        } else 
        {
            throw std::invalid_argument("Invalid response format smtp");
        }
    }

    void SMTPResponse::FormatResponse(const std::string& response)
    {
        m_formated_response = "S: " + response;
        m_formated_response = std::regex_replace(m_formated_response, std::regex("\n"), "\nS: ");
        m_formated_response.erase(m_formated_response.end() - 3, m_formated_response.end());
    }

    bool SMTPResponse::IsValidResponse(const std::string& response, std::smatch& matches) const
    {
        std::regex responsePattern(R"(^(\d{3})(?:[ -](\d\.\d\.\d))?[ -](.*)$)");
        return std::regex_match(response, matches, responsePattern);
    }

    auto SMTPResponse::SplitAtEndline(const std::string& response) const -> std::pair<std::string, std::string>
    {
        auto endline_pos = std::min(response.find("\n"), response.find("\r"));
        return std::make_pair(response.substr(0, endline_pos), response.substr(std::min(endline_pos, response.size())));
    }

    bool SMTPResponse::CodeEquals(uint16_t code) const 
    {
        return m_code == code;
    }

    bool SMTPResponse::StatusEquals(StatusType status) const 
    {
        return m_status == status;
    }

} // namespace ISXResponse