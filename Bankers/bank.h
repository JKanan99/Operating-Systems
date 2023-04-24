//
//  bank.h
//  bankers
//
//  Created by William McCarthy on 21 Mar 2022.
//

#ifndef __bank_h__
#define __bank_h__

#include <iostream>
#include <mutex>
#include <sstream>
#include "customer.h"
#include "vector_.h"


class Bank {
public:
  Bank() = default;
  Bank(const vector_<int>& available) : avail(available), customers() { }
  
  vector_<int> get_avail() const { 
    return avail; 
  }

  bool is_avail(const vector_<int>& req) const { 
    return req < avail; 
  }
 
  bool is_safe(int id, const vector_<int>& req) { //pretend to give the request to the customer. dont leave! 
    vector_<int> pretend_loan = this->avail;      //go through each and ask each if its enough money for each, if is, bool = true.
    pretend_loan -= req;                          //take money back and put it back into the vault 
    bool temporary_ans = false;                   //this function is not giving money. Asking-> is the req approved?
                                       
    for(size_t i = 0; i < this->avail.size(); ++i){ // going through each customer to see if theres enough rescources 
      if(i == id){                                
        continue;
      }
      if(pretend_loan >= this->customers[i]->create_req()){  
        temporary_ans = true;
      }
    }
    return temporary_ans; 
  }   

  bool req_approved(int id, const vector_<int>& req) { //what does it mean to approve a request? 
    if(req > avail){ // if the request is more than what's available, return false
      return false;
    }

    Customer* c = customers[id];

    if(c->too_much(req)){
      return false;
    }
    return !c->too_much(req); //customer is asking for too many requests
  }
  
  void add_customer(Customer* c) { 
    customers.push_back(c); 
  }
  
  void withdraw_resources(const vector_<int>& req) {
    if (!is_avail(req)) {
      Utils::print_locked("WARNING: req: %s is not available for withdrawing\n", (req.as_string().c_str()));
      return;
    }
    if (is_avail(req)) { 
      avail -= req; 
    }
  }
  void deposit_resources(const vector_<int>& req) { 
    avail += req; 
  }


  vector_<Customer*> get_customers() const { 
    return customers; 
  }
  
  void show() const {
    std::stringstream ss;
    ss << "\n+-----------------------------------------\n"
          "|   BANK   avail: [" << avail.as_string().c_str() << "]\n"
          "+-----------------------------------------\n";

    for (Customer* c : customers) {
      ss << "| P# " <<  std::setw(2) << c->get_idx() << "   "
         <<  (c->get_alloc().as_string().c_str()) << "   "
         << (c->get_max().as_string().c_str())    << "   "
         << (c->get_need().as_string().c_str())
         << "\n";
    }
    std::string s = ss.str();

    Utils::print_locked("%s"
                        "+----------------------------------------\n", (s.c_str()));
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Bank& bank) {
    bank.show();
    return os;
  }

private:
  vector_<int> avail;
  vector_<Customer*> customers;
};

#endif /* __bank_h__ */
