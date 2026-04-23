# ifndef LINKED_LIST_H
# define LINKED_LIST_H
#include <ostream>
#include <iostream>


template<typename T>
class LinkedList {
friend std::ostream& operator<<<T>(std::ostream& os, const LinkedList& list);
private:
    struct Node {
        Node(T value): value(value), prev(nullptr), next(nullptr) {}
        Node* prev;
        T value;
        Node* next;
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    size_t size_ = 0;
public:
    class ListIterator {
        Node* cur_ = nullptr;
        ListIterator(Node* head): cur_(head) {};
        friend LinkedList;
    public:
        bool has_next() const { return cur_; }
        T& next() {
            T& v = cur_->value;
            cur_ = cur_->next;
            return v;
        }
    };

    LinkedList() {}

    LinkedList(const LinkedList& list) {
        this->push_tail(list);
    }

    LinkedList& operator=(const LinkedList& list) {
        if (&list != this) {
            Node* temp = head_;
            while (head_) {
                temp = head_;
                head_ = head_->next;
                delete temp;
            }
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
            this->push_tail(list);
        }
        return *this;
    }

    ~LinkedList() {
        Node* temp = head_;
        while (head_) {
            temp = head_;
            head_ = head_->next;
            delete temp;
        }
        //std::cout << "LinkedList (" << size_ << ") destroyed\n";
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    ListIterator iterator() { return ListIterator(head_); }

    void push_head(T value) {
        Node* node = new Node(value);
        size_++;
        if (!head_) {
            head_ = node; 
            tail_ = node;
            return;
        }
        head_->prev = node;
        node->next = head_;
        head_ = node;
    }

    void push_tail(T value) {
        Node* node = new Node(value);
        size_++;
        if (!head_) {
            head_ = node; 
            tail_ = node;
            return;
        }
        tail_->next = node;
        node->prev = tail_;
        tail_ = node;
    }

    void push_head(const LinkedList& list) {
        Node* it = list.tail_;
        while (it) {
            this->push_head(it->value);
            it = it->prev;
        }
    }

    void push_tail(const LinkedList& list) {
        Node* it = list.head_;
        while (it) {
            this->push_tail(it->value);
            it = it->next;
        }
    }

    void pop_head() {
        if (head_) {
            Node* temp = head_;
            if (head_->next)
                head_->next->prev = nullptr;
            head_ = head_->next;
            delete temp;
            size_--;
        } 
    }

    void pop_tail() {
        if (tail_) {
            Node* temp = tail_;
            if(tail_->prev)
                tail_->prev->next = nullptr;
            tail_ = tail_->prev;
            delete temp;
            size_--;
        }
    }

    T& operator[](size_t idx) {
                if (idx >= size_) throw std::length_error("Индекс за пределами списка");

        Node* it = nullptr;
        if (size_ - idx > (size_ / 2)) {
            it = head_;
            for (size_t i = 0; i != idx; i++) { it = it->next; }
        }
        else {
            it = tail_;
            for (size_t i = size_ - 1; i != idx; i--) { it = it->prev; }
        }
        return it->value;
    }

    T operator[](size_t idx) const {
        if (idx >= size_) throw std::length_error("Индекс за пределами списка");

        Node* it = nullptr;
        if (size_ - idx > (size_ / 2)) {
            it = head_;
            for (size_t i = 0; i != idx; i++) { it = it->next; }
        }
        else {
            it = tail_;
            for (size_t i = size_ - 1; i != idx; i--) { it = it->prev; }
        }
        return it->value;
    }

    void delete_node(T value) {
        Node* it = head_;
        while (it) {
            if (it->value == value) {
                if (it == head_)
                    head_ = it->next;
                if (it == tail_)
                    tail_ = it->prev;

                if (it->prev)
                    it->prev->next = it->next;
                if (it->next)
                    it->next->prev = it->prev;
                Node* temp = it;
                it = it->next;
                delete temp;
                size_--;
            }
            else
                it = it->next;
        }
    }
};

template<typename T> // Решено ;) теперь оно линейно
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
    auto it = list.head_;
    while (it) {
        os << it->value << " ";
        it = it->next;
    }
    return os;
}

#endif // LINKED_LIST_H