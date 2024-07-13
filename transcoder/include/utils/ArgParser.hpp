#pragma once


#include <string>
#include <unordered_map>
#include <any>

// ArgParser 클래스는 명령행 인수를 구문 분석하는 유틸리티 클래스입니다.
// setFlagString, setFlagInt, setFlagFloat, setFlagDouble, setFlagBool 메서드를 사용하여 플래그와 해당 값을 설정할 수
// 있습니다. 
// parse 메서드를 사용하여 주어진 인수를 구문 분석합니다.

#define CHECK_ARGC(i) if (argc <= i + 1) { std::cout << "Missing value for flag (print help flag -h): " << arg << std::endl; exit(1); }

namespace tr {
    class ArgParser {
    public: // static
        static ArgParser& getInstance();
        static ArgParser instance;


    public: // flags parse
        static void printHelp();

        static void setFlagString(const std::string &flag, const std::string &description, std::string *str);
        static void setFlagInt(const std::string &flag, const std::string &description, int *i);
        static void setFlagFloat(const std::string &flag, const std::string &description, float *f);
        static void setFlagDouble(const std::string &flag, const std::string &description, double *d);
        static void setFlagBool(const std::string &flag, const std::string &description, bool *b);

        static bool parse(int argc, char *argv[]);


    private:
        enum class TYPE {
			STRING,
			INT,
			FLOAT,
			DOUBLE,
			BOOL
		};


    private:
        explicit ArgParser();
        virtual ~ArgParser();


    private:
        using TypePointer = std::tuple<TYPE, std::any>;
        std::unordered_map<std::string, TypePointer> arguments;

        // helps description
        std::unordered_map<std::string, std::string> helps;
    };
}; // namespace tr
