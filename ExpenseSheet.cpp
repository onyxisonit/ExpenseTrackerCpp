#include "ExpenseSheet.h"

bool ExpenseSheet::Add(std::string_view label, double value)
{
    //see if expense exists with label
    Entry e;
    e.label = label;
    e.value = value;
    
    bool canAdd = std::find(m_entries.begin(), m_entries.end(), e) == m_entries.end(); 
    if (canAdd){
        m_entries.push_back(std::move(e)); //use move to save space *
    }
    return canAdd; //validate we have inserted
}

bool ExpenseSheet::Del(std::string_view label)
{
    Entry e;
    e.label = label;
    auto element = std::find(m_entries.begin(), m_entries.end(), e);

    if (element != m_entries.end()){ //if element is in entries, erase and return true
        m_entries.erase(element);
        return true;
    }

    return false;
}

void ExpenseSheet::List(std::ostream &os) const
{
    size_t index = 0;
    auto precision = os.precision();
    os.precision(3);
    for (const Entry& e : m_entries){
        os << "(" << index << ")" << e.label << " " << e.value << std::endl;
    }
    os.precision(precision); //restore old precision
}

double ExpenseSheet::Eval() const
{
    double value = 0.0;
    for (const Entry& e : m_entries){
        value += e.value;
    } 
    return value;
}
