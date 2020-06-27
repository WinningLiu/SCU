#include <iostream>
#include <cassert>
#include <cstdlib>      // Provides size_t declaration
#include <cmath>
#include "keyed_bag.h"

using namespace std;

namespace coen79_lab4 {

    keyed_bag::keyed_bag() {
        for(int i = 0; i < CAPACITY; i++) {
            keys[i] = '\0';
            data[i] = NULL;
        }
        used = 0;       
    }

    void keyed_bag::erase() {
        for(int i = 0; i < used; i++) {            
            keys[i] = '\0';                         
            data[i] = NULL;                        
        }
        used = 0;                                 
    }
    
    bool keyed_bag::erase(const key_type& key) {
        assert(used > 0);                           
        for(int i = 0; i < used; i++) {           
            if(keys[i] == key) {                   
                for(int j = i; j < used-1 ; j++) {  
                    keys[j] = keys[j+1];            
                    data[j] = data[j+1];            
                }
                used--;
                return true;                      
            }
        }
        return false;                           
    }
    
    void keyed_bag::insert(const value_type& entry, const key_type& key) {
        assert((size() < CAPACITY) && (has_key(key) == false));   
        keys[used] = key;                      
        data[used] = entry;                      
        used++;
    }
    
    void keyed_bag::operator+=(const keyed_bag& addend) {
        assert(size() + addend.size() <= CAPACITY);                 
        for(int i = 0; i < addend.used; i++) {      
            if( !has_key(addend.keys[i]) ) {    
                keys[used] = addend.keys[i];      
                data[used] = addend.data[i];        
                used++;
            }
        }
    }

    bool keyed_bag::has_key(const key_type& key) const {
        for(int i = 0; i < used; i++) {             
            if(keys[i] == key) {                 
                return true;                       
            }
        }
        return false;                               
    }
    
    keyed_bag::value_type keyed_bag::get(const key_type& key) const {
        assert(has_key(key));                       
        for(int i = 0; i < used; i++) {           
            if(keys[i] == key) {                  
                return data[i];                     
            }
        }
        return NULL;                                
    }
    
    keyed_bag::size_type keyed_bag::size() const {
        return used;
    }

    keyed_bag::size_type keyed_bag::count(const value_type& target) const {
        size_type count = 0;                
        for(int i = 0; i < used; i++) {             
            if(data[i] == target) {                
                count++;                       
            }
        }
        return count;
    }
    
    bool keyed_bag::hasDuplicateKey(const keyed_bag& otherBag) const {
        for(int i = 0; i < otherBag.used; ++i) {   
            if( has_key(otherBag.keys[i]) ) {      
                return true;                        
            }
        }
        return false;
    }
    
    keyed_bag operator+(const keyed_bag& b1, const keyed_bag& b2) {
        assert((b1.size()+b2.size()) <= keyed_bag::CAPACITY);     
        keyed_bag temp;                             
        temp += b1;                              
        temp += b2;                                 
        return temp;                                
    }
}