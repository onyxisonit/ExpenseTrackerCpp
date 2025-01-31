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
        if (c != '\0'){
            ss << c;
        }
    } while (c != '\0');

    is.read((char*)&value, sizeof(double));
    label = ss.str();
    
}

bool ExpenseSheet::Save(const std::filesystem::path &dataFile)
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
    auto p = dataFile;
    if (dataFile.empty()){
        if (m_dataFile.empty()){
           return false;
        } else {
            p = m_dataFile;
        }
    }
    //create directories if they don't exist
    auto path = p;
    path.remove_filename(); //remove filename to ensure only directories are created (file created seperatelyt by ofstream constructor)
    if (!path.empty()){
        std::filesystem::create_directories(path);
    }
 
    p.replace_extension(".dat");
    std::ofstream fileOut(p, std::ios::out |  std::ios::trunc| std::ios::binary); //open file for writing
    if (fileOut.is_open()){
        //write number of entries to file
        size_t count = m_entries.size();
        fileOut.write((char*)&count, sizeof(size_t));

        //serialize each entry to file
        for (const Entry& e : m_entries){
            e.Serialize(fileOut);
        }
        m_unsavedChanges = false;
        return true; 
    }
    
    return false;

}

bool ExpenseSheet::Load(const std::filesystem::path &dataFile)
{
    auto path = dataFile;
    path.replace_extension(".dat");
    std::ifstream fileIn(path, std::ios::in | std::ios::binary);
   
    // while (is){
    //     e.Deserialize(is);
    //     m_entries.push_back(std::move(e));
    // }

    if (fileIn.is_open()){
        m_dataFile = path;
        //read number of entries
        size_t count;
        fileIn.read((char*)&count, sizeof(size_t));
        
        m_entries.clear(); //clear all elements out of vector before we read 
        // m_entries.reserve(count);
        for (size_t i = 0; i < count; i++){
            Entry e;
            e.Deserialize(fileIn);
            m_entries.push_back(std::move(e));
        }
        m_unsavedChanges = false;
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
        m_unsavedChanges = true;
    }
    return canAdd; //validate we have inserted
}

bool ExpenseSheet::Del(std::string_view label)
{
    //remove quotation marks before assigning to Entry object
    std::string labelStr(label);
    if (labelStr.size() >= 2 && labelStr[0] == '"' && labelStr[labelStr.size() - 1] == '"') {
        labelStr = labelStr.substr(1, labelStr.size() - 2);
    }
    
    Entry e;
    e.label = labelStr;
    auto element = std::find(m_entries.begin(), m_entries.end(), e);

    if (element != m_entries.end()){ //if element is in entries, erase and return true
        m_entries.erase(element);
        m_unsavedChanges = true;
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

void ExpenseSheet::Clear()
{
    m_dataFile = "";
    m_entries.clear();
    m_unsavedChanges = true;
}

bool ExpenseSheet::ExportCSV(const std::filesystem::path& dataFile) const
{
    auto xpath = dataFile;
    xpath.replace_extension(".csv");

    auto path = xpath;
    path.remove_filename();
    if (!path.empty()){
        std::filesystem::create_directories(path);
    }
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc); //open file for writing
    if (fileOut.is_open()){
        fileOut << "Position,Label,Value" << "\n";
        size_t index = 0;
        for (const Entry& e : m_entries){
            fileOut << ++index << "," <<e.label << "," << e.value << "\n";
        }
        return true;
    }
    return false;
}

bool ExpenseSheet::ExportHTML(const std::filesystem::path& dataFile) const
{
    auto xpath = dataFile;
    xpath.replace_extension(".html");

    auto path = xpath;
    path.remove_filename();
    if (!path.empty()){
        std::filesystem::create_directories(path);
    }
    std::ofstream fileOut(xpath, std::ios::out | std::ios::trunc); //open file for writing
    if (fileOut.is_open()){
        fileOut << 
R"(<!doctype html>
<html lang="en" data-bs-theme="dark">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>)" << m_dataFile.filename().string() << R"(</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH" crossorigin="anonymous">
  </head>
  <body>
    <nav class="navbar bg-body-tertiary">
    <div class="container-fluid">
        <span class="navbar-brand mb-0 h1 fw-bold fs-2">Expenses: )" << m_dataFile.stem().string() << R"(</span>
    </div>
    </nav>
    <div class="container mt-2">
        <div class="card">
            <div class="card-body">
            <table class="table">
                <thead>
                    <tr>
                        <th scope="col" class="fs-4">Position</th>
                        <th scope="col" class="fs-4">Label</th>
                        <th scope="col" class="fs-4">Value</th>
                    </tr>
                </thead>
                <tbody>)"; 

        size_t index = 0;   
        for (const Entry& e : m_entries){
            fileOut << "<tr><th scope=\"row\">" << ++index << "</th>"
            << "<td>" << e.label << "</td>"
            << "<td class=\"" << HTMLText(e.value) << "\">" << e.value << "</td></tr>";
        }
        fileOut << 
R"(
                </tbody>
            </table>
            <p class="fs-4 )" << HTMLText(Eval()) << R"(">Total: )" << Eval() << R"(</p>
        </div>
    </div>
</div>
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-YvpcrYf0tY3lHB60NNkmXc5s9fDVZLESaAA55NDzOxhy9GkcIdslK1eN7N6jIeHz" crossorigin="anonymous"></script>
  </body>
</html>)";
        
        return true;
    }
    return false;
}
