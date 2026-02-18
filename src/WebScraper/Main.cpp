#include "Action.hpp"
#include <JSON.hpp>
#include <String.hpp>
#include <FunctionT.hpp>
#include <Libc/HostFileSystem.hpp>
#include <FileSystem/Directory.hpp>
#include <Interfaces/Dictionary.hpp>
#include <Interfaces/ComparisionFunction.hpp>
#include <Interfaces/Sequence/SubSequence.hpp>
#include <Curl.cpp>
#include <iostream>

MathLib::String ToProtocol(const MathLib::Sequence<char>& url) {
    const size_t size = url.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) {
        const char chr = url.AtUnsafe(i);
        if (chr == '/') {
            if (++i == size || url.AtUnsafe(i) == '/') break;
            ret += '/';
        }
        ret += chr;
    }
    return ret;
}
MathLib::String ToBaseURL(const MathLib::Sequence<char>& url) {
    const size_t size = url.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) {
        const char chr = url.AtUnsafe(i);
        if (chr == '/') {
            if (++i == size || url.AtUnsafe(i) != '/') break;
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
size_t ScrapeInternal(MathLib::FileSystem& fs, MathLib::Curl& curl, const MathLib::Sequence<Action>& actions, const MathLib::Sequence<char>& url_, size_t maxDepth, MathLib::Dictionary<MathLib::String, size_t>& cache, Action::Type action) {
    if (action == Action::Type::Ignore) return SIZE_MAX;
    const MathLib::String url = ConvertURL(url_);
    size_t ret = cache.FindKey(url);
    if (ret == SIZE_MAX) ret = cache.GetSize();
    else if (maxDepth <= cache.AtUnsafe(ret).value) return ret;
    if (!cache.AddOrReplace(MathLib::DictionaryElement<MathLib::String, size_t>(url, maxDepth))) return SIZE_MAX;
    std::cout << url << std::endl;
    MathLib::String html = MathLib::CollectionToString(curl.Get<char>(url));
    if (action == Action::Type::Scrape) {
        const MathLib::String fileAttributtes[] = {
            "href", "src",
        };
        const size_t size = SizeOfArray(fileAttributtes);
        for (size_t i = 0; i < size; i++) {
            const MathLib::String attr = fileAttributtes[i];
            const MathLib::Function<MathLib::String, const MathLib::Sequence<char>&>& func = MathLib::MakeFunctionT<MathLib::String, const MathLib::Sequence<char>&>([&fs, &curl, &cache, &url, &actions, &attr, maxDepth, action](const MathLib::Sequence<char>& old) -> MathLib::String {
                MathLib::String absoluteURL = MathLib::CollectionToString(old);
                if (absoluteURL.AtUnsafe(0) == '/') {
                    if (absoluteURL.AtUnsafe(1) == '/') absoluteURL = ToProtocol(url) + absoluteURL;
                    else absoluteURL = ToBaseURL(url) + absoluteURL;
                }
                const size_t size = actions.GetSize();
                Action::Type type = Action::Type::Ignore;
                bool exception = false;
                for (size_t i = 0; i < size; i++) {
                    const Action action = actions.At(i);
                    if (absoluteURL.StartsWith(action.prefix)) {
                        if (action.type == Action::Type::IgnoreException) {
                            exception = true;
                            continue;
                        }
                        if (action.type == Action::Type::Ignore && exception) continue;
                        type = action.type;
                        break;
                    }
                }
                if (!maxDepth || type == Action::Type::Ignore) return attr + "=\"" + absoluteURL + '"';
                const size_t file = ScrapeInternal(fs, curl, actions, absoluteURL, maxDepth - 1, cache, type);
                if (file == SIZE_MAX) return "";
                return attr + "=\"" + MathLib::ToString(file, 10) + ".html\"";
            });
            html = Replace<char, MathLib::String>(html, attr + "=\"", '"'_M, func);
        }
    }
    MathLib::File file = fs.Open(MathLib::ToString(ret, 10) + ".html", MathLib::OpenMode::Write);
    return file.Puts(html) ? ret : SIZE_MAX;
}
bool Scrape(MathLib::FileSystem& fs, MathLib::Curl& curl, const MathLib::Sequence<Action>& actions, const MathLib::Sequence<char>& url, size_t maxDepth) {
    MathLib::Dictionary<MathLib::String, size_t> cache;
    return ScrapeInternal(fs, curl, actions, url, maxDepth, cache, Action::Type::Scrape) != SIZE_MAX;
}
bool AddActions(MathLib::JSON& pattern, MathLib::WritableSequence<Action>& actions, const MathLib::String& name, Action::Type type) {
    const MathLib::JSON list = pattern.Find(name).GetOr(MathLib::JSON());
    for (const MathLib::JSON& url : list)
        if (!actions.Add(Action(type, url.GetValue()))) return false;
    return true;
}
bool ParseRobotsLine(const MathLib::Sequence<char>& line, size_t& i, const MathLib::Sequence<char>& expected) {
    const size_t size = expected.GetSize();
    if (line.GetSize() - i < size) return false;
    for (size_t j = 0; j < size; j++)
        if (MathLib::ToUpper(line.AtUnsafe(i + j)) != expected[j]) return false;
    i += size;
    return true;
}
enum class RobotsState : uint8_t {
    None,
    FoundDefaultRules,
    FoundSpecificRules,
};
MathLib::Array<Action> ParseRobotsFile(MathLib::Curl& curl, const MathLib::String& url) {
    const MathLib::String baseURL = ToBaseURL(ConvertURL(url));
    const MathLib::Array<char> str = curl.Get<char>(baseURL + "/robots.txt");
    if (str.IsEmpty()) return MathLib::Array<Action>();
    const MathLib::Array<MathLib::String> split = MathLib::Split(str, '\n'_M, false);
    const size_t size = split.GetSize();
    MathLib::Array<Action> ret;
    MathLib::Array<Action> exceptions;
    RobotsState state = RobotsState::None;
    for (size_t i = 0; i < size; i++) {
        const MathLib::String curr = split.AtUnsafe(i);
        size_t j = 0;
        MathLib::SkipWhiteSpace(curr, j);
        const size_t currSize = curr.GetSize();
        if (j == currSize || curr.AtUnsafe(j) == '#') continue;
        if (ParseRobotsLine(curr, j, "USER-AGENT:"_M)) {
            if (state == RobotsState::FoundSpecificRules) break;
            MathLib::SkipWhiteSpace(curr, j);
            MathLib::String userAgent;
            while (j < size) {
                const char tmp = curr.AtUnsafe(j++);
                if (!tmp) break;
                userAgent += tmp;
            }
            if (userAgent == MathLib::Curl::userAgent) {
                if (!ret.Reset() || !exceptions.Reset()) return MathLib::Array<Action>();
                state = RobotsState::FoundSpecificRules;
            }
            if (userAgent == "*") state = RobotsState::FoundDefaultRules;
            continue;
        }
        if (state == RobotsState::None) continue;
        const bool allow = ParseRobotsLine(curr, j, "ALLOW:"_M);
        if (!allow && !ParseRobotsLine(curr, j, "DISALLOW:"_M)) continue;
        MathLib::SkipWhiteSpace(curr, j);
        MathLib::String url = baseURL;
        while (j < size) url += curr.AtUnsafe(j++);
        if (allow) {
            if (!exceptions.Add(Action(Action::Type::IgnoreException, url))) return MathLib::Array<Action>();
        }
        else if (!ret.Add(Action(Action::Type::Ignore, url))) return ret;
    }
    return exceptions.AddSequence(ret) ? exceptions : MathLib::Array<Action>();
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) MathLib::Panic("Usage: "_M + argv[0] + " <pattern file> <output directory>");
        MathLib::HostFileSystem fs;
        MathLib::JSON pattern;
        if (!pattern.LoadFromPath(fs, MathLib::String(argv[1]))) MathLib::Panic("Failed to load pattern file");
        MathLib::Array<Action> actions;
        if (!AddActions(pattern, actions, "ignore"_M, Action::Type::Ignore)) MathLib::Panic("Failed to load ignore list");
        if (!AddActions(pattern, actions, "scrape"_M, Action::Type::Scrape)) MathLib::Panic("Failed to load scrape list");
        if (!AddActions(pattern, actions, "save"_M, Action::Type::Save)) MathLib::Panic("Failed to load save list");
        if (!actions.CocktailShakerSort(MathLib::ComparisionFunction<Action>(MathLib::ComparisionFunctionType::LessThan))) MathLib::Panic("Failed to sort actions");
        const MathLib::String url = pattern.Find("url"_M).Get("No URL specified").GetValue();
        MathLib::Curl curl;
        if (pattern.Find("parseRobots"_M).Get("No action for parsing robots.txt specified").GetValue() == "true") {
            MathLib::Array<Action> tmp = ParseRobotsFile(curl, url);
            if (!tmp.AddSequence(actions)) MathLib::Panic("Failed to add data generated from robots.txt");
            actions = tmp;
        }
        MathLib::Directory directory = MathLib::Directory(fs, MathLib::String(argv[2]));
        if (!directory.CreateDirectory(""_M, false)) MathLib::Panic("Failed to create output directory");
        if (!Scrape(directory, curl, actions, url, MathLib::StringToNumber(pattern.Find("maxDepth"_M).Get("No max depth specified").GetValue()))) MathLib::Panic("Failed to scrape URL");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}