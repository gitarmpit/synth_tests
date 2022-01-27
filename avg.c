#include <stdio.h> 

static int filter(int new_data)
{
  static int data[8] = { 0,0,0,0,0,0,0,0 };
  static int oldiest_idx = 0;
  static int  sum = 0;
  static int cnt = 0;

  sum = sum - data[oldiest_idx] + new_data;

  data[oldiest_idx] = new_data;

  //p_oldiest_ptr = (p_oldiest_ptr==7) ? 0 :(p_oldiest_ptr++);
  oldiest_idx = (++oldiest_idx) % 8;

  printf ("%d: %d\n", ++cnt, sum>>3);

  return(sum>>3);
}

int main(void) 
{
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    

    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    
    filter(2);    

    filter(20);    
    filter(20);    
    filter(20);    
    filter(20);    
    filter(20);    
    filter(20);    
    filter(20);    
    filter(20);    

}