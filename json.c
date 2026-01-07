#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include "json.h"
#include "clog.h"
#include <limits.h>
#include <string.h>

bool parse_int(int* a_value,  char const** a_pos) {
	bool int_n = false;
	bool neg_n = false;
	//int nm = 0;

	if(**a_pos == '-') {
		(*a_pos)+=1;
		neg_n = true;
	}

	if(isdigit(**a_pos)) {
		int_n = true;
	}
	int numbb = 0;
	//*a_value = numbb; code fails here
	while(isdigit(**a_pos)) {
		int nm = (**a_pos) - '0';
		numbb = nm + (10 * numbb);
		(*a_pos) += 1;
	}
	*a_value = numbb; //code passes here
	if(neg_n && int_n) {
		*a_value = -1 * (*a_value);
	}
	return int_n;
}

bool parse_string(char** a_s, char const** a_pos){ //passing 18, 0 compiler errors,  1 fail maybe int invalid ?
	int sz = 0; //size
	bool booln;
	booln = false;

	if(**a_pos == '\"') {
		booln = true;
		(*a_pos)++;
	}
	while((**a_pos) != '\"'){
		log_char(**a_pos);
		if((**a_pos) == '\0' || ((**a_pos) == '\n')){
			booln = false;
			//break;  //check that again
		}
		//might need an else statement 
		sz++;
		(*a_pos)++;
	}
	if(booln){
		char* otp = malloc(sizeof(**a_s) * (sz+1));
		(*a_pos) -= sz;
		for(int id =0; id<sz; id++){
			(otp)[id] = **a_pos;
			(*a_pos)++;
		}
		otp[sz] = '\0';
		(*a_pos)++;
		*a_s = otp;
	}
	return booln; 
}
bool parse_element(Element* a_element, char const** a_pos){  // same twice

	while(isspace(**a_pos)){
		(*a_pos)++;
	}
	if((isdigit(**a_pos))|| (**a_pos == '-') ){
		a_element -> type = ELEMENT_INT;
		return parse_int(&(a_element -> as_int), a_pos);
	}
	else if((**a_pos) == '\"'){
		bool alp;    
		alp = parse_string(&(a_element -> as_string), a_pos);
		if(alp) {
			a_element -> type = ELEMENT_STRING;
			return true;
		}
		else {
			a_element -> type = ELEMENT_INT;
			return false;
		}


	}
	else if((**a_pos) == '['){
		a_element -> type = ELEMENT_LIST;
		return parse_list(&(a_element -> as_list), a_pos);
	}
	return false;
}

void _free_mem(Node* head) {  //helper function to free list ( call it in free element)
	while(head != NULL) {
		Node* temp = head;
		head = head -> next;
		free_element(temp -> element);	
		free(temp);
	}
}

void free_element(Element element){ // function to deallocate meory 
	//
	if(element.type == ELEMENT_STRING) {
		//deallocating memory wrong 
		//if(ELEMENT_STRING){
		free(element.as_string);
		//}
	}
	else if(element.type == ELEMENT_LIST){
		Node* head = element.as_list;
		_free_mem(head);

		/*while(head != NULL) {
		  Node* new_head = head;
		  head = head -> next;
		  free_element(temp -> element);	//breaks here : undeclared 'temp'
		  free(new_head); 
		  }*/
	}
}

/*void print_element(Element element){ //
  if(element.type == ELEMENT_INT){
  printf("%d", element.as_int);
  }
  else if(element.type == ELEMENT_STRING){
  printf("\"%s\"", element.as_string);
  }*/

void print_element(Element element) {	
	//printf("r");
	if(element.type == ELEMENT_INT) {
		printf("%d", element.as_int);
	}
	else if(element.type == ELEMENT_STRING) {
		printf("\"%s\"", element.as_string);
	}
	else if(element.type == ELEMENT_LIST) { 
		printf("[");
		//printf("x");
		for(Node* ind = element.as_list; ind != NULL; ind = ind-> next) {
			//printf("f");
			print_element(ind -> element);
			if(ind -> next != NULL) {
				printf(",");
			}
		}
		printf("]");
	}

}
// Passing 8/14 without parse_list // //passing 13/14, check coverage again // check valgrind issue   //42/43 with valrgind and partial
bool parse_list(Node** a_head, char const** a_pos){
	bool lsd = true;
	if((**a_pos) == '['){
		(*a_pos)++;
		Node* new_h = NULL;  // 
		Node* tail = NULL;
		while((**a_pos) != ']'){
			if((**a_pos) == ','){
				(*a_pos)++;
				if((**a_pos) == ','){
					lsd = false;
					_free_mem(new_h);
					break;
				}
				/* if(new_h == NULL){
				   new_h = new_n;
				   tail = new_n;
				   }*/

			}
			Node* newnode = malloc(sizeof(*newnode));
			newnode -> next  = NULL;
			bool is_success = parse_element(&(newnode -> element), a_pos);// holds bool

			if(new_h == NULL){
				tail = newnode;
				new_h = newnode;
			}
			else{
				tail -> next = newnode; // ????
				tail = newnode;
			}
			while(isspace(**a_pos)){
				(*a_pos)++;
			}
			if(**a_pos != ',' && **a_pos != ']') {
				_free_mem(new_h);
				lsd = false;
				break;
			}

			if(!is_success) {  //if fails it will free space for new node and head
				_free_mem(new_h); // check if we need to free new node
				lsd = false;
				break;
			}
			/*while(isspace(**a_pos)){
			  (*a_pos)++;
			  }*/

		}
		*a_head = new_h;
		(*a_pos)++;
	}
	else{
		lsd = false;
	}
	if(!lsd) {
		*a_head = NULL;
		//free_element(element);
	}
	return lsd;
}

//add helper function to free list, and call function in free_element
