// FILE: company.cpp
// CLASS implemented: company (see company.h for documentation)


#include <cassert>
#include "company.h"

//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif

namespace coen79_lab7
{
    company::company() {
        this->company_name = "";
        this->head_ptr = NULL;
        this->tail_ptr = NULL;
    }
    
    company::company(const std::string& company_name) {
        assert(company_name.length() > 0);

        // COMPLETE THE IMPLEMENTATION...
        this->company_name = company_name;
        this->head_ptr = NULL;
        this->tail_ptr = NULL;
    }
    
    company::company(const company &src) {
        Debug("Company copy constructor..." << std::endl);

        // COMPLETE THE IMPLEMENTATION...
        this->company_name = src.company_name;
        list_copy(src.get_head(), this->head_ptr, this->tail_ptr);
    }

    
    company& company::operator= (const company &src) {
        Debug("Company assignemnt operator..." << std::endl);

        // COMPLETE THE IMPLEMENTATION...
        this->company_name = src.company_name;
        list_copy(src.get_head(), this->head_ptr, this->tail_ptr);
        return *this;
    }
    
    
    company::~company() {
        list_clear(head_ptr);
    }
    
    
    std::string company::get_name() const {
        return company_name;
    }
    
    const node *company::get_head() const {
        return head_ptr;
    }
    
    const node *company::get_tail() const {
        return tail_ptr;
    }
    
    node* company::get_head() {
        return head_ptr;
    }
    
    node* company::get_tail() {
        return tail_ptr;
    }
    
    void company::print_items() {
        list_print(head_ptr);
    }
    
    
    bool company::insert(const std::string& product_name, const float& price) {
        
        assert(product_name.length() > 0);

        if (list_contains_item(head_ptr, product_name)) {
            return false;
        }
        
        if (head_ptr == NULL) {
            // COMPLETE THE IMPLEMENTATION...
            list_init(head_ptr, tail_ptr, product_name, price);
        }
        else {
            // COMPLETE THE IMPLEMENTATION...
            list_tail_insert(tail_ptr, product_name, price);
        }
        
        return true;
    }

    
    bool company::erase(const std::string& product_name) {
        assert(product_name.length() > 0);

        // COMPLETE THE IMPLEMENTATION...
         if(list_contains_item(head_ptr, product_name)) {                
            node* cursor = head_ptr;                                   
            node* delete_ptr;                                         
            while(cursor->getLink()->getName() != product_name)       
                cursor = cursor->getLink();
            delete_ptr = cursor;                             
            delete_ptr = delete_ptr->getLink();                       
            cursor->setLink(delete_ptr->getLink());                
            delete delete_ptr;
            return true;
        }
        else
            return false;
    }
    
    
}
