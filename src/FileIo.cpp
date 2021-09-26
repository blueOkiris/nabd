/*
 * Author: Dylan Turner
 * Description: Files used for building generated code
 */

#include <string>
#include <vector>
#include <sstream>
#include <Utility.hpp>
#include <FileIo.hpp>

nabd::InputArguments nabd::parseArguments(const int argc, const char **args) {
    InputArguments result;
    result.link = false;
    
    if(argc < 2) {
        errorOut("No file name provided!\n");
    }
    result.fileName = std::string(args[1]);

    for(int i = 2; i < argc; i++) {
        if(std::string(args[i]) == "-I" && i + 1 < argc) {
            result.includeFolders.push_back(std::string(args[i + 1]));
            i++;
        } else if(std::string(args[i]) == "-k") {
            result.link = true;
        } else if(std::string(args[i]) == "-L" && i + 1 < argc) {
            result.linkFolders.push_back(std::string(args[i + 1]));
            i++;
        } else if(std::string(args[i]) == "-l" && i + 1 < argc) {
            result.libraryNames.push_back(std::string(args[i + 1]));
            i++;
        } else {
            result.objects.push_back(std::string(args[i]));
        }
    }

    return result;
}

nabd::ModuleInfo nabd::extractModuleInfo(const nabd::InputArguments &inputs) {
    const auto baseStart = inputs.fileName.find_last_of("/\\");
    const auto baseFileName =
        baseStart != std::string::npos ?
            inputs.fileName.substr(baseStart + 1) :
            inputs.fileName;
    auto relativeDir =
        baseStart != std::string::npos ?
            inputs.fileName.substr(0, baseStart) :
            "";
    if(relativeDir.length() > 1
            && (
                relativeDir.substr(0, 2) == "./"
                    || relativeDir.substr(0, 2) == ".\\"
            )) {
        relativeDir = relativeDir.substr(2);
    } else if(relativeDir.length() > 0 && relativeDir[0] == '.') {
        relativeDir = relativeDir.substr(1);
    }
    
    const auto extensionStart = baseFileName.find(".nabd");
    const auto moduleName =
        extensionStart != std::string::npos ?
            baseFileName.substr(0, extensionStart) :
            baseFileName;
    std::cout << "Building module '" << moduleName << '\'' << std::endl;

    const auto buildFolder =
        (relativeDir != "" ? (relativeDir + "/") : "")
        + moduleName + "_nabdout";
    
    return {
        inputs.fileName, relativeDir, baseFileName,
        moduleName, buildFolder
    };
}

std::string nabd::generateMakefile(
        const InputArguments &inputs, const ModuleInfo &modInfo) {
    std::stringstream inc;
    for(const auto &folder : inputs.includeFolders) {
        inc << "\"-I" << getCurrentDir() << "/" << folder << "\" ";
    }
    inc << "\"-I" << getCurrentDir() << "/" << modInfo.buildFolder << "\" ";
    
    auto makeFileTemplateCp = g_makeFile;
    makeFileTemplateCp[1] = inc.str();

    std::stringstream makeFile;
    for(const auto &section : makeFileTemplateCp) {
        makeFile << section;
    }

    return makeFile.str();
}

const std::vector<std::string> nabd::g_makeFile = {
    "SRC_FILE :=\t\t\t$(wildcard *.cpp)\n"
    "OBJNAME :=\t\t\t$(subst .cpp,.o,$(SRC_FILE))\n"
    "CPPC :=\t\t\t\tg++\n"
    "CPPFLAGS :=\t\t\t-O2 -Wall -Werror -std=c++17\n"
    "INC :=\t\t\t\t",
    "", // Insert include folders
    "\n"
    ".PHONY : all\n"
    "all : $(OBJNAME)\n"
    "$(OBJNAME) : $(SRC_FILE)\n"
    "\t$(CPPC) $(INC) $(CPPFLAGS) -c $< -o $@\n"
};

const std::string nabd::g_varHpp =
    "#pragma once\n"
    "#include <string>\n"
    "#include <utility>\n"
    "#include <vector>\n"
    "#include <memory>\n"
    "enum class VariableType { String, Number, List, Tuple };\n"
    "struct Variable {\n"
    "    virtual std::shared_ptr<Variable> toString(void) const = 0;\n"
    "    virtual std::shared_ptr<Variable> toNumber(void) const = 0;\n"
    "    virtual std::shared_ptr<Variable> toList(\n"
    "        const std::vector<VariableType> &subTypes\n"
    "    ) const = 0;\n"
    "    virtual std::shared_ptr<Variable> toTuple(void) const = 0;\n"
    "};\n"
    "typedef std::shared_ptr<Variable> VariablePointer;\n"
    "struct StringVariable : public Variable {\n"
    "    StringVariable(const std::string &data);\n"
    "    VariablePointer toString(void) const override;\n"
    "    VariablePointer toNumber(void) const override;\n"
    "    VariablePointer toList(\n"
    "        const std::vector<VariableType> &subTypes\n"
    "    ) const override;\n"
    "    VariablePointer toTuple(void) const override;\n"
    "    const std::string value;\n"
    "};\n"
    "struct NumberVariable : public Variable {\n"
    "    NumberVariable(const double data);\n"
    "    VariablePointer toString(void) const override;\n"
    "    VariablePointer toNumber(void) const override;\n"
    "    VariablePointer toList(\n"
    "        const std::vector<VariableType> &subTypes\n"
    "    ) const override;\n"
    "    VariablePointer toTuple(void) const override;\n"
    "    const double value;\n"
    "};\n"
    "struct ListVariable : public Variable {\n"
    "    ListVariable(const std::vector<VariablePointer> &data);\n"
    "    VariablePointer toString(void) const override;\n"
    "    VariablePointer toNumber(void) const override;\n"
    "    VariablePointer toList(\n"
    "        const std::vector<VariableType> &subTypes\n"
    "    ) const override;\n"
    "    VariablePointer toTuple(void) const override;\n"
    "    const std::vector<VariablePointer> values;\n"
    "};\n"
    "struct TupleVariable : public Variable {\n"
    "    TupleVariable(\n"
    "        const std::pair<VariablePointer, VariablePointer> &data\n"
    "    );\n"
    "    VariablePointer toString(void) const override;\n"
    "    VariablePointer toNumber(void) const override;\n"
    "    VariablePointer toList(\n"
    "        const std::vector<VariableType> &subTypes\n"
    "    ) const override;\n"
    "    VariablePointer toTuple(void) const override;\n"
    "    const std::pair<VariablePointer, VariablePointer> values;\n"
    "};\n";

const std::string nabd::g_varCpp =
    "#include <string>\n"
    "#include <utility>\n"
    "#include <sstream>\n"
    "#include <vector>\n"
    "#include <memory>\n"
    "#include <Variable.hpp>\n"
    "VariablePointer listHelper(\n"
    "        VariablePointer self,\n"
    "        const std::vector<VariableType> &subTypes) {\n"
    "    switch(subTypes[0]) {\n"
    "        case VariableType::List: {\n"
    "            std::vector<VariableType> subSubTypes;\n"
    "            for(const auto &type : subTypes) {\n"
    "                if(type == *(subTypes.begin())) {\n"
    "                    continue;\n"
    "                }\n"
    "                subSubTypes.push_back(type);\n"
    "            }\n"
    "            return std::make_shared<ListVariable>(\n"
    "                std::vector<VariablePointer>({\n"
    "                    self->toList(subSubTypes)\n"
    "                })\n"
    "            );\n"
    "        }\n"
    "        case VariableType::Number:\n"
    "            return std::make_shared<ListVariable>(\n"
    "                std::vector<VariablePointer>({ self->toNumber() })\n"
    "            );\n"
    "        case VariableType::String:\n"
    "            return std::make_shared<ListVariable>(\n"
    "                std::vector<VariablePointer>({ self->toString() })\n"
    "            );\n"
    "        case VariableType::Tuple:\n"
    "            return std::make_shared<ListVariable>(\n"
    "                std::vector<VariablePointer>({ self->toTuple() })\n"
    "            );\n"
    "        default:\n"
    "            return std::make_shared<ListVariable>(\n"
    "                std::vector<VariablePointer>({\n"
    "                    std::make_shared<StringVariable>(\n"
    "                        \"randomly created list lol oops\"\n"
    "                    )\n"
    "                })\n"
    "            );\n"
    "    }\n"
    "}\n"
    "StringVariable::StringVariable(const std::string &data) : value(data) {}\n"
    "VariablePointer StringVariable::toString(void) const {\n"
    "    return std::make_shared<StringVariable>(value);\n"
    "}\n"
    "VariablePointer StringVariable::toNumber(void) const {\n"
    "    return std::make_shared<NumberVariable>(\n"
    "        value.length() > 0 ? static_cast<double>(value[0]) : 0\n"
    "    );\n"
    "}\n"
    "VariablePointer StringVariable::toList(\n"
    "        const std::vector<VariableType> &subTypes) const {\n"
    "    return listHelper(\n"
    "        std::make_shared<StringVariable>(value), subTypes\n"
    "    );\n"
    "}\n"
    "VariablePointer StringVariable::toTuple(void) const {\n"
    "    return std::make_shared<TupleVariable>(std::make_pair(\n"
    "        std::make_shared<StringVariable>(value),\n"
    "        std::make_shared<StringVariable>(value)\n"
    "    ));\n"
    "}\n"
    "NumberVariable::NumberVariable(const double data) : value(data) { }\n"
    "VariablePointer NumberVariable::toString(void) const {\n"
    "    return std::make_shared<StringVariable>(std::to_string(value));\n"
    "}\n"
    "VariablePointer NumberVariable::toNumber(void) const {\n"
    "    return std::make_shared<NumberVariable>(value);\n"
    "}\n"
    "VariablePointer NumberVariable::toList(\n"
    "        const std::vector<VariableType> &subTypes) const {\n"
    "    return listHelper(\n"
    "        std::make_shared<NumberVariable>(value), subTypes\n"
    "    );\n"
    "}\n"
    "VariablePointer NumberVariable::toTuple(void) const {\n"
    "    return std::make_shared<TupleVariable>(std::make_pair(\n"
    "        std::make_shared<NumberVariable>(value),\n"
    "        std::make_shared<NumberVariable>(value)\n"
    "    ));\n"
    "}\n"
    "TupleVariable::TupleVariable(\n"
    "        const std::pair<VariablePointer, VariablePointer> &data) :\n"
    "        values(data) { }\n"
    "VariablePointer TupleVariable::toString(void) const {\n"
    "    return std::make_shared<StringVariable>(\n"
    "        \"(\" +\n"
    "            std::dynamic_pointer_cast<StringVariable>(\n"
    "                values.first->toString()\n"
    "            )->value + \", \"\n"
    "            + std::dynamic_pointer_cast<StringVariable>(\n"
    "                values.second->toString()\n"
    "            )->value + \")\"\n"
    "    );\n"
    "}\n"
    "VariablePointer TupleVariable::toNumber(void) const {\n"
    "    return std::make_shared<NumberVariable>(\n"
    "        std::dynamic_pointer_cast<NumberVariable>(\n"
    "            values.first->toNumber()\n"
    "        )->value\n"
    "    );\n"
    "}\n"
    "VariablePointer TupleVariable::toList(\n"
    "        const std::vector<VariableType> &subTypes) const {\n"
    "    return listHelper(\n"
    "        std::make_shared<TupleVariable>(values), subTypes\n"
    "    );\n"
    "}\n"
    "VariablePointer TupleVariable::toTuple(void) const {\n"
    "    return std::make_shared<TupleVariable>(TupleVariable(values));\n"
    "}\n"
    "ListVariable::ListVariable(\n"
    "        const std::vector<VariablePointer> &data) :\n"
    "        values(data) {\n"
    "}\n"
    "VariablePointer ListVariable::toString(void) const {\n"
    "    std::stringstream listStr;\n"
    "    listStr << \"{\";\n"
    "    for(const auto value : values) {\n"
    "        listStr <<\n"
    "            std::dynamic_pointer_cast<StringVariable>(\n"
    "                value->toString()\n"
    "            )->value;\n"
    "        if(value != *(values.begin())) {\n"
    "            listStr << \", \";\n"
    "        }\n"
    "    }\n"
    "    return std::make_shared<StringVariable>(listStr.str());\n"
    "}\n"
    "VariablePointer ListVariable::toNumber(void) const {\n"
    "    return std::make_shared<NumberVariable>(\n"
    "        std::dynamic_pointer_cast<NumberVariable>(\n"
    "            values[0]->toNumber()\n"
    "        )->value\n"
    "    );\n"
    "}\n"
    "VariablePointer ListVariable::toList(\n"
    "        const std::vector<VariableType> &subTypes) const {\n"
    "    return std::make_shared<ListVariable>(values);\n"
    "}\n"
    "VariablePointer ListVariable::toTuple(void) const {\n"
    "    return std::make_shared<TupleVariable>(std::make_pair(\n"
    "        std::make_shared<ListVariable>(values),\n"
    "        std::make_shared<ListVariable>(values)\n"
    "    ));\n"
    "}\n";
