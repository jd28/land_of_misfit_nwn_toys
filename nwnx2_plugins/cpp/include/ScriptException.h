#include <stdexcept>
#include <string>

class ScriptException : public std::runtime_error {
    ScriptException(const std::string& what)
        : std::runtime_error(what) {}
};
