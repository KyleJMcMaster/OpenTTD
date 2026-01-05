#ifndef FORMAT_HPP
#define FORMAT_HPP

#include "../3rdparty/fmt/format.h"
#include "convertible_through_base.hpp"

/** Helper to choose the target type for formatting without invalidating std::underlying_type. */
template <typename T, bool IsEnum>
struct OpenTTD_FormatTarget {
    using type = typename T::BaseType;
};

template <typename T>
struct OpenTTD_FormatTarget<T, true> {
    using type = std::underlying_type_t<T>;
};

/**
 * Combined formatter for Enums and types that use ConvertibleThroughBase.
 * This specialization belongs in the fmt namespace.
 */
template <typename T, typename Char>
requires std::is_enum_v<T> || ConvertibleThroughBase<T>
struct fmt::formatter<T, Char> {
    using target_type = typename OpenTTD_FormatTarget<T, std::is_enum_v<T>>::type;
    fmt::formatter<target_type, Char> format_impl;

    constexpr auto parse(fmt::format_parse_context &ctx)
    {
        return format_impl.parse(ctx);
    }

    auto format(const T &t, fmt::format_context &ctx) const
    {
        if constexpr (std::is_enum_v<T>) {
            return format_impl.format(static_cast<target_type>(t), ctx);
        } else {
            return format_impl.format(t.base(), ctx);
        }
    }
};

template <class... Args>
void format_append(std::string &out, fmt::format_string<Args...> &&fmt, Args&&... args)
{
    fmt::format_to(std::back_inserter(out), std::forward<decltype(fmt)>(fmt), std::forward<decltype(args)>(args)...);
}

#endif /* FORMAT_HPP */