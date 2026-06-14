import std;


// #include<iostream>
// #include<string.h>
using namespace std;
struct Value;

using ArrayValue = vector<Value>;
using ObjectValue = map<std::string, Value>;

struct Value {
  std::variant<std::nullptr_t, bool, double, std::string, ArrayValue, ObjectValue> v;

  Value() : v(nullptr) {}

  Value(std::nullptr_t) : v(nullptr)      {} 
  Value(bool v)         : v(v)            {}
  Value(double v)       : v(v)            {}
  Value(std::string v)  : v(std::move(v)) {}
  Value(ArrayValue v)   : v(std::move(v)) {}
  Value(ObjectValue v)  : v(std::move(v)) {}
};

class Serializer
{
  public:
    virtual ~Serializer() = default;
    virtual string serialize(const Value& value) = 0;
};

class JsonSerializer : public Serializer
{
  public:
    string normalize(string_view string)
    {
      ostringstream oss;
      oss << quoted(string);
      return oss.str();
    }
    void write(const Value& val, ostringstream& out)
    {
      if (holds_alternative<nullptr_t>(val.v))
      {
        out << "null";
      }
      else if (holds_alternative<bool>(val.v))
      {
        bool b = get<bool>(val.v);
        out << (b ? "true" : "false");
      }
      else if (holds_alternative<double>(val.v))
      {
        double d = get<double>(val.v);
        out << d;
      }
      else if (holds_alternative<string>(val.v))
      {
        string s = get<string>(val.v);
        out << normalize(s);
      }
      else if (holds_alternative<ArrayValue>(val.v))
      {
        ArrayValue arr = get<ArrayValue>(val.v);
        out << "[";
        for (size_t i = 0; i < arr.size(); ++i)
        {
          if (i > 0)
            out << ",";
          write(arr[i], out);
        }
        out << "]";
      }
      else if (holds_alternative<ObjectValue>(val.v))
      {
        ObjectValue obj = get<ObjectValue>(val.v);
        out << "{";
        bool first = true;
        for (const auto& [key, value] : obj) 
        {
            if (!first) 
              out << ",";
            first = false;
            out << normalize(key) << ":";
            write(value, out);
        }
        out << "}";
      }
    }
    string serialize(const Value& value) override {
      ostringstream result;
      write(value, result);  
      return result.str();
    }

};


class XmlSerializer : public Serializer
{
  private:
    string normalize(string_view str)
    {
      string result;
      for (char c: str)
      {
        if (c == '<') 
          result += "&lt;";
        else if (c == '>') 
          result += "&gt;";
        else if (c == '&') 
          result += "&amp;";
        else if (c == '"') 
          result += "&quot;";
        else if (c == '\'') 
          result += "&apos;";
        else 
          result += c;
      }
      return result;
    }
    public:
      void write(const Value& val, ostringstream& out, int indent = 0)
      {
        string spaces(indent, ' ');
        if (holds_alternative<nullptr_t>(val.v)) 
        {
            out << spaces << "<null/>\n";
        }
        else if (holds_alternative<bool>(val.v)) 
        {
            bool b = get<bool>(val.v);
            out << spaces << "<bool>" << (b ? "true" : "false") << "</bool>\n";
        }
        else if (holds_alternative<double>(val.v)) 
        {
            double d = get<double>(val.v);
            out << spaces << "<number>" << d << "</number>\n";
        }
        else if (holds_alternative<string>(val.v)) 
        {
            string s = get<string>(val.v);
            out << spaces << "<string>" << normalize(s) << "</string>\n";
        }
        else if (holds_alternative<ArrayValue>(val.v)) 
        {
            ArrayValue arr = get<ArrayValue>(val.v);
            out << spaces << "<array>\n";
            for (const auto& item : arr) {
                write(item, out, indent + 2);
            }
            out << spaces << "</array>\n";
        }
        else if (holds_alternative<ObjectValue>(val.v)) 
        {
            ObjectValue obj = get<ObjectValue>(val.v);
            out << spaces << "<object>\n";
            for (const auto& [key, value] : obj) {
                out << spaces << "  <" << normalize(key) << ">\n";
                write(value, out, indent + 4);
                out << spaces << "  </" << normalize(key) << ">\n";
            }
            out << spaces << "</object>\n";
        }
      }

      string serialize(const Value& value) override 
      {
        ostringstream result;
        result << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        result << "<root>\n";
        write(value, result, 2);
        result << "</root>\n";
        return result.str();
      }
};

class BinarySerializer : public Serializer 
{
  private:
    enum Type : uint8_t 
    {
        TYPE_NULL = 0,
        TYPE_BOOL = 1,
        TYPE_DOUBLE = 2,
        TYPE_STRING = 3,
        TYPE_ARRAY = 4,
        TYPE_OBJECT = 5
    };
    
    // Вспомогательные функции для записи
    
    // little-endian
    void writeUint32(vector<char>& data, uint32_t value) 
    {
        for (int i = 0; i < 4; ++i) 
        {
            data.push_back(static_cast<char>((value >> (i * 8)) & 0xFF));
        }
    }
    
    // Запись 8-байтового числа double
    void writeDouble(vector<char>& data, double value) {
        uint64_t bits;
        memcpy(&bits, &value, 8);  // копируем биты double в 64-битное число
        for (int i = 0; i < 8; ++i) {
            data.push_back(static_cast<char>((bits >> (i * 8)) & 0xFF));
        }
    }
    
    // Запись строки: [длина 4 байта] + [символы]
    void writeString(vector<char>& data, const string& str) {
        writeUint32(data, static_cast<uint32_t>(str.size()));
        data.insert(data.end(), str.begin(), str.end());
    }
    
    // Основная функция записи
    void write(const Value& val, vector<char>& data) 
    {
        if (holds_alternative<nullptr_t>(val.v)) 
        {
            data.push_back(TYPE_NULL);
        }
        else if (holds_alternative<bool>(val.v)) 
        {
            data.push_back(TYPE_BOOL);
            bool b = get<bool>(val.v);
            data.push_back(b ? 1 : 0);
        }
        else if (holds_alternative<double>(val.v)) 
        {
            data.push_back(TYPE_DOUBLE);
            double d = get<double>(val.v);
            writeDouble(data, d);
        }
        else if (holds_alternative<string>(val.v)) 
        {
            data.push_back(TYPE_STRING);
            string s = get<string>(val.v);
            writeString(data, s);
        }
        else if (holds_alternative<ArrayValue>(val.v)) 
        {
            data.push_back(TYPE_ARRAY);
            ArrayValue arr = get<ArrayValue>(val.v);
            writeUint32(data, static_cast<uint32_t>(arr.size()));
            for (const auto& item : arr) {
                write(item, data);
            }
        }
        else if (holds_alternative<ObjectValue>(val.v)) 
        {
            data.push_back(TYPE_OBJECT);
            ObjectValue obj = get<ObjectValue>(val.v);
            writeUint32(data, static_cast<uint32_t>(obj.size()));
            for (const auto& [key, value] : obj) {
                writeString(data, key);
                write(value, data);
            }
        }
    }
    
public:
    string serialize(const Value& value) override {
        vector<char> data;
        write(value, data);
        return string(data.begin(), data.end());
    }
};



int main()
{
  return 0;
}