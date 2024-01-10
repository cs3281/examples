#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
int
main(int argc, char *argv[])
{
 
   std::vector<std::string> my_vector;
   my_vector.push_back("item1");
   my_vector.push_back("item content 2");
   my_vector.push_back("item3");
   my_vector.push_back("This is the fourth item");

   for (int i=0;i<my_vector.size();i++)
    std::cout<<my_vector[i]<<std::endl;


   char** string_array=0;
   //we reserve the first for name of the program and the last for null.
   int n=my_vector.size()+2;
   string_array = (char**) malloc (sizeof(char**) * n);
   //initialize the string_array
   memset(string_array,0,sizeof(char**)*n);
   //strdup returns an allocated string pointer. You should free the returned memory later.
   string_array[0]=strdup("sort");
   for (int i=0;i<my_vector.size();i++)
       string_array[i+1]=strdup(my_vector[i].c_str());

   string_array[n-1]=0;
 
    //Now the string array contains all the entries and can be used to invoke another program.

    for (int i=0;i<n;i++)
     std::cout<<"string "<<string_array[i]<<std::endl;

   


}
