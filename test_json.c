#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "miniunit.h"
 
typedef struct {
 bool is_success;
 union {  // anonymous union (C11)
   Element element;
   long int error_idx;
 };
} ParseResult;
 
static ParseResult _parse_json(char* s) {
 Element element;  // Not initialized because parse_element(…) *must* do so.
 char const* pos = s;
 //printf("DD");
 bool is_success = parse_element(&element, &pos);
   if(is_success) {
   return (ParseResult) { .is_success = is_success, .element = element };
 }
 else {
   return (ParseResult) { .is_success = is_success, .error_idx = pos - s };
 }
}
 
static int _test_int() {
 mu_start();
 //────────────────────
 ParseResult result = _parse_json("0");
 mu_check(result.is_success);
 if(result.is_success) {
   mu_check(result.element.type == ELEMENT_INT);
   mu_check(result.element.as_int == 0);
   free_element(result.element);  // should do nothing
 }
 //────────────────────
 mu_end();
}
 
 
static int _test_string() {
 mu_start();
 //────────────────────
 ParseResult result = _parse_json("\"abc\""); //maybe remove ParseResult if there are issues?
 mu_check(result.is_success);
 if(result.is_success) {
   mu_check(result.element.type == ELEMENT_STRING);
   mu_check(strcmp(result.element.as_string, "abc") == 0);
   mu_check(strlen(result.element.as_string) == 3);
   free_element(result.element);
 }
 //────────────────────
 mu_end();
}
 
static int _test_list_of_ints() {
 mu_start();
 //────────────────────
 ParseResult result = _parse_json("[1, 2]");
 mu_check(result.is_success);
 if(result.is_success) {
   mu_check(result.element.type == ELEMENT_LIST);
   mu_check(result.element.as_list != NULL);
   mu_check(result.element.as_list -> element.as_int == 1);
   mu_check(result.element.as_list -> element.type == ELEMENT_INT);
   mu_check(result.element.as_list -> next != NULL);
   mu_check(result.element.as_list -> next -> element.type == ELEMENT_INT);
   mu_check(result.element.as_list -> next -> element.as_int == 2);
   free_element(result.element);
 }
 //────────────────────
 mu_end();
}
 
static int _test_list(){
 mu_start();
 ParseResult List01 = _parse_json("[]");  //pass
 mu_check(List01.is_success);
 print_element(List01.element);
 fputc('\n',stdout);
 free_element(List01.element);
 
 ParseResult List02 = _parse_json("[9]");//pass
 mu_check(List02.is_success);
 print_element(List02.element);
 fputc('\n',stdout);
 free_element(List02.element);
 
 ParseResult List03 = _parse_json("[47]");//pass
 mu_check(List03.is_success);
 print_element(List03.element);
 fputc('\n',stdout);
 free_element(List03.element); 

ParseResult List04 = _parse_json("[1, 7]");//pass
 mu_check(List04.is_success);
 print_element(List04.element);
 fputc('\n',stdout);
 free_element(List04.element);

 ParseResult List05 = _parse_json("[\"X\", \"Z\"]");//pass
 mu_check(List05.is_success);
 print_element(List05.element);
 fputc('\n',stdout);
 free_element(List05.element);

 ParseResult List06 = _parse_json("[,,]");
 mu_check(!List06.is_success);
 print_element(List06.element);
 fputc('\n',stdout);
 free_element(List06.element);

 ParseResult List07 = _parse_json("[4,5");
 mu_check(!List07.is_success);
 print_element(List07.element);
 fputc('\n',stdout);
 free_element(List07.element);

 ParseResult List08 = _parse_json("[\"e\",\"n\"]"); //test failing  //pass
 mu_check(List08.is_success);
 print_element(List08.element);
 fputc('\n',stdout);
 free_element(List08.element);

 ParseResult List09 = _parse_json("[12,[4,9], 789]"); //check gcc //pass
 mu_check(List09.is_success);
 print_element(List09.element);
 fputc('\n',stdout);
 free_element(List09.element);

 ParseResult List10 = _parse_json("[\"Frank\",8,\"milk\",\"goofy\",321]"); //pass
 mu_check(List10.is_success);
 print_element(List10.element);
 fputc('\n',stdout);
 free_element(List10.element);

 ParseResult List11 = _parse_json("[\"F47\",8,\"milk\",\"goo8y\",321]"); //pass
 mu_check(List11.is_success);
 print_element(List11.element);
 fputc('\n',stdout);
 free_element(List11.element);

 ParseResult List13 = _parse_json("v[11,34]");
 mu_check(!List13.is_success);
 print_element(List13.element);
 fputc('\n',stdout);
 free_element(List13.element);

 ParseResult List14 = _parse_json("v[[3,6],[5,3]]");
 mu_check(!List14.is_success);
 print_element(List14.element);
 fputc('\n',stdout);
 free_element(List14.element);

 ParseResult List15 = _parse_json("[69, 69, [69,[69,69,[69]]]]"); //pass
 mu_check(List15.is_success);
 print_element(List15.element);
 fputc('\n',stdout);
 free_element(List15.element);

 ParseResult List16 = _parse_json("[      7     ]"); //pass
 mu_check(List16.is_success);
 print_element(List16.element);
 fputc('\n',stdout);
 free_element(List16.element);

 ParseResult List17 = _parse_json("[1,\"string]"); // creating one error 
 mu_check(!List17.is_success);
 print_element(List17.element);
 fputc('\n',stdout);
 free_element(List17.element);

 ParseResult List18 = _parse_json("[--6]"); //
 mu_check(!List18.is_success);
 print_element(List18.element);
 fputc('\n',stdout);
 free_element(List18.element);
 
 mu_end();
}
 
int main(int argc, char* argv[]) {
 mu_run(_test_int);
 mu_run(_test_string);
 mu_run(_test_list_of_ints);
 mu_run(_test_list);
 return EXIT_SUCCESS;
}

