#include "Curl.hpp"
#include <JSON.hpp>
#include <String.hpp>
#include <FunctionT.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Interfaces/Dictionary.hpp>
#include <Interfaces/SubSequence.hpp>
#include <Interfaces/ComparisionFunction.hpp>
#include <iostream>

struct Action : MathLib::Orderable {
    enum class Type : uint8_t {
        Ignore,
        Scrape,
        Save,
    };
    MathLib::String prefix;
    Type type;

    Action(void) : prefix(), type(Type::Ignore) {}
    Action(Type type, const MathLib::Sequence<char>& prefix) : prefix(MathLib::CollectionToString(prefix)), type(type) {}

    protected:
    [[nodiscard]] virtual bool LessThanEqual(const MathLib::Orderable& other_) const override {
        return prefix.GetSize() <= ((const Action&)other_).prefix.GetSize();
    }
};
MathLib::String ToBaseURL(const MathLib::Sequence<char>& url) {
    const size_t size = url.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) {
        char chr = url.At(i);
        if (chr == '/') {
            if (++i == size || url.At(i) != '/') break;
            ret += '/';
        }
        ret += chr;
    }
    return ret;
}
MathLib::String ConvertURL(const MathLib::Sequence<char>& url) {
    const size_t size = url.GetSize();
    MathLib::String ret;
    bool add = true;
    for (size_t i = 0; i < size; i++) {
        const char chr = url.At(i);
        if (chr == '?' || chr == '#' || (chr == '/' && i + 1 == size)) break;
        if (add) ret += chr;
    }
    return ret;
}
template <typename T, typename Arr>
Arr Replace(const MathLib::Sequence<T>& str, const MathLib::Sequence<T>& replaceStart, const MathLib::Sequence<T>& replaceEnd, const MathLib::Function<Arr, const MathLib::Sequence<T>&>& replacement) {
    const size_t size1 = str.GetSize();
    const size_t size2 = replaceStart.GetSize();
    const size_t size3 = replaceEnd.GetSize();
    size_t i = 0;
    Arr ret;
    while (i < size1) {
        const size_t start = str.Find(replaceStart, i);
        if (start == SIZE_MAX) {
            if (!ret.AddSequence(MathLib::SubSequence<T>(str, MathLib::Interval<size_t>(i, size1)))) return Arr();
            break;
        }
        if (!ret.AddSequence(MathLib::SubSequence<T>(str, MathLib::Interval<size_t>(i, start)))) return Arr();
        const size_t end = str.Find(replaceEnd, start + size2);
        if (end == SIZE_MAX) return Arr();
        i = end + size3;
        if (!ret.AddSequence(replacement(MathLib::SubSequence<T>(str, MathLib::Interval<size_t>(start + size2, end))))) return Arr();
    }
    return ret;
}
size_t ScrapeInternal(MathLib::FileSystem& fs, const MathLib::Sequence<char>& base, Curl& curl, const MathLib::Sequence<Action>& actions, const MathLib::Sequence<char>& url_, size_t maxDepth, MathLib::WritableSequence<MathLib::String>& cache, Action::Type action) {
    if (action == Action::Type::Ignore) return SIZE_MAX;
    const MathLib::String url = ConvertURL(url_);
    size_t ret = cache.Find(url);
    if (ret != SIZE_MAX) return ret;
    if (!cache.Add(url)) return SIZE_MAX;
    std::cout << url << std::endl;
    ret = cache.GetSize() - 1;
    MathLib::String html = MathLib::CollectionToString(curl.Get<char>(url));
    if (action == Action::Type::Scrape) html = Replace<char, MathLib::String>(html, "href=\""_M, '"'_M, MathLib::MakeFunctionT<MathLib::String, const MathLib::Sequence<char>&>(nullptr, [&fs, &base, &curl, &cache, &url, &actions, maxDepth, action](const void*, const MathLib::Sequence<char>& old) -> MathLib::String {
        MathLib::String absoluteURL = MathLib::CollectionToString(old);
        absoluteURL = absoluteURL.At(0) == '/' ? ToBaseURL(url) + absoluteURL : absoluteURL;
        const size_t size = actions.GetSize();
        Action::Type type = Action::Type::Ignore;
        for (size_t i = 0; i < size; i++) {
            const Action action = actions.At(i);
            if (absoluteURL.StartsWith(action.prefix)) {
                type = action.type;
                break;
            }
        }
        if (!maxDepth || type == Action::Type::Ignore) return "href=\""_M + absoluteURL + '"';
        const size_t file = ScrapeInternal(fs, base, curl, actions, absoluteURL, maxDepth - 1, cache, action);
        if (file == SIZE_MAX) return "";
        return "href=\""_M + MathLib::ToString(file, 10) + ".html\"";
    }));
    MathLib::File file = fs.Open(MathLib::CollectionToString(base) + MathLib::ToString(ret, 10) + ".html", MathLib::OpenMode::Write);
    return file.Puts(html) ? ret : SIZE_MAX;
}
bool Scrape(MathLib::FileSystem& fs, const MathLib::Sequence<char>& base, Curl& curl, const MathLib::Sequence<Action>& actions, const MathLib::Sequence<char>& url, size_t maxDepth) {
    MathLib::Array<MathLib::String> cache;
    return ScrapeInternal(fs, base, curl, actions, url, maxDepth, cache, Action::Type::Scrape) != SIZE_MAX;
}
bool AddActions(MathLib::JSON& pattern, MathLib::WritableSequence<Action>& actions, const MathLib::String& name, Action::Type type) {
    const MathLib::JSON list = pattern.Find(name).GetOr(MathLib::JSON());
    for (const MathLib::JSON& url : list)
        if (!actions.Add(Action(type, url.GetValue()))) return false;
    return true;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <pattern file>");
        MathLib::HostFileSystem fs;
        MathLib::JSON pattern;
        if (!pattern.LoadFromPath(fs, MathLib::String(argv[1]))) MathLib::Panic("Failed to load pattern file");
        MathLib::Array<Action> actions;
        if (!AddActions(pattern, actions, "ignore"_M, Action::Type::Ignore)) MathLib::Panic("Failed to load ignore list");
        if (!AddActions(pattern, actions, "scrape"_M, Action::Type::Scrape)) MathLib::Panic("Failed to load scrape list");
        if (!AddActions(pattern, actions, "save"_M, Action::Type::Save)) MathLib::Panic("Failed to load save list");
        if (!actions.CocktailShakerSort(MathLib::ComparisionFunction<Action>(MathLib::ComparisionFunctionType::LessThan))) MathLib::Panic("Failed to sort actions");
        Curl curl;
        if (!Scrape(fs, "t/"_M, curl, actions, pattern.Find("url"_M).Get("No URL specified").GetValue(), MathLib::StringToNumber(pattern.Find("maxDepth"_M).Get("No max depth specified").GetValue()))) MathLib::Panic("Failed to scrape URL");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}