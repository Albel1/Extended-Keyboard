#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include "Trie.h"
#include <codecvt>
#include <regex>
#include "ExcludeStatement.h"
#include <iostream>
/**
 * The utility class which parses all of the user-defined macros in the 
 * macros.txt file.
 **/ 
class MacroParser{
public:
    MacroParser(){
        parseMacros();
    }
    std::unordered_map<std::string,ExcludeStatement>& getExcludeStatements(){
        return excludeStatements;
    }
    const std::unordered_map<std::string,std::unordered_set<std::wstring>*>& getExcludesMap() const{
        return excludesMap;
    }
    const Trie& getTrie(){
        return trie;
    }
    /**
     * The method which parses all of the user-defined macros in the
     * macros.txt file.
     * 
     * @return The Trie which contains all of the mapping information
     * specified in macros.txt
     **/ 
    void parseMacros(){
        std::unordered_map<int,std::unordered_map<std::wstring,std::wstring> > macros;
        std::vector<std::wstring> lines;
        int lineIndex = 0;
        std::string groupName = "";
        std::shared_ptr<std::unordered_set<std::wstring>> excludes = nullptr;
        std::wstring line;
        std::wifstream macroFile("macros.txt");
        int lineNumber = 0;
        macroFile.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (!macroFile.is_open()){
            throw std::runtime_error("Could not open macros.txt");
        }
        while(getline(macroFile,line)){
            int index = 0;
            if (line == L""){
                
            }
            else if (checkPattern(L"\\s*GROUP\\s*(?=`)",line,index)){
                groupName = "";
                std::wstring wideGroupName = parseGroupName(line,index);
                std::transform(wideGroupName.begin(), wideGroupName.end(), std::back_inserter(groupName), 
                [] (wchar_t c) {
                    return (char)c;
                });
                assertPattern(L"\\s*",line,index);
                excludes = std::make_shared<std::unordered_set<std::wstring>>();
                excludesMap.emplace(groupName,excludes.get());
            }
            else if (checkPattern(L"\\s*EXCLUDE\\s+FROM\\s*\\[",line,index)){
                if (excludes == nullptr){
                    throw std::runtime_error("No group specified for exclude on line  "
                         + std::to_string(lineIndex) );
                }
                if (!excludes->empty()){
                    throw std::runtime_error("Repeated exclude for group on line "
                         + std::to_string(lineIndex) );
                }
                parseExclude(line,index,excludes.get());
                excludeStatements.emplace(groupName, ExcludeStatement{groupName,excludes.get(),lineIndex});
            }
            else if (checkPattern(L"\\s*(?=\")",line,index)){
                std::wstring mapFrom = parseQuote('"','"',line,index);
                assertPattern(L"\\s*-+>\\s*(?=\")" ,line,index);
                std::wstring mapTo = parseQuote('"','"',line,index);
                trie.addString(mapFrom,mapTo,excludes);
            }
            lineIndex++;
        }
        macroFile.close();
    }
private:
    std::unordered_map<std::string,ExcludeStatement> excludeStatements;
    std::unordered_map<std::string,std::unordered_set<std::wstring>*> excludesMap;
    Trie trie;
    static std::wstring parseGroupName(std::wstring& line,int& index){
        return parseQuote('`','`',line,index);
    }
    static std::wstring parseQuote(wchar_t start, wchar_t end, const std::wstring& line, int& index){
        std::wstring result;
        assertCharacter(start,line,index);
        while (index < line.size() && line[index]!=end){
            result += line[index];
            index++;
        }
        assertCharacter(end,line,index);
        return result;
        return L"";
    }
    static void parseExclude(const std::wstring& line,int& index,std::unordered_set<std::wstring>* excludes){
        if (checkPattern(L"\\s*\\]",line,index)){
            return;
        }
        while (true){
            excludes->emplace((parseQuote('"','"',line,index)));
            if (!checkPattern(L"\\s*,\\s*",line,index)){
                assertPattern(L"\\s*\\]",line,index);
                break;
            }
        }
    }
    static void assertString(const std::wstring& expected, const std::wstring& line, int& index){
        for (wchar_t character : expected){
            assertCharacter(character,line,index);
        }
    }
    static void assertCharacter(wchar_t expected, const std::wstring& line, int& index){
        if (line[index] != expected ){
            std::wcout <<"Parsing error on line:\n>"<< line 
                << "\nExpected '" <<  std::wstring(1,expected) 
                << "', but got '" << line[index] << "'";
            throw std::runtime_error("");
        }
        index++;
    }
    static void skipWhitespace(const std::wstring& line, int& index){
        while (index < line.size() && line[index] == ' ') index++;
    }
    static bool checkPattern(const std::wstring& pattern, const std::wstring& line, int& index){
        static std::wsmatch m;
        std::wregex r(pattern);
        std::wstring s = line.substr(index,line.size());
        std::regex_search(s, m, r);
        int pos = line.substr(0,index).size() + m.position();
        if (pos != index) return false;
        index += m.str().size();
        return true; 
    }
    static void assertPattern(const std::wstring& pattern, const std::wstring& line, int& index){
        if (!checkPattern(pattern,line,index)){
            std::wcout << L"Parsing error on line:\n>" << line << std::endl;
            throw std::runtime_error("\n");
        }
    }
};
