#include <iostream>

class Employee
{
public:
    Employee (const std::string& name , 
              const std::string& ssn);
    
    const std::string& get_name() const;
    void print(std::ostream& out) const;
    void print(std::ostream& out , 
               const std::string& msg) const;
protected:
    std::string m_name;
    std::string m_ssn;
};


class Manage : public Employee
{
public:
    Manage (const std::string& name , 
            const std::string& ssn ,
            const std::string& title);
    
    const std::string& title_name() const;
    void print(std::ostream& out) const;
private:
    std::string m_title;
};
