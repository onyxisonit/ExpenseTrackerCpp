#include <string>
#include <vector>

class ExpenseSheet{
    public:
        struct Entry{
            std::string label;
            double value = 0.0;
        };

    public:
        ExpenseSheet() = default;
        ExpenseSheet(const ExpenseSheet&) = default;

        ExpenseSheet& operator = (const ExpenseSheet&) = default;

    private:
        std::vector<Entry> m_entries;
};