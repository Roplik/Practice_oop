import std;
using namespace std;


class String {
private:
    char* data_;
    size_t size_;

    void free() 
    {
        delete[] data_;
    }

    void copy_from(const char* str) 
    {
        if (str) 
        {
            size_ = strlen(str);
            data_ = new char[size_ + 1];
            strcpy(data_, str);
        } 
        else 
        {
            data_ = nullptr;
            size_ = 0;
        }
    }

public:
    
    String() : data_(nullptr), size_(0) {}

    
    String(const char* str) : data_(nullptr), size_(0) 
    {
        copy_from(str);
    }

    
    String(const String& other) : data_(nullptr), size_(0) 
    {
        copy_from(other.data_);
    }

    
    String(String&& other) noexcept : data_(other.data_), size_(other.size_) 
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    
    ~String() 
    {
        free();
    }

    void swap(String& other) noexcept 
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    String& operator=(const String& other) 
    {
        String(other).swap(*this);  
        return *this;
    }

    
    String& operator=(String&& other) noexcept 
    {
        String(std::move(other)).swap(*this);  
        //other.swap(*this);
        return *this;
    }
    
    
    size_t size() const 
    {
        return size_;
    }

    
    char& operator[](size_t index) 
    {
        return data_[index];
    }

    const char& operator[](size_t index) const 
    {
        return data_[index];
    }

    
    const char* c_str() const 
    {
        return data_ ? data_ : "";
    } 
};

std::ostream& operator<<(std::ostream& os, const String& s) 
{
    os << s.c_str();
    return os;
}

void swap(String& a, String& b) noexcept 
{
    a.swap(b);
}
template<>
struct std::formatter<String> : std::formatter<const char*> 
{
    auto format(const String& s, format_context& ctx) const {
        return std::formatter<const char*>::format(s.c_str(), ctx);
    }
};

int main() 
{
   
    return 0;
}