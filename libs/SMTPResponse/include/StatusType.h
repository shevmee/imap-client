#pragma once

namespace ISXResponseI
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