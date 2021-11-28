#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

//g++ -o History_transaction main.cpp project4.cpp History_transaction_definitions.cpp -std=c++11

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor: definition
  //A constructor is a member function that is automatically called when the object is created/instantiated/allocated
  //Every time you make an object from the class, you must call the constructor
  //A destructor is a member function that is automatically called when the object is de-allocated.
    //When is an object/variable de-allocated?
    //1. For local variables, when they go out of scope.
    //2. For dynmaically allocated variables, when they are deleted.


// TASK 1
//

Transaction::Transaction (std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned int year_date, bool buy_sell_trans, unsigned int number_shares, double trans_amount):
  symbol{ticker_symbol},
  day{day_date},
   month{month_date},
   year{year_date},
   trans_type {buy_sell_trans},
   shares {number_shares},
   amount {trans_amount},
   trans_id{assigned_trans_id}
   {
     if(buy_sell_trans) {
       trans_type = "Buy";
     }
      else {
        trans_type = "Sell";
      }
      assigned_trans_id += 1; 
     }
   
   

   
  
// Destructor
// TASK 1
//

Transaction::~Transaction() {
  
}

// Overloaded < operator.
// TASK 2
//

bool Transaction::operator<(Transaction const &other) {

  if(year<other.year) {
    return true;
  }
  else if (other.year<year) {
    return false;
  }
  else {
    if(month<other.month) {
      return true;
    }
    else if (other.month<month) {
      return false;
    }
    else {
      if(day<other.day) {
        return true;
      }
      else if(other.day<day) {
        return false;
      }
    }
  }

  return false;

}


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//

History::History() {

  //////////////////AMEYA
  p_head = nullptr;

}
// Destructor
// TASK 3
//


History::~History() {

  if(this->p_head==nullptr) {
    //if the start of the linked list is nullptr, this means that there are no nodes so there is nothing to clean up
  }
  else {//if there is at least one node
    Transaction *current = p_head;
    Transaction *next = p_head;

    while(true) {
      if(current->get_next() == nullptr) { //on the last node
         delete current; //clean up last node
         current = nullptr;
         break; //breka while loop
      }
      next = current->get_next(); //save address of next node
      delete current; //delete current node
      current = nullptr;
      current = next;  //set current node to be the next node
    }
  }
}

// read_history(...): Read the transaction history from file.
// TASK 4
//

void History::read_history() {

  //You will implement a member function called read_history() for the History class that will read the transaction history from a text file called transaction_history.txt.
  // This member function will insert the transaction entries in the order in which they are read from the file in thelinked list. 
  //You will need to use the provided functions to access the transaction history file.
  //Hint: you should call the insert(...)member function from the Historyclass to insert it into thelinked list.

  ece150::open_file();

  while(ece150::next_trans_entry() == true) {
    Transaction *current_trans = new Transaction{ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()};
    insert(current_trans);
  }
  ece150::close_file();

  //read transactions one at a time
  //read specific parts: day, month, year etc
  // dynamically allocate a new Transaction object
  // call insert
  //close file
}

// insert(...): Insert transaction into linked list.
// TASK 5
//

void History::insert(Transaction * p_new_trans) {

  if(this->p_head==nullptr) {
    p_head = p_new_trans;
  }
  else {
    unsigned int current_trans_id {p_new_trans->get_trans_id()};
    unsigned int difference_from_head {current_trans_id - (p_head->get_trans_id())};
    Transaction *node_before = p_head;
    for(int i{1}; i<difference_from_head; i++) {
      node_before = node_before->get_next();
    }
    node_before->set_next(p_new_trans);
  }

  //get assigned transaction ID
  //get current transaction ID
  //Find difference
  //get.next() difference-1 number of times will give you the node before the node you are adding
}


// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

void History::sort_by_date() {

}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

void History::update_acb_cgl(){

}


// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl(unsigned int year){
  return 0.0;
}


// print() Print the transaction history.
//TASK 9
//

void History::print(){

  
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  int i = 0;
  ece150::open_file();
  while(ece150::next_trans_entry() == true) {
    if(ece150::get_trans_type()==true) {
        std::cout << i << "\t" << ece150::get_trans_symbol << "\t" << ece150::get_trans_day << "\t" << ece150::get_trans_month << "\t" << ece150::get_trans_year << "\t" << "Buy" << "\t" << ece150::get_trans_shares << "\t" << ece150::get_trans_amount << std::endl;
    }
    else {
        std::cout << i << "\t" << ece150::get_trans_symbol << "\t" << ece150::get_trans_day << "\t" << ece150::get_trans_month << "\t" << ece150::get_trans_year << "\t" << "Sell" << "\t" << ece150::get_trans_shares << "\t" << ece150::get_trans_amount << std::endl;

    }
    std::cout << i << "\t" << ece150::get_trans_symbol << "\t" << ece150::get_trans_day << "\t" << ece150::get_trans_month << "\t" << ece150::get_trans_year << "\t" << ece150::get_trans_type << "\t" << ece150::get_trans_shares << "\t" << ece150::get_trans_amount << std::endl;
    i+=1;
  }
  std::cout <<"========== END TRANSACTION HISTORY ============" << std::endl;
  ece150::close_file();
}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
