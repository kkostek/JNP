#include <iostream>
#include <map>
#include <list>
#include <memory>

class lookup_error : std::exception {
public:
    lookup_error(){}
};

template <class K, class V>
class keyed_queue {

    typedef typename std::list<std::pair<K, V>>::iterator list_iter;

private:

    class Data {
    public:
        std::list<std::pair<K, V>> elements;
        std::map<K, std::list<typename std::list<std::pair<K, V>> :: iterator>> map_of_iters;
        std::map<K, V> leaders;

        Data() {}

        Data(Data const & copy) {
            map_of_iters.clear();
            elements = std::list<std::pair<K, V>>(copy.elements);
            for (typename std::list<std::pair<K, V>>::iterator it = elements.begin() ; it != elements.end() ; ++it) {
                map_of_iters[it->first].push_back(it);
            }
            leaders = std::map<K, V>(copy.leaders);
        }
    };

    std::shared_ptr<Data> data;
    bool is_a_reference;

    void copy() {
        data = std::make_shared<Data>(*data);
    }

    void add_leader(const K& k, const V& v) {
        if (data->leaders.find(k) == data->leaders.end()) {
            data->leaders.insert(std::make_pair(k, v));
        }
    }

    void remove_leader(const K& k) {
        data->leaders.erase(k);
        if (data->map_of_iters.find(k) != data->map_of_iters.end()) {
            auto it = data->map_of_iters.find(k)->second.front();
            data->leaders.insert(std::make_pair((*it).first, (*it).second));
        }
    }

public:

    keyed_queue() {
        data = std::make_shared<Data>();
        is_a_reference = false;
    }

    keyed_queue(keyed_queue const& other) {
        if(other.is_a_reference) {
            data = std::make_shared<Data>(*(other.data));
        } else {
            data = other.data;
        }
        is_a_reference = false;
    }

    keyed_queue(keyed_queue&& other) {
        is_a_reference = other.is_a_reference;
        data = other.data;
    }

    keyed_queue &operator=(keyed_queue other) {
        data = other.data;
        is_a_reference = other.is_a_reference;
        return *this;
    }

    void push(K const& k, V const& v) {
        is_a_reference = false;
        if (data.use_count() != 1) {
            copy();
        }
        data->elements.push_back(std::make_pair(k, v));
        list_iter iter = data->elements.end();
        iter--;
        if (data->map_of_iters.find(k) == data->map_of_iters.end()) {
            std::list<list_iter> k_list;
            data->map_of_iters.insert(std::make_pair(k, k_list));
        }
        data->map_of_iters.find(k)->second.push_back(iter);
        add_leader(k, v);
    }

    void pop() {
        is_a_reference = false;
        if (data->elements.empty()) {
            throw lookup_error();
        }
        if (data.use_count() != 1) {
            copy();
        }
        auto k_pop = data->elements.front().first;
        data->map_of_iters.find(k_pop)->second.pop_front();
        data->elements.pop_front();
        if (!data->map_of_iters.find(k_pop)->second.empty()) {
             remove_leader(k_pop);
        }
    }

    void pop(K const& k) {
        is_a_reference = false;
        if (data->elements.empty() || data->map_of_iters.find(k) == data->map_of_iters.end()) {
            throw lookup_error();
        }
        if (data.use_count() != 1) {
            copy();
        }
        auto iter_pop = data->map_of_iters.find(k)->second.front();
        data->map_of_iters.find(k)->second.pop_front();
        data->elements.erase(iter_pop);
        if (!data->map_of_iters.find(k)->second.empty()) {
             remove_leader(k);
        }
    }

    void move_to_back(K const& k) {
        is_a_reference = false;
        if (data->map_of_iters.find(k) == data->map_of_iters.end()) {
            throw lookup_error();
        }
        if (data.use_count() != 1) {
            copy();
        }
        for (auto it = data->map_of_iters.find(k)->second.begin(); it != data->map_of_iters.find(k)->second.end(); ++it) {
            auto rec = *(*it);
            data->elements.erase(*it);
            data->elements.push_back(rec);
            *it = --data->elements.end();
        }
    }

    std::pair<K const&, V&> front() {
        if (data->elements.empty()) {
           throw lookup_error();
        }
        if (data.use_count() != 1) {
           copy();
        }
        K const& k_front = (K const&) data->elements.front().first;
        V& v_front = (V&) data->elements.front().second;
        std::pair<K const&, V&> front_pair({k_front, v_front});
        is_a_reference = true;
        return front_pair;
    }

    std::pair<K const&, V&> back() {
        if (data->elements.empty()) {
           throw lookup_error();
        }
        if (data.use_count() != 1) {
           copy();
        }
        const K& k_back = (const K&) data->elements.back().first;
        V& v_back = (V&) data->elements.back().second;
        std::pair<K const&, V&> back_pair({k_back, v_back});
        is_a_reference = true;
        return back_pair;
    }

    std::pair<K const&, V const&> front() const {
        if (data->elements.empty()) {
           throw lookup_error();
        }
        K const& k_front = (K const&) data->elements.front().first;
        const V& v_front = (V&) data->elements.front().second;
        std::pair<K const&, const V&> front_pair({k_front, v_front});
        return front_pair;
    }

    std::pair<K const&, V const&> back() const {
        if (data->elements.empty()) {
           throw lookup_error();
        }
        const K& k_back = (const K&) data->elements.back().first;
        const V& v_back = (V&) data->elements.back().second;
        std::pair<K const&, const V&> back_pair({k_back, v_back});
        return back_pair;
    }


    std::pair<K const&, V&> first(K const& key){
        if (data->elements.empty() || data->map_of_iters.find(key) == data->map_of_iters.end()) {
           throw lookup_error();
        }
        if (data.use_count() != 1) {
           copy();
        }
        const K& k_first = (K const&) data->map_of_iters.find(key)->second.front()->first;
        V& v_first = (V&) data->map_of_iters.find(key)->second.front()->second;
        std::pair<const K&, V&> first_pair({k_first, v_first});
        is_a_reference = true;
        return first_pair;
    }

    std::pair<K const&, V&> last(K const& key) {
        is_a_reference = false;
        if (data->elements.empty() || data->map_of_iters.find(key) == data->map_of_iters.end()) {
           throw lookup_error();
        }
        if (data.use_count() != 1) {
           copy();
        }
        const K& k_last = (K const&) data->map_of_iters.find(key)->second.back()->first;
        V& v_last = (V&) data->map_of_iters.find(key)->second.back()->second;
        std::pair<const K&, V&> last_pair({k_last, v_last});
        is_a_reference = true;
        return last_pair;
    }

    std::pair<K const&, V const&> first(K const& key) const {
        if (data->elements.empty() || data->map_of_iters.find(key) == data->map_of_iters.end()) {
           throw lookup_error();
        }
        const K& k_first = (K const&) data->map_of_iters.find(key)->second.front()->first;
        const V& v_first = (V&) data->map_of_iters.find(key)->second.front()->second;
        std::pair<const K&, const V&> first_pair({k_first, v_first});
        return first_pair;
    }

    std::pair<K const&, V const&> last(K const& key) const {
        if (data->elements.empty() || data->map_of_iters.find(key) == data->map_of_iters.end()) {
           throw lookup_error();
        }
        const K& k_last = (K const&) data->map_of_iters.find(key)->second.back()->first;
        const V& v_last = (V&) data->map_of_iters.find(key)->second.back()->second;
        std::pair<const K&, const V&> last_pair({k_last, v_last});
        return last_pair;
    }

    size_t size() const {
        return data->elements.size();
    }

    bool empty() const {
        return data->elements.empty();
    }

   void clear() {
        is_a_reference = false;
        if (data.use_count() != 1) {
           copy();
        }
        for (auto it = data->elements.begin(); it != data->elements.end(); ++it) {
           data->elements.erase(it);
        }
        for (auto it = data->map_of_iters.begin(); it != data->map_of_iters.end(); ++it) {
           data->map_of_iters.erase(it);
}
    }

    size_t count(K const& k) const {
       return data->map_of_iters.find(k) == data->map_of_iters.end() ? 0 : data->map_of_iters.find(k)->second.size();
       }

    class k_iterator {

    private:
        typename std::map<K, V>::iterator iter;

    public:

        k_iterator() = default;

        k_iterator(typename std::map<K, V>::iterator it) :
            iter(it)
        {}

        k_iterator operator ++() {
            ++iter;
            return *this;
        }

        bool operator ==(keyed_queue<K, V>::k_iterator& it) {
            return iter == it.iter;
        }

        bool operator !=(keyed_queue<K, V>::k_iterator& it) {
            return iter != it.iter;
        }

        K operator *() {
            return iter->first;
        }
    };

    k_iterator k_begin() {
        return k_iterator(data->leaders.begin());
    }

    k_iterator k_end() {
        return k_iterator(data->leaders.end());
    }

};


