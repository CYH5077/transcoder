#include "utils/ArgParser.hpp"

#include <iostream>


namespace tr {
    ArgParser ArgParser::instance;

	ArgParser& ArgParser::getInstance() {
		return ArgParser::instance;
	}


	ArgParser::ArgParser() {
	
	}


	ArgParser::~ArgParser() {
	
	}


	void ArgParser::printHelp() {
		std::cout << "Usage: transcoder [options]" << std::endl;
		std::cout << "Options:" << std::endl;

		for (auto &help : ArgParser::instance.helps) {
			std::cout << "  " << help.first << " : " << help.second << std::endl;
		}
	}

	void ArgParser::setFlagString(const std::string& flag, const std::string& description, std::string* str) {
        ArgParser::instance.helps[flag] = description;
        ArgParser::instance.arguments[flag] = std::make_tuple(TYPE::STRING, str);
	}


	void ArgParser::setFlagInt(const std::string& flag, const std::string& description, int* i) {
        ArgParser::instance.helps[flag] = description;
		ArgParser::instance.arguments[flag] = std::make_tuple(TYPE::INT, i);
	}


	void ArgParser::setFlagFloat(const std::string& flag, const std::string& description, float* f) {
        ArgParser::instance.helps[flag] = description;
		ArgParser::instance.arguments[flag] = std::make_tuple(TYPE::FLOAT, f);
	}


	void ArgParser::setFlagDouble(const std::string& flag, const std::string& description, double* d) {
        ArgParser::instance.helps[flag] = description;
		ArgParser::instance.arguments[flag] = std::make_tuple(TYPE::DOUBLE, d);
	}


	void ArgParser::setFlagBool(const std::string& flag, const std::string& description, bool* b) {
        ArgParser::instance.helps[flag] = description;
		ArgParser::instance.arguments[flag] = std::make_tuple(TYPE::BOOL, b);
	}


	bool ArgParser::parse(int argc, char* argv[]) {
		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];
           

            if (arg == "-h" || arg[0] != '-') {
				ArgParser::instance.printHelp();
                return false;
			}
			
			if (ArgParser::instance.arguments.find(arg) == ArgParser::instance.arguments.end()) {
				std::cout << "Unknown flag: " << arg << std::endl;
				ArgParser::instance.printHelp();
                return false;
			}

			TypePointer &typePointer = ArgParser::instance.arguments[arg];
			TYPE type = std::get<0>(typePointer);

			switch (type) {
				case TYPE::STRING:
                   CHECK_ARGC(i);
					*std::any_cast<std::string *>(std::get<1>(typePointer)) = argv[++i];
					break;
                case TYPE::INT:
                    CHECK_ARGC(i);
					*std::any_cast<int *>(std::get<1>(typePointer)) = std::stoi(argv[++i]);
					break;
                case TYPE::FLOAT:
                    CHECK_ARGC(i);
					*std::any_cast<float *>(std::get<1>(typePointer)) = std::stof(argv[++i]);
					break;
                case TYPE::DOUBLE:
                    CHECK_ARGC(i);
					*std::any_cast<double *>(std::get<1>(typePointer)) = std::stod(argv[++i]);
					break;
				case TYPE::BOOL:
					*std::any_cast<bool *>(std::get<1>(typePointer)) = true;
					break;
			}
		}

		return true;
	}

}; // namespace tr
