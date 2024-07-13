#pragma once

namespace tr {
    class Result {
    public:
        enum class TYPE { SUCCESS, ERROR };

    public:
        explicit Result(TYPE type) : type(type) {}
        explicit Result(TYPE type, const std::string& message) : type(type), message(message) {}
        virtual ~Result() {}

    public:
        TYPE getType() const {
            return type;
        }
        const std::string& getMessage() const {
            return message;
        }

    private:
        Type type;
        std::string message;
    };
};
