import std;
using namespace std;

template<typename T>
class IEnumerator 
{
    public:
        virtual ~IEnumerator() = default;
        virtual bool MoveNext() = 0;
        virtual T Current() = 0;
};

template<typename T>
class IEnumerable 
{
    public:
        virtual ~IEnumerable() = default;
        virtual unique_ptr<IEnumerator<T>> GetEnumerator() = 0;
};

template<typename T>
class ICollection : public IEnumerable<T> 
{
    public:
        virtual int  Count()          = 0;
        virtual void Add(T item)      = 0;
        virtual void Clear()          = 0;
        virtual bool Contains(T item) = 0;
        virtual bool Remove(T item)   = 0;
};

template<typename T>
class List : public ICollection<T> 
{
    private:
        vector<T> data;
        
        class ListEnumerator : public IEnumerator<T> 
        {
            private:
                const vector<T>* vec;
                size_t index;
            public:
                ListEnumerator(const vector<T>* v) : vec(v), index(0) {}
                bool MoveNext() override { return index < vec->size(); }
                T Current() override { return (*vec)[index++]; }
        };
    
public:
    List() = default;
    List(initializer_list<T> init) : data(init) {}
    
    int Count() override { return static_cast<int>(data.size()); }
    
    void Add(T item) override { data.push_back(item); }
    
    void Clear() override { data.clear(); }
    
    bool Contains(T item) override 
    {
        return find(data.begin(), data.end(), item) != data.end();
    }
    
    bool Remove(T item) override 
    {
        auto it = find(data.begin(), data.end(), item);
        if (it != data.end()) 
        {
            data.erase(it);
            return true;
        }
        return false;
    }
    
    unique_ptr<IEnumerator<T>> GetEnumerator() override 
    {
        return make_unique<ListEnumerator>(&data);
    }
    
    int Capacity() const { return static_cast<int>(data.capacity()); }
    void SetCapacity(int capacity) { data.reserve(capacity); }
    
    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }
    
    void Insert(int index, T item) 
    {
        if (index < 0 || index > static_cast<int>(data.size())) 
        {
            throw out_of_range("Index out of range");
        }
        data.insert(data.begin() + index, item);
    }
    
    void RemoveAt(int index) 
    {
        if (index < 0 || index >= static_cast<int>(data.size())) 
        {
            throw out_of_range("Index out of range");
        }
        data.erase(data.begin() + index);
    }
};

template<typename TKey, typename TValue>
struct KeyValuePair 
{
    TKey Key;
    TValue Value;
    
    KeyValuePair() = default;
    KeyValuePair(const TKey& key, const TValue& value) : Key(key), Value(value) {}
};

template<typename T, typename Hash = hash<T>, typename KeyEqual = equal_to<T>>
class HashSet : public ICollection<T> 
{
private:
    struct Node 
    {
        T value;
        bool occupied;
        Node() : occupied(false) {}
        Node(const T& v) : value(v), occupied(true) {}
    };
    
    vector<Node> buckets;
    int elementCount;
    Hash hasher;
    KeyEqual equaler;
    
    size_t GetBucketIndex(const T& item) const 
    {
        if (buckets.empty()) return 0;
        return hasher(item) % buckets.size();
    }
    
    void Rehash(int newCapacity) 
    {
        vector<Node> oldBuckets = move(buckets);
        buckets.clear();
        buckets.resize(newCapacity);
        elementCount = 0;
        
        for (const auto& node : oldBuckets) 
        {
            if (node.occupied) 
            {
                Add(node.value);
            }
        }
    }
    
public:
    HashSet() : elementCount(0)
    {
        buckets.resize(16);
    }
    
    explicit HashSet(int capacity) : elementCount(0) 
    {
        buckets.resize(capacity > 0 ? capacity : 16);
    }
    
    HashSet(const Hash& hash, const KeyEqual& equal) 
        : hasher(hash), equaler(equal), elementCount(0) 
    {
        buckets.resize(16);
    }
    
    int Count() override { return elementCount; }
    
    void Add(T item) override 
    {
        if (Contains(item)) return;
        
        if (elementCount >= static_cast<int>(buckets.size()) * 0.75) 
        {
            Rehash(static_cast<int>(buckets.size() * 2));
        }
        
        size_t index = GetBucketIndex(item);
        while (buckets[index].occupied) 
        {
            index = (index + 1) % buckets.size();
        }
        buckets[index] = Node(item);
        elementCount++;
    }
    
    void Clear() override 
    {
        for (auto& node : buckets) 
        {
            node.occupied = false;
        }
        elementCount = 0;
    }
    
    bool Contains(T item) override 
    {
        if (buckets.empty()) return false;
        size_t index = GetBucketIndex(item);
        size_t start = index;
        
        while (buckets[index].occupied) 
        {
            if (equaler(buckets[index].value, item)) 
            {
                return true;
            }
            index = (index + 1) % buckets.size();
            if (index == start) break;
        }
        return false;
    }
    
    bool Remove(T item) override 
    {
        if (buckets.empty()) return false;
        size_t index = GetBucketIndex(item);
        size_t start = index;
        
        while (buckets[index].occupied) 
        {
            if (equaler(buckets[index].value, item)) 
            {
                buckets[index].occupied = false;
                elementCount--;
                return true;
            }
            index = (index + 1) % buckets.size();
            if (index == start) break;
        }
        return false;
    }
    
    unique_ptr<IEnumerator<T>> GetEnumerator() override 
    {
        vector<T> items;
        for (const auto& node : buckets) 
        {
            if (node.occupied) 
            {
                items.push_back(node.value);
            }
        }
        
        class HashSetEnumerator : public IEnumerator<T> 
        {
            vector<T> data;
            size_t index;
            public:
                HashSetEnumerator(const vector<T>& v) : data(v), index(0) {}
                bool MoveNext() override { return index < data.size(); }
                T Current() override { return data[index++]; }
        };
        
        return make_unique<HashSetEnumerator>(items);
    }
    
    int Capacity() const { return static_cast<int>(buckets.size()); }
    void SetCapacity(int capacity) 
    {
        if (capacity > static_cast<int>(buckets.size())) 
        {
            Rehash(capacity);
        }
    }
};

template<typename TKey, typename TValue, 
         typename Hash = hash<TKey>, 
         typename KeyEqual = equal_to<TKey>>
class Dictionary : public ICollection<KeyValuePair<TKey, TValue>> 
{
    private:
        struct Node 
        {
            TKey key;
            TValue value;
            bool occupied;
            Node() : occupied(false) {}
            Node(const TKey& k, const TValue& v) : key(k), value(v), occupied(true) {}
        };
        
        vector<Node> buckets;
        int elementCount;
        Hash hasher;
        KeyEqual equaler;
        
        size_t GetBucketIndex(const TKey& key) const 
        {
            if (buckets.empty()) return 0;
            return hasher(key) % buckets.size();
        }
        
        int FindIndex(const TKey& key) const 
        {
            if (buckets.empty()) return -1;
            size_t index = GetBucketIndex(key);
            size_t start = index;
            
            while (buckets[index].occupied) 
            {
                if (equaler(buckets[index].key, key)) 
                {
                    return static_cast<int>(index);
                }
                index = (index + 1) % buckets.size();
                if (index == start) break;
            }
            return -1;
        }
        
        void Rehash(int newCapacity) 
        {
            vector<Node> oldBuckets = move(buckets);
            buckets.clear();
            buckets.resize(newCapacity);
            elementCount = 0;
            
            for (const auto& node : oldBuckets) 
            {
                if (node.occupied) 
                {
                    Add(KeyValuePair<TKey, TValue>(node.key, node.value));
                }
            }
        }
        
public:
    Dictionary() : elementCount(0) 
    {
        buckets.resize(16);
    }
    
    explicit Dictionary(int capacity) : elementCount(0) 
    {
        buckets.resize(capacity > 0 ? capacity : 16);
    }
    
    int Count() override { return elementCount; }
    
    void Add(KeyValuePair<TKey, TValue> item) override 
    {
        int idx = FindIndex(item.Key);
        if (idx != -1) 
        {
            throw runtime_error("Key already exists");
        }
        
        if (elementCount >= static_cast<int>(buckets.size()) * 0.75) 
        {
            Rehash(static_cast<int>(buckets.size() * 2));
        }
        
        size_t index = GetBucketIndex(item.Key);
        while (buckets[index].occupied) 
        {
            index = (index + 1) % buckets.size();
        }
        buckets[index] = Node(item.Key, item.Value);
        elementCount++;
    }
    
    void Clear() override 
    {
        for (auto& node : buckets) 
        {
            node.occupied = false;
        }
        elementCount = 0;
    }
    
    bool Contains(KeyValuePair<TKey, TValue> item) override 
    {
        int idx = FindIndex(item.Key);
        if (idx != -1) 
        {
            return buckets[idx].value == item.Value;  
        }
        return false;
    }
    
    bool ContainsKey(const TKey& key) const 
    {
        return FindIndex(key) != -1;
    }
    
    bool Remove(KeyValuePair<TKey, TValue> item) override 
    {
        int idx = FindIndex(item.Key);
        if (idx != -1 && buckets[idx].value == item.Value) 
        {
            buckets[idx].occupied = false;
            elementCount--;
            return true;
        }
        return false;
    }
    
    bool Remove(const TKey& key) 
    {
        int idx = FindIndex(key);
        if (idx != -1) 
        {
            buckets[idx].occupied = false;
            elementCount--;
            return true;
        }
        return false;
    }
    
    unique_ptr<IEnumerator<KeyValuePair<TKey, TValue>>> GetEnumerator() override 
    {
        vector<KeyValuePair<TKey, TValue>> items;
        for (const auto& node : buckets) 
        {
            if (node.occupied) {
                items.push_back(KeyValuePair<TKey, TValue>(node.key, node.value));
            }
        }
        
        class DictionaryEnumerator : public IEnumerator<KeyValuePair<TKey, TValue>> 
        {
            vector<KeyValuePair<TKey, TValue>> data;
            size_t index;
            public:
                DictionaryEnumerator(const vector<KeyValuePair<TKey, TValue>>& v) 
                    : data(v), index(0) {}
                bool MoveNext() override { return index < data.size(); }
                KeyValuePair<TKey, TValue> Current() override { return data[index++]; }
        };
        
        return make_unique<DictionaryEnumerator>(items);
    }
    
    int Capacity() const { return static_cast<int>(buckets.size()); }
    void SetCapacity(int capacity) 
    {
        if (capacity > static_cast<int>(buckets.size())) 
        {
            Rehash(capacity);
        }
    }
    
    TValue& operator[](const TKey& key) 
    {
        int idx = FindIndex(key);
        if (idx != -1) 
        {
            return buckets[idx].value;
        }
        
        Add(KeyValuePair<TKey, TValue>(key, TValue()));
        idx = FindIndex(key);
        return buckets[idx].value;
    }
};

template<typename T>
class Stack : public IEnumerable<T> 
{
private:
    vector<T> data;
    
    class StackEnumerator : public IEnumerator<T> {
    private:
        const vector<T>* vec;
        int index;
    public:
        StackEnumerator(const vector<T>* v) : vec(v), index(static_cast<int>(v->size()) - 1) {}
        bool MoveNext() override { return index >= 0; }
        T Current() override { return (*vec)[index--]; }
    };
    
public:
    Stack() = default;
    
    int Count() const { return static_cast<int>(data.size()); }
    
    void Push(T item) { data.push_back(item); }
    
    T Peek() {
        if (data.empty()) throw runtime_error("Stack is empty");
        return data.back();
    }
    
    T Pop() {
        if (data.empty()) throw runtime_error("Stack is empty");
        T item = data.back();
        data.pop_back();
        return item;
    }
    
    unique_ptr<IEnumerator<T>> GetEnumerator() override {
        return make_unique<StackEnumerator>(&data);
    }
};

template<typename T>
class Queue : public IEnumerable<T> {
    private:
        vector<T> data;
        size_t front;
        
        class QueueEnumerator : public IEnumerator<T> 
        {
        private:
            const vector<T>* vec;
            size_t frontPos;
            size_t current;
        public:
            QueueEnumerator(const vector<T>* v, size_t f) 
                : vec(v), frontPos(f), current(f) {}
            bool MoveNext() override { return current < vec->size(); }
            T Current() override { return (*vec)[current++]; }
        };
        
    public:
        Queue() : front(0) {}
        
        int Count() const { return static_cast<int>(data.size() - front); }
        
        void Enqueue(T item) { data.push_back(item); }
        
        T Peek() 
        {
            if (front >= data.size()) throw runtime_error("Queue is empty");
            return data[front];
        }
        
        T Dequeue() 
        {
            if (front >= data.size()) throw runtime_error("Queue is empty");
            T item = data[front++];
            
            // Оптимизация: если слишком много пустого места
            if (front > data.size() / 2) 
            {
                data.erase(data.begin(), data.begin() + static_cast<int>(front));
                front = 0;
            }
            return item;
        }
        
        unique_ptr<IEnumerator<T>> GetEnumerator() override 
        {
            return make_unique<QueueEnumerator>(&data, front);
        }
};

int main() 
{

    return 0;
}