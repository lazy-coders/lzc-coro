#pragma once
#include <iostream>
#include <format>
#include <string_view>
#include <source_location>
#include <thread>
#include <sstream>

struct auto_location
{
    std::source_location sl_;
    std::string_view fmt_;
    std::thread::id tid_;

    auto_location(const std::string_view& fmt, const std::source_location& sl = std::source_location::current())
    : sl_{sl}
    , fmt_{fmt}
    , tid_{std::this_thread::get_id()}
    {}

    auto_location(const char* fmt, const std::source_location& sl = std::source_location::current())
    : sl_{sl}
    , fmt_{fmt}
    , tid_{std::this_thread::get_id()}
    {}
};

#ifndef __cpp_lib_formatters
namespace std
{
template< class CharT >
struct formatter<std::thread::id, CharT>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        while(it != ctx.end() and *it != '}')
            ++it;
        return it;
    }

    template<class FmtContext>
    FmtContext::iterator format(std::thread::id id, FmtContext& ctx) const
    {
        std::ostringstream out;
        out << id;
 
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
};
#endif

namespace std
{
template<class CharT>
struct formatter<std::source_location, CharT>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        while(it != ctx.end() and *it != '}')
            ++it;
        return it;
    }

    template<class FmtContext>
    FmtContext::iterator format(std::source_location sl, FmtContext& ctx) const
    {
        std::ostringstream out;
        out << sl.file_name() << ":" << sl.function_name() << ":" << sl.line();
 
        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};
}

template<typename... Args>
void d(const auto_location& t, Args&&... args)
{
    static std::mutex m;
    std::lock_guard lock{m};

    const char* file_name = t.sl_.file_name();
    const char* function_name = t.sl_.function_name();
    const std::uint_least32_t line = t.sl_.line();
    std::clog << std::format("{{\"id\"={}, \"file\"=\"{}\", \"function\"=\"{}\", \"line\"={}, description=\"", t.tid_, file_name, function_name, line);
    std::clog << std::vformat(t.fmt_,  std::make_format_args(args...));
    std::clog << "\" }" << std::endl;
}

inline void d(const std::source_location& sl = std::source_location::current())
{
    d(auto_location("", sl));
}

template<typename... Args>
void d(const std::source_location& sl, std::string_view fmt, Args&&... args)
{
    d(auto_location(fmt, sl), std::forward<Args>(args)...);
}