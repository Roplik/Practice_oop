#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>


#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>  
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/optional.hpp>

struct Contact 
{
    std::string name;
    std::string phone;
    std::string email;
    std::string address;
    
    bool operator==(const Contact& other) const 
    {
        return name == other.name && 
               phone == other.phone && 
               email == other.email &&
               address == other.address;
    }
    template<class Archive>
    void serialize(Archive& archive) 
    {
        archive(
            CEREAL_NVP(name),
            CEREAL_NVP(phone),
            CEREAL_NVP(email),
            CEREAL_NVP(address)
        );
    }
};

class ContactManager {
    public:
        // Добавление контакта
        void addContact(const Contact& contact)
        {
            m_contacts.push_back(contact);
        }
        void addContact(const std::string& name, 
                        const std::string& phone,
                        const std::string& email,
                        const std::string& address = "")
        {
            m_contacts.push_back({name, phone, email, address});
        }
        
        // Поиск
        std::optional<Contact> findByName(const std::string& name) const
        {
            auto it = std::find_if(m_contacts.begin(), m_contacts.end(),
            [&name](const Contact& c) 
            {
                return c.name == name;
            });
            
            if (it != m_contacts.end()) 
            {
                return *it;
            }
            return std::nullopt;
        }

        std::vector<Contact> findByPhone(const std::string& phone) const
        {
            std::vector<Contact> result;
            std::copy_if(m_contacts.begin(), m_contacts.end(), std::back_inserter(result),
                [&phone](const Contact& c) 
                {
                    return c.phone == phone;
                });
            return result;
        }
        std::vector<Contact> findByEmail(const std::string& email) const
        {
            std::vector<Contact> result;
            std::copy_if(m_contacts.begin(), m_contacts.end(), std::back_inserter(result),
                [&email](const Contact& c) 
                {
                    return c.email == email;
                });
            return result;
        }
        std::vector<Contact> search(const std::string& query) const
        {
            std::vector<Contact> result;
            std::string lowerQuery = query;
            std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
            
            std::copy_if(m_contacts.begin(), m_contacts.end(), std::back_inserter(result),
            [&lowerQuery](const Contact& c) 
            {
            std::string name = c.name;
            std::string phone = c.phone;
            std::string email = c.email;
            std::string address = c.address;
            
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            std::transform(phone.begin(), phone.end(), phone.begin(), ::tolower);
            std::transform(email.begin(), email.end(), email.begin(), ::tolower);
            std::transform(address.begin(), address.end(), address.begin(), ::tolower);
            
            return name.find(lowerQuery) != std::string::npos ||
                   phone.find(lowerQuery) != std::string::npos ||
                   email.find(lowerQuery) != std::string::npos ||
                   address.find(lowerQuery) != std::string::npos;
            });
            return result;
        }
        
        bool removeByName(const std::string& name) 
        {
            auto it = std::find_if(m_contacts.begin(), m_contacts.end(),
                [&name](const Contact& c) { return c.name == name; });
            
            if (it != m_contacts.end()) 
            {
                m_contacts.erase(it);
                return true;
            }
            return false;
        }

        bool removeByPhone(const std::string& phone) 
        {
            auto it = std::find_if(m_contacts.begin(), m_contacts.end(),
                [&phone](const Contact& c) { return c.phone == phone; });
            
            if (it != m_contacts.end()) 
            {
                m_contacts.erase(it);
                return true;
            }
            return false;
        }

        size_t removeAll() 
        {
            size_t oldSize = m_contacts.size();
            m_contacts.clear();
            return oldSize;
        }

        
        const std::vector<Contact>& getAll() const { return m_contacts; }
        size_t size() const { return m_contacts.size(); }
        bool empty() const { return m_contacts.empty(); }
        
        void sortByName() 
        {
            std::sort(m_contacts.begin(), m_contacts.end(),
                [](const Contact& a, const Contact& b) 
                {
                    return a.name < b.name;
                });
        }

        void sortByPhone() 
        {
            std::sort(m_contacts.begin(), m_contacts.end(),
                [](const Contact& a, const Contact& b) 
                {
                    return a.phone < b.phone;
                });
        }
        
        // ===== СЕРИАЛИЗАЦИЯ CEREAL =====
        
        // Сохранение в бинарный файл
        void saveBinary(const std::string& filename) const 
        {
            std::ofstream file(filename, std::ios::binary);
            if (!file.is_open()) 
            {
                throw std::runtime_error("Cannot open file for writing: " + filename);
            }
            
            {
                cereal::BinaryOutputArchive archive(file);
                archive(m_contacts);
            }
        }

        void loadBinary(const std::string& filename) 
        {
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open()) 
            {
                throw std::runtime_error("Cannot open file for reading: " + filename);
            }
            
            {
                cereal::BinaryInputArchive archive(file);
                archive(m_contacts);
            }
        }
        
        
        
        // Сериализация в строку (для тестов)
        std::string serializeToString() const 
        {
            std::stringstream ss;
            {
                cereal::BinaryOutputArchive archive(ss);
                archive(m_contacts);
            }
            return ss.str();
        }

        void deserializeFromString(const std::string& data) 
        {
            std::stringstream ss(data);
            {
                cereal::BinaryInputArchive archive(ss);
                archive(m_contacts);
            }
        }
        

    void saveJson(const std::string& filename) const 
    {
        std::ofstream file(filename);
        if (!file.is_open()) 
        {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        
        {
            cereal::JSONOutputArchive archive(file);
            archive(cereal::make_nvp("contacts", m_contacts));
        }
    }

    void loadJson(const std::string& filename) 
    {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            throw std::runtime_error("Cannot open file for reading: " + filename);
        }
        
        {
            cereal::JSONInputArchive archive(file);
            archive(cereal::make_nvp("contacts", m_contacts));
        }
    }

    private:
        std::vector<Contact> m_contacts;
};

int main()
{
    Contact first  = {"Yarik", "88005553535", "ilovecats@mail.ru", "Samara"};
    Contact second = {"Yarik", "88005553535", "ilovecats@mail.ru", "Samara"};
    
    // Демонстрация работы менеджера
    ContactManager manager;
    manager.addContact(first);
    manager.addContact({"Bob", "+123456789", "bob@mail.com", "NY"});
    
    std::cout << "\nAll contacts (" << manager.size() << "):" << std::endl;
    for (const auto& contact : manager.getAll()) 
    {
        std::cout << "  Name: " << contact.name 
                  << ", Phone: " << contact.phone << std::endl;
    }
    
    // Поиск
    auto found = manager.findByName("Yarik");
    if (found.has_value()) 
    {
        std::cout << "\nFound Yarik: " << found->email << std::endl;
    }
    
    // Сохранение в файл
    manager.saveBinary("contacts.bin");
    manager.saveJson("contacts.json");
    std::cout << "\nSaved to files!" << std::endl;
    
    return 0;

}