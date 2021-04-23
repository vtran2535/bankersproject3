//
//  customer.h
//  bankers
//
//

#ifndef _Customer_h
#define _Customer_h

#include "bank.h"
#include <pthread.h>

class Bank;

extern pthread_mutex_t mutex_;


class Customer {
public:
//  Customer() = default;
  Customer
(int index, const ext_vector<int>& allocate, const ext_vector<int>& maximum,
                  Bank* bank_=nullptr)
  : idx(index), alloc(allocate), max_(maximum), need(max_ - alloc), bank(bank_) { }
  
  int get_id() const { return idx; }
  pthread_t* get_threadid() { return &thread_id; }
  
  Bank* get_bank() const { return bank; }
  ext_vector<int> get_max() const { return max_; }
  
  bool needs_met() const { return alloc == max_; }
  
  void alloc_req(  const ext_vector<int>& req) { alloc += req;  need -= req; }
  void dealloc_req(const ext_vector<int>& req) { alloc -= req;  need += req; }
  
  void release_all_resources() {
    ext_vector<int> zero(alloc.size(), 0);
    alloc = max_ = need = zero;
    std::cout << "Customer p#" << idx << " has released all resources and is shutting down\n";
  }

  void show() const {
    pthread_mutex_lock(&mutex_);
    std::cout << "P#" << idx << "    " << alloc << "    " << max_ << "    " << need << "\n";
    pthread_mutex_unlock(&mutex_);
  }

  friend std::ostream& operator<<(std::ostream& os, const Customer
& c) {
    c.show();
    return os;
  }
  
  ext_vector<int> create_req() { return ext_vector<int>({ 1, 1, 1 }); }
  
private:
  int idx;
  ext_vector<int> alloc;
  ext_vector<int> max_;
  ext_vector<int> need;
  pthread_t        thread_id;
  
  Bank* bank;
};

#endif /* Customer_h */
