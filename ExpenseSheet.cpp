#include "ExpenseSheet.h"

//serialize Entry objects to stream
void ExpenseSheet::Entry::Serialize(std::ostream &os) const
{
    //write label and value fields of Entry object to os
    os.write(label.c_str(), label.length() + 1);
    os.write((char*)&value, sizeof(double));
}

//deserialize Entry objects from stream
void ExpenseSheet::Entry::Deserialize(std::istream &is)
{
    std::stringstream ss;
    char c;
    //read serialize representation from is
    do{
        is.read(&c, 1);
        ss << c;
    } while (c != '\0');

    is.read((char*)&value, sizeof(double));
    label = ss.str();
    
}

bool ExpenseSheet::Save(std::filesystem::path &dataFile) const
{
    // //make sure all directories are created
    // std::filesystem::create_directories(dataFile.parent_path());
    
    // std::ofstream fileOut(dataFile,  std::ios::out |  std::ios::trunc| std::ios::binary);
    // if (fileOut.is_open()){
    //     for (const Entry& e : m_entries){
    //         e.Serialize(fileOut);
    //     }
    //     return true; 
    // }
    
    // return false;   

    //create directories if they don't exist
    auto path = dataFile;
    path.remove_filename(); //remove filename to ensure only directories are created (file created seperatelyt by ofstream constructor)
    std::filesystem::create_directories(path);

    std::ofstream fileOut(dataFile,  std::ios::out |  std::ios::trunc| std::ios::binary); //open file for writing
    if (fileOut.is_open()){
        //write number of entries to file
        size_t count = m_entries.size();
        fileOut.write((char*)&count, sizeof(size_t));

        //serialize each entry to file
        for (const Entry& e : m_entries){
            e.Serialize(fileOut);
        }
        return true; 
    }
    
    return false;

}

bool ExpenseSheet::Load(std::filesystem::path &dataFile)
{
    std::ifstream fileIn(dataFile, std::ios::in | std::ios::binary);
    Entry e;
    // while (is){
    //     e.Deserialize(is);
    //     m_entries.push_back(std::move(e));
    // }

    if (fileIn.is_open()){
        //read number of entries
        size_t count;
        fileIn.read((char*)&count, sizeof(size_t));
        
        m_entries.clear(); //clear all elements out of vector before we read 
        m_entries.reserve(count);
        for (size_t i = 0; i < count; i++){
            e.Deserialize(fileIn);
            m_entries.push_back(std::move(e));
        }
        return true;
    }
    return false;
}



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
    // auto precision = os.precision();
    // os.precision(3);
    for (const Entry& e : m_entries){
        os << "(" << index++ << ")" << e.label << " " << e.value << std::endl;
    }
    // os.precision(precision); //restore old precision
}

double ExpenseSheet::Eval() const
{
    double value = 0.0;
    for (const Entry& e : m_entries){
        value += e.value;
    } 
    return value;
}


