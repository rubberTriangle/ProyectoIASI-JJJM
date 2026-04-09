#ifndef MODIFIABLEHEAP_H
#define MODIFIABLEHEAP_H

//Puede que le cambie el nombre más tarde, se me ocurría resistant_heap, healing_heap, resistant_priority_queue, editable_priority_queue
#include <functional>
template<class T, class comp = std::less<T>>
class ModifiableHeap//max-heap the same way std::priority_queue is a max-heap by default
{
private:
    std::vector<T> v;
public:
    ModifiableHeap();
    
    T const& top() const;
    bool empty() const;
    int size() const;
    
    void push(T const& value);
    void push(T && value);
    template<class... Args> void emplace(Args&&... args);
    
    void pop();
    
    //if you modify a value you'll have to call update(i) to maintain the heap
    T& operator[](int i);
    
    void upheap(int i);
    void downheap(int i);
    void remove(int i);
    void update(int i);//call after modifying the value stored at i
};

#include <algorithm>

template<class T, class comp>
ModifiableHeap<T, comp>::ModifiableHeap() { }

template<class T, class comp>
const T &ModifiableHeap<T, comp>::top() const{
    return v.front();
}

template<class T, class comp>
bool ModifiableHeap<T, comp>::empty() const{
    return v.empty();
}

template<class T, class comp>
int ModifiableHeap<T, comp>::size() const{
    return v.size();
}

template<class T, class comp>
void ModifiableHeap<T, comp>::push(const T &value){
    v.push_back(value);
    std::push_heap(v.begin(), v.end(), comp());
}

template<class T, class comp>
void ModifiableHeap<T, comp>::push(T &&value){
    v.push_back(std::forward<T>(value));
    std::push_heap(v.begin(), v.end(), comp());    
}

template<class T, class comp>
template<class... Args>
inline void ModifiableHeap<T, comp>::emplace(Args&&... args){
    v.emplace_back(std::forward<Args>(args)...);
    std::push_heap(v.begin(), v.end(), comp());
}

template<class T, class comp>
void ModifiableHeap<T, comp>::pop(){
    std::pop_heap(v.begin(), v.end(), comp());
    v.pop_back();
}

template<class T, class comp>
T &ModifiableHeap<T, comp>::operator [](int i){
    return v[i];
}

//found some info on heap ops on https://en.wikipedia.org/wiki/Binary_heap (my bwain kinda fried sowwy :( )

template<class T, class comp>
void ModifiableHeap<T, comp>::upheap(int i){
    for(int j = i;;){
        int parent = (j-1)/2;//ints implicitly round down already
        if(parent >= 0 && comp()(v[parent], v[j])){
            std::swap(v[j], v[parent]);
            j = parent;
        }else
            break;
    }    
    
}

template<class T, class comp>
void ModifiableHeap<T, comp>::downheap(int i){
    for(int j = i; j < v.size();){
        int left = j*2+1;
        int right = left+1;
        int imax = j;
        if(left < v.size() && comp()(v[j], v[left]))
            imax = left;
        if(right < v.size() && comp()(v[j], v[right]))
            imax = right;
        if(imax == j)
            break;
        else{
            std::swap(v[j], v[imax]);
            j = imax;
        }
    }    
}

template<class T, class comp>
void ModifiableHeap<T, comp>::remove(int i){
    std::swap(v[i], v.back());
    v.pop_back();
    downheap(i);
}

template<class T, class comp>
void ModifiableHeap<T, comp>::update(int i){
    if((i-1)/2 >= 0 && comp()(v[(i-1)/2], v[i]))
        upheap(i);
    else
        downheap(i);
}


#endif // MODIFIABLEHEAP_H
