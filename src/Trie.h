#pragma once
#include <unordered_map>
#include <string>

/**
 * A modified version of the Trie data-structure which is used to add
 * and retrieve mappings defined by the user. 
 **/
class Trie{
public:
    /**
     * Adds a macro mapping to the Trie, which can then be retrieved via getMapping.
     * 
     * @param mapFrom The part of the mapping which is used to trigger the macro,
     * e.g '\lambda'.
     * @param mapTo The part of the mapping which replaces the macro text e.g 'λ'.
     * @param excludes A pointer to an unordered set which represents the progam
     * names for which this macro is exluded from. E.g. we may want the mapping
     * '>=' -> '≥' to be exluded in {"IntelliJ IDE", "Visual Studio Code", "Vim"}
     * 
     **/
    void addString(const std::wstring& mapFrom, const std::wstring& mapTo,
        std::shared_ptr<std::unordered_set<std::wstring>> excludes){
        if (mapFrom.size() == 0){
            this->excludes = excludes;
            this->result = mapTo;
            return;
        }
        if (!mapping.contains(mapFrom.back())) mapping.emplace(mapFrom.back(),Trie());
        auto& x = mapping.at(mapFrom.back());
        x.addString( mapFrom.substr(0,mapFrom.size()-1), mapTo,excludes);
    }
    /**
     * Retrieves a macro mapping from the parameter buffer if one is available,
     * AND the user is not in an excluded program.
     * 
     * @tparam getProcessName The function which retrieves the name of the 
     * active window's process for the user's OS. Used to check if the user
     * is in an excluded program.
     * @param buffer The string buffer from which a macro trigger is scanned for.
     * @param index The index of the buffer from which to start scanning from. The
     * index is passed by reference and will be modified if a macro is found.
     * @return A pair, with the first element being the macro mapping, and the
     * second element being the length of the macro trigger. E.g. if the macro
     * trigger '\lambda' is found in the buffer, then the result will be 
     * ("λ",7). If no suitable macro is found, then the empty string accompanied 
     * with a 0 will be returned, i.e. ("",0).
     * 
     **/
    template <std::wstring (*getProcessName)(void)> 
    std::pair<std::wstring,int> getMapping(const std::wstring& buffer, int& index){
        Trie* trie = this;
        int backspaceLength=0;
        int i;
        for (i=index;i>=1;i--){
            if (!trie->mapping.contains(buffer[i-1])){
                break;
            }
            else{
                backspaceLength++;
                trie = &trie->mapping.at(buffer[i-1]);
            }
        }
        if (trie->result != L"" && !trie->excludes->contains(getProcessName())){
            index = i;
            return {trie->result,backspaceLength};
        }
        return {L"",0};
    }
private:
    std::shared_ptr<std::unordered_set<std::wstring>> excludes = nullptr;
    std::unordered_map<wchar_t,Trie> mapping;
    std::wstring result = L"";
};