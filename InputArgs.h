#include <string>
#include <vector>
#include <string_view>
#include <sstream>

class InputArgs{
    public:
    //big three
        InputArgs() = default;
        InputArgs(const InputArgs&) = default;
        InputArgs(std::string_view line); //custom constructor to parse input
        InputArgs& operator = (const InputArgs&) = default;

        InputArgs Subset(size_t offset, size_t count = std::numeric_limits<size_t>::max()); //offset:where we start (produce subset of args); count:how many elements should be transferred at max

        inline size_t Count() const {return m_args.size();} //get argument count
        inline const std::string& Get(size_t idx) const {return m_args[idx];} 
        inline const std::string& operator[](size_t idx) const {return Get(idx);}

    private:
        std::vector<std::string> m_args;
};