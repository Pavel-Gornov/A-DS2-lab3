#ifndef HASH_MAP_H
#define HASH_MAP_H
#include "LinkedList.h"
#include <iostream>
#include <vector>

template<typename K ,typename V>
struct Pair {
        Pair(const K& key, const V& value): key(key), value(value) {};
        bool operator==(const Pair& other) const { return key == other.key && value == other.value; }
        K key = K();
        V value = V();
};

template<typename K ,typename V>
std::ostream& operator<<(std::ostream& os, const Pair<K, V>& p) {
        os << "{ " << p.key << " : " << p.value << " }";
        return os;
}

template<typename K ,typename V>
class HashMap {
friend std::ostream& operator<<<K, V>(std::ostream& os, const HashMap& map);
public:
    HashMap(size_t m = 0) {
        if (m > 0) {
            buckets_ = new LinkedList<Pair<K, V>>[m];
            m_ = m;
        }
    }

    ~HashMap() {
        delete[] buckets_;
    }

    HashMap(const HashMap& map) {
        this->buckets_ = new LinkedList<Pair<K, V>>[map.m_];
        this->size_ = map.size_;
        this->m_ = map.m_;
        for (size_t i = 0; i < map.m_; i++) {
            buckets_[i] = map.buckets_[i];
        }
    }

    HashMap& operator=(const HashMap& map) {
        if (&map != this) {
            delete[] this->buckets_;
            this->buckets_ = new LinkedList<Pair<K, V>>[map.m_];
            this->size_ = map.size_;
            this->m_ = map.m_;
            for (size_t i = 0; i < map.m_; i++) {
                buckets_[i] = map.buckets_[i];
            }
        }
        return *this;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void print() const {
        for (size_t i = 0; i < m_; i++) {
            std::cout << buckets_[i] << "\n";
        }
    }

    bool contains_value(const V& value) const {
        for (size_t i = 0; i < m_; i++) {
            auto it = buckets_[i].iterator();
            while (it.has_next()) {
                if (it.next().value == value) return true;
            }
        }
        return false;
    }

    bool contains_key(const K& key) const {
        if (this->empty()) return false;

        size_t idx = hash_(key);
        auto it = buckets_[idx].iterator();
        while (it.has_next()) {
            if (it.next().key == key) return true;
        }
        return false;
    }

    V* search(const K& key) const {
        size_t idx = hash_(key);
        auto it = buckets_[idx].iterator();
        while (it.has_next()) {
            Pair<K, V>& temp = it.next();
            if (temp.key == key) return &temp.value;
        }
        return nullptr;
    }
    
    size_t count(const K& key) const {
        size_t idx = hash_(key);
        return buckets_[idx].size() - 1;
    }

    /* Доступ по ключу */
    V operator[](const K& key) const {
        size_t idx = hash_(key);
        auto it = buckets_[idx].iterator();
        while (it.has_next()) {
            Pair<K, V>& temp = it.next();
            if (temp.key == key) return temp.value;
        }
        throw std::logic_error("Ошибка ключа!");
    }

    /* Вставка или изменение значения по ключу */
    V& operator[](const K& key) {
        if (load_factor_() > 2)
            this->expand_table_();

        size_t idx = hash_(key);
        auto it = buckets_[idx].iterator();
        while (it.has_next()) {
            Pair<K, V>& temp = it.next();
            if (temp.key == key) return temp.value;
        }
        Pair p = Pair(key, V());
        buckets_[idx].push_tail(p);
        size_++;

        return buckets_[idx][buckets_[idx].size() - 1].value;
    }

    void insert_or_assign(const K& key, V& value) {
        this->operator[](key) = value;
    }


    bool insert(const K& key, const V& value) {
        if (load_factor_() > 2)
            this->expand_table_();

        size_t idx = hash_(key);
        Pair p = Pair(key, value);

        if (this->contains_key(key)) return false;
        buckets_[idx].push_tail(p);
        size_++;
        return true;
    }

    bool erase(const K& key) {
        size_t idx = hash_(key);

        auto it = buckets_[idx].iterator();
        while (it.has_next()) {
            Pair p = it.next();
            if (p.key == key) {
                buckets_[idx].delete_node(p);
                size_--;
                return true;
            }
        }
        return false;
    }

    /* Дополнение */
    std::vector<K> keys() const {
        std::vector<K> res;
        res.reserve(this->m_);

        for (size_t i = 0; i < m_; i++) {
            auto it = buckets_[i].iterator();
            while (it.has_next()) {
                res.push_back(it.next().key);
            }
        }

        return res;
    }

private:
    LinkedList<Pair<K, V>>* buckets_ = nullptr;
    size_t m_ = 0;
    size_t size_ = 0;

    // TODO: сделать хоть чуть-чуть лучше
    inline static size_t hash(const K& key, size_t m) {
        return key % m;
    }
    size_t hash_(const K& key) const {
        return hash(key, m_);
    }

    inline double load_factor_() const {
        if (m_ == 0) return INFINITY;
        return ( (double) size_ / (double) m_);
    }

    void expand_table_() {
        size_t new_m = 2;
        if (m_ != 0) new_m = m_ * 2;
        
        LinkedList<Pair<K, V>>* new_buckets = new LinkedList<Pair<K, V>>[new_m];

        size_t new_idx = 0;
        for (size_t i = 0; i < m_; i++) {
            auto it = buckets_[i].iterator();
            while (it.has_next()) {
                Pair<K, V>& temp = it.next();
                new_idx = hash(temp.key, new_m);
                new_buckets[new_idx].push_tail(temp); // По-хорошему использовать std::move, но по условию нельзя :(
            }
        }
        
        delete[] buckets_;
        buckets_ = new_buckets;
        m_ = new_m;
    }
};

template<typename K ,typename V>
std::ostream& operator<<(std::ostream& os, const HashMap<K, V>& map) {
    for (size_t i = 0; i < map.m_; i++) {
        os << map.buckets_[i] << "\n";
    }
    return os;
}

#endif /* HASH_MAP_H */