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
        bool Load(const std::filesystem::path& dataFile);
        bool Save(const std::filesystem::path& dataFile = "");

        //factor out argument parsing from actual expenses itself
        bool Add(std::string_view label, double amt);
        bool Del(std::string_view label);
        void List(std::ostream& os) const; //write to arbitrary ostream instead of std out
        double Eval() const;
        void Clear();

        //NOTE: can implement interfaces later but will bring eprfromance down 
        bool ExportCSV(const std::filesystem::path& dataFile) const;
        bool ExportHTML(const std::filesystem::path& dataFile) const;

        bool HasUnsavedChanges() const {
            return m_unsavedChanges;
        }
    
    private:
        static inline std::string HTMLText(double value){
            return value >= 0.0 ? "text-success" : "text-danger";
        }

    private:
        std::filesystem::path m_dataFile;
        std::vector<Entry> m_entries;
        bool m_unsavedChanges;
};