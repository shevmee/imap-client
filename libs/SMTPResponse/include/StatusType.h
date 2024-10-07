#pragma once

namespace ISXResponse
{
    enum class StatusType
    {
        OK,
        NO,
        BAD,
        PositiveCompletion,
        PositiveIntermediate,
        TransientNegative,
        PermanentNegative,
        Undefined
    };
} // namespace ISXResponse