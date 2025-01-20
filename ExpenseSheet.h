#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <filesystem>
#include <fstream>

#include <string_view>

class ExpenseSheet{
    public:
        struct Entry{
            std::string label;
            double value = 0.0;

            void Serialize(std::ostream& os) const;
            void Deserialize(std::istream& is);

            inline bool operator==(const Entry& rhs) const{
                return label == rhs.label;
            }
        };

    public:
        ExpenseSheet() = default;
        ExpenseSheet(const ExpenseSheet&) = default;

        ExpenseSheet& operator = (const ExpenseSheet&) = default;

        //functions to load and save to data file
        bool Load(std::filesystem::path& dataFile);
        bool Save(std::filesystem::path& dataFile) const;
        //factor out argument parsing from actual expenses itself
        bool Add(std::string_view label, double amt);
        bool Del(std::string_view label);
        void List(std::ostream& os) const; //write to arbitrary ostream instead of std out
        double Eval() const;

    private:
        std::vector<Entry> m_entries;
};