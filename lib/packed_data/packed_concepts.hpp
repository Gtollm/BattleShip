#pragma once

#include <concepts>
#include <type_traits>

template <typename T>
concept TwoBitValue = std::integral<T> && (T{} >= 0 && T{} <= 3);

template <typename T>
concept TwoBitEnumClass =
    std::is_enum_v<T> && std::is_integral_v<std::underlying_type_t<T>> &&
    (static_cast<std::underlying_type_t<T>>(T{}) >= 0 &&
     static_cast<std::underlying_type_t<T>>(T{}) <= 3);

template <typename T>
concept Valid2Bit = TwoBitValue<T> || TwoBitEnumClass<T>;

template <std::size_t BitWidth>
concept ValidBitWidth =
    (BitWidth == 1 || BitWidth == 2 || BitWidth == 4 || BitWidth == 8);
